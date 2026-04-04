#include <StopTheSlimes/Frontend.hpp>

#include <StopTheSlimes/State.hpp>

#include <Canis/AssetManager.hpp>
#include <Canis/AudioManager.hpp>
#include <Canis/Canis.hpp>
#include <Canis/ConfigHelper.hpp>
#include <Canis/Data/Key.hpp>
#include <Canis/InputManager.hpp>
#include <Canis/Time.hpp>
#include <Canis/Window.hpp>
#include <Canis/Yaml.hpp>

#include <SDL3/SDL_misc.h>

#include <algorithm>
#include <array>
#include <fstream>

namespace StopTheSlimes
{
    namespace
    {
        Canis::ScriptConf g_mainMenuControllerConf = {};
        Canis::ScriptConf g_mainMenuButtonConf = {};
        Canis::ScriptConf g_retryLevelButtonConf = {};
        Canis::ScriptConf g_finalScoreDisplayConf = {};
        Canis::ScriptConf g_verticalCenterConf = {};
        Canis::ScriptConf g_debugCamera2DConf = {};
        Canis::ScriptConf g_splashLoaderConf = {};
        Canis::ScriptConf g_controllerContinueButtonConf = {};
        Canis::ScriptConf g_settingsMenuControllerConf = {};
        Canis::ScriptConf g_dialogControllerConf = {};
        Canis::ScriptConf g_fadeColorInConf = {};

        std::string LoadAssetVersionLabel()
        {
            std::ifstream file("assets/sts/configs/game.conf");
            std::string token = "";
            std::string assetVersion = "unknown";

            while (file >> token)
            {
                if (token == "assetVersion" && file >> assetVersion)
                    break;
            }

            return "Asset Version : " + assetVersion;
        }

        void SetEntityText(Canis::Entity* _entity, const std::string& _value)
        {
            if (_entity == nullptr || !_entity->HasComponent<Canis::Text>())
                return;

            _entity->GetComponent<Canis::Text>().SetText(_value);
        }

        bool IsAdvancePressed(Canis::InputManager& _input)
        {
            return _input.JustPressedKey(Canis::Key::SPACE) ||
                   _input.JustPressedKey(Canis::Key::RETURN) ||
                   _input.JustLeftClicked() ||
                   _input.JustPressedButton(Canis::ControllerButton::A);
        }
    }

    void RegisterMainMenuControllerScript(Canis::App& _app)
    {
        DEFAULT_CONFIG(g_mainMenuControllerConf, StopTheSlimes::MainMenuController);
        RegisterUIAction<StopTheSlimes::MainMenuController>(g_mainMenuControllerConf, "OnClickPlay", &StopTheSlimes::MainMenuController::OnClickPlay);
        RegisterUIAction<StopTheSlimes::MainMenuController>(g_mainMenuControllerConf, "OnClickSettings", &StopTheSlimes::MainMenuController::OnClickSettings);
        RegisterUIAction<StopTheSlimes::MainMenuController>(g_mainMenuControllerConf, "OnClickQuit", &StopTheSlimes::MainMenuController::OnClickQuit);
        RegisterUIAction<StopTheSlimes::MainMenuController>(g_mainMenuControllerConf, "OnClickYoutube", &StopTheSlimes::MainMenuController::OnClickYoutube);
        RegisterUIAction<StopTheSlimes::MainMenuController>(g_mainMenuControllerConf, "OnClickDiscord", &StopTheSlimes::MainMenuController::OnClickDiscord);
        RegisterUIAction<StopTheSlimes::MainMenuController>(g_mainMenuControllerConf, "OnClickSurvey", &StopTheSlimes::MainMenuController::OnClickSurvey);
        RegisterUIAction<StopTheSlimes::MainMenuController>(g_mainMenuControllerConf, "OnClickAudioToggle", &StopTheSlimes::MainMenuController::OnClickAudioToggle);
        RegisterUIAction<StopTheSlimes::MainMenuController>(g_mainMenuControllerConf, "OnClickQRCode", &StopTheSlimes::MainMenuController::OnClickQRCode);
        g_mainMenuControllerConf.DEFAULT_DRAW_INSPECTOR(StopTheSlimes::MainMenuController);
        _app.RegisterScript(g_mainMenuControllerConf);
    }

