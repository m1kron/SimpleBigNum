#pragma once
#include "../iAllocator.h"

namespace sbn
{
namespace tools
{

// Alloctor which allocates memory aligned to 16 byte boundary.
class AlignedAllocator : public IAllocator
{
public:
    // IAllocator interface impl: 
    void* Allocate( size_t requestedSize ) override;
    void Free( void* ptr ) override;
    // -------------------------
};

}
}