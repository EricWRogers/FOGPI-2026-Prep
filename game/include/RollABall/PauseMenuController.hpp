#pragma once

#include <Canis/Entity.hpp>

namespace Canis
{
    struct UIActionContext;
}

namespace RollABall
{
    class PauseMenuController : public Canis::ScriptableEntity
    {
    public:
        static constexpr const char* ScriptName = "RollABall::PauseMenuController";
        Canis::Entity* pauseMenu = nullptr;
        Canis::SceneAssetHandle mainMenuScene;
        Canis::AudioAssetHandle uiClickSFX = {};
        float clickVolume = 0.45f;

        explicit PauseMenuController(Canis::Entity& _entity) : Canis::ScriptableEntity(_entity) {}

        void Create() override;
        void Ready() override;
        void Destroy() override;
        void Update(float _dt) override;
        bool UpdateWhenPaused() const override { return true; }

        void ResumeAction(const Canis::UIActionContext& _context);
        void MainMenuAction(const Canis::UIActionContext& _context);
        void QuitAction(const Canis::UIActionContext& _context);

    private:
        float m_resumeTimeScale = 1.0f;
        bool m_restoreMouseLock = false;

        bool IsMenuVisible() const;
        void SetMenuVisible(bool _visible);
    };

    void RegisterPauseMenuControllerScript(Canis::App& _app);
    void UnRegisterPauseMenuControllerScript(Canis::App& _app);
}
