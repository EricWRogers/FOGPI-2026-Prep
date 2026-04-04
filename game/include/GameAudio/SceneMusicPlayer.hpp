#pragma once

#include <Canis/Entity.hpp>

namespace GameAudio
{
    class SceneMusicPlayer : public Canis::ScriptableEntity
    {
    public:
        static constexpr const char* ScriptName = "GameAudio::SceneMusicPlayer";

        Canis::AudioAssetHandle music = {};
        float volume = 0.6f;
        int loops = -1;
        bool playOnReady = true;
        bool stopOnDestroy = true;

        explicit SceneMusicPlayer(Canis::Entity& _entity) : Canis::ScriptableEntity(_entity) {}

        void Create() override;
        void Ready() override;
        void Destroy() override;
        void Update(float _dt) override;
    };

    void RegisterSceneMusicPlayerScript(Canis::App& _app);
    void UnRegisterSceneMusicPlayerScript(Canis::App& _app);
}