    DEFAULT_UNREGISTER_SCRIPT(g_mainMenuControllerConf, MainMenuController)

    void RegisterMainMenuButtonScript(Canis::App& _app)
    {
        DEFAULT_CONFIG(g_mainMenuButtonConf, StopTheSlimes::MainMenuButton);
        RegisterUIAction<StopTheSlimes::MainMenuButton>(g_mainMenuButtonConf, "OnClickMainMenu", &StopTheSlimes::MainMenuButton::OnClickMainMenu);
        RegisterUIAction<StopTheSlimes::MainMenuButton>(g_mainMenuButtonConf, "OnClickIslandMap", &StopTheSlimes::MainMenuButton::OnClickIslandMap);
        g_mainMenuButtonConf.DEFAULT_DRAW_INSPECTOR(StopTheSlimes::MainMenuButton);
        _app.RegisterScript(g_mainMenuButtonConf);
    }

    DEFAULT_UNREGISTER_SCRIPT(g_mainMenuButtonConf, MainMenuButton)

    void RegisterRetryLevelButtonScript(Canis::App& _app)
    {
        DEFAULT_CONFIG(g_retryLevelButtonConf, StopTheSlimes::RetryLevelButton);
        RegisterUIAction<StopTheSlimes::RetryLevelButton>(g_retryLevelButtonConf, "OnClickRetryLevel", &StopTheSlimes::RetryLevelButton::OnClickRetryLevel);
        g_retryLevelButtonConf.DEFAULT_DRAW_INSPECTOR(StopTheSlimes::RetryLevelButton);
        _app.RegisterScript(g_retryLevelButtonConf);
    }

    DEFAULT_UNREGISTER_SCRIPT(g_retryLevelButtonConf, RetryLevelButton)

    void RegisterFinalScoreDisplayScript(Canis::App& _app)
    {
        DEFAULT_CONFIG(g_finalScoreDisplayConf, StopTheSlimes::FinalScoreDisplay);
        g_finalScoreDisplayConf.DEFAULT_DRAW_INSPECTOR(StopTheSlimes::FinalScoreDisplay);
        _app.RegisterScript(g_finalScoreDisplayConf);
    }

    DEFAULT_UNREGISTER_SCRIPT(g_finalScoreDisplayConf, FinalScoreDisplay)

    void RegisterVerticalCenterScript(Canis::App& _app)
    {
        DEFAULT_CONFIG(g_verticalCenterConf, StopTheSlimes::VerticalCenter);
        g_verticalCenterConf.DEFAULT_DRAW_INSPECTOR(StopTheSlimes::VerticalCenter);
        _app.RegisterScript(g_verticalCenterConf);
    }

    DEFAULT_UNREGISTER_SCRIPT(g_verticalCenterConf, VerticalCenter)

    void RegisterDebugCamera2DScript(Canis::App& _app)
    {
        REGISTER_PROPERTY(g_debugCamera2DConf, StopTheSlimes::DebugCamera2D, moveSpeed);
        REGISTER_PROPERTY(g_debugCamera2DConf, StopTheSlimes::DebugCamera2D, zoomSpeed);
        DEFAULT_CONFIG(g_debugCamera2DConf, StopTheSlimes::DebugCamera2D);
        g_debugCamera2DConf.DEFAULT_DRAW_INSPECTOR(StopTheSlimes::DebugCamera2D);
        _app.RegisterScript(g_debugCamera2DConf);
    }

    DEFAULT_UNREGISTER_SCRIPT(g_debugCamera2DConf, DebugCamera2D)

    void RegisterSplashLoaderScript(Canis::App& _app)
    {
        DEFAULT_CONFIG(g_splashLoaderConf, StopTheSlimes::SplashLoader);
        g_splashLoaderConf.DEFAULT_DRAW_INSPECTOR(StopTheSlimes::SplashLoader);
        _app.RegisterScript(g_splashLoaderConf);
    }

    DEFAULT_UNREGISTER_SCRIPT(g_splashLoaderConf, SplashLoader)

