#pragma once
#include "../typedefs.h"

namespace sbn
{
namespace internal
{
namespace generic
{

// Implements aritheticImpl functions.
void AddInplaceImpl( TRawBufferPtr thisNumberBuffer, const TConstRawBufferPtr otherNumberBuffer, const uint32_t otherNumberSize );
void SustructInplaceImpl( TRawBufferPtr thisNumberBuffer, const TConstRawBufferPtr otherNumberBuffer, const uint32_t otherNumberSize );
void MultiplyInplaceImpl(
    TConstRawBufferPtr thisNumberBuffer, const uint32_t thisNumberSize,
    TConstRawBufferPtr otherNumberBuffer, const uint32_t otherNumberSize,
    TRawBufferPtr out_resultBuffer
);
// -----------------------------------------

}
}
}