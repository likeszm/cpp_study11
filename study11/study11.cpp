/*
* 名称：类测试2: 
* 
* 收获:
* 1、友元函数的使用
* 2、拷贝构造函数的使用
* 3、const修饰指针和函数的情况
* 4、const的bug: 对于const修饰的函数，不允许修改值，不允许调用非const方法，
*   但是可以通过调用友元函数的方式走后门修改，所以应当不这么做
* 5、例外：然而加上 mutable 修饰符的数据成员,对于任何情况下通过任何手段都可修改,自然此时的 const 成员函数是可以修改它的；
* 6、实验发现，如果传入一个类，不是传地址，那么会自动调用拷贝构造函数，新构造一个临时的对象再操作
*/

#include <iostream>
#define DEBUG
#ifdef DEBUG
std::string debug_str;
#endif // DEBUG

class Line_class
{
public:

    void set_length(double len);
    double get_length(void);

    Line_class(double len);             //构造函数的声明不要void
    ~Line_class();                      //析构函数既没有void也必须无参数
    Line_class(const Line_class& obj);

    friend double visit_by_friend(Line_class temp);
    friend class Line_friend_class;

    void const_and_fun_2(void) const;

private:
    double length;
};

class Line_friend_class
{
public:
    double friend_class_fun(Line_class temp)
    {
#ifdef DEBUG && 1
        debug_str = "friend_class_fun";
        std::clog << "---LOG: running: " << debug_str << "()" << std::endl;
#endif // DEBUG
        return temp.length;
    }
};

void constructor_test(void);
void copy_constructor_test(void);
void friend_fun_test(void);
void const_and_pointer_test(void);
const int * const_and_fun_1(void);
void const_and_fun_test_1(void);
void friend_class_test(void);

int count = 0;

int main()
{
    //constructor_test();
    //copy_constructor_test();
    //friend_fun_test();
    //const_and_pointer_test();
    //const_and_fun_test_1();
    //friend_class_test();

    std::cout << "\n测试完成\n";
    return 0;
}

void Line_class::set_length(double len)
{
#ifdef DEBUG && 1
    debug_str = "set_length";
    std::clog << "---LOG: running: " << debug_str << "()" << std::endl;
#endif // DEBUG

    length = len;
}

double Line_class::get_length(void)
{
#ifdef DEBUG && 1
    debug_str = "get_length";
    std::clog << "---LOG: running: " << debug_str << "()" << std::endl;
#endif // DEBUG
    return length;
}

Line_class::Line_class(double len): length(len)
{
#ifdef DEBUG && 1
    debug_str = "Line_class";
    std::clog << "---LOG: running: " << debug_str << "()" << "len = " << length << std::endl;
#endif // DEBUG
    //   length = len;      //用初始化字段列表实现了
}

Line_class::~Line_class()
{
#ifdef DEBUG && 1
    debug_str = "~Line_class";
    std::clog << "---LOG: running: " << debug_str << "()"  <<"len = " <<length  << std::endl;
#endif
}

Line_class::Line_class(const Line_class& obj)
{
#ifdef DEBUG && 1
    debug_str = "Line_class(copy)";              //由于没有初始化列表，这里len还没有获取到值
    std::clog << "---LOG: running: " << debug_str << "()" << "len = " << length << std::endl;
#endif // DEBUG

    length = obj.length + 1;
}

double visit_by_friend(Line_class temp)
{
#ifdef DEBUG && 1
    debug_str = "visit_by_friend";
    std::clog << "---LOG: running: " << debug_str << "()" << std::endl;
#endif // DEBUG
    return temp.length;
}

const int* const_and_fun_1(void)
{
#ifdef DEBUG && 1
    debug_str = "const_and_fun_1";
    std::clog << "---LOG: running: " << debug_str << "()" << std::endl;
#endif // DEBUG
    const int* p_data = (++count,&count);     //先自增，然后返回地址
    return p_data;
}

void Line_class::const_and_fun_2(void) const
{
//    length = 2;     //报错说明const起作用了
 //   get_length();      //报错说明即使函数内部没有修改值，但是没有const就认为可能会修改值
    visit_by_friend(*this);     //说明友元函数还是防不住
}


void constructor_test(void)
{
    std::cout << "尝试创建并初始化对象：" << std::endl;

    Line_class line_1(5.1), line_2(3.4);

    std::cout << "线1信息：长度 = " << line_1.get_length() << std::endl;
    std::cout << "线2信息：长度 = " << line_2.get_length() << std::endl;

    //通过构造函数和析构函数打印的信息可以推断出对象存储在栈内
    //遵循先创建的后取出来销毁的规律
}

void copy_constructor_test(void)
{
    std::cout << "尝试通过拷贝构造创建对象" << std::endl;

    //既可以用函数原型，也可以用等号运算符重载
    Line_class line_1(1.1), line_2(line_1), line_3 = line_2;        
    
    std::cout << "线1信息：长度 = " << line_1.get_length() << std::endl;
    std::cout << "线2信息：长度 = " << line_2.get_length() << std::endl;
    std::cout << "线3信息：长度 = " << line_3.get_length() << std::endl;
}

void friend_fun_test(void)
{
    Line_class line_test(11.12);
    std::cout << "通过友元函数访问私有成员: length = " << visit_by_friend(line_test) << std::endl;

}

void const_and_pointer_test(void)
{
    int test_1 = 1112 , test_2 = 211;

    const int* p_test_1;
    p_test_1 = &test_1;
 //   *p_test_1 = 10;       //报错说明*p不可以修改
    p_test_1 = &test_2;

    int const* p_test_2;
    p_test_2 = &test_1;
    p_test_2 = &test_2;
 //   *p_test_2 = 12;       //报错说明*p不可以修改

 //   int* const p_test_3;  //报错说明p不可修改
    int* const p_test_3 = &test_1;
 //   p_test_3 = &test_2;   //报错说明p不可修改

}

void const_and_fun_test_1(void)
{
    const int* p;
    p = const_and_fun_1();
    std::cout << "p = " << p << "\t*p = " << *p << std::endl;
    p = const_and_fun_1();
    std::cout << "p = " << p << "\t*p = " << *p << std::endl;
 //  (*p)++;            //此行报错，说明*p的值不能更改
//    int* p_1 = p;     //此行报错，说明const保护的很好
    const int* p_1 = p; //说明const有传承性

}

void friend_class_test(void)
{
    Line_class line_1(112);
    Line_friend_class friend_1;

    std::cout << "尝试通过友元类访问: length = " << friend_1.friend_class_fun(line_1) << std::endl;
    //根据结果发现个有趣的现象，因为传入的值不是地址，会自动调用拷贝构造函数，然后读取拷贝构造出来的数据
}