    void RegisterControllerContinueButtonScript(Canis::App& _app)
    {
        DEFAULT_CONFIG(g_controllerContinueButtonConf, StopTheSlimes::ControllerContinueButton);
        g_controllerContinueButtonConf.DEFAULT_DRAW_INSPECTOR(StopTheSlimes::ControllerContinueButton);
        _app.RegisterScript(g_controllerContinueButtonConf);
    }

    DEFAULT_UNREGISTER_SCRIPT(g_controllerContinueButtonConf, ControllerContinueButton)

    void RegisterSettingsMenuControllerScript(Canis::App& _app)
    {
        DEFAULT_CONFIG(g_settingsMenuControllerConf, StopTheSlimes::SettingsMenuController);
        RegisterUIAction<StopTheSlimes::SettingsMenuController>(g_settingsMenuControllerConf, "Main_Menu_Button", &StopTheSlimes::SettingsMenuController::MainMenuAction);
        RegisterUIAction<StopTheSlimes::SettingsMenuController>(g_settingsMenuControllerConf, "OnClickCredits", &StopTheSlimes::SettingsMenuController::CreditsAction);
        RegisterUIAction<StopTheSlimes::SettingsMenuController>(g_settingsMenuControllerConf, "OnClickVSync", &StopTheSlimes::SettingsMenuController::VSyncAction);
        RegisterUIAction<StopTheSlimes::SettingsMenuController>(g_settingsMenuControllerConf, "OnClickFPS", &StopTheSlimes::SettingsMenuController::FPSAction);
        RegisterUIAction<StopTheSlimes::SettingsMenuController>(g_settingsMenuControllerConf, "OnClickShadow", &StopTheSlimes::SettingsMenuController::ShadowAction);
        RegisterUIAction<StopTheSlimes::SettingsMenuController>(g_settingsMenuControllerConf, "OnClickOceanDetails", &StopTheSlimes::SettingsMenuController::OceanDetailsAction);
        RegisterUIAction<StopTheSlimes::SettingsMenuController>(g_settingsMenuControllerConf, "OnClickMasterVolume", &StopTheSlimes::SettingsMenuController::MasterVolumeAction);
        RegisterUIAction<StopTheSlimes::SettingsMenuController>(g_settingsMenuControllerConf, "OnClickMusicVolume", &StopTheSlimes::SettingsMenuController::MusicVolumeAction);
        RegisterUIAction<StopTheSlimes::SettingsMenuController>(g_settingsMenuControllerConf, "OnClickSFXVolume", &StopTheSlimes::SettingsMenuController::SFXVolumeAction);
        g_settingsMenuControllerConf.DEFAULT_DRAW_INSPECTOR(StopTheSlimes::SettingsMenuController);
        _app.RegisterScript(g_settingsMenuControllerConf);
    }

    DEFAULT_UNREGISTER_SCRIPT(g_settingsMenuControllerConf, SettingsMenuController)

    void RegisterDialogControllerScript(Canis::App& _app)
    {
        DEFAULT_CONFIG(g_dialogControllerConf, StopTheSlimes::DialogController);
        g_dialogControllerConf.DEFAULT_DRAW_INSPECTOR(StopTheSlimes::DialogController);
        _app.RegisterScript(g_dialogControllerConf);
    }

    DEFAULT_UNREGISTER_SCRIPT(g_dialogControllerConf, DialogController)

    void RegisterFadeColorInScript(Canis::App& _app)
    {
        REGISTER_PROPERTY(g_fadeColorInConf, StopTheSlimes::FadeColorIn, maxTime);
        DEFAULT_CONFIG(g_fadeColorInConf, StopTheSlimes::FadeColorIn);
        g_fadeColorInConf.DEFAULT_DRAW_INSPECTOR(StopTheSlimes::FadeColorIn);
        _app.RegisterScript(g_fadeColorInConf);
    }

    DEFAULT_UNREGISTER_SCRIPT(g_fadeColorInConf, FadeColorIn)

    void MainMenuController::Create() {}

    void MainMenuController::Ready()
    {
        Canis::AudioManager::PlayMusic(GetSupportedAudioPath("assets/sts/audio/music/AlexandrZhelanovSongs/improvisation1.mp3"), -1, 0.1f);

        SetEntityText(entity.scene.GetEntityWithTag("build_version"), "Build Version : StopTheSlimes Port");
        SetEntityText(entity.scene.GetEntityWithTag("asset_version"), LoadAssetVersionLabel());

        UpdateMuteButton();
    }

