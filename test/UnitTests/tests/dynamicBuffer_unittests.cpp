#include "pch.h"
#include "../../lib/SimpleBigNum/src/tools/allocator/alignedAllocator/alignedAllocator.h"
#include "../../lib/SimpleBigNum/src/tools/dynamicBuffer/dynamicBuffer.h"

TEST( AllocatorUnitests, resize_operation )
{
    sbn::tools::AlignedAllocator allocator;
    sbn::tools::DynamicBuffer< int > buffer( allocator );

    ASSERT_TRUE( buffer.Size() == 0 );

    buffer.PushBack( 0 );
    buffer.PushBack( 1 );
    buffer.PushBack( 2 );
    buffer.PushBack( 3 );
    buffer.PushBack( 4 );
    buffer.PushBack( 5 );
    buffer.PushBack( 6 );
    buffer.PushBack( 7 );

    ASSERT_TRUE( buffer.Size() == 8 );
    ASSERT_TRUE( buffer.Back() == 7 );
    ASSERT_TRUE( buffer.Front() == 0 );
    ASSERT_TRUE( buffer.At( 2 ) == 2 );
    ASSERT_TRUE( buffer.At( 5 ) == 5 );

    buffer.Resize( 1000 );

    for( int i = 8; i < 1000; ++i )
        ASSERT_TRUE( buffer.At( i ) == 0 );

    ASSERT_TRUE( buffer.Size() == 1000 );
    ASSERT_TRUE( buffer.Front() == 0 );
    ASSERT_TRUE( buffer.At( 2 ) == 2 );
    ASSERT_TRUE( buffer.At( 5 ) == 5 );
    ASSERT_TRUE( buffer.At( 7 ) == 7 );

    buffer.Resize( 100 );

    ASSERT_TRUE( buffer.Size() == 100 );
    ASSERT_TRUE( buffer.Front() == 0 );
    ASSERT_TRUE( buffer.At( 2 ) == 2 );
    ASSERT_TRUE( buffer.At( 5 ) == 5 );
    ASSERT_TRUE( buffer.At( 7 ) == 7 );

    buffer.Resize( 1 );

    ASSERT_TRUE( buffer.Size() == 1 );
    ASSERT_TRUE( buffer.Front() == 0 );
    ASSERT_TRUE( buffer.Back() == 0 );

    buffer.Resize( 0 );

    ASSERT_TRUE( buffer.Size() == 0 );

}

TEST( AllocatorUnitests, move_operation )
{
    sbn::tools::AlignedAllocator allocator;
    sbn::tools::DynamicBuffer< int > buffer( allocator );

    for( int i = 0; i < 1000; ++i )
        buffer.PushBack( i );

    sbn::tools::DynamicBuffer< int > buffer2( allocator );

    std::swap( buffer, buffer2 );

    for( int i = 0; i < 1000; ++i )
        ASSERT_TRUE( buffer2.At( i ) == i );

}