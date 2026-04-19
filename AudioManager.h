#pragma once
#include <string>
#include <map>
#include <iostream>
#include <SDL3_mixer/SDL_mixer.h>

/// <summary>
/// Singleton que gestiona carga de sonidos y reproduccion de musica y SFX
/// </summary>
class AudioManager
{
public:
    // Instancia Singleton
    static AudioManager& instance() {
        static AudioManager instance;
        return instance;
    }

    bool Init();                                                // Inicializacion
    bool Close();                                               // Liberacion de recursos
    void PlaySFX(std::string path);                             // Reproduce un sonido
    void SetSFXVolume(int volume);                              // Controla el volumen global
    void SetMusicVolume(int volume);                            // Controla el volumen de la musica
    void PlayMusic(std::string path, bool loop = true);         // Reproduce musica
    void StopMusic();                                           // Detiene la musica

private:
    AudioManager() : mixer(nullptr), musicTrack(nullptr) {}     // Constructor nulo para que nadie cree AudioManager
    std::map<std::string, MIX_Audio*> sounds;                   // Vector de los sonidos cargados
    MIX_Mixer* mixer;                                           // Mezclador global
    MIX_Track* musicTrack;                                      // Canal dedicado a musica
    std::string currentMusic = "";                              // Para guardar el nombre de la musica actual
};
