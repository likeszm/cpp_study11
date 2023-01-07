/*
* 名称：类测试2: 
* 
* 收获:
* 1、
* 2、
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
private:
    double length;
};

void constructor_test(void);
void copy_constructor_test(void);

int main()
{
    //constructor_test();
    //copy_constructor_test();


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