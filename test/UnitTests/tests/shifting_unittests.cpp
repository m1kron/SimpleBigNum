#include "pch.h"

using namespace sbn;

class ShiftingUnittests : public BaseTestWithRandomGenerator< uint64_t >
{
public:
    ShiftingUnittests() : BaseTestWithRandomGenerator( 0, 0x7FFFFFFFFFFFFFFF ) {}
};

TEST_F( ShiftingUnittests, simple_shifting_left )
{
    SimpleBigNum number( 65536 );
    number << 3;

    ASSERT_EQ( number.ToString(), "1099511627776" );
}

TEST_F( ShiftingUnittests, simple_shifting_right )
{
    SimpleBigNum number( 1099511627776 );
    number >> 3;

    ASSERT_EQ( number.ToString(), "65536" );

    number >> 2;
    ASSERT_EQ( number.ToString(), "1" );

    number >> 1;
    ASSERT_EQ( number.ToString(), "0" );

    number >> 10;
    ASSERT_EQ( number.ToString(), "0" );

    SimpleBigNum number2( 2143265475432 );
    number >> 100;
    ASSERT_TRUE( number.IsZero() );
}

TEST_F( ShiftingUnittests, shifting_left_then_right_should_alwyas_give_the_same_number )
{
    for( uint32_t i = 0; i < 1000; ++i )
    {
        const uint32_t shift = ( uint32_t )( GetNextRandomNumber() % 12548 );
        const uint64_t value = GetNextRandomNumber();
        SimpleBigNum number( value );
        number << shift;
        number >> shift;

        ASSERT_EQ( number.ToString(), std::to_string( value ) );
    }
}

TEST_F( ShiftingUnittests, shifting_right_should_be_equal_to_dividing_by_256 )
{
    uint64_t initialValue = 9354867864213878635;

    SimpleBigNum dividedValue( initialValue );
    SimpleBigNum shiftedValue( initialValue );

    for( uint32_t i = 0; i < 6; ++i )
    {
        initialValue /= 256;
        dividedValue /= 256;
        shiftedValue >> 1;

        ASSERT_TRUE( dividedValue == shiftedValue );
        ASSERT_EQ( shiftedValue.ToString(), std::to_string( initialValue ) );
    }
}

TEST_F( ShiftingUnittests, shifting_left_should_be_equal_to_multipyling_by_256 )
{
    uint64_t initialValue = 133;

    SimpleBigNum mutipliedValue( initialValue );
    SimpleBigNum shiftedValue( initialValue );

    for( uint32_t i = 0; i < 6; ++i )
    {
        initialValue *= 256;
        mutipliedValue *= 256;
        shiftedValue << 1;

        ASSERT_TRUE( mutipliedValue == shiftedValue );
        ASSERT_EQ( shiftedValue.ToString(), std::to_string( initialValue ) );
    }

    for( uint32_t i = 0; i < 10; ++i )
    {
        uint32_t shiftVal = ( uint32_t )( GetNextRandomNumber() % 57 );
        shiftedValue << shiftVal;

        for( uint32_t j = 0; j < shiftVal; ++j )
            mutipliedValue *= 256;

        ASSERT_TRUE( mutipliedValue == shiftedValue );
    }
}