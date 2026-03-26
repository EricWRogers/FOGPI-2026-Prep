#include <Items/Rock.hpp>

#include <SuperPupUtilities/Inventory.hpp>

#include <Canis/App.hpp>
#include <Canis/InputManager.hpp>
#include <Canis/ConfigHelper.hpp>

ScriptConf rockConf = {};

void RegisterRockScript(App& _app)
{
    DEFAULT_CONFIG_AND_REQUIRED(rockConf, Rock, RectTransform);

    rockConf.DEFAULT_DRAW_INSPECTOR(Rock);

    _app.RegisterScript(rockConf);
}

DEFAULT_UNREGISTER_SCRIPT(rockConf, Rock)

void Rock::Create() {}

void Rock::Ready() {}

void Rock::Destroy() {}

void Rock::Update(float _dt) {}

std::string Rock::GetName()
{
    return "Rock";
}

std::string Rock::GetMessage(Canis::Entity* _interactingEntity)
{
    return std::string("Press E to Pickup ") + ScriptName;
}

bool Rock::HandleInteraction(Canis::Entity* _interactingEntity)
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
