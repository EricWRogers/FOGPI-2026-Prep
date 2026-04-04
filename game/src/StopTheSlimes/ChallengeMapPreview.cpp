#include <StopTheSlimes/ChallengeMapPreview.hpp>

#include <Canis/AssetManager.hpp>

#include <algorithm>
#include <array>
#include <cstdint>
#include <string>

namespace StopTheSlimes
{
    namespace
    {
        constexpr const char* kDefaultMaterialPath = "assets/sts/materials/default.material";
        constexpr const char* kPortalMaterialPath = "assets/sts/materials/portal.material";
        constexpr const char* kWaterPrefabPath = "assets/prefabs/sts/environment/water.prefab";

        const char* GetGroundTopModelPath(const EnvironmentType _environment)
        {
            switch (_environment)
            {
            case EnvironmentType::Desert:
                return "assets/sts/models/environment/desert/sand_center.obj";
            case EnvironmentType::Lava:
                return "assets/sts/models/environment/lava/lava_ground_center.obj";
            case EnvironmentType::Forest:
            default:
                return "assets/sts/models/environment/grass_center.obj";
            }
        }

        const char* GetSupportBlockModelPath(const EnvironmentType _environment)
        {
            if (_environment == EnvironmentType::Lava)
                return "assets/sts/models/environment/lava/lava_under_water_center_00.obj";

            return "assets/sts/models/blocks/white_block.obj";
        }

        std::uint32_t HashCell(const MapDefinition& _map, const int _x, const int _y, const int _z)
        {
            std::uint32_t value = static_cast<std::uint32_t>(_map.seed);
            value ^= static_cast<std::uint32_t>(_x * 73856093);
            value ^= static_cast<std::uint32_t>(_y * 19349663);
            value ^= static_cast<std::uint32_t>(_z * 83492791);
            value ^= static_cast<std::uint32_t>(_map.environmentName.size() * 2654435761u);
            return value;
        }

        const char* GetDecorationModelPath(const EnvironmentType _environment, const std::uint32_t _hash)
        {
            static constexpr std::array<const char*, 7> forestModels = {
                "assets/sts/models/tree_00.obj",
                "assets/sts/models/tree_01.obj",
                "assets/sts/models/tree_02.obj",
                "assets/sts/models/tree_03.obj",
                "assets/sts/models/berry_bush.obj",
                "assets/sts/models/mushrooms.obj",
                "assets/sts/models/stump_00.obj",
            };

            static constexpr std::array<const char*, 8> desertModels = {
                "assets/sts/models/environment/desert/cactus_0.obj",
                "assets/sts/models/environment/desert/cactus_1.obj",
                "assets/sts/models/environment/desert/cactus_2.obj",
                "assets/sts/models/environment/desert/cactus_3.obj",
                "assets/sts/models/environment/desert/cactus_4.obj",
                "assets/sts/models/environment/desert/palm_tree.obj",
                "assets/sts/models/environment/desert/desert_rock_1.obj",
                "assets/sts/models/environment/desert/bull_skull.obj",
            };

            static constexpr std::array<const char*, 6> lavaModels = {
                "assets/sts/models/environment/lava/lava_rock_0.obj",
                "assets/sts/models/environment/lava/lava_rock_1.obj",
                "assets/sts/models/environment/lava/lava_rock_2.obj",
                "assets/sts/models/environment/lava/lava_rock_3.obj",
                "assets/sts/models/environment/lava/lava_rock_4.obj",
                "assets/sts/models/environment/lava/lava_rock_5.obj",
            };

            switch (_environment)
            {
            case EnvironmentType::Desert:
                return desertModels[_hash % desertModels.size()];
            case EnvironmentType::Lava:
                return lavaModels[_hash % lavaModels.size()];
            case EnvironmentType::Forest:
            default:
                return forestModels[_hash % forestModels.size()];
            }
        }

