#include "arithmeticImpl.h"
#include "generic/arithmeticImplGeneric.h"

namespace sbn
{
namespace internal
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AddInplaceImpl( TRawBufferPtr thisNumberBuffer, const TConstRawBufferPtr otherNumberBuffer, const uint32_t otherNumberSize )
{
    generic::AddInplaceImpl( thisNumberBuffer, otherNumberBuffer, otherNumberSize );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SustructInplaceImpl( TRawBufferPtr thisNumberBuffer, const TConstRawBufferPtr otherNumberBuffer, const uint32_t otherNumberSize )
{
    generic::SustructInplaceImpl( thisNumberBuffer, otherNumberBuffer, otherNumberSize );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MultiplyInplaceImpl( TConstRawBufferPtr thisNumberBuffer, const uint32_t thisNumberSize, TConstRawBufferPtr otherNumberBuffer, const uint32_t otherNumberSize, TRawBufferPtr out_resultBuffer )
{
    generic::MultiplyInplaceImpl( thisNumberBuffer, thisNumberSize, otherNumberBuffer, otherNumberSize, out_resultBuffer );
}

}
}