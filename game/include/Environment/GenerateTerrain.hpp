#pragma once

#include <Canis/Entity.hpp>

class GenerateTerrain : public Canis::ScriptableEntity
{
private:

public:
    static constexpr const char* ScriptName = "GenerateTerrain";
    
    Canis::SceneAssetHandle rockPrefab;
    Canis::SceneAssetHandle icePrefab;
    Canis::SceneAssetHandle goldPrefab;
    Canis::SceneAssetHandle uraniumPrefab;

    GenerateTerrain(Canis::Entity &_entity) : Canis::ScriptableEntity(_entity) {}

    void Create();
    void Ready();
    void Destroy();
    void Update(float _dt);
};

extern void RegisterGenerateTerrainScript(Canis::App& _app);
extern void UnRegisterGenerateTerrainScript(Canis::App& _app);
