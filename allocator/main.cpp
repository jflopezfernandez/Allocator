
/** The groundwork for a custom dynamic memory allocator in C++
 *
 */

#include <cstddef>
#include <cstdlib>

#include <allocators>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

template <class CharT, class Traits = std::char_traits<CharT>>
std::basic_ostream<CharT, Traits>&
NL(std::basic_ostream<CharT, Traits>& outputStream)
{
    return outputStream << outputStream.widen('\n');
}

// Forward Declaration
template <class T>
class MyAlloc
{
public:
    using value_type = T;

    // Constructors - Nothing to do because the allocator has no state
    MyAlloc() noexcept { }

    template <typename U>
    MyAlloc(const MyAlloc<U>&) noexcept
    {
        // No state to copy
    }

    // Allocate but don't initialize n elements of type T
    T* allocate(std::size_t n)
    {
        // Allocate memory with global `new`
        return static_cast<T *>(::operator new(n * sizeof(T)));
    }

    // Deallocate storage `p` of deleted elements
    void deallocate(T *p, std::size_t n)
    {
        // Deallocate memory with global delete
        ::operator delete(p);
    }

    // construct() and destroy() are not implemented because their default
    // implementations usually work fine.
};

// Return that all specializations of this allocator are interchangeable

template <typename T1, typename T2>
bool operator==(const MyAlloc<T1>&,
                const MyAlloc<T2>&) noexcept
{
    return true;
}

template <typename T1, typename T2>
bool operator!=(const MyAlloc<T1>&,
                const MyAlloc<T2>&) noexcept
{
    return false;
}

// Special string type that uses custom allocator
using MyString = std::basic_string<char, std::char_traits<char>, MyAlloc<char>>;

// Special string/string map type that uses custom allocator
using MyMap = std::map<MyString, MyString, std::less<MyString>, MyAlloc<std::pair<const MyString, MyString>>>;

// Custom allocated vector using an alias template
template <typename T>
using Vec = std::vector<T, MyAlloc<T>>;

int main()
{
    MyString s("Hello, world!");

    MyAlloc<char> charAllocator;
    MyAlloc<int> intAllocator;

    std::cout << std::boolalpha << "char_allocator == int_allocator? " << (charAllocator == intAllocator) << NL;
}