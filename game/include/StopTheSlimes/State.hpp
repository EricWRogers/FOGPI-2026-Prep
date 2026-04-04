#pragma once

#include <Canis/App.hpp>
#include <Canis/Entity.hpp>

#include <map>
#include <string>
#include <vector>

namespace StopTheSlimes
{
    struct WorldDefinition
    {
        std::string code = "";
        std::string displayName = "";
        std::string levelPath = "";
        std::string challengeMapPath = "";
    };

    struct LevelSummary
    {
        std::string levelPath = "";
        std::string mapPath = "";
        std::string wavesPath = "";
        int deckCount = 3;
        int waveCount = 0;
        int totalEnemies = 0;
        bool endless = false;
    };

    struct RuntimeState
    {
        int selectedWorldIndex = 0;
        std::string currentLevelPath = "";
        int lastScore = 0;
        std::map<std::string, int> highScores = {};
        bool firstChallengeDialogShown = false;
    };

    RuntimeState& GetRuntimeState();

    const std::vector<WorldDefinition>& GetWorldDefinitions();
    const WorldDefinition& GetWorldDefinition(int _index);
    const WorldDefinition& GetSelectedWorld();
    int GetWorldIndexByCode(const std::string& _code);
    void SelectWorld(int _index);

    std::string GetScenePath(const std::string& _sceneName);
    void LoadScene(Canis::App& _app, const std::string& _sceneName);
    void LoadScene(Canis::Entity& _entity, const std::string& _sceneName);

    std::string GetSupportedAudioPath(const std::string& _path);
    void PlayMenuClick(float _volume = 0.4f);

    LevelSummary LoadLevelSummary(const std::string& _levelPath);

    void SetCurrentLevelPath(const std::string& _levelPath);
    const std::string& GetCurrentLevelPath();

    void SetLastScore(int _score);
    int GetLastScore();
    void SaveHighScore(const std::string& _worldCode, int _score);
    int GetHighScore(const std::string& _worldCode);
}
