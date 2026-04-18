#pragma once
#include <string>
#include <map>
#include <iostream>
#include <SDL3_mixer/SDL_mixer.h>

class AudioManager
{
public:
    static AudioManager& instance() {
        static AudioManager instance;
        return instance;
    }
    bool init();
    bool close();
    void playSFX(std::string path);
    void setSFXVolume(int volume);
    void setMusicVolume(int volume);
    void playMusic(std::string path, bool loop = true);
    void stopMusic();
private:
    AudioManager() : mixer(nullptr), musicTrack(nullptr) {}
    std::map<std::string, MIX_Audio*> sounds;
    MIX_Mixer* mixer;
    MIX_Track* musicTrack;
};
