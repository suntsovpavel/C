#include "List.h"

using namespace std;

//Список не годится для стандартных типов, для double приходится писать свой класс
class MyDouble {
    double d;
public:
    double get()const { return d; }
    MyDouble(const double _d) : d(_d){}
    void clear(){}
    bool operator==(const MyDouble& other){ return fabs(d-other.d) < 1.e-7; }
    bool operator!=(const MyDouble& other){ return !(*this==other); }
    friend std::ostream& operator<< (std::ostream &out, const MyDouble &);
};
std::ostream& operator<< (std::ostream &out, const MyDouble & obj){
    out << obj.d;
    return out;
}

class Cell {
    int x, y;
public:
    int get_x()const { return x; }
    int get_y()const { return y; }
    Cell(const int _x, const int _y) :x(_x), y(_y){}
    void clear(){}
    bool operator==(const Cell& other){ return x==other.x && y==other.y; }
    bool operator!=(const Cell& other){ return !(*this==other); }
    friend std::ostream& operator<< (std::ostream &out, const Cell &);
};
std::ostream& operator<< (std::ostream &out, const Cell & obj){
    out << "(" << obj.x << "," << obj.y << ")";
    return out;
}

int main()
{
    //1. using class MyDouble
    {
        cout << "--- test class MyDouble ---"<<endl;
        List<MyDouble> list({3, 12.3, 14.});
        list.push(list[list.getLen()-1]->t.get() + list[list.getLen()-2]->t.get());
        list.print(cout);

        double value = 12.3;        
        cout << "try remove "<<value<<" ..."<<endl;
        switch(list.remove(value)){
        case 0: cout << "element "<<value<<" is removed"<<endl; break;
        case 1: cout << "list is empty"<<endl;   break;
        case 2: cout << "element "<<value<<" not found"<<endl;
        }

        value = 123.;
        cout << "try remove "<<value<<" ..."<<endl;
        switch(list.remove(value)){
        case 0: cout << "element "<<value<<" is removed"<<endl; break;
        case 1: cout << "list is empty"<<endl;   break;
        case 2: cout << "element "<<value<<" not found"<<endl;
        }

        list.push(value); cout << "push_back value "<<value<<endl;
        list.print(cout);

        cout << "try remove first..."<<endl;
        if(!list.removeFirst()) cout << "list is empty, nothing to remove"<<endl;
        else cout << "first element is removed"<<endl;
        list.print(cout);

        cout << "try remove last..."<<endl;
        if(!list.removeLast()) cout << "list is empty, nothing to remove"<<endl;
        else cout << "last element is removed"<<endl;
        list.print(cout);

        cout << "try get [0]..."<<endl;
        const Node<MyDouble>* item = list[0];
        if (item == nullptr) cout << "returned nullptr"<<endl;
        else cout << "list[0] = "<<item->t<<endl;
    }

    //2. using class Cell
    {
        cout << "--- test class Cell ---"<<endl;
        List<Cell> list({Cell(2,3), Cell(4,5)});
        list.print(cout);

        cout << "try get [0]..."<<endl;
        const Node<Cell>* item = list[0];
        if (item == nullptr) cout << "returned nullptr"<<endl;
        else cout << "list[0] = "<<item->t<<endl;
        {
            Cell cell(2,3);
            cout <<"try find element "<<cell<<endl;
            const Node<Cell>* elem = list.find(cell);
            if (elem == nullptr) cout << "element "<<cell<<" is not found"<<endl;
            else  cout << "element "<<cell<<" is found"<<endl;
        }
        {
            Cell cell(6,7);
            cout <<"try find element "<<cell<<endl;
            const Node<Cell>* elem = list.find(cell);
            if (elem == nullptr) cout << "element "<<cell<<" is not found"<<endl;
            else  cout << "element "<<cell<<" is found"<<endl;
        }
    }
    cout << "ok"<<endl;
    return 0;
}


/*
// https://habr.com/ru/articles/205772/, see comments...
struct foo_detector
{
    static auto check(void*) -> void; // раз уж С++11, напишем в постфиксной форме - так красивее и однообразнее :)
    template<class T>
    static auto check(T* p) -> decltype(p->foo(42))*; // указатель на чего бы то ни было отличается от не-указателя

    typedef void* match_type; // но мы ожидаем конкретно void foo(int-compatible), поэтому проверяем на void*
};

// менее громоздкий способ писать метафункции - это наследоваться от готовых
template<class T> struct has_foo : std::is_same<foo_detector::match_type, decltype(foo_detector::check((T*)nullptr))> {};


struct P { void foo(int); };
struct Q0 { };
struct Q1 { void foo(void); };
struct Q2 { int  foo(void); };
struct Q3 { int  foo(int ); };

int main()
{
    std::cout << has_foo<P >::value << std::endl;
    std::cout << has_foo<Q0>::value << std::endl;
    std::cout << has_foo<Q1>::value << std::endl;
    std::cout << has_foo<Q2>::value << std::endl;
    std::cout << has_foo<Q3>::value << std::endl;
}
*/
