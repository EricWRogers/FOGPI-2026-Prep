#include <Items/Ice.hpp>

#include <SuperPupUtilities/Inventory.hpp>

#include <Canis/App.hpp>
#include <Canis/InputManager.hpp>
#include <Canis/ConfigHelper.hpp>

ScriptConf iceConf = {};

void RegisterIceScript(App& _app)
{
    DEFAULT_CONFIG_AND_REQUIRED(iceConf, Ice, RectTransform);

    iceConf.DEFAULT_DRAW_INSPECTOR(Ice);

    _app.RegisterScript(iceConf);
}

DEFAULT_UNREGISTER_SCRIPT(iceConf, Ice)

void Ice::Create() {}

void Ice::Ready() {}

void Ice::Destroy() {}

void Ice::Update(float _dt) {}

std::string Ice::GetName()
{
    return "Ice";
}

std::string Ice::GetMessage(Canis::Entity* _interactingEntity)
{
    return std::string("Press E to Pickup ") + ScriptName;
}

bool Ice::HandleInteraction(Canis::Entity* _interactingEntity)
{
    InputManager& input = entity.scene.GetInputManager();

    if (input.GetKey(Key::E))
    {
        if (_interactingEntity != nullptr)
            if (SuperPupUtilities::Inventory* inventory = _interactingEntity->GetScript<SuperPupUtilities::Inventory>())
                inventory->Add(*this, 1);

        entity.Destroy();
        return true;
    }

    return false;
}
