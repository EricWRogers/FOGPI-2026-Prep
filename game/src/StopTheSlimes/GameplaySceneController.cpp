#include <StopTheSlimes/GameplaySceneController.hpp>

#include <StopTheSlimes/State.hpp>

#include <Canis/AudioManager.hpp>
#include <Canis/ConfigHelper.hpp>
#include <Canis/Data/Key.hpp>
#include <Canis/InputManager.hpp>

#include <algorithm>

namespace StopTheSlimes
{
    namespace
    {
        Canis::ScriptConf g_gameplaySceneControllerConf = {};

        void SetText(Canis::Entity* _entity, const std::string& _value)
        {
            if (_entity == nullptr || !_entity->HasComponent<Canis::Text>())
                return;

            _entity->GetComponent<Canis::Text>().SetText(_value);
        }
    }

    void RegisterGameplaySceneControllerScript(Canis::App& _app)
    {
        DEFAULT_CONFIG(g_gameplaySceneControllerConf, StopTheSlimes::GameplaySceneController);
        RegisterUIAction<StopTheSlimes::GameplaySceneController>(g_gameplaySceneControllerConf, "OnClickBackToMap", &StopTheSlimes::GameplaySceneController::OnClickBackToMap);
        RegisterUIAction<StopTheSlimes::GameplaySceneController>(g_gameplaySceneControllerConf, "OnClickSimulateWin", &StopTheSlimes::GameplaySceneController::OnClickSimulateWin);
        RegisterUIAction<StopTheSlimes::GameplaySceneController>(g_gameplaySceneControllerConf, "OnClickSimulateLoss", &StopTheSlimes::GameplaySceneController::OnClickSimulateLoss);
        g_gameplaySceneControllerConf.DEFAULT_DRAW_INSPECTOR(StopTheSlimes::GameplaySceneController);
        _app.RegisterScript(g_gameplaySceneControllerConf);
    }

    DEFAULT_UNREGISTER_SCRIPT(g_gameplaySceneControllerConf, GameplaySceneController)

    void GameplaySceneController::Create() {}

    void GameplaySceneController::Ready()
    {
        if (GetCurrentLevelPath().empty())
            SetCurrentLevelPath(GetSelectedWorld().levelPath);

        Canis::AudioManager::PlayMusic(GetSupportedAudioPath("assets/sts/audio/music/AlexandrZhelanovSongs/Brirfing_theme.mp3"), -1, 0.1f);
        RefreshTexts();
    }

    void GameplaySceneController::Destroy() {}

    void GameplaySceneController::Update(float _dt)
    {
        (void)_dt;

        Canis::InputManager& input = entity.scene.GetInputManager();
        if (input.JustPressedKey(Canis::Key::ESCAPE))
            OnClickBackToMap(Canis::UIActionContext{});
        else if (input.JustPressedKey(Canis::Key::ALPHA1))
            OnClickSimulateWin(Canis::UIActionContext{});
        else if (input.JustPressedKey(Canis::Key::ALPHA2))
            OnClickSimulateLoss(Canis::UIActionContext{});
    }

    void GameplaySceneController::OnClickBackToMap(const Canis::UIActionContext& _context)
    {
        (void)_context;
        PlayMenuClick();
        LoadScene(entity, "challenge");
    }

    void GameplaySceneController::OnClickSimulateWin(const Canis::UIActionContext& _context)
    {
        (void)_context;
        PlayMenuClick();
        FinishRun(true);
    }

    void GameplaySceneController::OnClickSimulateLoss(const Canis::UIActionContext& _context)
    {
        (void)_context;
        PlayMenuClick();
        FinishRun(false);
    }

    void GameplaySceneController::RefreshTexts()
    {
        const LevelSummary summary = LoadLevelSummary(GetCurrentLevelPath());
        m_projectedScore = std::max(100, (summary.totalEnemies * 100) + (summary.waveCount * 25));

        SetText(entity.scene.GetEntityWithTag("WORLD_TITLE"), GetSelectedWorld().displayName);
        SetText(entity.scene.GetEntityWithTag("LEVEL_PATH"), GetCurrentLevelPath());
        SetText(
            entity.scene.GetEntityWithTag("LEVEL_DETAILS"),
            "Map: " + summary.mapPath +
                "\nWaves: " + std::to_string(summary.waveCount) +
                "\nEnemies: " + std::to_string(summary.totalEnemies) +
                "\nDeck Count: " + std::to_string(summary.deckCount) +
                "\nEndless: " + std::string(summary.endless ? "Yes" : "No")
        );
        SetText(
            entity.scene.GetEntityWithTag("HELP_TEXT"),
            "This scene is the gameplay entry slice for the port.\n"
            "Press 1 to simulate a win, 2 to simulate a loss, or Esc to return to the island map."
        );
    }

    void GameplaySceneController::FinishRun(const bool _victory)
    {
        const int score = _victory ? m_projectedScore : std::max(25, m_projectedScore / 2);
        SetLastScore(score);
        SaveHighScore(GetSelectedWorld().code, score);
        LoadScene(entity, _victory ? "win" : "game_over");
    }
}