    void MainMenuController::Destroy() {}

    void MainMenuController::Update(float _dt)
    {
        (void)_dt;
    }

    void MainMenuController::OnClickPlay(const Canis::UIActionContext& _context)
    {
        (void)_context;
        PlayMenuClick();
        LoadScene(entity, "controlls_splash");
    }

    void MainMenuController::OnClickSettings(const Canis::UIActionContext& _context)
    {
        (void)_context;
        PlayMenuClick();
        LoadScene(entity, "settings");
    }

    void MainMenuController::OnClickQuit(const Canis::UIActionContext& _context)
    {
        (void)_context;
        PlayMenuClick();
        entity.scene.QuitGame();
    }

    void MainMenuController::OnClickYoutube(const Canis::UIActionContext& _context)
    {
        (void)_context;
        PlayMenuClick();
        SDL_OpenURL("https://www.youtube.com/@yourgeekeric");
    }

    void MainMenuController::OnClickDiscord(const Canis::UIActionContext& _context)
    {
        (void)_context;
        PlayMenuClick();
        SDL_OpenURL("https://discord.gg/E8bHB3kc9f");
    }

    void MainMenuController::OnClickSurvey(const Canis::UIActionContext& _context)
    {
        (void)_context;
        PlayMenuClick();
        SDL_OpenURL("https://forms.gle/M3dUnAV6Qad5q7Am6");
    }

    void MainMenuController::OnClickAudioToggle(const Canis::UIActionContext& _context)
    {
        (void)_context;
        Canis::ProjectConfig& projectConfig = Canis::GetProjectConfig();

        if (projectConfig.mute)
            Canis::AudioManager::UnMute();
        else
            Canis::AudioManager::Mute();

        UpdateMuteButton();
        PlayMenuClick();
    }

    void MainMenuController::OnClickQRCode(const Canis::UIActionContext& _context)
    {
        (void)_context;
        PlayMenuClick();
        LoadScene(entity, "qr_code");
    }

    void MainMenuController::UpdateMuteButton()
    {
        Canis::Entity* muteButton = entity.scene.GetEntityWithTag("mute_button");
        if (muteButton == nullptr || !muteButton->HasComponent<Canis::Sprite2D>())
            return;

        const std::string iconPath = Canis::GetProjectConfig().mute
            ? "assets/sts/textures/ui/mute_icon.png"
            : "assets/sts/textures/ui/unmute_icon.png";

        muteButton->GetComponent<Canis::Sprite2D>().textureHandle = Canis::AssetManager::GetTextureHandle(iconPath);
    }

    void MainMenuButton::Create() {}
    void MainMenuButton::Ready() {}
    void MainMenuButton::Destroy() {}
    void MainMenuButton::Update(float _dt)
    {
        (void)_dt;
    }

    void MainMenuButton::OnClickMainMenu(const Canis::UIActionContext& _context)
    {
        (void)_context;
        PlayMenuClick();
        LoadScene(entity, "main_menu");
    }

    void MainMenuButton::OnClickIslandMap(const Canis::UIActionContext& _context)
    {
        (void)_context;
        PlayMenuClick();
        LoadScene(entity, "challenge");
    }

    void RetryLevelButton::Create() {}
    void RetryLevelButton::Ready() {}
    void RetryLevelButton::Destroy() {}
    void RetryLevelButton::Update(float _dt)
    {
        (void)_dt;
    }

    void RetryLevelButton::OnClickRetryLevel(const Canis::UIActionContext& _context)
    {
        (void)_context;
        PlayMenuClick();

        if (GetCurrentLevelPath().empty())
            SetCurrentLevelPath(GetSelectedWorld().levelPath);

        LoadScene(entity, "demo");
    }

    void FinalScoreDisplay::Create() {}

    void FinalScoreDisplay::Ready()
    {
        const int score = GetLastScore();
        SaveHighScore(GetSelectedWorld().code, score);

        if (entity.HasComponent<Canis::Text>())
            entity.GetComponent<Canis::Text>().SetText("Score: " + std::to_string(score));
    }

