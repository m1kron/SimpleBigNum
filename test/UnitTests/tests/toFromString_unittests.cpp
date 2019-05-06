#include "pch.h"

using namespace sbn;

class ToFromStringUnittests : public BaseTestWithRandomGenerator< uint64_t >
{
public:
    ToFromStringUnittests() : BaseTestWithRandomGenerator( 0, 0x7FFFFFFFFFFFFFFF ) {}
};

TEST_F( ToFromStringUnittests, to_string_stochastic_test )
{
    for( uint32_t i = 0; i < 10000; ++i )
    {
        const auto value = GetNextRandomNumber();
        sbn::SimpleBigNum bigNumber( value );

        ASSERT_EQ( bigNumber.ToString(), std::to_string( value ) );
    }
}

TEST_F( ToFromStringUnittests, from_string_stochastic_test )
{
    for( uint32_t i = 0; i < 10000; ++i )
    {
        const auto value = GetNextRandomNumber();
        const auto wantedString = std::to_string( value );

        sbn::SimpleBigNum bigNumber;
        bigNumber.FromString( wantedString );

        ASSERT_EQ( bigNumber.ToString(), wantedString );
    }
}