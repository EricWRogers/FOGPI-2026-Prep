#pragma once
#include <string>

class I_Item
{
public:
    virtual ~I_Item() = default;
    virtual std::string GetName() = 0;
};