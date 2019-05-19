#pragma once
#include "../../include/bigNum.h"

namespace sbn
{
namespace internal
{

// Class estimaties (1/number) << shift using Newton's method.
class ReciprocalEstimator
{
public:
    static SimpleBigNum Estimate( const SimpleBigNum& number, uint32_t shift, uint32_t maxSteps );
};

}
}