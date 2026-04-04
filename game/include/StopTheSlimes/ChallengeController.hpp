#pragma once

#include <StopTheSlimes/ChallengeMapPreview.hpp>

#include <Canis/Entity.hpp>

#include <vector>

namespace Canis
{
    class App;
    struct UIActionContext;
}

namespace StopTheSlimes
{
    class ChallengeController : public Canis::ScriptableEntity
    {
    public:
        static constexpr const char* ScriptName = "StopTheSlimes::ChallengeController";

        explicit ChallengeController(Canis::Entity& _entity) : Canis::ScriptableEntity(_entity) {}

        void Create() override;
        void Ready() override;
        void Destroy() override;
        void Update(float _dt) override;

        void OnClickChallengeContinue(const Canis::UIActionContext& _context);
        void OnClickChallengeRight(const Canis::UIActionContext& _context);
        void OnClickChallengeLeft(const Canis::UIActionContext& _context);
        void OnClickMainMenu(const Canis::UIActionContext& _context);

    private:
        void CollectTargets();
        void EnsureCameraRig(const MapDefinition& _map);
        void LoadChallengePreviews();
        void FocusCameraOnTarget(bool _snap = false);
        void UpdateCameraPivot(float _dt);
        void SelectTarget(int _index, bool _snap = false);
        void RefreshTargetVisuals();
        void RefreshQuestPanel();

        std::vector<Canis::Entity*> m_targets = {};
        std::vector<ChallengeMapPreview> m_previews = {};
        int m_currentTarget = 0;
        MapDefinition m_challengeSceneMap = {};
        Canis::Vector3 m_cameraTargetPosition = Canis::Vector3(0.0f);
        bool m_cameraTargetValid = false;
    };

    void RegisterChallengeControllerScript(Canis::App& _app);
    void UnRegisterChallengeControllerScript(Canis::App& _app);
}
