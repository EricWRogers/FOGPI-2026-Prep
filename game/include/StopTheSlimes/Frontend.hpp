#pragma once

#include <Canis/Entity.hpp>

#include <string>
#include <vector>

namespace Canis
{
    class App;
    struct UIActionContext;
}

namespace StopTheSlimes
{
    class MainMenuController : public Canis::ScriptableEntity
    {
    public:
        static constexpr const char* ScriptName = "StopTheSlimes::MainMenuController";

        explicit MainMenuController(Canis::Entity& _entity) : Canis::ScriptableEntity(_entity) {}

        void Create() override;
        void Ready() override;
        void Destroy() override;
        void Update(float _dt) override;

        void OnClickPlay(const Canis::UIActionContext& _context);
        void OnClickSettings(const Canis::UIActionContext& _context);
        void OnClickQuit(const Canis::UIActionContext& _context);
        void OnClickYoutube(const Canis::UIActionContext& _context);
        void OnClickDiscord(const Canis::UIActionContext& _context);
        void OnClickSurvey(const Canis::UIActionContext& _context);
        void OnClickAudioToggle(const Canis::UIActionContext& _context);
        void OnClickQRCode(const Canis::UIActionContext& _context);

    private:
        void UpdateMuteButton();
    };

    class MainMenuButton : public Canis::ScriptableEntity
    {
    public:
        static constexpr const char* ScriptName = "StopTheSlimes::MainMenuButton";

        explicit MainMenuButton(Canis::Entity& _entity) : Canis::ScriptableEntity(_entity) {}

        void Create() override;
        void Ready() override;
        void Destroy() override;
        void Update(float _dt) override;

        void OnClickMainMenu(const Canis::UIActionContext& _context);
        void OnClickIslandMap(const Canis::UIActionContext& _context);
    };

    class RetryLevelButton : public Canis::ScriptableEntity
    {
    public:
        static constexpr const char* ScriptName = "StopTheSlimes::RetryLevelButton";

        explicit RetryLevelButton(Canis::Entity& _entity) : Canis::ScriptableEntity(_entity) {}

        void Create() override;
        void Ready() override;
        void Destroy() override;
        void Update(float _dt) override;

        void OnClickRetryLevel(const Canis::UIActionContext& _context);
    };

    class FinalScoreDisplay : public Canis::ScriptableEntity
    {
    public:
        static constexpr const char* ScriptName = "StopTheSlimes::FinalScoreDisplay";

        explicit FinalScoreDisplay(Canis::Entity& _entity) : Canis::ScriptableEntity(_entity) {}

        void Create() override;
        void Ready() override;
        void Destroy() override;
        void Update(float _dt) override;
    };

    class VerticalCenter : public Canis::ScriptableEntity
    {
    public:
        static constexpr const char* ScriptName = "StopTheSlimes::VerticalCenter";

        explicit VerticalCenter(Canis::Entity& _entity) : Canis::ScriptableEntity(_entity) {}

        void Create() override;
        void Ready() override;
        void Destroy() override;
        void Update(float _dt) override;
    };

    class DebugCamera2D : public Canis::ScriptableEntity
    {
    public:
        static constexpr const char* ScriptName = "StopTheSlimes::DebugCamera2D";

        float moveSpeed = 600.0f;
        float zoomSpeed = 0.8f;

        explicit DebugCamera2D(Canis::Entity& _entity) : Canis::ScriptableEntity(_entity) {}

        void Create() override;
        void Ready() override;
        void Destroy() override;
        void Update(float _dt) override;
    };

    class SplashLoader : public Canis::ScriptableEntity
    {
    public:
        static constexpr const char* ScriptName = "StopTheSlimes::SplashLoader";

        explicit SplashLoader(Canis::Entity& _entity) : Canis::ScriptableEntity(_entity) {}

        void Create() override;
        void Ready() override;
        void Destroy() override;
        void Update(float _dt) override;

    private:
        float m_timer = 0.0f;
        bool m_hasRequestedScene = false;
    };

    class ControllerContinueButton : public Canis::ScriptableEntity
    {
    public:
        static constexpr const char* ScriptName = "StopTheSlimes::ControllerContinueButton";

        explicit ControllerContinueButton(Canis::Entity& _entity) : Canis::ScriptableEntity(_entity) {}

        void Create() override;
        void Ready() override;
        void Destroy() override;
        void Update(float _dt) override;

