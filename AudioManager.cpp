#include "AudioManager.h"
#include <SDL3/SDL.h>

bool AudioManager::init() {
	if (!MIX_Init()) {
		std::cout << "MIX_Init: Failed to init SDL_mixer!\n";
		std::cout << "Error: " << SDL_GetError() << std::endl;
		return false;
	}

	mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
	if (mixer == nullptr) {
		std::cout << "MIX_CreateMixerDevice Error: " << SDL_GetError() << std::endl;
		return false;
	}

	musicTrack = MIX_CreateTrack(mixer);
	if (musicTrack == nullptr) {
		std::cout << "MIX_CreateTrack Error: " << SDL_GetError() << std::endl;
		return false;
	}

	MIX_SetMixerGain(mixer, 0.2f);   // 20% del volumen global
	MIX_SetTrackGain(musicTrack, 0.2f); // 20% del volumen de música

	return true;
}

bool AudioManager::close() {
	stopMusic();

	if (musicTrack != nullptr) {
		MIX_SetTrackAudio(musicTrack, nullptr);
		MIX_DestroyTrack(musicTrack);
		musicTrack = nullptr;
	}

	for (auto it = sounds.begin(); it != sounds.end(); ++it) {
		MIX_DestroyAudio(it->second);
	}
	sounds.clear();

	if (mixer != nullptr) {
		MIX_DestroyMixer(mixer);
		mixer = nullptr;
	}

	MIX_Quit();
	return true;
}

void AudioManager::playSFX(std::string path) {
	if (!mixer) return;

	MIX_Audio* audio = nullptr;
	auto it = sounds.find(path);

	if (it == sounds.end()) {
		audio = MIX_LoadAudio(mixer, path.c_str(), true); // predecode=true for SFX
		if (audio == nullptr) {
			std::cout << "Error loading " << path << ": " << SDL_GetError() << std::endl;
			return;
		}
		sounds[path] = audio;
	}
	else {
		audio = it->second;
	}

	if (audio != nullptr) {
		MIX_PlayAudio(mixer, audio);
	}
}

void AudioManager::setSFXVolume(int volume) {
	if (!mixer) return;
	float gain = (float)volume / 128.0f;
	MIX_SetMixerGain(mixer, gain);
}

void AudioManager::setMusicVolume(int volume) {
	if (!musicTrack) return;
	float gain = (float)volume / 128.0f;
	MIX_SetTrackGain(musicTrack, gain);
}

void AudioManager::playMusic(std::string path, bool loop) {
	if (!musicTrack) return;

	stopMusic();

	MIX_Audio* audio = nullptr;
	auto it = sounds.find(path);
	if (it == sounds.end()) {
		audio = MIX_LoadAudio(mixer, path.c_str(), false); // false for streaming music
		if (audio == nullptr) {
			std::cout << "Failed to load music (" << path << "): " << SDL_GetError() << std::endl;
			return;
		}
		sounds[path] = audio;
	}
	else {
		audio = it->second;
	}

	MIX_SetTrackAudio(musicTrack, audio);

	if (loop) {
		SDL_PropertiesID props = SDL_CreateProperties();
		SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
		MIX_PlayTrack(musicTrack, props);
		SDL_DestroyProperties(props);
	}
	else {
		MIX_PlayTrack(musicTrack, 0);
	}
}

void AudioManager::stopMusic() {
	if (musicTrack != nullptr) {
		MIX_StopTrack(musicTrack, 0);
	}
}
