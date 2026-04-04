#pragma once

#include <Canis/Entity.hpp>

#include <StopTheSlimes/LevelData.hpp>

#include <string>
#include <vector>

namespace StopTheSlimes
{
    struct ChallengeMapPreview
    {
        Canis::Entity* root = nullptr;
        std::vector<Canis::Entity*> entities = {};

        void SetActive(bool _active) const;
    };

    void ConfigureChallengePresentation(Canis::Scene& _scene, const MapDefinition& _map, const std::vector<Canis::Entity*>& _anchors);
    ChallengeMapPreview BuildChallengeMapPreview(
        Canis::Scene& _scene,
        const MapDefinition& _map,
        const Canis::Vector3& _anchorPosition,
        const std::string& _name);
}
