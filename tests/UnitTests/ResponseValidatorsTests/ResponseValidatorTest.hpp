#include <Traceroute/interface/IValidateResponse.hpp>
#include <Traceroute/SocketAddress.hpp>
#include <gtest/gtest.h>
#include <memory>

namespace traceroute
{

struct ResponseValidatorTest : public ::testing::Test
{
    std::unique_ptr<IValidateResponse> validator;
    const SocketAddress requestSource;
    const SocketAddress requestDestination;
    ResponseValidatorTest(std::unique_ptr<IValidateResponse> _validator, std::string source, std::string destination)
        : validator(std::move(_validator)), requestSource(source), requestDestination(destination)
    {
    }
};

struct ResponseValidatorTestV4 : public ResponseValidatorTest
{
    ResponseValidatorTestV4(std::unique_ptr<IValidateResponse> _validator)
        : ResponseValidatorTest(std::move(_validator), "1.1.1.1", "2.2.2.2")
    {
    }
};

struct ResponseValidatorTestV6 : public ResponseValidatorTest
{
    ResponseValidatorTestV6(std::unique_ptr<IValidateResponse> _validator)
        : ResponseValidatorTest(std::move(_validator), "aa:bb::1", "bb:cc::2")
    {
    }
};

} // namespace traceroute