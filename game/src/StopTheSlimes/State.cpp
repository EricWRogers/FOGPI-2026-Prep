#include <StopTheSlimes/State.hpp>

#include <StopTheSlimes/LevelData.hpp>

#include <Canis/AudioManager.hpp>

#include <algorithm>
#include <filesystem>
#include <stdexcept>

namespace StopTheSlimes
{
    namespace
    {
        const std::vector<WorldDefinition> g_worldDefinitions = {
            {
                .code = "demo_forest",
                .displayName = "Forest Island",
                .levelPath = "assets/sts/configs/levels/demo_rouge_light_level_01.config",
                .challengeMapPath = "assets/sts/configs/maps/demo_challenge.map",
            },
            {
                .code = "demo_desert",
                .displayName = "Desert Island",
                .levelPath = "assets/sts/configs/levels/demo_rouge_light_level_04.config",
                .challengeMapPath = "assets/sts/configs/maps/demo_challenge_desert.map",
            },
            {
                .code = "demo_forest_boss",
                .displayName = "Forest Boss",
                .levelPath = "assets/sts/configs/levels/demo_rouge_light_level_07.config",
                .challengeMapPath = "assets/sts/configs/maps/demo_challenge_boss.map",
            },
            {
                .code = "slimes_flight",
                .displayName = "Slimes Flight",
                .levelPath = "assets/sts/configs/levels/slimes_flight_level_01.config",
                .challengeMapPath = "assets/sts/configs/maps/challenge_slimes_flight.map",
            },
            {
                .code = "endless_lava",
                .displayName = "Endless Lava",
                .levelPath = "assets/sts/configs/levels/endless_lava.config",
                .challengeMapPath = "assets/sts/configs/maps/demo_challenge_lava.map",
            },
            {
                .code = "big_green_army",
                .displayName = "Big Green Army",
                .levelPath = "assets/sts/configs/levels/big_green_army.config",
                .challengeMapPath = "assets/sts/configs/maps/demo_challenge_big_slime_army.map",
            },
        };

        RuntimeState g_runtimeState = {};

        bool FileExists(const std::string& _path)
        {
            return !_path.empty() && std::filesystem::exists(_path);
        }
    }

    RuntimeState& GetRuntimeState()
    {
        return g_runtimeState;
    }

    const std::vector<WorldDefinition>& GetWorldDefinitions()
    {
        return g_worldDefinitions;
    }

    const WorldDefinition& GetWorldDefinition(int _index)
    {
        if (g_worldDefinitions.empty())
            throw std::runtime_error("StopTheSlimes world definitions are empty.");

        const int clampedIndex = std::clamp(_index, 0, static_cast<int>(g_worldDefinitions.size()) - 1);
        return g_worldDefinitions[static_cast<std::size_t>(clampedIndex)];
    }

    const WorldDefinition& GetSelectedWorld()
    {
        return GetWorldDefinition(GetRuntimeState().selectedWorldIndex);
    }

    int GetWorldIndexByCode(const std::string& _code)
    {
        for (int i = 0; i < static_cast<int>(g_worldDefinitions.size()); ++i)
        {
            if (g_worldDefinitions[static_cast<std::size_t>(i)].code == _code)
                return i;
        }

        return 0;
    }

    void SelectWorld(int _index)
    {
        GetRuntimeState().selectedWorldIndex = std::clamp(_index, 0, static_cast<int>(g_worldDefinitions.size()) - 1);
    }

    std::string GetScenePath(const std::string& _sceneName)
    {
        if (_sceneName.ends_with(".scene"))
            return _sceneName;

        return "assets/scenes/sts/" + _sceneName + ".scene";
    }

    void LoadScene(Canis::App& _app, const std::string& _sceneName)
    {
        _app.LoadScene(GetScenePath(_sceneName));
    }

    void LoadScene(Canis::Entity& _entity, const std::string& _sceneName)
    {
        if (_entity.scene.app == nullptr)
            return;

        LoadScene(*_entity.scene.app, _sceneName);
    }

    std::string GetSupportedAudioPath(const std::string& _path)
    {
        if (_path.ends_with(".mp3"))
        {
            const std::string oggPath = _path.substr(0, _path.size() - 4) + ".ogg";
            if (FileExists(oggPath))
                return oggPath;
        }

        return _path;
    }

    void PlayMenuClick(const float _volume)
    {
        Canis::AudioManager::PlaySFX("assets/sts/audio/sounds/click1.wav", _volume);
    }

    LevelSummary LoadLevelSummary(const std::string& _levelPath)
    {
        LevelSummary summary = {};
        const LevelDefinition level = LoadLevelDefinition(_levelPath);
        summary.levelPath = level.levelPath;
        summary.mapPath = level.mapPath;
        summary.wavesPath = level.wavesPath;
        summary.deckCount = level.deckCount;
        summary.waveCount = static_cast<int>(level.waves.size());
        summary.totalEnemies = level.GetTotalEnemyCount();
        summary.endless = level.endless;
        return summary;
    }

    void SetCurrentLevelPath(const std::string& _levelPath)
    {
        GetRuntimeState().currentLevelPath = _levelPath;
    }

    const std::string& GetCurrentLevelPath()
    {
        return GetRuntimeState().currentLevelPath;
    }

    void SetLastScore(const int _score)
    {
        GetRuntimeState().lastScore = std::max(0, _score);
    }

    int GetLastScore()
    {
        return GetRuntimeState().lastScore;
    }

    void SaveHighScore(const std::string& _worldCode, const int _score)
    {
        RuntimeState& state = GetRuntimeState();
        const auto it = state.highScores.find(_worldCode);
        if (it == state.highScores.end())
        {
            state.highScores[_worldCode] = std::max(0, _score);
            return;
        }

        it->second = std::max(it->second, std::max(0, _score));
    }

    int GetHighScore(const std::string& _worldCode)
    {
        RuntimeState& state = GetRuntimeState();
        const auto it = state.highScores.find(_worldCode);
        return (it == state.highScores.end()) ? 0 : it->second;
    }
}
