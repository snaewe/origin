// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

namespace origin { namespace blas { namespace detail {
template <class T> class allocator {
public:
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;
    
    template <class U> struct rebind {
        typedef allocator<U> other;
    };
    
    allocator() noexcept;
    allocator(const allocator&) noexcept;
    template <class U>
    allocator(const allocator<U>&) noexcept;
    
    ~allocator();
    pointer address(reference x) const noexcept;
    const_pointer address(const_reference x) const noexcept;
    pointer allocate(size_type, allocator<void>::const_pointer hint = 0);
    void deallocate(pointer p, size_type n) noexcept;
    size_type max_size() const noexcept;
    
    template<class U, class... Args>
    void construct(U* p, Args&&... args);
    
    template <class U>
    void destroy(U* p);
};

}}} // end origin::blas::detail