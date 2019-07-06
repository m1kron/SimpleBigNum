#pragma once
#include "../typedefs.h"

namespace sbn
{
namespace internal
{
namespace sse
{

// Implements aritheticImpl functions.
void AddInplaceImpl( TRawBufferPtr thisNumberBuffer, const TConstRawBufferPtr otherNumberBuffer, const uint32_t otherNumberSize );
// -----------------------------------------

}
}
}