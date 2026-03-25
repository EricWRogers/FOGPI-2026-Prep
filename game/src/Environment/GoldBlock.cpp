#include <Environment/GoldBlock.hpp>

#include <Canis/App.hpp>
#include <Canis/ConfigHelper.hpp>

ScriptConf goldBlockConf = {};

void RegisterGoldBlockScript(App& _app)
{
    DEFAULT_CONFIG_AND_REQUIRED(goldBlockConf, GoldBlock, RectTransform);

    goldBlockConf.DEFAULT_DRAW_INSPECTOR(GoldBlock);

    _app.RegisterScript(goldBlockConf);
}

DEFAULT_UNREGISTER_SCRIPT(goldBlockConf, GoldBlock)

void GoldBlock::Create() {}

void GoldBlock::Ready() {}

void GoldBlock::Destroy() {}

void GoldBlock::Update(float _dt) {}

std::string GoldBlock::GetName()
{
    return "Gold Block";
}

std::string GoldBlock::GetMessage()
{
    return GetName();
}

bool GoldBlock::HandleInteraction()
{
    return false;
}
