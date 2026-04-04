#include <StopTheSlimes/ChallengeController.hpp>

#include <StopTheSlimes/Frontend.hpp>
#include <StopTheSlimes/State.hpp>

#include <Canis/AudioManager.hpp>
#include <Canis/ConfigHelper.hpp>
#include <Canis/Data/Key.hpp>
#include <Canis/InputManager.hpp>

#include <algorithm>
#include <array>
#include <cmath>

namespace StopTheSlimes
{
    namespace
    {
        Canis::ScriptConf g_challengeControllerConf = {};
        constexpr const char* kChallengeCameraPivotTag = "CAMERAPIVOT";
        constexpr const char* kChallengeCameraName = "ChallengeCamera";
        constexpr float kChallengeCameraMoveSpeed = 20.0f;

        Canis::Vector3 LookRotationForDirection(const Canis::Vector3& _direction)
        {
            const float length = std::sqrt(
                (_direction.x * _direction.x) +
                (_direction.y * _direction.y) +
                (_direction.z * _direction.z));
            if (length <= 0.0001f)
                return Canis::Vector3(0.0f);

            const Canis::Vector3 normalized = _direction / length;
            return Canis::Vector3(
                std::asin(std::clamp(normalized.y, -1.0f, 1.0f)),
                std::atan2(-normalized.x, -normalized.z),
                0.0f);
        }

        void SetText(Canis::Entity* _entity, const std::string& _value)
        {
            if (_entity == nullptr || !_entity->HasComponent<Canis::Text>())
                return;

            _entity->GetComponent<Canis::Text>().SetText(_value);
        }
    }

    void RegisterChallengeControllerScript(Canis::App& _app)
    {
        DEFAULT_CONFIG(g_challengeControllerConf, StopTheSlimes::ChallengeController);
        RegisterUIAction<StopTheSlimes::ChallengeController>(g_challengeControllerConf, "OnClickChallengeContinue", &StopTheSlimes::ChallengeController::OnClickChallengeContinue);
        RegisterUIAction<StopTheSlimes::ChallengeController>(g_challengeControllerConf, "OnClickChallengeRight", &StopTheSlimes::ChallengeController::OnClickChallengeRight);
        RegisterUIAction<StopTheSlimes::ChallengeController>(g_challengeControllerConf, "OnClickChallengeLeft", &StopTheSlimes::ChallengeController::OnClickChallengeLeft);
        RegisterUIAction<StopTheSlimes::ChallengeController>(g_challengeControllerConf, "OnClickMainMenu", &StopTheSlimes::ChallengeController::OnClickMainMenu);
        g_challengeControllerConf.DEFAULT_DRAW_INSPECTOR(StopTheSlimes::ChallengeController);
        _app.RegisterScript(g_challengeControllerConf);
    }

    DEFAULT_UNREGISTER_SCRIPT(g_challengeControllerConf, ChallengeController)

    void ChallengeController::Create() {}

    void ChallengeController::Ready()
    {
        Canis::AudioManager::PlayMusic(GetSupportedAudioPath("assets/sts/audio/music/AlexandrZhelanovSongs/Brirfing_theme.mp3"), -1, 0.1f);

        m_challengeSceneMap = LoadMapDefinition("assets/sts/configs/maps/demo_challenge.map");
        EnsureCameraRig(m_challengeSceneMap);
        CollectTargets();
        ConfigureChallengePresentation(entity.scene, m_challengeSceneMap, m_targets);
        LoadChallengePreviews();
        SelectTarget(GetRuntimeState().selectedWorldIndex, true);

        if (!GetRuntimeState().firstChallengeDialogShown)
        {
            if (Canis::Entity* dialogEntity = entity.scene.GetEntityWithTag("DIALOG"))
            {
                if (DialogController* dialogController = dialogEntity->GetScript<DialogController>())
                {
                    dialogController->Show(DialogController::LoadFromFile("assets/sts/dialog/first_challenge.dialog"));
                    GetRuntimeState().firstChallengeDialogShown = true;
                }
            }
        }
    }

    void ChallengeController::Destroy()
    {
        m_previews.clear();
        m_targets.clear();
        m_cameraTargetValid = false;
    }