        Canis::Vector3 GetDecorationScale(const EnvironmentType _environment, const std::uint32_t _hash)
        {
            const float variation = 0.8f + (static_cast<float>(_hash % 40u) * 0.01f);
            switch (_environment)
            {
            case EnvironmentType::Desert:
                return Canis::Vector3(0.9f + (variation * 0.2f));
            case EnvironmentType::Lava:
                return Canis::Vector3(0.6f + (variation * 0.15f));
            case EnvironmentType::Forest:
            default:
                return Canis::Vector3(0.75f + (variation * 0.25f));
            }
        }

        void SetLocalTransform(
            Canis::Transform& _transform,
            const Canis::Vector3& _position,
            const Canis::Vector3& _rotation = Canis::Vector3(0.0f),
            const Canis::Vector3& _scale = Canis::Vector3(1.0f))
        {
            _transform.position = _position;
            _transform.rotation = _rotation;
            _transform.scale = _scale;
            _transform.active = true;
        }

        Canis::Entity* SpawnModelEntity(
            ChallengeMapPreview& _preview,
            Canis::Scene& _scene,
            Canis::Entity* _parent,
            const std::string& _name,
            const Canis::Vector3& _localPosition,
            const char* _modelPath,
            const char* _materialPath,
            const Canis::Vector3& _rotation = Canis::Vector3(0.0f),
            const Canis::Vector3& _scale = Canis::Vector3(1.0f),
            const Canis::Color& _color = Canis::Color(1.0f))
        {
            Canis::Entity* entity = _scene.CreateEntity(_name);
            Canis::Transform& transform = *entity->AddComponent<Canis::Transform>();
            if (_parent != nullptr)
                transform.SetParent(_parent);
            SetLocalTransform(transform, _localPosition, _rotation, _scale);

            Canis::Model& model = *entity->AddComponent<Canis::Model>();
            model.modelId = Canis::AssetManager::LoadModel(_modelPath);
            model.color = _color;

            Canis::Material& material = *entity->AddComponent<Canis::Material>();
            material.materialId = Canis::AssetManager::LoadMaterial(_materialPath);
            material.color = _color;

            _preview.entities.push_back(entity);
            return entity;
        }

        void SpawnTowerPreview(
            ChallengeMapPreview& _preview,
            Canis::Scene& _scene,
            Canis::Entity* _parent,
            const TileType _tileType,
            const Canis::Vector3& _localPosition)
        {
            const char* modelPath = nullptr;
            switch (_tileType)
            {
            case TileType::GemMineTower:
                modelPath = "assets/sts/models/towers/gem_mine_tower.obj";
                break;
            case TileType::FireTower:
                modelPath = "assets/sts/models/towers/fire_crystal_tower.obj";
                break;
            case TileType::IceTower:
                modelPath = "assets/sts/models/towers/ice_crystal_tower.obj";
                break;
            case TileType::SpikeTower:
                modelPath = "assets/sts/models/towers/tree_tower.obj";
                break;
            default:
                break;
            }

            if (modelPath == nullptr)
                return;

            (void)SpawnModelEntity(
                _preview,
                _scene,
                _parent,
                "PreviewTower",
                _localPosition + Canis::Vector3(0.0f, -0.5f, 0.0f),
                modelPath,
                kDefaultMaterialPath);
        }
    }

    void ChallengeMapPreview::SetActive(const bool _active) const
    {
        for (Canis::Entity* entity : entities)
        {
            if (entity == nullptr)
                continue;

            entity->active = _active;

            if (entity->HasComponent<Canis::Transform>())
                entity->GetComponent<Canis::Transform>().active = _active;
        }
    }

