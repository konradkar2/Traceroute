#include <Traceroute/IValidateResponse.hpp>
#include <memory>
#include <gtest/gtest.h>

namespace traceroute
{
struct ResponseValidatorTest : public ::testing::Test
{
    std::unique_ptr<IValidateResponse> validator;
    ResponseValidatorTest(std::unique_ptr<IValidateResponse> _validator) : validator(std::move(_validator))
    {

    }
};

} // namespace traceroute