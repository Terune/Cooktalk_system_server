#pragma once
#include <cstddef>
template <typename Object>
class Stack
{
protected:
    // 리스트의 노드
    //
    struct Node
    {
        Object obj;
        Node* next;
    };
    typedef Node* NodePtr;
 
public:
    Stack();
    virtual ~Stack();
 
    void push(const Object& o);
    void pop();
    Object& top();
    const Object top() const;
 
    int size() const;
    bool empty() const;
 
private:
    NodePtr top_;
    int size_;
};


// 생성자(): top_과 size_를 초기화한다.
//
template <typename Object>
Stack<Object>::Stack() : top_(NULL), size_(0) {}
 
// 소멸자(): 남은 원소가 없을 때까지 pop한다. (메모리 누수 방지)
//
template <typename Object>
Stack<Object>::~Stack()
{
    while (size_ > 0)
        pop();
}
 
// push(): 입력받은 데이터를 노드로 만들어 스택에 넣는다.
//
template <typename Object>
void Stack<Object>::push(const Object& o)
{
    NodePtr tmp = new Node;
    tmp->obj = o;
    tmp->next = empty()? NULL : top_;
 
    top_ = tmp;
    size_++;
}
 
// pop(): 맨 위의 원소를 리스트에서 제거하고 힙에 할당한 메모리를 해제한다.
//
template <typename Object>
void Stack<Object>::pop()
{
    NodePtr tmp = top_;
 
    top_ = top_->next;
    size_--;
 
    delete tmp;
}
 
// top(): 맨 위의 원소를 반환한다.
//
template <typename Object>
Object& Stack<Object>::top()
{
    return top_->obj;
}
 
// top(): 맨 위의 원소를 상수 객체로 반환한다.

template <typename Object>
const Object Stack<Object>::top() const
{
    return top_->obj;
}
 
// size(): 스택에 저장된 데이터의 개수를 반환한다.
//
template <typename Object>
int Stack<Object>::size() const
{
    return size_;
}
 
// empty(): 스택이 비었으면 true, 아니면 false를 반환한다.
//
template <typename Object>
bool Stack<Object>::empty() const
{
    return (size_ == 0);
}

