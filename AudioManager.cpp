#include "AudioManager.h"
#include <SDL3/SDL.h>

bool AudioManager::Init()
{
	// Inicializa SDL_Mixer y si falla, aborta
	if (!MIX_Init())
	{
		std::cout << "MIX_Init: Failed to init SDL_mixer!\n";
		std::cout << "Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Crea un mixer y si falla, aborta
	mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
	if (mixer == nullptr)
	{
		std::cout << "MIX_CreateMixerDevice Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Crea un track para la musica y si falla, aborta
	musicTrack = MIX_CreateTrack(mixer);
	if (musicTrack == nullptr)
	{
		std::cout << "MIX_CreateTrack Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Hacemos un set al mixer global y al track de musica de un 20%
	MIX_SetMixerGain(mixer, 0.2f);
	MIX_SetTrackGain(musicTrack, 0.2f);

	return true;
}

bool AudioManager::Close()
{
	// Detiene la musica
	StopMusic();

	// Limpia el track
	if (musicTrack != nullptr)
	{
		MIX_SetTrackAudio(musicTrack, nullptr);
		MIX_DestroyTrack(musicTrack);
		musicTrack = nullptr;
	}

	// Libera todos los sonidos cargados
	for (auto it = sounds.begin(); it != sounds.end(); ++it)
		MIX_DestroyAudio(it->second);

	sounds.clear();

	// Destruye el mixer
	if (mixer != nullptr)
	{
		MIX_DestroyMixer(mixer);
		mixer = nullptr;
	}

	// Cierra SDL_Mixer
	MIX_Quit();
	return true;
}

void AudioManager::PlaySFX(std::string path)
{
	// Si el mixer no existe, abortamos
	if (!mixer) return;

	// Encontramos el sonido
	MIX_Audio* audio = nullptr;
	auto it = sounds.find(path);

	//Si el sonido no esta cargado, lo carga
	if (it == sounds.end())
	{
		audio = MIX_LoadAudio(mixer, path.c_str(), true); // predecode = true para SFX, por menos latencia
		if (audio == nullptr)
		{
			std::cout << "Error loading " << path << ": " << SDL_GetError() << std::endl;
			return;
		}
		sounds[path] = audio;
	}
	else
		audio = it->second;

	// Reproduce el sonido
	if (audio != nullptr)
		MIX_PlayAudio(mixer, audio);
}

void AudioManager::SetSFXVolume(int volume)
{
	// Si el mixer no existe, abortamos
	if (!mixer) return;

	// Convertimos el volumen de 0-128 a 0-1 y seteamos los sfx al volumen deseado
	float gain = (float)volume / 128.0f;
	MIX_SetMixerGain(mixer, gain);
}

void AudioManager::SetMusicVolume(int volume)
{
	// Si el track no existe, abortamos
	if (!musicTrack) return;

	// Convertimos el volumen de 0-128 a 0-1 y seteamos la musica al volumen deseado
	float gain = (float)volume / 128.0f;
	MIX_SetTrackGain(musicTrack, gain);
}

void AudioManager::PlayMusic(std::string path, bool loop)
{
	// Si el track no existe, abortamos
	if (!musicTrack) return;

	// Si ya esta sonando esta musica, no hagas nada
	if (currentMusic == path)
		return;

	// Detenemos la musica anterior y guardamos el nombre de la nueva
	StopMusic();
	currentMusic = path;

	// Encontramos la cancion
	MIX_Audio* audio = nullptr;
	auto it = sounds.find(path);

	//Si la cancion no esta cargada, la carga
	if (it == sounds.end())
	{
		audio = MIX_LoadAudio(mixer, path.c_str(), false); // predecode = false para Musica, más latencia pero más eficiente
		if (audio == nullptr)
		{
			std::cout << "Failed to load music (" << path << "): " << SDL_GetError() << std::endl;
			return;
		}
		sounds[path] = audio;
	}
	else
		audio = it->second;

	// Reproduce la cancion
	MIX_SetTrackAudio(musicTrack, audio);

	// Si debe ser un loop, hacemos que la cancion haga loop infinitamente, si no, la detiene cuando acaba
	if (loop)
	{
		SDL_PropertiesID props = SDL_CreateProperties();
		SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
		MIX_PlayTrack(musicTrack, props);
		SDL_DestroyProperties(props);
	}
	else
		MIX_PlayTrack(musicTrack, 0);
}

void AudioManager::StopMusic()
{
	//Detenemos la musica y limpiamos el nombre
	if (musicTrack != nullptr)
		MIX_StopTrack(musicTrack, 0);
	currentMusic = "";
}
