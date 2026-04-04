#pragma once

#include <Canis/Math.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace StopTheSlimes
{
    enum class EnvironmentType
    {
        Forest,
        Desert,
        Lava,
    };

    enum class TileType : int
    {
        None = 0,
        Grass = 1,
        Dirt = 2,
        Portal = 3,
        Castle = 4,
        TreeGroup0 = 5,
        FishWavePoints = 6,
        T0 = 7,
        T1 = 8,
        T2 = 9,
        Spike = 10,
        GemMineTower = 11,
        FireTower = 12,
        IceTower = 13,
        SpikeTower = 14,
        CauldronTower = 15,
        CannonTower = 16,
        PoisonTower = 17,
        SeagullTower = 18,
        BallistaTower = 19,
        LightningTower = 20,
        BlockBlockPlacement = 21,
    };

    struct RewardOptionDefinition
    {
        int showNumber = 3;
        int pickCount = 1;
        std::vector<std::string> rewards = {};
    };

    struct WaveDefinition
    {
        float delay = 0.0f;
        std::vector<int> enemies = {};
    };

    struct MapDefinition
    {
        std::string path = "";
        bool valid = false;
        bool dayLevel = true;
        EnvironmentType environment = EnvironmentType::Forest;
        std::string environmentName = "forest";
        Canis::Vector3 directionalLightPosition = Canis::Vector3(0.0f, 30.0f, 30.0f);
        Canis::Vector3 directionalLightTarget = Canis::Vector3(12.0f, 0.0f, 15.0f);
        Canis::Color ambientLight = Canis::Color(0.1f, 0.1f, 0.1f, 1.0f);
        Canis::Color diffuseLight = Canis::Color(0.961f, 0.953f, 0.863f, 1.0f);
        Canis::Color specularLight = Canis::Color(0.6f, 0.6f, 0.6f, 1.0f);
        float cameraX = -30.0f;
        float cameraY = 30.0f;
        float cameraZ = 4.0f;
        Canis::Vector3 cameraPivot = Canis::Vector3(11.5f, 0.5f, 14.0f);
        float cameraPitch = 0.0f;
        float cameraYaw = 0.0f;
        bool water = true;
        float waterLevel = 0.0f;
        int width = 0;
        int depth = 0;
        int below = 0;
        int height = 0;
        std::vector<int> tiles = {};
        std::uint64_t seed = 0u;

        bool HasTile(int _x, int _y, int _z) const;
        int GetTile(int _x, int _y, int _z) const;
        int GetTopFilledLayer(int _x, int _z) const;
    };

    struct LevelDefinition
    {
        std::string levelPath = "";
        bool valid = false;
        bool endless = false;
        std::string mapPath = "";
        std::string wavesPath = "";
        std::string nextLevel = "";
        int deckCount = 3;
        int overrideStartingHandSize = -1;
        int overrideStartingEnergy = -1;
        int overrideRoundOverEnergy = -1;
        int overrideCardsPerRound = -1;
        std::vector<RewardOptionDefinition> rewardOptions = {};
        std::vector<WaveDefinition> waves = {};
        MapDefinition map = {};

        int GetTotalEnemyCount() const;
    };

    MapDefinition LoadMapDefinition(const std::string& _path);
    std::vector<WaveDefinition> LoadWaveDefinitions(const std::string& _path);
    LevelDefinition LoadLevelDefinition(const std::string& _path);
}
