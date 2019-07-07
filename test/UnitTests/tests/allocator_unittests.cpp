#include "pch.h"
#include "../../lib/SimpleBigNum/src/tools/allocator/alignedAllocator/alignedAllocator.h"
#include "../../lib/SimpleBigNum/src/tools/alignmentTools.h"

TEST( AllocatorUnitests, allocated_mem_should_be_aligned )
{
    sbn::tools::AlignedAllocator allocator;

    for( uint32_t i = 0; i < 100000; ++i )
    {
        void* mem = nullptr;
        mem = allocator.Allocate( i );
        ASSERT_TRUE( mem != nullptr );
        ASSERT_TRUE( ( sbn::tools::isAligned< uintptr_t, 16 >( ( uintptr_t )mem ) ) );
        allocator.Free( mem );
    }
}