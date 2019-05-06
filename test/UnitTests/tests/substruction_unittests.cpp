#include "pch.h"

using namespace sbn;

class SubstructionUnittests : public BaseTestWithRandomGenerator< uint64_t >
{
public:
    SubstructionUnittests() : BaseTestWithRandomGenerator( 0, 0x7FFFFFFFFFFFFFFF ) {}
};

TEST_F( SubstructionUnittests, simple_substruction_with_carry )
{
    SimpleBigNum::TRawNumberDigits digits = { 0, 0, 1 };
    SimpleBigNum bigNumber( digits.begin(), digits.end() );
    bigNumber -= 1;
    ASSERT_EQ( bigNumber.ToString(), std::to_string( 65535 ) );
}

TEST_F( SubstructionUnittests, simple_substruction_with_zero )
{
    const uint64_t num = GetNextRandomNumber();
    SimpleBigNum bigNumber( num );
    bigNumber -= 0;
    ASSERT_EQ( bigNumber.ToString(), std::to_string( num ) );
}

TEST_F( SubstructionUnittests, substructions_stochastic_test )
{
    for( uint32_t i = 0; i < 10000; ++i )
    {
        const auto value1 = GetNextRandomNumber();
        const auto value2 = GetNextRandomNumber();
        const auto bigger = std::max( value1, value2 );
        const auto smaller = std::min( value1, value2 );
        const auto diff = bigger - smaller;
        sbn::SimpleBigNum bigNumber( bigger );
        bigNumber -= smaller;

        ASSERT_EQ( bigNumber.ToString(), std::to_string( diff ) );
    }
}

TEST_F( SubstructionUnittests, self_substructions_stochastic_test )
{
    for( uint32_t i = 0; i < 10000; ++i )
    {
        const auto value1 = GetNextRandomNumber();
        sbn::SimpleBigNum bigNumber( value1 );
        bigNumber -= bigNumber;

        ASSERT_TRUE( bigNumber.IsZero() );
    }
}

TEST_F( SubstructionUnittests, const_decrement )
{
    const uint64_t iterations = 1000;
    const uint64_t dec = 4372473254326;
    SimpleBigNum val( iterations * dec );

    for( uint64_t i = 0; i < iterations/2; ++i )
        val -= dec;

    ASSERT_EQ( val.ToString(), std::to_string( iterations / 2 * 4372473254326 ) );
}

TEST_F( SubstructionUnittests, substracting_bigger_from_smaller_should_give_zero )
{
    for( uint32_t i = 0; i < 10000; ++i )
    {
        const auto value1 = GetNextRandomNumber();
        const auto value2 = GetNextRandomNumber();
        const auto bigger = std::max( value1, value2 );
        const auto smaller = std::min( value1, value2 );
        sbn::SimpleBigNum number( smaller );
        number -= bigger;

        ASSERT_EQ( number.ToString(), "0" );
    }
}