#pragma once

#include <Canis/Entity.hpp>

namespace SuperPupUtilities
{
    class FPSCounter : public Canis::ScriptableEntity
    {
    private:

    public:
        static constexpr const char* ScriptName = "SuperPupUtilities::FPSCounter";

        FPSCounter(Canis::Entity &_entity) : Canis::ScriptableEntity(_entity) {}

        void Create();
        void Ready();
        void Destroy();
        void Update(float _dt);
    };

    extern void RegisterFPSCounterScript(Canis::App& _app);
    extern void UnRegisterFPSCounterScript(Canis::App& _app);
}