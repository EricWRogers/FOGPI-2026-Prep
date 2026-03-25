#pragma once
#include <string>

class I_Interactable
{
public:
    virtual ~I_Interactable() = default;
    virtual std::string GetMessage() = 0;
    virtual bool HandleInteraction() = 0; // return true if interacted with
};