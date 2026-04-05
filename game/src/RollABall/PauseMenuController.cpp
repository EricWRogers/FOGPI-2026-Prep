#include <RollABall/PauseMenuController.hpp>

#include <Canis/App.hpp>
#include <Canis/AudioManager.hpp>
#include <Canis/ConfigHelper.hpp>
#include <Canis/InputManager.hpp>
#include <Canis/Scene.hpp>
#include <Canis/Time.hpp>
#include <Canis/Window.hpp>

namespace RollABall
{
    namespace
    {
        float GetSafeResumeTimeScale(const float _timeScale)
        {
            return _timeScale > 0.0f ? _timeScale : 1.0f;
        }
    }

    ScriptConf pauseMenuConf = {};

    void RegisterPauseMenuControllerScript(Canis::App& _app)
    {
        REGISTER_PROPERTY(pauseMenuConf, RollABall::PauseMenuController, pauseMenu);
        REGISTER_PROPERTY(pauseMenuConf, RollABall::PauseMenuController, mainMenuScene);
        REGISTER_PROPERTY(pauseMenuConf, RollABall::PauseMenuController, uiClickSFX);
        REGISTER_PROPERTY(pauseMenuConf, RollABall::PauseMenuController, clickVolume);

        DEFAULT_CONFIG_AND_REQUIRED(pauseMenuConf, RollABall::PauseMenuController, Canis::Canvas);

        RegisterUIAction<RollABall::PauseMenuController>(pauseMenuConf, "resume", &RollABall::PauseMenuController::ResumeAction);
        RegisterUIAction<RollABall::PauseMenuController>(pauseMenuConf, "MainMenu", &RollABall::PauseMenuController::MainMenuAction);
        RegisterUIAction<RollABall::PauseMenuController>(pauseMenuConf, "quit", &RollABall::PauseMenuController::QuitAction);

        pauseMenuConf.DEFAULT_DRAW_INSPECTOR(RollABall::PauseMenuController);

        _app.RegisterScript(pauseMenuConf);
    }

    DEFAULT_UNREGISTER_SCRIPT(pauseMenuConf, PauseMenuController)

    void PauseMenuController::Create()
    {
        
    }

    void PauseMenuController::Ready()
    {
        if (pauseMenu)
            pauseMenu->active = false;
        m_resumeTimeScale = 1.0f;
        m_restoreMouseLock = false;
    }

    void PauseMenuController::Destroy() {}

    bool PauseMenuController::IsMenuVisible() const
    {
        if (pauseMenu)
            return pauseMenu->active;
        
        return false;
    }

    void PauseMenuController::SetMenuVisible(const bool _visible)
    {
        if (!entity.HasComponent<Canis::Canvas>())
            return;

        if (pauseMenu == nullptr)
            return;
        
        if (pauseMenu->active == _visible)
            return;
        
        Canis::Canvas& canvas = entity.GetComponent<Canis::Canvas>();
        (void)canvas;

        if (_visible)
        {
            m_resumeTimeScale = GetSafeResumeTimeScale(Canis::Time::GetTimeScale());
            m_restoreMouseLock = entity.scene.GetWindow().IsMouseLocked();

            pauseMenu->active = true;
            entity.scene.SetPaused(true);
            Canis::Time::SetTimeScale(0.0f);

            if (m_restoreMouseLock)
                entity.scene.GetWindow().LockMouse(false);
        }
        else
        {
            pauseMenu->active = false;
            entity.scene.SetPaused(false);
            Canis::Time::SetTimeScale(GetSafeResumeTimeScale(m_resumeTimeScale));
            entity.scene.GetWindow().LockMouse(m_restoreMouseLock);
            m_restoreMouseLock = false;
        }

        Canis::AudioManager::PlaySFX(uiClickSFX, clickVolume);
    }

    void PauseMenuController::Update(float)
    {
        if (entity.scene.GetInputManager().JustPressedKey(Canis::Key::ESCAPE))
            SetMenuVisible(!IsMenuVisible());
    }

    void PauseMenuController::ResumeAction(const Canis::UIActionContext&)
    {
        SetMenuVisible(false);
    }

    void PauseMenuController::MainMenuAction(const Canis::UIActionContext&)
    {
        SetMenuVisible(false);
        entity.scene.app->LoadScene(mainMenuScene);
    }

    void PauseMenuController::QuitAction(const Canis::UIActionContext&)
    {
        SetMenuVisible(false);
        entity.scene.QuitGame();
    }
}
