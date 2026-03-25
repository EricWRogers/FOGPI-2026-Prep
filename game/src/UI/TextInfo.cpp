#include <UI/InfoText.hpp>

#include <Canis/App.hpp>
#include <Canis/ConfigHelper.hpp>
#include <RollABall/PlayerController.hpp>

ScriptConf infoTextConf = {};

void RegisterInfoTextScript(App& _app)
{
    REGISTER_PROPERTY(infoTextConf, InfoText, timer);
    REGISTER_PROPERTY(infoTextConf, InfoText, fadeoutTime);

    DEFAULT_CONFIG_AND_REQUIRED(infoTextConf, InfoText, RectTransform, Text);

    infoTextConf.DEFAULT_DRAW_INSPECTOR(InfoText);

    _app.RegisterScript(infoTextConf);
}

DEFAULT_UNREGISTER_SCRIPT(infoTextConf, InfoText)

void InfoText::Create() {}

void InfoText::Ready() {}

void InfoText::Destroy() {}

void InfoText::Update(float _dt)
{
    if (!entity.HasComponents<RectTransform, Text>())
        return;
    
    Text& text = entity.GetComponent<Text>();

    // this should start the fadeout at 10%
    text.color.a = (timer <= 0.0f) ? 0.0f : std::clamp((timer/fadeoutTime)*10, 0.0f, 1.0f);

    if (timer > 0.0f)
    {
        timer -= _dt;
    }
}

void InfoText::SetText(const std::string &_info)
{
    if (!entity.HasComponents<RectTransform, Text>())
        return;
    
    timer = fadeoutTime;
    Text& text = entity.GetComponent<Text>();
    text.SetText(_info);
}

void InfoText::EarlyFadeout() {
    float earlyFadeout = fadeoutTime * 0.1;
    timer = std::min(timer, earlyFadeout);
}
