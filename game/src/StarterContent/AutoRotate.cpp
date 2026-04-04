#include <StarterContent/AutoRotate.hpp>

#include <Canis/App.hpp>
#include <Canis/ConfigHelper.hpp>

namespace StarterContent
{
    namespace
    {
        Canis::ComponentConf autoRotateComponentConf = {};
        Canis::SystemConf autoRotateSystemConf = {};
    }

    void RegisterAutoRotateComponent(Canis::App& _app)
    {
        REGISTER_PROPERTY(autoRotateComponentConf, StarterContent::AutoRotate, active);
        REGISTER_PROPERTY(autoRotateComponentConf, StarterContent::AutoRotate, eulerSpeed);

        DEFAULT_COMPONENT_CONFIG_AND_REQUIRED(autoRotateComponentConf, StarterContent::AutoRotate, Canis::Transform);

        autoRotateComponentConf.DEFAULT_DRAW_COMPONENT_INSPECTOR(StarterContent::AutoRotate);

        _app.RegisterComponent(autoRotateComponentConf);
    }

    DEFAULT_UNREGISTER_COMPONENT(autoRotateComponentConf, AutoRotate)

    void RegisterAutoRotateSystem(Canis::App& _app)
    {
        DEFAULT_SYSTEM_CONFIG(autoRotateSystemConf, StarterContent::AutoRotateSystem, Canis::SystemPipeline::Update);
        _app.RegisterSystem(autoRotateSystemConf);
    }

    DEFAULT_UNREGISTER_SYSTEM(autoRotateSystemConf, AutoRotate)

    void AutoRotateSystem::Update(entt::registry& _registry, float _deltaTime)
    {
        auto view = _registry.view<Canis::Transform, StarterContent::AutoRotate>();

        for (const entt::entity entityHandle : view)
        {
            Canis::Transform& transform = view.get<Canis::Transform>(entityHandle);
            StarterContent::AutoRotate& autoRotate = view.get<StarterContent::AutoRotate>(entityHandle);

            if (!autoRotate.active)
                continue;

            transform.rotation += autoRotate.eulerSpeed * _deltaTime;
        }
    }
}
