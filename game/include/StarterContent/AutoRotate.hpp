#pragma once

#include <Canis/Entity.hpp>
#include <Canis/System.hpp>

namespace Canis
{
    class App;
}

namespace StarterContent
{
    struct AutoRotate
    {
    public:
        static constexpr const char* ScriptName = "StarterContent::AutoRotate";

        AutoRotate() = default;
        explicit AutoRotate(Canis::Entity& _entity) : entity(&_entity) {}

        void Create() {}
        void Destroy() {}

        Canis::Entity* entity = nullptr;
        bool active = true;
        Canis::Vector3 eulerSpeed = Canis::Vector3(0.0f, 1.5f, 0.0f);
    };

    class AutoRotateSystem : public Canis::System
    {
    public:
        AutoRotateSystem() : Canis::System()
        {
            m_name = type_name<AutoRotateSystem>();
        }

        void Update(entt::registry& _registry, float _deltaTime) override;
    };

    void RegisterAutoRotateComponent(Canis::App& _app);
    void UnRegisterAutoRotateComponent(Canis::App& _app);
    void RegisterAutoRotateSystem(Canis::App& _app);
    void UnRegisterAutoRotateSystem(Canis::App& _app);
}
