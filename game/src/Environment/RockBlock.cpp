#include <Environment/RockBlock.hpp>

#include <Canis/App.hpp>
#include <Canis/ConfigHelper.hpp>

ScriptConf rockBlockConf = {};

void RegisterRockBlockScript(App& _app)
{
    DEFAULT_CONFIG_AND_REQUIRED(rockBlockConf, RockBlock, RectTransform);

    rockBlockConf.DEFAULT_DRAW_INSPECTOR(RockBlock);

    _app.RegisterScript(rockBlockConf);
}

DEFAULT_UNREGISTER_SCRIPT(rockBlockConf, RockBlock)

void RockBlock::Create() {}

void RockBlock::Ready() {}

void RockBlock::Destroy() {}

void RockBlock::Update(float _dt) {}

std::string RockBlock::GetName()
{
    return "Rock Block";
}

std::string RockBlock::GetMessage()
{
    return GetName();
}

bool RockBlock::HandleInteraction()
{
    return false;
}