    private:
        bool m_hasRequestedScene = false;
    };

    class SettingsMenuController : public Canis::ScriptableEntity
    {
    public:
        static constexpr const char* ScriptName = "StopTheSlimes::SettingsMenuController";

        explicit SettingsMenuController(Canis::Entity& _entity) : Canis::ScriptableEntity(_entity) {}

        void Create() override;
        void Ready() override;
        void Destroy() override;
        void Update(float _dt) override;

        void MainMenuAction(const Canis::UIActionContext& _context);
        void CreditsAction(const Canis::UIActionContext& _context);
        void VSyncAction(const Canis::UIActionContext& _context);
        void FPSAction(const Canis::UIActionContext& _context);
        void ShadowAction(const Canis::UIActionContext& _context);
        void OceanDetailsAction(const Canis::UIActionContext& _context);
        void MasterVolumeAction(const Canis::UIActionContext& _context);
        void MusicVolumeAction(const Canis::UIActionContext& _context);
        void SFXVolumeAction(const Canis::UIActionContext& _context);

    private:
        void SetButtonText(const std::string& _tag, const std::string& _value);
        void RefreshLabels();
        void StepVolume(float& _value);

        int m_shadowIndex = 1;
        int m_oceanDetailsIndex = 2;
    };

    class DialogController : public Canis::ScriptableEntity
    {
    public:
        struct DialogData
        {
            bool allowEarlyContinue = true;
            bool waitForUserInput = true;
            std::vector<std::string> messages = {};
        };

        static constexpr const char* ScriptName = "StopTheSlimes::DialogController";

        explicit DialogController(Canis::Entity& _entity) : Canis::ScriptableEntity(_entity) {}

        void Create() override;
        void Ready() override;
        void Destroy() override;
        void Update(float _dt) override;

        void Show(const DialogData& _data);
        bool IsShowing() const { return m_showing; }

        static DialogData LoadFromFile(const std::string& _path);

    private:
        void BeginCurrentMessage();
        void Advance();

        Canis::Entity* m_dialogText = nullptr;
        Canis::Entity* m_dialogContinue = nullptr;
        DialogData m_data = {};
        std::size_t m_currentMessageIndex = 0;
        std::size_t m_visibleCharacters = 0;
        float m_characterCountdown = 0.0f;
        float m_characterInterval = 0.03f;
        bool m_showing = false;
    };

    class FadeColorIn : public Canis::ScriptableEntity
    {
    public:
        static constexpr const char* ScriptName = "StopTheSlimes::FadeColorIn";

        float maxTime = 3.0f;

        explicit FadeColorIn(Canis::Entity& _entity) : Canis::ScriptableEntity(_entity) {}

        void Create() override;
        void Ready() override;
        void Destroy() override;
        void Update(float _dt) override;

    private:
        float m_timer = 0.0f;
    };

    void RegisterMainMenuControllerScript(Canis::App& _app);
    void UnRegisterMainMenuControllerScript(Canis::App& _app);
    void RegisterMainMenuButtonScript(Canis::App& _app);
    void UnRegisterMainMenuButtonScript(Canis::App& _app);
    void RegisterRetryLevelButtonScript(Canis::App& _app);
    void UnRegisterRetryLevelButtonScript(Canis::App& _app);
    void RegisterFinalScoreDisplayScript(Canis::App& _app);
    void UnRegisterFinalScoreDisplayScript(Canis::App& _app);
    void RegisterVerticalCenterScript(Canis::App& _app);
    void UnRegisterVerticalCenterScript(Canis::App& _app);
    void RegisterDebugCamera2DScript(Canis::App& _app);
    void UnRegisterDebugCamera2DScript(Canis::App& _app);
    void RegisterSplashLoaderScript(Canis::App& _app);
    void UnRegisterSplashLoaderScript(Canis::App& _app);
    void RegisterControllerContinueButtonScript(Canis::App& _app);
    void UnRegisterControllerContinueButtonScript(Canis::App& _app);
    void RegisterSettingsMenuControllerScript(Canis::App& _app);
    void UnRegisterSettingsMenuControllerScript(Canis::App& _app);
    void RegisterDialogControllerScript(Canis::App& _app);
    void UnRegisterDialogControllerScript(Canis::App& _app);
    void RegisterFadeColorInScript(Canis::App& _app);
    void UnRegisterFadeColorInScript(Canis::App& _app);
}