    void ConfigureChallengePresentation(
        Canis::Scene& _scene,
        const MapDefinition& _map,
        const std::vector<Canis::Entity*>& _anchors)
    {
        if (Canis::Entity* lightEntity = _scene.GetEntityWithTag("DIRECTIONLIGHT"))
        {
            if (!lightEntity->HasComponent<Canis::Transform>())
                lightEntity->AddComponent<Canis::Transform>();
            if (!lightEntity->HasComponent<Canis::DirectionalLight>())
                lightEntity->AddComponent<Canis::DirectionalLight>();

            lightEntity->active = true;

            Canis::Transform& lightTransform = lightEntity->GetComponent<Canis::Transform>();
            lightTransform.position = _map.directionalLightPosition;
            lightTransform.active = true;

            Canis::DirectionalLight& light = lightEntity->GetComponent<Canis::DirectionalLight>();
            light.enabled = true;
            light.color = Canis::Color(_map.diffuseLight.r, _map.diffuseLight.g, _map.diffuseLight.b, 1.0f);
            light.intensity = 1.0f;

            Canis::Vector3 direction = _map.directionalLightTarget - _map.directionalLightPosition;
            if (glm::length(direction) > 0.0001f)
                light.direction = glm::normalize(direction);
        }

        if (!_map.water || _anchors.empty())
            return;

        std::vector<Canis::Entity*> waterEntities = _scene.Instantiate(Canis::SceneAssetHandle{
            .path = kWaterPrefabPath,
        });

        if (waterEntities.empty() || waterEntities.front() == nullptr)
            return;

        Canis::Entity* water = waterEntities.front();
        water->name = "ChallengeWater";
        water->active = true;

        if (!water->HasComponent<Canis::Transform>())
            water->AddComponent<Canis::Transform>();

        float minX = _anchors.front()->GetComponent<Canis::Transform>().GetGlobalPosition().x;
        float maxX = minX;
        float minZ = _anchors.front()->GetComponent<Canis::Transform>().GetGlobalPosition().z;
        float maxZ = minZ;

        for (Canis::Entity* anchor : _anchors)
        {
            if (anchor == nullptr || !anchor->HasComponent<Canis::Transform>())
                continue;

            const Canis::Vector3 position = anchor->GetComponent<Canis::Transform>().GetGlobalPosition();
            minX = std::min(minX, position.x);
            maxX = std::max(maxX, position.x);
            minZ = std::min(minZ, position.z);
            maxZ = std::max(maxZ, position.z);
        }

        const float spanX = std::max(25.0f, (maxX - minX) + static_cast<float>(_map.width) + 40.0f);
        const float spanZ = std::max(25.0f, (maxZ - minZ) + static_cast<float>(_map.depth) + 40.0f);

        Canis::Transform& waterTransform = water->GetComponent<Canis::Transform>();
        waterTransform.position = Canis::Vector3(
            (minX + maxX) * 0.5f,
            _map.waterLevel,
            (minZ + maxZ) * 0.5f);
        waterTransform.scale = Canis::Vector3(spanX / 25.0f, 1.0f, spanZ / 25.0f);
        waterTransform.active = true;

        if (water->HasComponent<Canis::SphereCollider>())
            water->GetComponent<Canis::SphereCollider>().radius = std::max(spanX, spanZ);
    }

