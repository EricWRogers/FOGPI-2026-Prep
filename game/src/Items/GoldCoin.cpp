#include <Items/GoldCoin.hpp>

#include <SuperPupUtilities/Inventory.hpp>

#include <Canis/App.hpp>
#include <Canis/InputManager.hpp>
#include <Canis/ConfigHelper.hpp>

ScriptConf goldCoinConf = {};

void RegisterGoldCoinScript(App& _app)
{
    DEFAULT_CONFIG_AND_REQUIRED(goldCoinConf, GoldCoin, RectTransform);

    goldCoinConf.DEFAULT_DRAW_INSPECTOR(GoldCoin);

    _app.RegisterScript(goldCoinConf);
}

DEFAULT_UNREGISTER_SCRIPT(goldCoinConf, GoldCoin)

void GoldCoin::Create() {}

void GoldCoin::Ready() {}

void GoldCoin::Destroy() {}

void GoldCoin::Update(float _dt) {}

std::string GoldCoin::GetName()
{
    return "Gold Coin";
}

std::string GoldCoin::GetMessage(Canis::Entity* _interactingEntity)
{
    return std::string("Press E to Pickup ") + GetName();
}

bool GoldCoin::HandleInteraction(Canis::Entity* _interactingEntity)
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
