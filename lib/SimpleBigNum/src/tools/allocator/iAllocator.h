#pragma once
#include <cstdint>

namespace sbn
{
namespace tools
{

// Interface for all allocators used by sbn.
class IAllocator
{
public:
    // Allocates memory of given requested size. Returns pointer to allocated memory.
    virtual void* Allocate( size_t requestedSize ) = 0;

    // Frees memory.
    virtual void Free( void* ptr ) = 0;

    // Dtor.
    virtual ~IAllocator() {}
};

}
}