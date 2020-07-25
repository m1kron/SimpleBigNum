#include "alignedAllocator.h"
#include <stdlib.h>
#include "../../alignmentTools.h"

namespace sbn
{
namespace tools
{

static const size_t SIMD_ALIGNMENT = 16;

/////////////////////////////////////////////////////////////////////////////////////////
void* AlignedAllocator::Allocate( size_t requestedSize )
{
    uintptr_t neededOffset = SIMD_ALIGNMENT + sizeof( void* );
    void* raw = malloc( requestedSize + neededOffset );
    void** ptr = ( void** )tools::align<uintptr_t, SIMD_ALIGNMENT>( ( uintptr_t )raw + sizeof( void* ) );
    ptr[ -1 ] = raw;
    return ptr;
}

/////////////////////////////////////////////////////////////////////////////////////////
void AlignedAllocator::Free( void* ptr )
{
    if( ptr != nullptr )
    {
        void* original = ( ( void** )ptr )[ -1 ];
        free( original );
    }
}

}
}