#pragma once
#include <iostream>
#include <math.h>
#include <vector>

//класс T должен реализовать методы:
// - очистки ресурсов void clear()
// - оператор сравнения ==
// - оператор вывода содержимого в поток ostream& operator<<

template<typename T>
struct Node {
    T t;
    Node* next;
    Node(const T& _t) : t(_t), next(nullptr) {}
};

template<typename T>
class List {
    Node<T>* first;
    Node<T>* last;
    size_t length;

    //Запрещаем поверхностное копирование и присваивание:
    List(const List &);
    List& operator=(const List &);
public:
    List(const std::vector<T> & v = {});  //инициируем список набором объектов
    ~List();

    size_t getLen()const { return length; }

    void push(const T& _t);

    //return false, if list is empty
    bool removeFirst();

    //return false, if list is empty
    bool removeLast();

    //return 0, if ok; 1, if list is empty; 2, if not found
    int remove(const T& _t);

    const Node<T>* find(const T& _t);

    const Node<T>* operator[](const size_t index);

    void print(std::ostream& ,
               const std::string delimiter=",",
               const bool isEndlMidterm = false, //все элементы в одну строку
               const bool isEndl = true)const;  //по завершении перенос
};

template<typename T>
List<T>::List(const std::vector<T> & v) : first(nullptr), last(nullptr), length(0)
{
    for (auto &node: v) push(node);    //for(size_t j=0; j<v.size(); j++){ push(v[j]); }
}

template<typename T>
List<T>::~List()
{
    while(getLen()>0)
        removeLast();
}

template<typename T>
void List<T>::push(const T& _t)
{
    Node<T>* p = new Node<T>(_t);
    if (getLen()==0) {
        first = p;
        last = p;
    }else{
        last->next = p;
        last = p;
    }
    length++;
}

//return false, if list is empty
template<typename T>
bool List<T>::removeFirst()
{
    if (getLen()==0) return false;
    Node<T>* p = first;
    first = p->next;
    p->t.clear();
    delete p;
    length--;
    return true;
}

//return false, if list is empty
template<typename T>
bool List<T>::removeLast()
{
    if (getLen()==0) return false;
    if (getLen()==1) {
        removeFirst();
        return true;
    }
    Node<T>* p = first;
    while (p->next != last) p = p->next;
    p->next = nullptr;
    last->t.clear();
    delete last;
    length--;
    last = p;
    return true;
}

//return 0, if ok; 1, if list is empty; 2, if element not found
template<typename T>
int List<T>::remove(const T& _t)
{
    if (getLen()==0) return 1;
    if (first->t == _t) {
        removeFirst();
        return 0;
    }
    if (last->t == _t) {
        removeLast();
        return 0;
    }
    Node<T> *p0 = first, *p1 = first->next;
    while (p1->t != _t) {
        if(p1 == last) return 2;
        p0 = p1;
        p1 = p1->next;
    }
    p0->next = p1->next;
    p1->t.clear();
    delete p1;
    length--;
    return 0;
}

template<typename T>
const Node<T>* List<T>::find(const T& _t) {
    Node<T>* p = first;
    while (p && p->t != _t){
        p = p->next;
    }
    return p? p : nullptr;  //return (p && p->t.isEqual(_t)) ? p : nullptr;
}

template<typename T>
const Node<T>* List<T>::operator[] (const size_t index)
{
    if (getLen()==0) return nullptr;
    Node<T>* p = first;
    for (size_t i = 0; i < index; i++) {
        p = p->next;
        if (!p) return nullptr;
    }
    return p;
}

template<typename T>
void List<T>::print(std::ostream& s,
                    const std::string delimiter,
                    const bool isEndlMidterm,
                    const bool isEndl)const
{
    if (getLen()==0) return;
    Node<T>* p = first;
    s <<"list content: ";
    while (p) {
        s << p->t;
        if(p != last) s << delimiter;
        if(isEndlMidterm) s<<std::endl;
        p = p->next;
    }
    if(isEndl) s<<std::endl;
}
