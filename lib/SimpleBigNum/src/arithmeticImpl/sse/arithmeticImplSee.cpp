#include "arithmeticImplSee.h"
#include <smmintrin.h>

namespace sbn
{
namespace internal
{
namespace sse
{
#define SIMD_SIZE 16

// Greater or equal for unsigned bytes vector.
#define _mm_cmpge_epu8(a, b) \
        _mm_cmpeq_epi8(_mm_max_epu8(a, b), a)

////////////////
// 3 classes:
// N - 0 - 00b - not carry
// C - 1 - 01b - carry
// M - 2 - 10b - maybe carry
//
// Operation table:
// NN -> N : 0000b -> 00b : 0 -> 0
// NC -> N : 0001b -> 00b : 1 -> 0
// NM -> N : 0010b -> 00b : 2 -> 0
// MM -> M : 1010b -> 10b : 10 -> 2
// MC -> C : 1001b -> 01b : 9 -> 1
// MN -> N : 1000b -> 00b : 8 -> 0
// CC -> C : 0101b -> 01b : 5 -> 1
// CM -> C : 0110B -> 01b : 6 -> 1
// CN -> C : 0100b -> 01b : 4 -> 1

///////////////////////////////////////////////////////////////////////////////////////////////////////////
static inline __m128i transformToClassOp( const __m128i thisNumber, const __m128i otherNumber )
{
    const __m128i BASE_MINUS_ONE_CONST = _mm_set1_epi8( -1 );
    const __m128i MAYBE_FLAG = _mm_set1_epi8( 2 );
    const __m128i CARRY_FLAG = _mm_set1_epi8( 1 );

    const __m128i baseMinusOther = _mm_sub_epi8( BASE_MINUS_ONE_CONST, otherNumber );

    // Calc MAYBE op.
    __m128i maybeVec = _mm_cmpeq_epi8( baseMinusOther, thisNumber );
    maybeVec = _mm_and_si128( maybeVec, MAYBE_FLAG );

    // Calc CARRY op.
    __m128i carryVec = _mm_cmpge_epu8( baseMinusOther, thisNumber );
    carryVec = _mm_andnot_si128( carryVec, CARRY_FLAG );

    // Merge results, as NOT CARRY op is 0.
    return  _mm_or_si128( carryVec, maybeVec );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
static inline __m128i prefixSumOfClassOp( __m128i classOpVec, int& in_out_prevCarry )
{
    // Implements Hills and Steele parallel scan algorithm.
    //
    // NOTE: in_out_prevOverflow keeps the information wether last opertion from previous group has to carry or not.
    //
    // NOTE2: in_out_prevCarry could be stored on TTypeDigit, but then msvc generates code where _mm_set_epi38 is implemented via memory:
    // vector is constructed on the stack where in_out_prevCarry is moved to stack via al register,
    // later everything is being loaded into xmm register. This creates store forwarding stall.
    // Using int avoids it, since msvc implements _mm_set_epi32 as a series of movd/punpckldq.

    const __m128i LOOKUP = _mm_set_epi8( -1, -1, -1, -1, -1, 2, 1, 0, -1, 1, 1, 1, -1, 0, 0, 0 );
    const __m128i MASK1 = _mm_set_epi8( 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0 );
    const __m128i MASK2 = _mm_set_epi8( 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 0 );
    const __m128i MASK3 = _mm_set_epi8( 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 3, 2, 1, 0 );
    const __m128i MASK4 = _mm_set_epi8( 7, 6, 5, 4, 3, 2, 1, 0, 7, 6, 5, 4, 3, 2, 1, 0 );

    __m128i prev = _mm_set_epi32( 0, 0, 0, in_out_prevCarry );

    in_out_prevCarry = ( int )_mm_extract_epi8( classOpVec, 15 );

    __m128i temp1 = _mm_or_si128( _mm_bslli_si128( classOpVec, 1 ), prev );

    __m128i temp2 = _mm_shuffle_epi8( temp1, MASK1 );
    temp1 = _mm_slli_epi16( temp1, 2 );
    temp1 = _mm_or_si128( temp2, temp1 );
    temp1 = _mm_shuffle_epi8( LOOKUP, temp1 );

    temp2 = _mm_shuffle_epi8( temp1, MASK2 );
    temp1 = _mm_slli_epi16( temp1, 2 );
    temp1 = _mm_or_si128( temp2, temp1 );
    temp1 = _mm_shuffle_epi8( LOOKUP, temp1 );

    temp2 = _mm_shuffle_epi8( temp1, MASK3 );
    temp1 = _mm_slli_epi16( temp1, 2 );
    temp1 = _mm_or_si128( temp2, temp1 );
    temp1 = _mm_shuffle_epi8( LOOKUP, temp1 );

    temp2 = _mm_shuffle_epi8( temp1, MASK4 );
    temp1 = _mm_slli_epi16( temp1, 2 );
    temp1 = _mm_or_si128( temp2, temp1 );
    temp1 = _mm_shuffle_epi8( LOOKUP, temp1 );

    if( in_out_prevCarry > 1 )
        in_out_prevCarry = ( int )_mm_extract_epi8( temp1, 15 );

    return temp1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AddInplaceImpl( TRawBufferPtr thisNumberBuffer, const TConstRawBufferPtr otherNumberBuffer, const uint32_t otherNumberSize )
{
    int carry = 0;
    uint32_t digitIdx = 0;
    for( ; digitIdx < otherNumberSize; digitIdx += SIMD_SIZE )
    {
        const __m128i thisNumberBufferVec = _mm_load_si128( ( __m128i* const )&thisNumberBuffer[ digitIdx ] );
        const __m128i otherNumberBufferVec = _mm_load_si128( ( __m128i* const )&otherNumberBuffer[ digitIdx ] );

        __m128i classOpVec = transformToClassOp( thisNumberBufferVec, otherNumberBufferVec );
        __m128i overflowVec = prefixSumOfClassOp( classOpVec, carry );
        __m128i addResult = _mm_add_epi8( thisNumberBufferVec, otherNumberBufferVec );
        addResult = _mm_add_epi8( addResult, overflowVec );
        _mm_store_si128( ( __m128i* )&thisNumberBuffer[ digitIdx ], addResult );
    }

    const __m128i ZERO = _mm_set1_epi8( 0 );

    while( carry != 0 )
    {
        const __m128i thisNumberBufferVec = _mm_load_si128( ( __m128i* const )&thisNumberBuffer[ digitIdx ] );

        __m128i classOpVec = transformToClassOp( thisNumberBufferVec, ZERO );
        __m128i overflowVec = prefixSumOfClassOp( classOpVec, carry );
        __m128i addResult = _mm_add_epi8( thisNumberBufferVec, overflowVec );
        _mm_store_si128( ( __m128i* )&thisNumberBuffer[ digitIdx ], addResult );

        digitIdx += SIMD_SIZE;
    }
}

}
}
}