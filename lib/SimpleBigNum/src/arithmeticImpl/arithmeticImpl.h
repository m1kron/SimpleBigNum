#pragma once
#include "typedefs.h"

namespace sbn
{
namespace internal
{

// Adds inplace otherNumber to this number. Result is accumulated in thisNumberBuffer.
// Assumes that thisNumberBuffer is big enough to hold the result.
void AddInplaceImpl( TRawBufferPtr thisNumberBuffer, const TConstRawBufferPtr otherNumberBuffer, const uint32_t otherNumberSize );

// Subtructs inplace otherNumber to this number. Result is accumulated in thisNumberBuffer.
// Assumes that thisNumberBuffer is big enough to hold the result.
// WARNING! - Assumes that thisNumber is bigger then other, so that the result is always a positive number or zero.
void SustructInplaceImpl( TRawBufferPtr thisNumberBuffer, const TConstRawBufferPtr otherNumberBuffer, const uint32_t otherNumberSize );

// Multiplies thisNumberBuffer and otherNumberBuffer. Result is stored in out_resultBuffer.
// Assumes that thisNumberBuffer is big enough to hold the result.
void MultiplyInplaceImpl(
    TConstRawBufferPtr thisNumberBuffer, const uint32_t thisNumberSize,
    TConstRawBufferPtr otherNumberBuffer, const uint32_t otherNumberSize,
    TRawBufferPtr out_resultBuffer
);

}
}