    ChallengeMapPreview BuildChallengeMapPreview(
        Canis::Scene& _scene,
        const MapDefinition& _map,
        const Canis::Vector3& _anchorPosition,
        const std::string& _name)
    {
        ChallengeMapPreview preview = {};

        if (!_map.valid || _map.width <= 0 || _map.depth <= 0 || _map.height <= 0)
            return preview;

        Canis::Entity* root = _scene.CreateEntity(_name);
        root->active = true;
        Canis::Transform& rootTransform = *root->AddComponent<Canis::Transform>();
        rootTransform.position = Canis::Vector3(
            _anchorPosition.x - (static_cast<float>(_map.width) * 0.5f),
            0.0f,
            _anchorPosition.z - (static_cast<float>(_map.depth) * 0.5f));
        rootTransform.rotation = Canis::Vector3(0.0f);
        rootTransform.scale = Canis::Vector3(1.0f);
        rootTransform.active = true;

        preview.root = root;
        preview.entities.push_back(root);

        for (int x = 0; x < _map.width; ++x)
        {
            for (int z = 0; z < _map.depth; ++z)
            {
                const int topLayer = _map.GetTopFilledLayer(x, z);
                if (topLayer < _map.below)
                    continue;

                for (int y = _map.below; y <= topLayer; ++y)
                {
                    const int tileValue = _map.GetTile(x, y, z);
                    if (tileValue == 0)
                        continue;

                    const TileType tileType = static_cast<TileType>(tileValue);
                    const Canis::Vector3 cellPosition(
                        static_cast<float>(x),
                        static_cast<float>(y),
                        static_cast<float>(z));

                    (void)SpawnModelEntity(
                        preview,
                        _scene,
                        root,
                        "PreviewBlock",
                        cellPosition,
                        GetSupportBlockModelPath(_map.environment),
                        kDefaultMaterialPath);

                    if (y != topLayer)
                        continue;

                    (void)SpawnModelEntity(
                        preview,
                        _scene,
                        root,
                        "PreviewGround",
                        cellPosition,
                        GetGroundTopModelPath(_map.environment),
                        kDefaultMaterialPath);

                    if (tileType == TileType::Dirt)
                    {
                        (void)SpawnModelEntity(
                            preview,
                            _scene,
                            root,
                            "PreviewRoad",
                            cellPosition + Canis::Vector3(0.0f, 0.02f, 0.0f),
                            "assets/sts/models/roads/road_000.obj",
                            kDefaultMaterialPath);
                    }
                    else if (tileType == TileType::TreeGroup0)
                    {
                        const std::uint32_t hash = HashCell(_map, x, y, z);
                        const float offsetX = ((hash & 3u) - 1.5f) * 0.12f;
                        const float offsetZ = (((hash >> 2u) & 3u) - 1.5f) * 0.12f;
                        (void)SpawnModelEntity(
                            preview,
                            _scene,
                            root,
                            "PreviewDecoration",
                            cellPosition + Canis::Vector3(offsetX, -0.5f, offsetZ),
                            GetDecorationModelPath(_map.environment, hash),
                            kDefaultMaterialPath,
                            Canis::Vector3(0.0f, static_cast<float>(hash % 360u) * Canis::DEG2RAD, 0.0f),
                            GetDecorationScale(_map.environment, hash));
                    }
                    else if (tileType == TileType::Portal)
                    {
                        (void)SpawnModelEntity(
                            preview,
                            _scene,
                            root,
                            "PreviewPortalBase",
                            cellPosition + Canis::Vector3(0.0f, -0.5f, 0.0f),
                            "assets/sts/models/portal_base.obj",
                            kDefaultMaterialPath);
                        (void)SpawnModelEntity(
                            preview,
                            _scene,
                            root,
                            "PreviewPortalFrame",
                            cellPosition + Canis::Vector3(0.0f, -0.48f, 0.0f),
                            "assets/sts/models/portal_frame.obj",
                            kDefaultMaterialPath);
                        (void)SpawnModelEntity(
                            preview,
                            _scene,
                            root,
                            "PreviewPortalWarp",
                            cellPosition + Canis::Vector3(0.0f, -0.47f, 0.0f),
                            "assets/sts/models/portal_warp.obj",
                            kPortalMaterialPath);
                    }
                    else if (tileType == TileType::Castle)
                    {
                        (void)SpawnModelEntity(
                            preview,
                            _scene,
                            root,
                            "PreviewCastle",
                            cellPosition + Canis::Vector3(0.0f, -0.5f, 0.0f),
                            "assets/sts/models/gate_arch.obj",
                            kDefaultMaterialPath,
                            Canis::Vector3(0.0f),
                            Canis::Vector3(0.5f));
                    }
                    else
                    {
                        SpawnTowerPreview(preview, _scene, root, tileType, cellPosition);
                    }
                }
            }
        }

        return preview;
    }
}
