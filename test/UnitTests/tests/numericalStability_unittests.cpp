#include "pch.h"

using namespace sbn;

class NumericalStabilityUnittests : public BaseTestWithRandomGenerator< uint64_t >
{
public:
    NumericalStabilityUnittests() : BaseTestWithRandomGenerator( 0, 0xFFFFFFFFFFFFFFFF ) {}
};

TEST_F( NumericalStabilityUnittests, addition_should_be_equal_to_multiplication_stochastic_test )
{
    for( uint32_t i = 0; i < 10000; ++i )
    {
        const SimpleBigNum diff( GetNextRandomNumber() );

        const uint64_t multiplicator = GetNextRandomNumber() % 2146ll;
        SimpleBigNum addition;

        for( uint64_t multIt = 0; multIt < multiplicator; ++multIt )
            addition += diff;

        SimpleBigNum multiplication = diff;
        multiplication *= multiplicator;

        ASSERT_EQ( multiplication, addition );
    }
}

TEST_F( NumericalStabilityUnittests, addition_then_substruction_stochastic_test )
{
    for( uint32_t i = 0; i < 10000; ++i )
    {
        const SimpleBigNum diff( GetNextRandomNumber() );

        const uint64_t times = GetNextRandomNumber() % 2146ll;
        const SimpleBigNum wantedNumber( GetNextRandomNumber() );
        SimpleBigNum number = wantedNumber;

        for( uint64_t j = 0; j < times; ++j )
            number += diff;

        for( uint64_t j = 0; j < times; ++j )
            number -= diff;

        ASSERT_EQ( wantedNumber, number );
    }
}

TEST_F( NumericalStabilityUnittests, multiplication_then_division_stochastic_test )
{
    for( uint32_t i = 0; i < 10000; ++i )
    {
        const SimpleBigNum a( GetNextRandomNumber() );
        const SimpleBigNum b( GetNextRandomNumber() );

        SimpleBigNum number = a;
        number *= b;
        number /= a;
        ASSERT_EQ( number, b );
        number *= a;
        number /= b;
        ASSERT_EQ( number, a );
    }
}