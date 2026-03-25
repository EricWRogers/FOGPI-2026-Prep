#include <Environment/IceBlock.hpp>

#include <Canis/App.hpp>
#include <Canis/ConfigHelper.hpp>

ScriptConf iceBlockConf = {};

void RegisterIceBlockScript(App& _app)
{
    DEFAULT_CONFIG_AND_REQUIRED(iceBlockConf, IceBlock, RectTransform);

    iceBlockConf.DEFAULT_DRAW_INSPECTOR(IceBlock);

    _app.RegisterScript(iceBlockConf);
}

DEFAULT_UNREGISTER_SCRIPT(iceBlockConf, IceBlock)

void IceBlock::Create() {}

void IceBlock::Ready() {}

void IceBlock::Destroy() {}

void IceBlock::Update(float _dt) {}

std::string IceBlock::GetName()
{
    return "Ice Block";
}

std::string IceBlock::GetMessage()
{
    return GetName();
}

bool IceBlock::HandleInteraction()
{
    return false;
}