    void FinalScoreDisplay::Destroy() {}
    void FinalScoreDisplay::Update(float _dt)
    {
        (void)_dt;
    }

    void VerticalCenter::Create() {}
    void VerticalCenter::Ready() {}
    void VerticalCenter::Destroy() {}
    void VerticalCenter::Update(float _dt)
    {
        (void)_dt;
    }

    void DebugCamera2D::Create() {}
    void DebugCamera2D::Ready() {}
    void DebugCamera2D::Destroy() {}

    void DebugCamera2D::Update(float _dt)
    {
        if (!entity.HasComponent<Canis::Camera2D>())
            return;

        if (entity.scene.GetInputManager().JustPressedKey(Canis::Key::F8))
            entity.scene.GetWindow().LockMouse(!entity.scene.GetWindow().IsMouseLocked());

        if (!entity.scene.GetWindow().IsMouseLocked())
            return;

        Canis::Camera2D& camera = entity.GetComponent<Canis::Camera2D>();
        Canis::Vector2 position = camera.GetPosition();
        float scale = camera.GetScale();

        if (entity.scene.GetInputManager().GetKey(Canis::Key::W) || entity.scene.GetInputManager().GetKey(Canis::Key::UP))
            position.y += moveSpeed * _dt;
        if (entity.scene.GetInputManager().GetKey(Canis::Key::S) || entity.scene.GetInputManager().GetKey(Canis::Key::DOWN))
            position.y -= moveSpeed * _dt;
        if (entity.scene.GetInputManager().GetKey(Canis::Key::A) || entity.scene.GetInputManager().GetKey(Canis::Key::LEFT))
            position.x -= moveSpeed * _dt;
        if (entity.scene.GetInputManager().GetKey(Canis::Key::D) || entity.scene.GetInputManager().GetKey(Canis::Key::RIGHT))
            position.x += moveSpeed * _dt;

        if (entity.scene.GetInputManager().GetKey(Canis::Key::Q))
            scale += zoomSpeed * _dt;
        if (entity.scene.GetInputManager().GetKey(Canis::Key::E))
            scale = std::max(0.1f, scale - (zoomSpeed * _dt));

        camera.SetPosition(position);
        camera.SetScale(scale);
    }

    void SplashLoader::Create() {}

    void SplashLoader::Ready()
    {
        m_hasRequestedScene = false;
        m_timer = (entity.name == "Loader") ? 3.0f : 3.5f;
    }

    void SplashLoader::Destroy() {}

    void SplashLoader::Update(float _dt)
    {
        if (m_hasRequestedScene)
            return;

        if (IsAdvancePressed(entity.scene.GetInputManager()))
            m_timer = 0.0f;

        m_timer -= _dt;
        if (m_timer > 0.0f)
            return;

        m_hasRequestedScene = true;
        LoadScene(entity, (entity.name == "Loader") ? "splash_screen" : "main_menu");
    }

    void ControllerContinueButton::Create() {}
    void ControllerContinueButton::Ready()
    {
        m_hasRequestedScene = false;
    }
    void ControllerContinueButton::Destroy() {}

    void ControllerContinueButton::Update(float _dt)
    {
        (void)_dt;

        if (m_hasRequestedScene)
            return;

        if (!IsAdvancePressed(entity.scene.GetInputManager()))
            return;

        m_hasRequestedScene = true;
        PlayMenuClick();
        LoadScene(entity, "splash_screen");
    }

    void SettingsMenuController::Create() {}

    void SettingsMenuController::Ready()
    {
        Canis::AudioManager::PlayMusic(GetSupportedAudioPath("assets/sts/audio/music/AlexandrZhelanovSongs/improvisation1.mp3"), -1, 0.1f);
        RefreshLabels();
    }

    void SettingsMenuController::Destroy() {}
    void SettingsMenuController::Update(float _dt)
    {
        (void)_dt;
    }

    void SettingsMenuController::MainMenuAction(const Canis::UIActionContext& _context)
    {
        (void)_context;
        PlayMenuClick();
        LoadScene(entity, "main_menu");
    }

    void SettingsMenuController::CreditsAction(const Canis::UIActionContext& _context)
    {
        (void)_context;
        PlayMenuClick();
        LoadScene(entity, "credits");
    }

