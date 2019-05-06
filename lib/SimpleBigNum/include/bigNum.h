#pragma once
#include <cstdint>
#include <vector>
#include <string>

namespace sbn
{

// Class represent number of arbitrary size.
class SimpleBigNum
{
public:
    // Raw digits type container.
    using TRawNumberDigits = std::vector< uint8_t >;

    // Ctor. Sets number to 0.
    SimpleBigNum();

    // Ctor. Initilalizes big num from given number.
    SimpleBigNum( uint64_t number );

    // Ctor. Initilalizes big num from given raw number. This just copies given digits into internal structures.
    // The number has to be expressed as number base 256 and stored in little endian format.
    SimpleBigNum( TRawNumberDigits::const_iterator digitsBegin, TRawNumberDigits::const_iterator digitsEnd );

    // Adds inplace other number.
    void Add( const SimpleBigNum& other );

    // Substructs inplace other number.
    // [WARNING]: Since BigNum cannot represent ( yet ) negative numbers, this operation makes only sense
    // when other number is smaller then this number. In the cases where other number is bigger then this one, this one will be set to zero.
    void Substruct( const SimpleBigNum& other );

    // Multiplies inplace by other number.
    void Multiply( const SimpleBigNum& other );

    // Divides inplace by other number.
    void Divide( const SimpleBigNum& other );

    // Shifts left by value. Effectively works as multiplying number by 256^value.
    void ShitfLeft( uint32_t value );

    // Shifts right by value. Effectively works as dividing number by 256^value.
    void ShitfRight( uint32_t value );

    // Returns true of this number is greater then the other one.
    bool IsGreaterThen( const SimpleBigNum& other ) const;

    // Returns true of this number is less then the other one.
    bool IsLessThen( const SimpleBigNum& other ) const;

    // Returns true of this number is greater or equal then the other one.
    bool IsGreaterOrEqualTo( const SimpleBigNum& other ) const;

    // Returns true of this number is less or equal then the other one.
    bool IsLessOrEqualTo( const SimpleBigNum& other ) const;

    // Returns true if other number is equal to current one.
    bool IsEqualTo( const SimpleBigNum& other ) const;

    // Returns if number is equal to zero.
    bool IsZero() const;

    // Returns if number is equal to one.
    bool IsOne() const;

    // Sets number to 0.
    void SetZero();

    // Sets number to 1.
    void SetOne();

    // Returns number of digits.
    uint32_t GetNumberOfDigits() const;

    // Stores number to string.
    std::string ToString( bool addSeparators = false ) const;

    // Initializes number from string numbers, that has to represent decimal number.
    void FromString( const std::string& numberBase10 );

    // --- operators ----------------
    SimpleBigNum& operator+=( const SimpleBigNum& other );
    SimpleBigNum& operator-=( const SimpleBigNum& other );
    SimpleBigNum& operator*=( const SimpleBigNum& other );
    SimpleBigNum& operator/=( const SimpleBigNum& other );
    SimpleBigNum& operator<<( uint32_t value );
    SimpleBigNum& operator>>( uint32_t value );
    bool operator>( const SimpleBigNum& other ) const;
    bool operator<( const SimpleBigNum& other ) const;
    bool operator<=( const SimpleBigNum& other ) const;
    bool operator>=( const SimpleBigNum& other ) const;
    bool operator==( const SimpleBigNum& other ) const;
    bool operator!=( const SimpleBigNum& other ) const;
    // ------------------------------

private:
    // Removes leading zeros.
    void RemoveLeadingZeros();

    // Implementation of multiplication for small numbers.
    void MultiplyImpl_Basecase( const SimpleBigNum& other );

    // Implementation of multiplication using Karatsuba method.
    void MultiplyImpl_Karatsuba( const SimpleBigNum& other );

    // Converts srcNumber with srcBase to new number with dstBase.
    std::vector< uint8_t > ConvertNumber( const std::vector< uint8_t >& srcNumber, uint16_t srcBase, uint16_t dstBase ) const;

    // [NOTE]: number is keeped as little endian with base 256.
    TRawNumberDigits m_numberLittleEndian;
};

////////////////////////////////////////////////////////////////////////
//
// INLINES:
//
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
inline bool SimpleBigNum::IsGreaterOrEqualTo( const SimpleBigNum& other ) const
{
    return !IsLessThen( other );
}

////////////////////////////////////////////////////////////////////////
inline bool SimpleBigNum::IsLessOrEqualTo( const SimpleBigNum& other ) const
{
    return !IsGreaterThen( other );
}

////////////////////////////////////////////////////////////////////////
inline SimpleBigNum& SimpleBigNum::operator+=( const SimpleBigNum& other )
{
    Add( other );
    return *this;
}

////////////////////////////////////////////////////////////////////////
inline SimpleBigNum& SimpleBigNum::operator-=( const SimpleBigNum& other )
{
    Substruct( other );
    return *this;
}

////////////////////////////////////////////////////////////////////////
inline SimpleBigNum& SimpleBigNum::operator*=( const SimpleBigNum& other )
{
    Multiply( other );
    return *this;
}

////////////////////////////////////////////////////////////////////////
inline SimpleBigNum& SimpleBigNum::operator/=( const SimpleBigNum& other )
{
    Divide( other );
    return *this;
}

////////////////////////////////////////////////////////////////////////
inline SimpleBigNum& SimpleBigNum::operator<<( uint32_t value )
{
    ShitfLeft( value );
    return *this;
}

////////////////////////////////////////////////////////////////////////
inline SimpleBigNum& SimpleBigNum::operator>>( uint32_t value )
{
    ShitfRight( value );
    return *this;
}

////////////////////////////////////////////////////////////////////////
inline bool SimpleBigNum::operator>( const SimpleBigNum& other ) const 
{
    return IsGreaterThen( other );
}

////////////////////////////////////////////////////////////////////////
inline bool SimpleBigNum::operator<( const SimpleBigNum& other ) const
{
    return IsLessThen( other );
}

////////////////////////////////////////////////////////////////////////
inline bool SimpleBigNum::operator<=( const SimpleBigNum& other ) const
{
    return IsLessOrEqualTo( other );
}

////////////////////////////////////////////////////////////////////////
inline bool SimpleBigNum::operator>=( const SimpleBigNum& other ) const
{
    return IsGreaterOrEqualTo( other );
}

////////////////////////////////////////////////////////////////////////
inline bool SimpleBigNum::operator==( const SimpleBigNum& other ) const
{
    return IsEqualTo( other );
}

////////////////////////////////////////////////////////////////////////
inline bool SimpleBigNum::operator!=( const SimpleBigNum& other ) const
{
    return !IsEqualTo( other );
}

}