#include <Environment/UraniumBlock.hpp>

#include <Canis/App.hpp>
#include <Canis/ConfigHelper.hpp>

ScriptConf uraniumBlockConf = {};

void RegisterUraniumBlockScript(App& _app)
{
    DEFAULT_CONFIG_AND_REQUIRED(uraniumBlockConf, UraniumBlock, RectTransform);

    uraniumBlockConf.DEFAULT_DRAW_INSPECTOR(UraniumBlock);

    _app.RegisterScript(uraniumBlockConf);
}

DEFAULT_UNREGISTER_SCRIPT(uraniumBlockConf, UraniumBlock)

void UraniumBlock::Create() {}

void UraniumBlock::Ready() {}

void UraniumBlock::Destroy() {}

void UraniumBlock::Update(float _dt) {}

std::string UraniumBlock::GetName()
{
    return "Uranium Block";
}

std::string UraniumBlock::GetMessage()
{
    return GetName();
}

bool UraniumBlock::HandleInteraction()
{
    return false;
}
