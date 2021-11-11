#pragma once

namespace traceroute {


class IVisitor
{
public:
    virtual void visit();
};

} // namespace traceroute