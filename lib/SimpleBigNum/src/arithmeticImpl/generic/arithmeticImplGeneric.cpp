#pragma once
#include "arithmeticImplGeneric.h"

namespace sbn
{
namespace internal
{
namespace generic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AddInplaceImpl( TRawBufferPtr thisNumberBuffer, const TConstRawBufferPtr otherNumberBuffer, const uint32_t otherNumberSize )
{
    TDigitType overflow = 0;
    for( uint32_t digitIdx = 0; digitIdx < otherNumberSize; ++digitIdx )
    {
        const TDigitType thisDigit = thisNumberBuffer[ digitIdx ];
        const TDigitType otherDigit = otherNumberBuffer[ digitIdx ];
        const TDigitType value = thisDigit + otherDigit + overflow;
        thisNumberBuffer[ digitIdx ] = value;
        const TDigitType thisDigitPlusOverflow = thisDigit + overflow;
        overflow = thisDigitPlusOverflow < thisDigit;
        overflow = ( BASE_MINUS_ONE - thisDigitPlusOverflow ) < otherDigit || overflow;
    }

    // Carry overflow:
    uint32_t digitIdx = otherNumberSize;
    while( overflow != 0 )
    {
        const TDigitType thisDigit = thisNumberBuffer[ digitIdx ];
        const TDigitType value = thisDigit + overflow;
        thisNumberBuffer[ digitIdx ] = value;
        overflow = value < thisDigit;
        ++digitIdx;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SustructInplaceImpl( TRawBufferPtr thisNumberBuffer, const TConstRawBufferPtr otherNumberBuffer, const uint32_t otherNumberSize )
{
    TDigitType overflow = 0;
    for( uint32_t digitIdx = 0; digitIdx < otherNumberSize; ++digitIdx )
    {
        TDigitType thisDigit = thisNumberBuffer[ digitIdx ];
        TDigitType otherDigit = otherNumberBuffer[ digitIdx ];
        TDigitType final = thisDigit - otherDigit - overflow;
        overflow = ( thisDigit < otherDigit ) || ( ( thisDigit == otherDigit ) && overflow );
        thisNumberBuffer[ digitIdx ] = final;
    }

    // Carry overflow:
    uint32_t digitIdx = otherNumberSize;
    while( overflow != 0 )
    {
        TDigitType thisDigit = thisNumberBuffer[ digitIdx ];
        TDigitType final = thisDigit - overflow;
        overflow = thisDigit < overflow;
        thisNumberBuffer[ digitIdx ] = final;
        ++digitIdx;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MultiplyInplaceImpl( TConstRawBufferPtr thisNumberBuffer, const uint32_t thisNumberSize, TConstRawBufferPtr otherNumberBuffer, const uint32_t otherNumberSize, TRawBufferPtr out_resultBuffer )
{
    uint16_t overflow = 0;

    for( uint32_t thisDigitIdx = 0; thisDigitIdx < thisNumberSize; ++thisDigitIdx )
    {
        const uint8_t thisDigit = thisNumberBuffer[ thisDigitIdx ];
        for( uint32_t otherDigitIdx = 0; otherDigitIdx < otherNumberSize; ++otherDigitIdx )
        {
            uint16_t mult = ( uint16_t )thisDigit * ( uint16_t )otherNumberBuffer[ otherDigitIdx ];
            overflow += mult + ( uint16_t )out_resultBuffer[ thisDigitIdx + otherDigitIdx ];
            out_resultBuffer[ thisDigitIdx + otherDigitIdx ] = ( TDigitType )overflow;
            overflow = overflow >> 8;
        }

        out_resultBuffer[ thisDigitIdx + otherNumberSize ] += ( TDigitType )overflow;
        overflow = overflow >> 8;
    }
}

}
}
}