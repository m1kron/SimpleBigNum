#pragma once
#include <cstdint>

namespace sbn
{
namespace internal
{

typedef uint8_t TDigitType;
typedef TDigitType* TRawBufferPtr;
typedef const TDigitType* TConstRawBufferPtr;
constexpr TDigitType BASE_MINUS_ONE = 255;

}
}