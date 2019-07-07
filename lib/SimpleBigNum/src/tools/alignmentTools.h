#pragma once
#include <cstdint>
#include <type_traits>

namespace sbn
{
namespace tools
{

// Returns true if given numeric value is power of 2.
template< typename T > constexpr bool isPowerOfTwo( T value );

// Returns true if given value is aligned to given Alignment. Alignment has to be power of 2.
template < typename T, size_t Alignment > constexpr bool isAligned( T value );

// Returns aligned value to given Alignment. Alignment has to be power of 2.
template < typename T, size_t Alignment > constexpr T align( T value );

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// IMPLEMENTATION:
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template< typename T > constexpr bool isPowerOfTwo( T value )
{
    static_assert( std::is_integral<T>::value, "isPowerOfTwo works only for numeric values." );
    return ( value & ( value - 1 ) ) == 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template < typename T, size_t Alignment > constexpr bool isAligned( T value )
{
    static_assert( isPowerOfTwo( Alignment ), "Alignment have to be power of 2." );
    return ( ( ( size_t )value ) & ( Alignment - 1 ) ) == 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template < typename T, size_t Alignment > constexpr T align( T value )
{
    return isAligned<T, Alignment>( value ) ? value : ( T )( ( ( size_t )value + Alignment ) & ~( Alignment - 1 ) );
}

}
}