#include "reciprocalEstimator.h"

namespace sbn
{
namespace internal
{

///////////////////////////////////////////////////////////////////////////////////////////
SimpleBigNum ReciprocalEstimator::Estimate( const SimpleBigNum& number, uint32_t shift, uint32_t maxSteps )
{
    // Estimates reciprocal using Netwon's method:
    // Function used:
    // f(x) = 1/x - ( number >> shift );
    // f'(x) =  - 1/x^2
    //
    // Final Newton's method step:
    // x1 = 2 * x0 - ( number >> shift ) * x0^2

    SimpleBigNum estimatedValue( 1 );
    estimatedValue << ( shift - number.GetNumberOfDigits() );

    for( uint32_t step = 0; step < maxSteps; ++step )
    {
        SimpleBigNum subtrahend = estimatedValue;
        subtrahend *= subtrahend;       // x0^2
        subtrahend *= number;           // x0^2 * number
        subtrahend >> shift;            // ( number >> shift ) * x0^2

        if( estimatedValue == subtrahend ) // The result won't be impromved.
            break;

        estimatedValue *= 2;            // 2 * x0
        estimatedValue -= subtrahend;   // 2 * x0 - ( number >> shift ) * x0^2
    }

    return estimatedValue;
}

}
}