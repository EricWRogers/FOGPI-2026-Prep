#include <StopTheSlimes/LevelData.hpp>

#include <algorithm>
#include <fstream>
#include <map>
#include <string>
#include <vector>

namespace StopTheSlimes
{
    namespace
    {
        struct ParsedLayer
        {
            int layer = 0;
            std::vector<std::vector<int>> rows = {};
        };

        EnvironmentType ParseEnvironment(const std::string& _name)
        {
            if (_name == "desert")
                return EnvironmentType::Desert;
            if (_name == "lava")
                return EnvironmentType::Lava;
            return EnvironmentType::Forest;
        }
    }

    bool MapDefinition::HasTile(const int _x, const int _y, const int _z) const
    {
        if (_x < 0 || _z < 0 || _x >= width || _z >= depth)
            return false;

        if (_y < below || _y >= (below + height))
            return false;

        return true;
    }

    int MapDefinition::GetTile(const int _x, const int _y, const int _z) const
    {
        if (!HasTile(_x, _y, _z))
            return 0;

        const int layerIndex = _y - below;
        const int tileIndex = ((layerIndex * depth) + _z) * width + _x;
        if (tileIndex < 0 || tileIndex >= static_cast<int>(tiles.size()))
            return 0;

        return tiles[static_cast<std::size_t>(tileIndex)];
    }

    int MapDefinition::GetTopFilledLayer(const int _x, const int _z) const
    {
        for (int y = (below + height) - 1; y >= below; --y)
        {
            if (GetTile(_x, y, _z) != 0)
                return y;
        }

        return below - 1;
    }

    int LevelDefinition::GetTotalEnemyCount() const
    {
        int totalEnemies = 0;
        for (const WaveDefinition& wave : waves)
            totalEnemies += static_cast<int>(wave.enemies.size());
        return totalEnemies;
    }

    MapDefinition LoadMapDefinition(const std::string& _path)
    {
        MapDefinition map = {};
        map.path = _path;

        std::ifstream file(_path);
        if (!file.is_open())
            return map;

        std::vector<ParsedLayer> parsedLayers = {};
        std::string token = "";

        while (file >> token)
        {
            if (token == "day_level")
            {
                std::string value = "";
                if (file >> value)
                    map.dayLevel = (value == "true");
            }
            else if (token == "environment")
            {
                if (file >> map.environmentName)
                    map.environment = ParseEnvironment(map.environmentName);
            }
            else if (token == "camera_x")
            {
                file >> map.cameraX;
            }
            else if (token == "camera_y")
            {
                file >> map.cameraY;
            }
            else if (token == "camera_z")
            {
                file >> map.cameraZ;
            }
            else if (token == "camera_pivot_x")
            {
                file >> map.cameraPivot.x;
            }
            else if (token == "camera_pivot_y")
            {
                file >> map.cameraPivot.y;
            }
            else if (token == "camera_pivot_z")
            {
                file >> map.cameraPivot.z;
            }
            else if (token == "camera_pitch")
            {
                file >> map.cameraPitch;
            }
            else if (token == "camera_yaw")
            {
                file >> map.cameraYaw;
            }
            else if (token == "directional_light_pos_x")
            {
                file >> map.directionalLightPosition.x;
            }
            else if (token == "directional_light_pos_y")
            {
                file >> map.directionalLightPosition.y;
            }
            else if (token == "directional_light_pos_z")
            {
                file >> map.directionalLightPosition.z;
            }
            else if (token == "directional_light_target_x")
            {
                file >> map.directionalLightTarget.x;
            }
            else if (token == "directional_light_target_y")
            {
                file >> map.directionalLightTarget.y;
            }
            else if (token == "directional_light_target_z")
            {
                file >> map.directionalLightTarget.z;
            }
            else if (token == "ambient_light_rgb")
            {
                file >> map.ambientLight.r;
                map.ambientLight.g = map.ambientLight.r;
                map.ambientLight.b = map.ambientLight.r;
            }
            else if (token == "ambient_light_r")
            {
                file >> map.ambientLight.r;
            }
            else if (token == "ambient_light_g")
            {
                file >> map.ambientLight.g;
            }
            else if (token == "ambient_light_b")
            {
                file >> map.ambientLight.b;
            }
            else if (token == "diffuse_light_rgb")
            {
                file >> map.diffuseLight.r;
                map.diffuseLight.g = map.diffuseLight.r;
                map.diffuseLight.b = map.diffuseLight.r;
            }
            else if (token == "diffuse_light_r")
            {
                file >> map.diffuseLight.r;
            }
            else if (token == "diffuse_light_g")
            {
                file >> map.diffuseLight.g;
            }
            else if (token == "diffuse_light_b")
            {
                file >> map.diffuseLight.b;
            }
            else if (token == "specular_light_rgb")
            {
                file >> map.specularLight.r;
                map.specularLight.g = map.specularLight.r;
                map.specularLight.b = map.specularLight.r;
            }
            else if (token == "specular_light_r")
            {
                file >> map.specularLight.r;
            }
            else if (token == "specular_light_g")
            {
                file >> map.specularLight.g;
            }
            else if (token == "specular_light_b")
            {
                file >> map.specularLight.b;
            }
            else if (token == "water")
            {
                std::string value = "";
                if (file >> value)
                    map.water = (value == "true");
            }
            else if (token == "water_level")
            {
                file >> map.waterLevel;
            }
            else if (token == "seed")
            {
                file >> map.seed;
            }
            else if (token == "layer")
            {
                ParsedLayer layer = {};
                file >> layer.layer;

                std::vector<int> row = {};
                int value = 0;
                while (file >> value)
                {
                    if (value == -1)
                    {
                        layer.rows.push_back(row);
                        row.clear();
                    }
                    else if (value == -2)
                    {
                        break;
                    }
                    else
                    {
                        row.push_back(value);
                    }
                }

                if (!row.empty())
                    layer.rows.push_back(row);

                parsedLayers.push_back(layer);
            }
        }

        if (parsedLayers.empty())
            return map;

        int topLayer = parsedLayers.front().layer;
        map.below = parsedLayers.front().layer;
        map.width = 0;
        map.depth = 0;

        for (const ParsedLayer& layer : parsedLayers)
        {
            map.below = std::min(map.below, layer.layer);
            topLayer = std::max(topLayer, layer.layer);
            map.depth = std::max(map.depth, static_cast<int>(layer.rows.size()));
            for (const std::vector<int>& row : layer.rows)
                map.width = std::max(map.width, static_cast<int>(row.size()));
        }

        map.height = (topLayer - map.below) + 1;
        map.tiles.assign(static_cast<std::size_t>(map.width * map.depth * map.height), 0);

        for (const ParsedLayer& layer : parsedLayers)
        {
            const int y = layer.layer - map.below;
            for (int z = 0; z < static_cast<int>(layer.rows.size()); ++z)
            {
                const std::vector<int>& row = layer.rows[static_cast<std::size_t>(z)];
                for (int x = 0; x < static_cast<int>(row.size()); ++x)
                {
                    const int index = ((y * map.depth) + z) * map.width + x;
                    map.tiles[static_cast<std::size_t>(index)] = row[static_cast<std::size_t>(x)];
                }
            }
        }

        map.valid = true;
        return map;
    }

