#include "pch.h"

using namespace sbn;

class ComparsionUnittests : public BaseTestWithRandomGenerator< uint64_t >
{
public:
    ComparsionUnittests() : BaseTestWithRandomGenerator( 0, 0x7FFFFFFFFFFFFFFF ) {}
};

TEST_F( ComparsionUnittests, stochastic_greater_less_comparsion_stochastic_test )
{
    for( uint32_t i = 0; i < 100000; ++i )
    {
        const auto value1 = GetNextRandomNumber();
        const auto value2 = GetNextRandomNumber();
        const auto bigger = std::max( value1, value2 );
        const auto smaller = std::min( value1, value2 );
        const auto diff = bigger - smaller;
        SimpleBigNum biggerNum( bigger );
        SimpleBigNum biggerNum2( bigger );
        SimpleBigNum smallerNum( smaller );

        ASSERT_TRUE( biggerNum2 == biggerNum );
        ASSERT_FALSE( biggerNum2 != biggerNum );
        ASSERT_TRUE( biggerNum == biggerNum );
        ASSERT_FALSE( biggerNum != biggerNum );
        ASSERT_TRUE( biggerNum >= biggerNum2 );
        ASSERT_TRUE( biggerNum2 <= biggerNum );

        ASSERT_TRUE( biggerNum != smallerNum );
        ASSERT_TRUE( biggerNum > smallerNum );
        ASSERT_TRUE( smallerNum < biggerNum );
    }
}