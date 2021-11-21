#include <Traceroute/SocketAddress.hpp>
#include <Traceroute/interface/IValidateResponse.hpp>
#include <gtest/gtest.h>
#include <memory>

namespace traceroute {

struct ResponseValidatorTest : public ::testing::Test
{
  public:
    std::unique_ptr<IValidateResponse> validator;
    const SocketAddress                requestSource;
    const SocketAddress                requestDestination;

    void setValidator(std::unique_ptr<IValidateResponse> _validator)
    {
        validator = std::move(_validator);
    }

    ResponseValidatorTest(std::string source, std::string destination)
        : requestSource(source), requestDestination(destination)
    {
    }
};

struct ResponseValidatorTestV4 : public ResponseValidatorTest
{
    ResponseValidatorTestV4() : ResponseValidatorTest("1.1.1.1", "2.2.2.2")
    {
    }
};

struct ResponseValidatorTestV6 : public ResponseValidatorTest
{
    ResponseValidatorTestV6() : ResponseValidatorTest("aa:bb::1", "bb:cc::2")
    {
    }
};

} // namespace traceroute