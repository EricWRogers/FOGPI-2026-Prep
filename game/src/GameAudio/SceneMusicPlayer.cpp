#include <GameAudio/SceneMusicPlayer.hpp>

#include <Canis/App.hpp>
#include <Canis/AudioManager.hpp>
#include <Canis/ConfigHelper.hpp>

namespace GameAudio
{
    namespace
    {
        ScriptConf sceneMusicPlayerConf = {};
    }

    void RegisterSceneMusicPlayerScript(Canis::App& _app)
    {
        REGISTER_PROPERTY(sceneMusicPlayerConf, GameAudio::SceneMusicPlayer, music);
        REGISTER_PROPERTY(sceneMusicPlayerConf, GameAudio::SceneMusicPlayer, volume);
        REGISTER_PROPERTY(sceneMusicPlayerConf, GameAudio::SceneMusicPlayer, loops);
        REGISTER_PROPERTY(sceneMusicPlayerConf, GameAudio::SceneMusicPlayer, playOnReady);
        REGISTER_PROPERTY(sceneMusicPlayerConf, GameAudio::SceneMusicPlayer, stopOnDestroy);

        DEFAULT_CONFIG(sceneMusicPlayerConf, GameAudio::SceneMusicPlayer);

        sceneMusicPlayerConf.DEFAULT_DRAW_INSPECTOR(GameAudio::SceneMusicPlayer);

        _app.RegisterScript(sceneMusicPlayerConf);
    }

    DEFAULT_UNREGISTER_SCRIPT(sceneMusicPlayerConf, SceneMusicPlayer)

    void SceneMusicPlayer::Create() {}

    void SceneMusicPlayer::Ready()
    {
        if (!playOnReady || music.Empty())
            return;

        Canis::AudioManager::PlayMusic(music, loops, volume);
    }

    void SceneMusicPlayer::Destroy()
    {
        if (stopOnDestroy)
            Canis::AudioManager::StopMusic();
    }

    void SceneMusicPlayer::Update(float _dt)
    {
        (void)_dt;
    }
}
