#include <iostream>
using namespace std;

class Shared_Count
{
private:
    size_t count;//引用计数
public:
    Shared_Count()noexcept :count(1) {}
    void addCount() noexcept
    {
        ++count;
    }
    bool reduceCount()noexcept
    {
        if (count == 0)return true;
        --count;
        return count == 0;
    }
    size_t getCount()const noexcept { return count; }
};
template<typename T>
class MyShared_Ptr
{
private:

    Shared_Count* count;
    T* ptr;
    //用于转换
    template<typename U>
    MyShared_Ptr(const MyShared_Ptr<U>& that, T* t)noexcept
    {
        ptr = t;
        if (ptr)
        {
            that.count->addCount();
            count = that.count;
        }
    }
public:
    //构造函数
    explicit MyShared_Ptr(T* p = nullptr) noexcept :ptr(p),count(nullptr)
    {
        if (this->ptr)
        {
            count = new Shared_Count();
        }
    }
    //析构函数
    ~MyShared_Ptr()
    {
        if (ptr && count->reduceCount())
        {
            delete ptr;delete count;
        }
        ptr = nullptr;
        count = nullptr;
    }
    //拷贝构造拷贝赋值
    MyShared_Ptr(const MyShared_Ptr& that)noexcept
    {
        this->count = that.count;
        count->addCount();
        this->ptr = that.ptr;
    }
    MyShared_Ptr& operator=(const MyShared_Ptr& that)
    {
        if (ptr && count->reduceCount())
        {
            delete ptr;delete count;
        }
        this->count = that.count;
        count->addCount();
        this->ptr = that.ptr;
    }
    //移动构造移动赋值
    MyShared_Ptr(MyShared_Ptr&& that)
    {
        ptr = that.ptr;
        count = that.count;
        that.ptr = nullptr;
    }
    MyShared_Ptr& operator=(MyShared_Ptr&& that)
    {
        ptr = that.ptr;
        count = that.count;
        that.ptr = nullptr;
    }
    //返回引用计数
    size_t getCount()const noexcept
    {
        if (ptr)return count->getCount();
        return 0;
    }
    //指针操作
    T& operator *()const noexcept { return *ptr; }
    T* operator ->()const noexcept { return ptr; }
    operator bool()const noexcept { return ptr != nullptr; }
    //成员函数
    void reset(T* that)noexcept
    {
        if (ptr && count->reduceCount())
        {
            delete ptr;delete count;
        }
        ptr = that;
        count = new Shared_Count();
    }
    T* release()noexcept
    {
        T* t = ptr;
        ptr = nullptr;
        return t;
    }
    T* get()const noexcept { return ptr; }
    void swap(MyShared_Ptr& that)noexcept
    {
        swap(that.count, count);
        swap(that.ptr, ptr);
    }
    //友元函数
    template<typename U>
    friend void swap(MyShared_Ptr<U>& lt, MyShared_Ptr<U>& rt)noexcept;
    template <typename TT, typename U>
    friend MyShared_Ptr<TT> static_pointer_cast(const MyShared_Ptr<U>& that) noexcept;
    template <typename TT, typename U>
    friend MyShared_Ptr<TT> reinterpret_pointer_cast(const MyShared_Ptr<U>& that) noexcept;
    template <typename TT, typename U>
    friend MyShared_Ptr<TT> const_pointer_cast(const MyShared_Ptr<U>& that) noexcept;
    template <typename TT, typename U>
    friend MyShared_Ptr<TT> dynamic_pointer_cast(const MyShared_Ptr<U>& that) noexcept;
};

//显示偏特化
template<typename T>
class MyShared_Ptr<T[]>
{
private:

    Shared_Count* count;
    T* ptr;

public:
    //构造函数
    explicit MyShared_Ptr(T* p = nullptr) noexcept :ptr(p),count(nullptr)
    {
        if (this->ptr)
        {
            count = new Shared_Count();
        }
    }
    //析构函数
    ~MyShared_Ptr()
    {
        if (ptr && count->reduceCount())
        {
            delete ptr;delete count;
        }
        ptr = nullptr;
        count = nullptr;
    }
    //拷贝构造拷贝赋值
    MyShared_Ptr(const MyShared_Ptr& that)noexcept
    {
        this->count = that.count;
        count->addCount();
        this->ptr = that.ptr;
    }
    MyShared_Ptr& operator=(const MyShared_Ptr& that)
    {
        if (ptr && count->reduceCount())
        {
            delete ptr;delete count;
        }
        this->count = that.count;
        count->addCount();
        this->ptr = that.ptr;
    }
    //移动构造移动赋值
    MyShared_Ptr(MyShared_Ptr&& that)
    {
        ptr = that.ptr;
        count = that.count;
        that.ptr = nullptr;
    }
    MyShared_Ptr& operator=(MyShared_Ptr&& that)
    {
        ptr = that.ptr;
        count = that.count;
        that.ptr = nullptr;
    }
    //返回引用计数
    size_t getCount()const noexcept
    {
        if (ptr)return count->getCount();
        return 0;
    }
    //指针操作
    T& operator[](size_t t)noexcept
    {
        return ptr[t];
    }
    operator bool()const noexcept { return ptr != nullptr; }
    //成员函数
    void reset(T* that)noexcept
    {
        if (ptr && count->reduceCount())
        {
            delete ptr;delete count;
        }
        ptr = that;
        count = new Shared_Count();
    }
    T* release()noexcept
    {
        T* t = ptr;
        ptr = nullptr;
        return t;
    }
    T* get()const noexcept { return ptr; }
    void swap(MyShared_Ptr& that)noexcept
    {
        swap(that.count, count);
        swap(that.ptr, ptr);
    }
    //友元函数
    template<typename U>
    friend void swap(MyShared_Ptr<U>& lt, MyShared_Ptr<U>& rt)noexcept;

};
template<typename U>
void swap(MyShared_Ptr<U>& lt, MyShared_Ptr<U>& rt)noexcept;

template <typename T, typename U>
MyShared_Ptr<T> static_pointer_cast(const MyShared_Ptr<U>& that) noexcept
{
    T* t = static_cast<T*>(that.get());
    return MyShared_Ptr<T>(that, t);
}

template <typename T, typename U>
MyShared_Ptr<T> reinterpret_pointer_cast(const MyShared_Ptr<U>& that) noexcept
{
    T* t = reinterpret_cast<T*>(that.get());
    return MyShared_Ptr<T>(that, t);
}

template <typename T, typename U>
MyShared_Ptr<T> const_pointer_cast(const MyShared_Ptr<U>& that) noexcept
{
    T* t = const_cast<T*>(that.get());
    return MyShared_Ptr<T>(that, t);
}

template <typename T, typename U>
MyShared_Ptr<T> dynamic_pointer_cast(const MyShared_Ptr<U>& that) noexcept
{
    T* t = dynamic_cast<T*>(that.get());
    return MyShared_Ptr<T>(that, t);
}

int main()
{
    MyShared_Ptr<int> my(new int(10));
    cout << *my << endl;
    cout << "Hello World!\n";
}