    void SettingsMenuController::VSyncAction(const Canis::UIActionContext& _context)
    {
        (void)_context;
        PlayMenuClick();

        Canis::ProjectConfig& config = Canis::GetProjectConfig();
        config.syncMode = (config.syncMode == Canis::PROJECT_SYNC_VSYNC)
            ? Canis::PROJECT_SYNC_OFF
            : Canis::PROJECT_SYNC_VSYNC;

        entity.scene.GetWindow().SetSync(static_cast<Canis::Window::Sync>(config.syncMode));
        RefreshLabels();
    }

    void SettingsMenuController::FPSAction(const Canis::UIActionContext& _context)
    {
        (void)_context;
        PlayMenuClick();

        static const std::array<int, 5> fpsModes = {30, 60, 120, 240, 100000};
        Canis::ProjectConfig& config = Canis::GetProjectConfig();

        int currentFPS = config.useFrameLimit ? config.frameLimit : 100000;
        int currentIndex = 0;
        for (int i = 0; i < static_cast<int>(fpsModes.size()); ++i)
        {
            if (fpsModes[static_cast<std::size_t>(i)] == currentFPS)
            {
                currentIndex = i;
                break;
            }
        }

        currentIndex = (currentIndex + 1) % static_cast<int>(fpsModes.size());
        const int targetFPS = fpsModes[static_cast<std::size_t>(currentIndex)];

        config.useFrameLimit = targetFPS < 100000;
        config.frameLimit = targetFPS;
        Canis::Time::SetTargetFPS(static_cast<float>(targetFPS));

        RefreshLabels();
    }

    void SettingsMenuController::ShadowAction(const Canis::UIActionContext& _context)
    {
        (void)_context;
        PlayMenuClick();
        m_shadowIndex = (m_shadowIndex + 1) % 4;
        RefreshLabels();
    }

    void SettingsMenuController::OceanDetailsAction(const Canis::UIActionContext& _context)
    {
        (void)_context;
        PlayMenuClick();
        m_oceanDetailsIndex = (m_oceanDetailsIndex + 1) % 4;
        RefreshLabels();
    }

    void SettingsMenuController::MasterVolumeAction(const Canis::UIActionContext& _context)
    {
        (void)_context;
        StepVolume(Canis::GetProjectConfig().volume);
    }

    void SettingsMenuController::MusicVolumeAction(const Canis::UIActionContext& _context)
    {
        (void)_context;
        StepVolume(Canis::GetProjectConfig().musicVolume);
    }

    void SettingsMenuController::SFXVolumeAction(const Canis::UIActionContext& _context)
    {
        (void)_context;
        StepVolume(Canis::GetProjectConfig().sfxVolume);
    }

    void SettingsMenuController::SetButtonText(const std::string& _tag, const std::string& _value)
    {
        SetEntityText(entity.scene.GetEntityWithTag(_tag), _value);
    }

    void SettingsMenuController::RefreshLabels()
    {
        SetButtonText("VSyncButton", (Canis::GetProjectConfig().syncMode == Canis::PROJECT_SYNC_VSYNC) ? "ON" : "OFF");

        static const std::array<const char*, 4> qualityLabels = {"Low", "Medium", "High", "Ultra"};
        SetButtonText("ShadowButton", qualityLabels[static_cast<std::size_t>(m_shadowIndex)]);
        SetButtonText("OceanDetailsButton", qualityLabels[static_cast<std::size_t>(m_oceanDetailsIndex)]);

        const int currentFPS = Canis::GetProjectConfig().useFrameLimit ? Canis::GetProjectConfig().frameLimit : 100000;
        SetButtonText("FPSButton", (currentFPS >= 100000) ? "Unlimited Power" : std::to_string(currentFPS));
    }

    void SettingsMenuController::StepVolume(float& _value)
    {
        _value += 0.25f;
        if (_value > 1.001f)
            _value = 0.0f;

        _value = std::clamp(_value, 0.0f, 1.0f);
        Canis::AudioManager::RefreshMixLevels();
        PlayMenuClick();
    }

    void DialogController::Create() {}

