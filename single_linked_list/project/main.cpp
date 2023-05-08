#include <iostream>
#include <math.h>
using namespace std;

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


//класс T должен реализовать методы:
// - очистки ресурсов void clear()
// - сравнения bool isEqual(const T&)
// - оператор вывода содержимого в поток ostream& operator<<

template<typename T>
struct Node {
    T t;
    Node* next;
    Node(const T& _t) : t(_t), next(nullptr) {}
};

template<typename T>
class list {
    Node<T>* first;
    Node<T>* last;
public:
    list() : first(nullptr), last(nullptr) {}
    ~list(){
        while(!is_empty())
            remove_last();
    }

    bool is_empty() {
        return first == nullptr;
    }

    void push_back(const T& _t) {
        Node<T>* p = new Node<T>(_t);
        if (is_empty()) {
            first = p;
            last = p;
            return;
        }
        last->next = p;
        last = p;
    }

    void print(const string delimiter=",",
               const bool isEndl = false) {
        if (is_empty()) return;
        Node<T>* p = first;
        cout <<"list content: ";
        while (p) {
            cout << p->t;
            if(p != last) cout << delimiter;
            if(isEndl) cout<<endl;
            p = p->next;
        }
        cout << endl;
    }

    const Node<T>* find(const T& _t) {
        Node<T>* p = first;
        while (p && !p->t.isEqual(_t)){
            p = p->next;
        }
        return p? p : nullptr;  //return (p && p->t.isEqual(_t)) ? p : nullptr;
    }

    //return false, if list is empty
    bool remove_first() {
        if (is_empty()) return false;
        Node<T>* p = first;
        first = p->next;
        p->t.clear();
        delete p;
        return true;
    }

    //return false, if list is empty
    bool remove_last() {
        if (is_empty()) return false;
        if (first == last) {
            remove_first();
            return true;
        }
        Node<T>* p = first;
        while (p->next != last) p = p->next;
        p->next = nullptr;
        last->t.clear();
        delete last;
        last = p;
        return true;
    }

    //0: Элемент найден и удален
    //1: Список пуст
    //2: элемент не найден
    int remove(const T& _t) {
        if (is_empty()) return 1;
        if (first->t.isEqual(_t)) {
            remove_first();
            return 0;
        }
        if (last->t.isEqual(_t)) {
            remove_last();
            return 0;
        }
        Node<T>* slow = first;
        Node<T>* fast = first->next;
        while (fast && !fast->t.isEqual(_t)) {
            fast = fast->next;
            slow = slow->next;
        }
        if (!fast) return 2;
        slow->next = fast->next;
        fast->t.clear();
        delete fast;
        return 0;
    }

    const Node<T>* operator[] (const size_t index) {
        if (is_empty()) return nullptr;
        Node<T>* p = first;
        for (size_t i = 0; i < index; i++) {
            p = p->next;
            if (!p) return nullptr;
        }
        return p;
    }
};


class MyDouble {
    double d;
public:
    double get()const { return d; }
    MyDouble(const double _d) : d(_d){}
    void clear(){}
    bool isEqual(const MyDouble& other){ return fabs(d-other.d) < 1.e-7; }
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
    bool isEqual(const Cell& other){ return x==other.x && y==other.y; }
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
        list<MyDouble> listDbl;
        cout << "is empty: " << listDbl.is_empty() << endl;
        listDbl.push_back(3); cout << "push_back value 3"<<endl;
        listDbl.push_back(12.3); cout << "push_back value 12.3"<<endl;
        listDbl.push_back(listDbl[0]->t.get() + listDbl[1]->t.get()); cout << "push_back sum previous two"<<endl;
        listDbl.print();
        cout << "is empty: " << listDbl.is_empty() << endl;
        double value = 12.3;
        cout << "try remove "<<value<<" ..."<<endl;
        int flag = listDbl.remove(value);
        if(flag==1) cout << "list is empty"<<endl;
        else if(flag==2) cout << "element "<<value<<" not found"<<endl;
        else cout << "element "<<value<<" is removed"<<endl;
        value = 123.;
        flag = listDbl.remove(value);
        if(flag==1) cout << "list is empty"<<endl;
        else if(flag==2) cout << "element "<<value<<" not found"<<endl;
        else cout << "element "<<value<<" is removed"<<endl;
        listDbl.print();
        value = 123.;
        listDbl.push_back(123.); cout << "push_back value "<<value<<endl;
        cout << "try remove first..."<<endl;
        if(!listDbl.remove_first()) cout << "list is empty, nothing to remove"<<endl;
        else cout << "first element is removed"<<endl;
        listDbl.print();
        cout << "try remove last..."<<endl;
        if(!listDbl.remove_last()) cout << "list is empty, nothing to remove"<<endl;
        else cout << "last element is removed"<<endl;
        listDbl.print();
        cout << "try get [0]..."<<endl;
        const Node<MyDouble>* item = listDbl[0];
        if (item == nullptr) cout << "returned nullptr"<<endl;
        else cout << "list[0] = "<<item->t<<endl;
    }

    //2. using class Cell
    {
        list<Cell> listCell;
        listCell.push_back(Cell(2,3)); cout << "push_back Cell(2,3)"<<endl;
        listCell.push_back(Cell(4,5)); cout << "push_back Cell(4,5)"<<endl;
        listCell.print();
        cout << "try get [0]..."<<endl;
        const Node<Cell>* item = listCell[0];
        if (item == nullptr) cout << "returned nullptr"<<endl;
        else cout << "list[0] = "<<item->t<<endl;
        {
            Cell cell(2,3);
            cout <<"try find element "<<cell<<endl;
            const Node<Cell>* finded = listCell.find(cell);
            if (finded == nullptr) cout << "element "<<cell<<" is not found"<<endl;
            else  cout << "element "<<cell<<" is found"<<endl;
        }
        {
            Cell cell(6,7);
            cout <<"try find element "<<cell<<endl;
            const Node<Cell>* finded = listCell.find(cell);
            if (finded == nullptr) cout << "element "<<cell<<" is not found"<<endl;
            else  cout << "element "<<cell<<" is found"<<endl;
        }
    }
    cout << "ok"<<endl;
    return 0;
}
