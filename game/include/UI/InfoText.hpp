#pragma once

#include <Canis/Entity.hpp>

class InfoText : public Canis::ScriptableEntity
{
private:

public:
    static constexpr const char* ScriptName = "InfoText";

    float timer = 0.0f;
    float fadeoutTime = 2.0f;

    InfoText(Canis::Entity &_entity) : Canis::ScriptableEntity(_entity) {}

    void Create();
    void Ready();
    void Destroy();
    void Update(float _dt);

    void SetText(const std::string &_info);
    void EarlyFadeout();
};

extern void RegisterInfoTextScript(Canis::App& _app);
extern void UnRegisterInfoTextScript(Canis::App& _app);