    void ChallengeController::EnsureCameraRig(const MapDefinition& _map)
    {
        Canis::Entity* pivot = entity.scene.GetEntityWithTag(kChallengeCameraPivotTag);
        if (pivot == nullptr)
        {
            pivot = entity.scene.CreateEntity("ChallengeCameraPivot", kChallengeCameraPivotTag);
            pivot->AddComponent<Canis::Transform>();
        }

        if (!pivot->HasComponent<Canis::Transform>())
            pivot->AddComponent<Canis::Transform>();

        Canis::Transform& pivotTransform = pivot->GetComponent<Canis::Transform>();
        pivotTransform.active = true;
        pivotTransform.position = _map.valid ? _map.cameraPivot : Canis::Vector3(11.5f, 0.5f, 14.0f);
        pivot->active = true;

        Canis::Entity* camera = entity.scene.FindEntityWithName(kChallengeCameraName);
        if (camera == nullptr)
        {
            camera = entity.scene.CreateEntity(kChallengeCameraName, "MainCamera");
            camera->AddComponent<Canis::Transform>();
        }

        if (!camera->HasComponent<Canis::Transform>())
            camera->AddComponent<Canis::Transform>();
        if (!camera->HasComponent<Canis::Camera>())
            camera->AddComponent<Canis::Camera>();

        Canis::Transform& cameraTransform = camera->GetComponent<Canis::Transform>();
        if (cameraTransform.parent != pivot)
            cameraTransform.SetParent(pivot);

        const Canis::Vector3 localOffset = _map.valid
            ? Canis::Vector3(_map.cameraX, _map.cameraY, _map.cameraZ)
            : Canis::Vector3(-30.0f, 30.0f, 4.0f);

        cameraTransform.active = true;
        cameraTransform.position = localOffset;
        cameraTransform.rotation = LookRotationForDirection(-localOffset);
        cameraTransform.scale = Canis::Vector3(1.0f);
        camera->active = true;

        Canis::Camera& cameraComponent = camera->GetComponent<Canis::Camera>();
        cameraComponent.primary = true;
        cameraComponent.fovDegrees = 30.0f;
        cameraComponent.nearClip = 0.1f;
        cameraComponent.farClip = 500.0f;
    }

    void ChallengeController::Update(float _dt)
    {
        UpdateCameraPivot(_dt);

        Canis::Entity* dialogEntity = entity.scene.GetEntityWithTag("DIALOG");
        if (dialogEntity != nullptr)
        {
            if (DialogController* dialogController = dialogEntity->GetScript<DialogController>())
            {
                if (dialogController->IsShowing())
                    return;
            }
        }

        Canis::InputManager& input = entity.scene.GetInputManager();
        if (input.JustPressedKey(Canis::Key::LEFT) || input.JustPressedButton(Canis::ControllerButton::LEFTSHOULDER))
            SelectTarget(m_currentTarget - 1);

        if (input.JustPressedKey(Canis::Key::RIGHT) || input.JustPressedButton(Canis::ControllerButton::RIGHTSHOULDER))
            SelectTarget(m_currentTarget + 1);

        if (input.JustPressedKey(Canis::Key::RETURN) || input.JustPressedButton(Canis::ControllerButton::A))
            OnClickChallengeContinue(Canis::UIActionContext{});
    }

    void ChallengeController::OnClickChallengeContinue(const Canis::UIActionContext& _context)
    {
        (void)_context;
        PlayMenuClick();
        SelectWorld(m_currentTarget);
        SetCurrentLevelPath(GetSelectedWorld().levelPath);
        LoadScene(entity, "demo");
    }

    void ChallengeController::OnClickChallengeRight(const Canis::UIActionContext& _context)
    {
        (void)_context;
        PlayMenuClick();
        SelectTarget(m_currentTarget + 1);
    }

    void ChallengeController::OnClickChallengeLeft(const Canis::UIActionContext& _context)
    {
        (void)_context;
        PlayMenuClick();
        SelectTarget(m_currentTarget - 1);
    }

    void ChallengeController::OnClickMainMenu(const Canis::UIActionContext& _context)
    {
        (void)_context;
        PlayMenuClick();
        LoadScene(entity, "main_menu");
    }

    void ChallengeController::CollectTargets()
    {
        m_targets.clear();

        const std::vector<WorldDefinition>& worlds = GetWorldDefinitions();
        for (int i = 0; i < static_cast<int>(worlds.size()); ++i)
            m_targets.push_back(entity.scene.GetEntityWithTag("Q" + std::to_string(i)));
    }

    void ChallengeController::LoadChallengePreviews()
    {
        m_previews.clear();
        m_previews.reserve(GetWorldDefinitions().size());

        const std::vector<WorldDefinition>& worlds = GetWorldDefinitions();
        for (int i = 0; i < static_cast<int>(worlds.size()); ++i)
        {
            ChallengeMapPreview preview = {};
            Canis::Entity* target = (i < static_cast<int>(m_targets.size()))
                ? m_targets[static_cast<std::size_t>(i)]
                : nullptr;

            if (target != nullptr && target->HasComponent<Canis::Transform>())
            {
                const MapDefinition map = LoadMapDefinition(worlds[static_cast<std::size_t>(i)].challengeMapPath);
                if (map.valid)
                {
                    preview = BuildChallengeMapPreview(
                        entity.scene,
                        map,
                        target->GetComponent<Canis::Transform>().GetGlobalPosition(),
                        "ChallengePreview " + std::to_string(i));

                    target->active = false;
                    target->GetComponent<Canis::Transform>().active = false;
                }
            }

            m_previews.push_back(preview);
        }
    }

    void ChallengeController::SelectTarget(int _index, const bool _snap)
    {
        const int worldCount = static_cast<int>(GetWorldDefinitions().size());
        if (worldCount <= 0)
            return;

        if (_index < 0)
            _index = worldCount - 1;
        if (_index >= worldCount)
            _index = 0;

        m_currentTarget = _index;
        SelectWorld(_index);
        FocusCameraOnTarget(_snap);
        RefreshTargetVisuals();
        RefreshQuestPanel();
    }