    void DialogController::Ready()
    {
        m_dialogText = nullptr;
        m_dialogContinue = nullptr;
        m_showing = false;

        if (!entity.HasComponent<Canis::RectTransform>())
            return;

        Canis::RectTransform& rectTransform = entity.GetComponent<Canis::RectTransform>();
        if (!rectTransform.children.empty())
            m_dialogText = rectTransform.children[0];
        if (rectTransform.children.size() > 1)
            m_dialogContinue = rectTransform.children[1];
    }

    void DialogController::Destroy() {}

    void DialogController::Update(float _dt)
    {
        if (!m_showing || m_dialogText == nullptr || !m_dialogText->HasComponent<Canis::Text>())
            return;

        if (!entity.HasComponent<Canis::RectTransform>() || !entity.GetComponent<Canis::RectTransform>().active)
            return;

        const std::string& message = m_data.messages[static_cast<std::size_t>(m_currentMessageIndex)];

        if (m_visibleCharacters < message.size())
        {
            m_characterCountdown -= _dt;
            while (m_characterCountdown <= 0.0f && m_visibleCharacters < message.size())
            {
                ++m_visibleCharacters;
                m_characterCountdown += m_characterInterval;
            }

            m_dialogText->GetComponent<Canis::Text>().SetText(message.substr(0, m_visibleCharacters));
        }

        const bool finished = m_visibleCharacters >= message.size();
        if (m_dialogContinue != nullptr && m_dialogContinue->HasComponent<Canis::RectTransform>())
            m_dialogContinue->GetComponent<Canis::RectTransform>().active = finished || m_data.allowEarlyContinue;

        if (!IsAdvancePressed(entity.scene.GetInputManager()))
            return;

        if (!finished && m_data.allowEarlyContinue)
        {
            m_visibleCharacters = message.size();
            m_dialogText->GetComponent<Canis::Text>().SetText(message);
            return;
        }

        if (finished)
            Advance();
    }

    void DialogController::Show(const DialogData& _data)
    {
        m_data = _data;
        m_currentMessageIndex = 0;
        m_showing = !m_data.messages.empty();

        if (entity.HasComponent<Canis::RectTransform>())
            entity.GetComponent<Canis::RectTransform>().active = m_showing;

        if (m_showing)
            BeginCurrentMessage();
    }

    DialogController::DialogData DialogController::LoadFromFile(const std::string& _path)
    {
        DialogData data = {};
        YAML::Node root = YAML::LoadFile(_path);

        if (YAML::Node dialog = root["Dialog"])
        {
            data.allowEarlyContinue = dialog["allowEarlyContinue"].as<bool>(true);
            data.waitForUserInput = dialog["waitForUserInput"].as<bool>(true);
        }

        if (YAML::Node messages = root["Messages"])
        {
            for (std::size_t i = 0; i < messages.size(); ++i)
                data.messages.push_back(messages[i].as<std::string>(""));
        }

        return data;
    }

    void DialogController::BeginCurrentMessage()
    {
        m_visibleCharacters = 0;
        m_characterCountdown = 0.0f;

        if (m_dialogText != nullptr && m_dialogText->HasComponent<Canis::Text>())
            m_dialogText->GetComponent<Canis::Text>().SetText("");
    }

    void DialogController::Advance()
    {
        if ((m_currentMessageIndex + 1) < m_data.messages.size())
        {
            ++m_currentMessageIndex;
            BeginCurrentMessage();
            return;
        }

        m_showing = false;
        if (entity.HasComponent<Canis::RectTransform>())
            entity.GetComponent<Canis::RectTransform>().active = false;
    }

    void FadeColorIn::Create() {}
    void FadeColorIn::Ready()
    {
        m_timer = 0.0f;
    }
    void FadeColorIn::Destroy() {}

    void FadeColorIn::Update(float _dt)
    {
        m_timer = std::min(maxTime, m_timer + _dt);
        const float alpha = (maxTime <= 0.0f) ? 1.0f : Canis::Lerp(0.0f, 1.0f, m_timer / maxTime);

        if (entity.HasComponent<Canis::Sprite2D>())
            entity.GetComponent<Canis::Sprite2D>().color.a = alpha;

        if (entity.HasComponent<Canis::Text>())
            entity.GetComponent<Canis::Text>().color.a = alpha;
    }
}
