#include <Environment/GenerateTerrain.hpp>


#include <Canis/App.hpp>
#include <Canis/ConfigHelper.hpp>
#include <Canis/InputManager.hpp>

ScriptConf generateTerrainConf = {};

void RegisterGenerateTerrainScript(App& _app)
{
    DEFAULT_CONFIG_AND_REQUIRED(generateTerrainConf, GenerateTerrain, Transform);
    
    REGISTER_PROPERTY(generateTerrainConf, GenerateTerrain, rockPrefab);
    REGISTER_PROPERTY(generateTerrainConf, GenerateTerrain, icePrefab);
    REGISTER_PROPERTY(generateTerrainConf, GenerateTerrain, goldPrefab);
    REGISTER_PROPERTY(generateTerrainConf, GenerateTerrain, uraniumPrefab);

    generateTerrainConf.DEFAULT_DRAW_INSPECTOR(GenerateTerrain);

    _app.RegisterScript(generateTerrainConf);
}

DEFAULT_UNREGISTER_SCRIPT(generateTerrainConf, GenerateTerrain)

void GenerateTerrain::Create() {}

void GenerateTerrain::Ready() {
    if (!entity.HasComponent<Transform>())
        return;

    Vector3 spawnOffset = entity.GetComponent<Transform>().GetGlobalPosition();
    
    for (int x = 0; x < 10; x++) {
        for (int z = 0; z < 10; z++) {
            for (int y = 0; y > -5; y--) {
                Vector3 offset = Vector3(x,y,z);

                SceneAssetHandle prefab;

                switch (rand() % 4)
                {
                case 0:
                    prefab = rockPrefab;
                    break;
                case 1:
                    prefab = icePrefab;
                    break;
                case 2:
                    prefab = goldPrefab;
                    break;
                case 3:
                    prefab = uraniumPrefab;
                    break;
                default:
                    break;
                }

                for (Entity *root : entity.scene.Instantiate(prefab))
                {
                    if (root != nullptr && root->HasComponent<Transform>()) {
                        root->GetComponent<Transform>().position += offset + spawnOffset;
                        root->GetComponent<Transform>().SetParent(&entity);
                    }
                }
            }
        }
    }
}

void GenerateTerrain::Destroy() {}

void GenerateTerrain::Update(float _dt) {}
