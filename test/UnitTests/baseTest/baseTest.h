#pragma once

#include <random>
#include <limits>

template< typename T>
class BaseTestWithRandomGenerator : public ::testing::Test
{
public:
    // Ctor
    BaseTestWithRandomGenerator( T minRandomVal, T maxRandomVal );

    // Returns next random number:
    T GetNextRandomNumber();

private:
    std::default_random_engine m_engine;
    std::uniform_int_distribution<T> m_random;
};

///////////////////////////////////////////////////////////////////////////
//
// IMPLEMENTATION:
//
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
template< typename T >
inline BaseTestWithRandomGenerator< T >::BaseTestWithRandomGenerator( T minRandomVal, T maxRandomVal )
    : m_engine( 954253 )
    , m_random( minRandomVal, maxRandomVal )
{
}

///////////////////////////////////////////////////////////////////////////
template<typename T>
inline T BaseTestWithRandomGenerator<T>::GetNextRandomNumber()
{
    return m_random( m_engine );
}
