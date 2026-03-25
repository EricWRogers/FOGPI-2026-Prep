#pragma once

#include <string>

class I_Block
{
public:
    virtual ~I_Block() = default;
    virtual std::string GetName() = 0;
};
