#include <iostream>
#include <vector>
#include <memory>
#include <map>

template<typename T, std::size_t MaxItems>
class CustomAllocator {
public:

    using value_type = T;
    CustomAllocator() = default;
    template<typename U, std::size_t S>
    constexpr CustomAllocator(const CustomAllocator<U, S>&) noexcept {}

    template<class Other>
        struct rebind { using other =  CustomAllocator<Other, MaxItems>; };

    T* allocate(std::size_t n) {
        // std::cout << "Allocating " << n * sizeof(T) << " bytes" << std::endl;
        if (n + size > MaxItems) {
            // std::cout << "POOTIS\n";
            throw std::bad_alloc();
        }
        size += n;
        // std::cout << "NEW ALLOCATION "  << n << "\nCUR SIZE " << size << std::endl;
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }
    void deallocate(T* p, std::size_t n) noexcept {
        // std::cout << "Deallocating memory" << std::endl;
        ::operator delete(p);
        size -= n;
    }
    template<typename U, typename... Args>
    void construct(U* p, Args&&... args) {
        // std::cout << "Constructing element" << std::endl;
        new(p) U(std::forward<Args>(args)...);
    }
    template<typename U>
    void destroy(U* p) noexcept {
        // std::cout << "Destroying element" << std::endl;
        p->~U();
    }
    friend bool operator==(const CustomAllocator&, const CustomAllocator&) { return true; }
    friend bool operator!=(const CustomAllocator&, const CustomAllocator&) { return false; }
private:
    std::size_t size = 0;
};

template<typename T, typename A = std::allocator<T>>
class CustomList
{
public:
    // using value_type = T;
    typedef T value_type;
    struct Node
    {
        typedef T value_type;
        Node* next = nullptr;
        T val;
    };
    typename A::template rebind<Node>::other nodeAlloc;
    CustomList()
    {
        _begin = _end = nullptr;
    }

    void push_back(const T& val)
    {
        // typename A::template rebind<Node>::other nodeAlloc;
        // Node* newNode = std::allocator_traits<Node>::allocate(nodeAlloc, 1);
        Node* newNode = nodeAlloc.allocate(1);
        if(_size == 0)
        {
            _begin = _end = newNode;
        }
        else
        {
            _end->next = newNode;
            _end = newNode;
        }
        // std::allocator_traits<Node>::construct(newNode, Node());
        nodeAlloc.construct(newNode);
        _end->val = val;
        _end->next = nullptr;
        _size++;
    }

    Node* get_begin()
    {
        return _begin;
    }

    Node* get_end()
    {
        return _end;
    }
private:
    Node* _begin;
    Node* _end;
    std::size_t _size = 0;
    
    A alloc;
};

int fact(int x)
{
    if(x < 1)
        return 1;
    int b = 1;
    for (int i = 1; i <= x; i++)
       b *= i;
    return b;
    
}

int main()
{
    int fact_arr[10];
    std::map<int, int> m1;
    for (int i = 0; i < 10; i++)
    {
        fact_arr[i] = fact(i);
        m1[i] = fact_arr[i];
    }
    // for (int i = 0; i < 10; i++)
    // {
    //     std::cout << m1[i] << std::endl;
    // }

    std::map<int, int, std::less<int>, CustomAllocator<int, 10> > m2;
    for (int i = 0; i < 10; i++)
    {
        m2[i] = fact_arr[i];
    }
    for (auto item : m2)
    {
        std::cout << item.first << " " << item.second << "\n";
    }
    
    CustomList<int> lst1;
    for (int i = 0; i < 10; i++)
    {
        lst1.push_back(fact_arr[i]);
    }
    // std::cout << "CustomList<int, std::allocator<int>>\n";
    // for (auto item = lst1.get_begin(); item != nullptr; item = item->next)
    // {
    //     std::cout << item->val << std::endl;
    // }
    
    CustomList<int, CustomAllocator<int, 10>> lst2;
    for (int i = 0; i < 10; i++)
    {
        lst2.push_back(fact_arr[i]);
    }
    // std::cout << "CustomList<int, CustomAllocator<int, 10>>\n";
    for (auto item = lst2.get_begin(); item != nullptr; item = item->next)
    {
        std::cout << item->val << std::endl;
    }

    return 0;
}
