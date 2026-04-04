#pragma once

#include <Canis/Entity.hpp>

namespace Canis
{
    class App;
    struct UIActionContext;
}

namespace StopTheSlimes
{
    class GameplaySceneController : public Canis::ScriptableEntity
    {
    public:
        static constexpr const char* ScriptName = "StopTheSlimes::GameplaySceneController";

        explicit GameplaySceneController(Canis::Entity& _entity) : Canis::ScriptableEntity(_entity) {}

        void Create() override;
        void Ready() override;
        void Destroy() override;
        void Update(float _dt) override;

        void OnClickBackToMap(const Canis::UIActionContext& _context);
        void OnClickSimulateWin(const Canis::UIActionContext& _context);
        void OnClickSimulateLoss(const Canis::UIActionContext& _context);

    private:
        void RefreshTexts();
        void FinishRun(bool _victory);

        int m_projectedScore = 0;
    };

    void RegisterGameplaySceneControllerScript(Canis::App& _app);
    void UnRegisterGameplaySceneControllerScript(Canis::App& _app);
}
