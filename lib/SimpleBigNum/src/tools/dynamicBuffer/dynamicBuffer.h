#pragma once
#include <cstring>
#include "../allocator/iAllocator.h"

namespace sbn
{
namespace tools
{

// Implements linear buffer with dynamic size.
template< typename T >
class DynamicBuffer
{
public:
    // Ctor. Takes instance of IAllocator.
    DynamicBuffer( IAllocator& allocator );

    // Move ctor.
    DynamicBuffer( DynamicBuffer&& other );

    // Copy ctor.
    DynamicBuffer( const DynamicBuffer& other );

    // Dtor. Releases all used resources.
    ~DynamicBuffer();

    // Resizes buffer to have given new size. 
    void Resize( size_t newSize );

    // Grows the buffer if needed and places newElement at the end of the buffer.
    void PushBack( const T& newElement );

    // Getter/setter of element at index idx.
    T& At( size_t idx );
    const T& At( size_t idx ) const;

    // Returns reference to last element.
    T& Back();
    const T& Back() const;

    // Retruns reference to first element.
    T& Front();
    const T& Front() const;

    // Returns current size of the buffer.
    size_t Size() const;

    // Returns current capacity of the buffer.
    size_t Capacity() const;

    // Returns pointer to raw data.
    T* Data();
    const T* Data() const;

    // Returns true if data of other is equal to data of this buffer.
    bool IsEqualTo( const DynamicBuffer< T >& other );

    // Operators:
    DynamicBuffer& operator=( const DynamicBuffer& other );
    DynamicBuffer& operator=( DynamicBuffer&& other );
    // -------

private:
    const size_t MIN_CAPACITY = 16;

    IAllocator& m_allocator;
    T* m_buffer;
    size_t m_currentSize;
    size_t m_currentCapacity;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// IMPLEMENTATION:
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline DynamicBuffer<T>::DynamicBuffer( IAllocator& allocator )
    : m_allocator( allocator )
    , m_currentSize( 0 )
    , m_currentCapacity( MIN_CAPACITY )
{
    m_buffer = static_cast< T* >( allocator.Allocate( m_currentCapacity * sizeof( T ) ) );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline DynamicBuffer<T>::DynamicBuffer( DynamicBuffer&& other )
    : m_allocator( other.m_allocator )
    , m_currentSize( other.m_currentSize )
    , m_currentCapacity( other.m_currentCapacity )
{
    m_buffer = other.m_buffer;
    other.m_currentSize = 0;
    other.m_buffer = nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline DynamicBuffer<T>::DynamicBuffer( const DynamicBuffer& other )
    : m_allocator( other.m_allocator )
    , m_currentSize( other.m_currentSize )
    , m_currentCapacity( other.m_currentCapacity )
{
    m_buffer = m_allocator.Allocate( m_currentCapacity );
    memcpy( m_buffer, other.m_buffer, m_currentCapacity * sizeof( T ) );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline DynamicBuffer<T>::~DynamicBuffer()
{
    m_allocator.Free( m_buffer );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void DynamicBuffer<T>::Resize( size_t newSize )
{
    auto wantedCapacity = m_currentCapacity;

    if( newSize > m_currentCapacity || newSize <= m_currentCapacity / 4 )
    {
        // If current buffer is to small, allocate a new one, with 2x bigger capacity.
        wantedCapacity = std::max< size_t >( MIN_CAPACITY, newSize * 2 );
    }

    if( wantedCapacity != m_currentCapacity )
    {
        // Reallocate the buffer.
        T* newMem = static_cast< T* >( m_allocator.Allocate( wantedCapacity * sizeof( T ) ) );
        auto copySize = wantedCapacity > m_currentCapacity ? m_currentSize : newSize;
        memcpy( newMem, m_buffer, copySize * sizeof( T ) );
        m_allocator.Free( m_buffer );
        m_buffer = newMem;
        m_currentCapacity = wantedCapacity;
        memset( newMem + copySize, 0, ( m_currentCapacity - copySize ) * sizeof( T ) );
    }

    m_currentSize = newSize;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void DynamicBuffer<T>::PushBack( const T& newElement )
{
    Resize( m_currentSize + 1 );
    m_buffer[ m_currentSize - 1 ] = newElement;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T& DynamicBuffer<T>::At( size_t idx )
{
    return m_buffer[ idx ];
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline const T & DynamicBuffer<T>::At( size_t idx ) const
{
    return m_buffer[ idx ];
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T & DynamicBuffer<T>::Back()
{
    return At( m_currentSize - 1 );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline const T & DynamicBuffer<T>::Back() const
{
    return At( m_currentSize - 1 );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T& DynamicBuffer<T>::Front()
{
    return At( 0 );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline const T & DynamicBuffer<T>::Front() const
{
    return At( 0 );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline size_t DynamicBuffer<T>::Size() const
{
    return m_currentSize;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline size_t DynamicBuffer<T>::Capacity() const
{
    return m_currentCapacity;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T* DynamicBuffer<T>::Data()
{
    return m_buffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline const T* DynamicBuffer<T>::Data() const
{
    return m_buffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline bool DynamicBuffer<T>::IsEqualTo( const DynamicBuffer<T>& other )
{
    if( Size() == other.Size() )
        return memcmp( m_buffer, other.m_buffer, m_currentSize * sizeof( T ) ) == 0;

    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline DynamicBuffer<T>& DynamicBuffer<T>::operator=( const DynamicBuffer& other )
{
    *this = DynamicBuffer( other );
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline DynamicBuffer<T>& DynamicBuffer<T>::operator=( DynamicBuffer&& other )
{
    m_allocator = other.m_allocator;
    m_currentSize = other.m_currentSize;
    m_currentCapacity = other.m_currentCapacity;
    m_buffer = other.m_buffer;
    other.m_currentSize = 0;
    other.m_buffer = nullptr;
    return *this;
}

}
}