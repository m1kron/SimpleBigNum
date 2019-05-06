#include "pch.h"
#include "../../lib/SimpleBigNum/src/reciprocalEstimator/reciprocalEstimator.h"

using namespace sbn;
using namespace sbn::internal;

static void PerformTest( const SimpleBigNum& number, uint32_t shift, uint32_t steps, const std::string& wantedAnswer )
{
    SimpleBigNum reciprocal = ReciprocalEstimator::Estimate( number, shift, steps );
    ASSERT_EQ( reciprocal.ToString(), wantedAnswer );
}

TEST( ReciprocalEstimatorUnittests, reciprocal_of_2 )
{
    PerformTest( SimpleBigNum( 2 ), 4, 15, "2147483648" );
}

TEST( ReciprocalEstimatorUnittests, reciprocal_of_1256 )
{
    PerformTest( SimpleBigNum( 1256 ), 8, 15, "14686898147857924" );
}

TEST( ReciprocalEstimatorUnittests, reciprocal_of_6548625354375474 )
{
    PerformTest( SimpleBigNum( 6548625354375474 ), 10, 15, "184607571" );
}