    std::vector<WaveDefinition> LoadWaveDefinitions(const std::string& _path)
    {
        std::vector<WaveDefinition> waves = {};

        std::ifstream file(_path);
        if (!file.is_open())
            return waves;

        std::string token = "";
        while (file >> token)
        {
            if (token == "delay")
            {
                WaveDefinition wave = {};
                file >> wave.delay;
                waves.push_back(wave);
            }
            else if (token == "enemies" && !waves.empty())
            {
                int enemyType = 0;
                while (file >> enemyType)
                {
                    if (enemyType == -1)
                        break;

                    waves.back().enemies.push_back(enemyType);
                }
            }
        }

        return waves;
    }

    LevelDefinition LoadLevelDefinition(const std::string& _path)
    {
        LevelDefinition level = {};
        level.levelPath = _path;

        std::ifstream file(_path);
        if (!file.is_open())
            return level;

        std::string token = "";
        while (file >> token)
        {
            if (token == "endless")
            {
                std::string value = "";
                if (file >> value)
                    level.endless = (value == "true");
            }
            else if (token == "override_starting_hand_size")
            {
                file >> level.overrideStartingHandSize;
            }
            else if (token == "override_starting_energy")
            {
                file >> level.overrideStartingEnergy;
            }
            else if (token == "override_round_over_energy")
            {
                file >> level.overrideRoundOverEnergy;
            }
            else if (token == "override_cards_per_round")
            {
                file >> level.overrideCardsPerRound;
            }
            else if (token == "map")
            {
                file >> level.mapPath;
            }
            else if (token == "waves")
            {
                file >> level.wavesPath;
            }
            else if (token == "next_level")
            {
                file >> level.nextLevel;
            }
            else if (token == "deck_count")
            {
                file >> level.deckCount;
            }
            else if (token == "reward_options")
            {
                RewardOptionDefinition rewardOption = {};
                while (file >> token)
                {
                    if (token == "-1")
                        break;

                    if (token == "show_number")
                    {
                        file >> rewardOption.showNumber;
                    }
                    else if (token == "pick_count")
                    {
                        file >> rewardOption.pickCount;
                    }
                    else
                    {
                        rewardOption.rewards.push_back(token);
                    }
                }

                level.rewardOptions.push_back(rewardOption);
            }
        }

        level.map = LoadMapDefinition(level.mapPath);
        level.waves = LoadWaveDefinitions(level.wavesPath);
        level.valid = !level.mapPath.empty();
        return level;
    }
}