    void ChallengeController::FocusCameraOnTarget(const bool _snap)
    {
        if (m_currentTarget < 0 || m_currentTarget >= static_cast<int>(m_targets.size()))
            return;

        Canis::Entity* pivot = entity.scene.GetEntityWithTag(kChallengeCameraPivotTag);
        Canis::Entity* target = m_targets[static_cast<std::size_t>(m_currentTarget)];
        if (pivot == nullptr || target == nullptr)
            return;

        if (!pivot->HasComponent<Canis::Transform>() || !target->HasComponent<Canis::Transform>())
            return;

        m_cameraTargetPosition = target->GetComponent<Canis::Transform>().GetGlobalPosition();
        m_cameraTargetValid = true;

        if (_snap)
            pivot->GetComponent<Canis::Transform>().position = m_cameraTargetPosition;
    }

    void ChallengeController::UpdateCameraPivot(const float _dt)
    {
        if (!m_cameraTargetValid)
            return;

        Canis::Entity* pivot = entity.scene.GetEntityWithTag(kChallengeCameraPivotTag);
        if (pivot == nullptr || !pivot->HasComponent<Canis::Transform>())
            return;

        Canis::Transform& pivotTransform = pivot->GetComponent<Canis::Transform>();
        const Canis::Vector3 delta = m_cameraTargetPosition - pivotTransform.position;
        const float distance = glm::length(delta);
        if (distance <= 0.001f)
        {
            pivotTransform.position = m_cameraTargetPosition;
            return;
        }

        const float step = kChallengeCameraMoveSpeed * std::max(0.0f, _dt);
        if (step >= distance)
        {
            pivotTransform.position = m_cameraTargetPosition;
            return;
        }

        pivotTransform.position += (delta / distance) * step;
    }

    void ChallengeController::RefreshTargetVisuals()
    {
        for (int i = 0; i < static_cast<int>(GetWorldDefinitions().size()); ++i)
        {
            const bool selected = (i == m_currentTarget);
            if (i < static_cast<int>(m_previews.size()) && !m_previews[static_cast<std::size_t>(i)].entities.empty())
            {
                m_previews[static_cast<std::size_t>(i)].SetActive(selected);
            }

            if (i >= static_cast<int>(m_targets.size()))
                continue;

            Canis::Entity* target = m_targets[static_cast<std::size_t>(i)];
            if (target == nullptr || !target->HasComponent<Canis::Transform>())
                continue;

            const bool hasPreview = (i < static_cast<int>(m_previews.size())) &&
                !m_previews[static_cast<std::size_t>(i)].entities.empty();

            target->active = !hasPreview;
            target->GetComponent<Canis::Transform>().active = !hasPreview;

            if (!hasPreview)
            {
                target->GetComponent<Canis::Transform>().scale = selected
                    ? Canis::Vector3(3.5f)
                    : Canis::Vector3(3.0f);

                if (target->HasComponent<Canis::Model>())
                {
                    target->GetComponent<Canis::Model>().color = selected
                        ? Canis::Color(1.0f, 0.95f, 0.45f, 1.0f)
                        : Canis::Color(1.0f);
                }
            }
        }
    }

    void ChallengeController::RefreshQuestPanel()
    {
        static const std::array<const char*, 6> questOne = {
            "Victory!!",
            "Victory!!",
            "Victory!!",
            "Victory!!",
            "100 Rounds of Lava",
            "Stop Big Green Army!",
        };

        static const std::array<const char*, 6> questTwo = {
            "Victory without using a Tree Tower",
            "Victory without using a Fire Tower",
            "Victory without using a Cauldron",
            "Victory without using a Fire Tower",
            "Victory without using a Cannon",
            "Victory without using a Cannon",
        };

        static const std::array<const char*, 6> questThree = {
            "Victory without losing Health",
            "Victory without losing Health",
            "Victory without losing Health",
            "Victory without losing Health",
            "Victory without losing Health",
            "Victory without losing Health",
        };

        const WorldDefinition& world = GetSelectedWorld();

        SetText(entity.scene.GetEntityWithTag("QUEST_TITLE"), world.displayName);
        SetText(
            entity.scene.GetEntityWithTag("HIGH_SCORE"),
            "Score: " + std::to_string(GetHighScore(world.code))
        );
        SetText(entity.scene.GetEntityWithTag("QUEST_1_TEXT"), questOne[static_cast<std::size_t>(m_currentTarget)]);
        SetText(entity.scene.GetEntityWithTag("QUEST_2_TEXT"), questTwo[static_cast<std::size_t>(m_currentTarget)]);
        SetText(entity.scene.GetEntityWithTag("QUEST_3_TEXT"), questThree[static_cast<std::size_t>(m_currentTarget)]);

        if (Canis::Entity* continueEntity = entity.scene.GetEntityWithTag("CONTINUE"))
        {
            if (continueEntity->HasComponent<Canis::RectTransform>())
                continueEntity->GetComponent<Canis::RectTransform>().active = true;
        }
    }
}
