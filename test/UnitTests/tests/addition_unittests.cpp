#include "pch.h"

using namespace sbn;

class AdditionUnittests : public BaseTestWithRandomGenerator< uint64_t >
{
public:
    AdditionUnittests() : BaseTestWithRandomGenerator( 0, 0x7FFFFFFFFFFFFFFF ) {}
};

TEST_F( AdditionUnittests, simple_addition_with_carry )
{
    SimpleBigNum::TRawNumberDigits digits = { 255, 255, 255 };

    SimpleBigNum bigNumber( digits.begin(), digits.end() );

    bigNumber += 1;

    ASSERT_EQ( bigNumber.ToString(), std::to_string( 16777216 ) );
}

TEST_F( AdditionUnittests, simple_addition_with_zero )
{
    const uint64_t num = GetNextRandomNumber();
    SimpleBigNum bigNumber( num );
    bigNumber += 0;
    ASSERT_EQ( bigNumber.ToString(), std::to_string( num ) );
}

TEST_F( AdditionUnittests, additions_stochastic_test )
{
    for( uint32_t i = 0; i < 10000; ++i )
    {
        const auto value1 = GetNextRandomNumber();
        const auto value2 = GetNextRandomNumber();
        const auto sum = value1 + value2;
        sbn::SimpleBigNum bigNumber( value1 );
        bigNumber += value2;

        ASSERT_EQ( bigNumber.ToString(), std::to_string( sum ) );
    }
}

TEST_F( AdditionUnittests, self_additions_stochastic_test )
{
    for( uint32_t i = 0; i < 10000; ++i )
    {
        const uint64_t value1 = GetNextRandomNumber() % 0x0FFFFFFFFFFFFFFFll;
        const auto sum = value1 + value1 + value1 + value1;
        sbn::SimpleBigNum bigNumber( value1 );
        bigNumber += bigNumber;
        bigNumber += bigNumber;

        ASSERT_EQ( bigNumber.ToString(), std::to_string( sum ) );
    }
}

TEST_F( AdditionUnittests, sum_with_const_increment )
{
    const SimpleBigNum inc( 99999999 );
    SimpleBigNum sum;
    for( uint32_t i = 0; i < 10000000; ++i )
        sum += inc;
    
    ASSERT_EQ( sum.ToString(), "999999990000000" );
}

TEST_F( AdditionUnittests, arithmetic_progression_sum )
{
    const SimpleBigNum progress( 91813813 );
    SimpleBigNum currElement;
    SimpleBigNum sum;
    for( uint32_t i = 0; i < 10000000; ++i )
    {
        sum += currElement;
        currElement += progress;
    }
        
    ASSERT_EQ( sum.ToString(), "4590690190930935000000" );
}