#include "../include/bigNum.h"
#include <algorithm>
#include "arithmeticImpl/arithmeticImpl.h"
#include "reciprocalEstimator/reciprocalEstimator.h"

namespace sbn
{
namespace helpers
{

constexpr static uint32_t KARATSUBA_THRESHOLD = 1000;

// Helper uninon for converting number binary number to number base 256.
union BigNumUnion_u64
{
    uint64_t m_uint64_t;
    uint8_t m_uint8_t[ 8 ];
};

}

////////////////////////////////////////////////////////////////////////////////////////////////////
SimpleBigNum::SimpleBigNum()
{
    SetZero();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
SimpleBigNum::SimpleBigNum( uint64_t number )
{
    helpers::BigNumUnion_u64 num;
    num.m_uint64_t = number;

    // Code will work on little endian systems only:
    m_numberLittleEndian.reserve( 8 );
    m_numberLittleEndian.push_back( num.m_uint8_t[ 0 ] );
    m_numberLittleEndian.push_back( num.m_uint8_t[ 1 ] );
    m_numberLittleEndian.push_back( num.m_uint8_t[ 2 ] );
    m_numberLittleEndian.push_back( num.m_uint8_t[ 3 ] );
    m_numberLittleEndian.push_back( num.m_uint8_t[ 4 ] );
    m_numberLittleEndian.push_back( num.m_uint8_t[ 5 ] );
    m_numberLittleEndian.push_back( num.m_uint8_t[ 6 ] );
    m_numberLittleEndian.push_back( num.m_uint8_t[ 7 ] );

    RemoveLeadingZeros();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
SimpleBigNum::SimpleBigNum( TRawNumberDigits::const_iterator digitsBegin, TRawNumberDigits::const_iterator digitsEnd )
{
    m_numberLittleEndian.assign( digitsBegin, digitsEnd );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void SimpleBigNum::Add( const SimpleBigNum& other )
{
    const uint32_t otherDigits = other.GetNumberOfDigits();
    m_numberLittleEndian.resize( std::max( otherDigits, GetNumberOfDigits() ) + 1 );
    sbn::internal::AddInplaceImpl( m_numberLittleEndian.data(), other.m_numberLittleEndian.data(), otherDigits );
    RemoveLeadingZeros();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void SimpleBigNum::Subtruct( const SimpleBigNum& other )
{
    if( other.IsGreaterThen( *this ) )
    {
        SetZero();
        return;
    }

    const uint32_t otherDigits = other.GetNumberOfDigits();
    sbn::internal::SustructInplaceImpl( m_numberLittleEndian.data(), other.m_numberLittleEndian.data(), otherDigits );
    RemoveLeadingZeros();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void SimpleBigNum::Multiply( const SimpleBigNum& other )
{
    if( IsZero() || other.IsZero() )
    {
        SetZero();
        return;
    }

    MultiplyImpl_Karatsuba( other );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void SimpleBigNum::Divide( const SimpleBigNum& other )
{
    if( IsEqualTo( other ) )
    {
        SetOne();
        return;
    }

    if( IsLessThen( other ) )
    {
        SetZero();
        return;
    }

    const auto shift = GetNumberOfDigits() + GetNumberOfDigits()/4;
    const auto reciprocal = internal::ReciprocalEstimator::Estimate( other, shift, 100 );

    Multiply( reciprocal );
    ShitfRight( shift );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void SimpleBigNum::ShitfLeft( uint32_t value )
{
    const auto orginalDigits = GetNumberOfDigits();
    m_numberLittleEndian.resize( orginalDigits + value );
    memmove( m_numberLittleEndian.data() + value, m_numberLittleEndian.data(), orginalDigits * sizeof( TRawNumberDigits::value_type ) );
    memset( m_numberLittleEndian.data(), 0, value * sizeof( TRawNumberDigits::value_type ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void SimpleBigNum::ShitfRight( uint32_t value )
{
    const auto orginalDigits = GetNumberOfDigits();
    if( value >= orginalDigits )
    {
        SetZero();
        return;
    }

    const auto digitsAfterShift = orginalDigits - value;
    memmove( m_numberLittleEndian.data(), m_numberLittleEndian.data() + value, digitsAfterShift * sizeof( TRawNumberDigits::value_type ) );
    m_numberLittleEndian.resize( digitsAfterShift );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
#define COMPARE_IMPL( op )                                                                              \
    if( other.GetNumberOfDigits() == GetNumberOfDigits() )                                              \
    {                                                                                                   \
        uint32_t i = GetNumberOfDigits() - 1;                                                           \
        while( i > 0 && ( m_numberLittleEndian[ i ] == other.m_numberLittleEndian[ i ] ) ) --i;         \
        return  m_numberLittleEndian[ i ] op other.m_numberLittleEndian[ i ];                           \
    }                                                                                                   \
    return  GetNumberOfDigits() op other.GetNumberOfDigits();

////////////////////////////////////////////////////////////////////////////////////////////////////
bool SimpleBigNum::IsGreaterThen( const SimpleBigNum& other ) const
{
    COMPARE_IMPL( > );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool SimpleBigNum::IsLessThen( const SimpleBigNum & other ) const
{
    COMPARE_IMPL( < );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool SimpleBigNum::IsEqualTo( const SimpleBigNum& other ) const
{
    return m_numberLittleEndian == other.m_numberLittleEndian;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
#define IS_NUMBER_EQUAL_TO( digit )                                                     \
    if( ( GetNumberOfDigits() == 1 ) && ( m_numberLittleEndian.back() == digit ) )      \
        return true;                                                                    \
    return false;

////////////////////////////////////////////////////////////////////////////////////////////////////
bool SimpleBigNum::IsZero() const
{
    IS_NUMBER_EQUAL_TO( 0 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool SimpleBigNum::IsOne() const
{
    IS_NUMBER_EQUAL_TO( 1 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
#define SET_NUMBER_TO( digit )                  \
       m_numberLittleEndian.resize( 1 );        \
       m_numberLittleEndian.back() = digit;     \

////////////////////////////////////////////////////////////////////////////////////////////////////
void SimpleBigNum::SetZero()
{
    SET_NUMBER_TO( 0 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void SimpleBigNum::SetOne()
{
    SET_NUMBER_TO( 1 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t SimpleBigNum::GetNumberOfDigits() const
{
    return ( uint32_t )m_numberLittleEndian.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
std::string SimpleBigNum::ToString( bool addSeparators ) const
{
    std::string outString;
    auto numberBase10 = ConvertNumber( m_numberLittleEndian, 256, 10 );

    for( size_t i = numberBase10.size(); i > 0; --i )
    {
        outString += std::to_string( numberBase10[ i - 1 ] );
        if( addSeparators && ( i - 1 ) % 3 == 0 )
            outString += ",";
    }

    if( addSeparators )
        outString.resize( outString.size() - 1 );
    return outString;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void SimpleBigNum::FromString( const std::string& numberBase10 )
{
    TRawNumberDigits rawNumberBase10;
    rawNumberBase10.reserve( numberBase10.size() );

    for( size_t i = numberBase10.size(); i > 0; --i )
        rawNumberBase10.push_back( numberBase10[ i - 1 ] - '0' );

    m_numberLittleEndian = ConvertNumber( rawNumberBase10, 10, 256 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void SimpleBigNum::RemoveLeadingZeros()
{
    size_t i = m_numberLittleEndian.size() - 1;
    size_t toRemove = 0;

    while( i > 0 && m_numberLittleEndian[ i-- ] == 0 )
        ++toRemove;

    m_numberLittleEndian.resize( m_numberLittleEndian.size() - toRemove );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void SimpleBigNum::MultiplyImpl_Basecase( const SimpleBigNum& other )
{
    // Simple O(n^2) multiplication algorithm:
    const uint32_t thisSize = GetNumberOfDigits();
    const uint32_t otherSize = other.GetNumberOfDigits();
    const uint32_t newSize = thisSize + otherSize;

    TRawNumberDigits newNumber( newSize );
    sbn::internal::MultiplyInplaceImpl( m_numberLittleEndian.data(), thisSize, other.m_numberLittleEndian.data(), otherSize, newNumber.data() );
    std::swap( m_numberLittleEndian, newNumber );
    RemoveLeadingZeros();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void SimpleBigNum::MultiplyImpl_Karatsuba( const SimpleBigNum& other )
{
    // Karatsuba method:
    // thisNumber = ( thisHightPart*B + thisLowPart )
    // otherNumber = ( otherHightPart*B + otherLowPart )
    //
    // z1 = thisHightPart * otherHightPart
    // z2 = thisLowPart * otherLowPart
    // z3 = ( thisHightPart + thisLowPart ) * ( otherHightPart + otherLowPart ) - z1 - z2
    // final = z1*B*B + z3*B + z2

    const SimpleBigNum& bigger = std::max( *this, other );
    const SimpleBigNum& smaller = std::min( *this, other );

    if( bigger.GetNumberOfDigits() < helpers::KARATSUBA_THRESHOLD )
        return MultiplyImpl_Basecase( other );

    const auto exponent = bigger.GetNumberOfDigits() / 2;

    SimpleBigNum z2( bigger.m_numberLittleEndian.begin(), bigger.m_numberLittleEndian.begin() + exponent );
    SimpleBigNum z1( bigger.m_numberLittleEndian.begin() + exponent, bigger.m_numberLittleEndian.end() );

    SimpleBigNum otherLowPart;
    SimpleBigNum otherHighPart;

    if( smaller.GetNumberOfDigits() > exponent )
    {
        otherLowPart = SimpleBigNum( smaller.m_numberLittleEndian.begin(), smaller.m_numberLittleEndian.begin() + exponent );
        otherHighPart = SimpleBigNum( smaller.m_numberLittleEndian.begin() + exponent, smaller.m_numberLittleEndian.end() );
    }
    else
    {
        otherLowPart = SimpleBigNum( smaller.m_numberLittleEndian.begin(), smaller.m_numberLittleEndian.end() );
    }

    SimpleBigNum z3 = z2;
    z3.Add( z1 );

    SimpleBigNum sum2 = otherLowPart;
    sum2.Add( otherHighPart );

    z1.MultiplyImpl_Karatsuba( otherHighPart );
    z2.MultiplyImpl_Karatsuba( otherLowPart );
    z3.MultiplyImpl_Karatsuba( sum2 );
    z3.Subtruct( z1 );
    z3.Subtruct( z2 );

    z1.ShitfLeft( exponent + exponent );
    z3.ShitfLeft( exponent );
    z1.Add( z3 );
    z1.Add( z2 );

    *this = std::move( z1 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<uint8_t> SimpleBigNum::ConvertNumber( const std::vector<uint8_t>& srcNumber, uint16_t srcBase, uint16_t dstBase ) const
{
    std::vector< uint8_t > dstNumber = { 0 };
    dstNumber.reserve( srcNumber.size() );

    // [NOTE]: To convert srcNumber with srcBase to dstNumber with dstBase, function 
    // uses general alogithm of the form:
    // dstValue = 0
    // foreach srcDigit in srcNumber:
    //      dstNumber = dstNumber * srcBase
    //      dstNumber = dstNumber + srcDigit

    const uint16_t dstBaseU16 = dstBase;

    for( size_t i = srcNumber.size(); i > 0; --i )
    {
        // Multiply current dst number by srcBase:
        uint16_t multOverflow = 0;
        for( size_t j = 0; j < dstNumber.size(); ++j )
        {
            uint16_t wantedDstDigit = ( dstNumber[ j ] * srcBase ) + multOverflow;
            uint16_t finalDstDigit = wantedDstDigit % dstBaseU16;
            dstNumber[ j ] = ( uint8_t )finalDstDigit;
            multOverflow = wantedDstDigit / dstBaseU16;
        }

        while( multOverflow != 0 )
        {
            uint8_t dstDigit = ( uint8_t )( multOverflow % dstBaseU16 );
            dstNumber.push_back( dstDigit );
            multOverflow /= dstBaseU16;
        };

        // Add current srcDigit expressed in dstBase:
        size_t dstDigitIdx = 0;
        uint16_t srcDigit = srcNumber[ i - 1 ];
        do
        {
            uint16_t dstDigit = srcDigit % dstBaseU16;
            uint16_t wantedDstDigit = dstDigit + dstNumber[ dstDigitIdx ];

            dstNumber[ dstDigitIdx ] = ( uint8_t )( wantedDstDigit % dstBaseU16 );
            uint16_t dstDigitOverflow = ( wantedDstDigit / dstBaseU16 );
            srcDigit = ( srcDigit / dstBaseU16 );
            srcDigit += dstDigitOverflow;
            ++dstDigitIdx;
        } while( srcDigit != 0 && dstDigitIdx < dstNumber.size() );

        while( srcDigit != 0 )
        {
            uint8_t dstDigit = ( uint8_t )( srcDigit % dstBaseU16 );
            dstNumber.push_back( dstDigit );
            srcDigit = ( srcDigit / dstBaseU16 );
        };
    }
    return dstNumber;
}
}