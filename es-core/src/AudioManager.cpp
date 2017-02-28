#include "AudioManager.h"

#include <SDL.h>
#include "Log.h"

std::vector<std::shared_ptr<Sound>> AudioManager::sSoundVector;
std::vector<std::shared_ptr<Music>> AudioManager::sMusicVector;
std::shared_ptr<AudioManager> AudioManager::sInstance;

AudioManager::AudioManager(): currentMusic(NULL)
{
	init();
}

AudioManager::~AudioManager()
{
	deinit();
}

std::shared_ptr<AudioManager> & AudioManager::getInstance()
{
	//check if an AudioManager instance is already created, if not create one
	if (sInstance == nullptr) {
		sInstance = std::shared_ptr<AudioManager>(new AudioManager);
	}
	return sInstance;
}

void AudioManager::init()
{
	if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
	{
		LOG(LogError) << "Error initializing SDL audio!\n" << SDL_GetError();
		return;
	}


	//Open the audio device and pause
	if (Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
		LOG(LogError) << "Music Error - Unable to open SDL_mixer audio: " << SDL_GetError() << std::endl;
	}
}

void AudioManager::deinit()
{
	//stop all playback
	stop();
	//completely tear down SDL audio. else SDL hogs audio resources and emulators might fail to start...
	Mix_HaltMusic();
        Mix_CloseAudio();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void AudioManager::stopMusic()
{
       Mix_FadeOutMusic(1000);

       Mix_HaltMusic();
       currentMusic = NULL;
}

void AudioManager::startMusic(const std::shared_ptr<ThemeData>& theme)
{
    std::shared_ptr<Music> bgsound = Music::getFromTheme(theme,"system", "bgsound");
    if(bgsound){
         stopMusic();
         bgsound->play();
         currentMusic = bgsound;
    }else {
        LOG(LogError) << "NO SOUND FOUND";
        stopMusic();
    }
}

void AudioManager::resumeMusic(){
    init();
    if(currentMusic != NULL){
        currentMusic->play();
    }
}

void AudioManager::registerSound(std::shared_ptr<Sound> & sound)
{
	getInstance();
	sSoundVector.push_back(sound);
}

void AudioManager::registerMusic(std::shared_ptr<Music> & music)
{
	getInstance();
	sMusicVector.push_back(music);
}

void AudioManager::unregisterSound(std::shared_ptr<Sound> & sound)
{
	getInstance();
	for(unsigned int i = 0; i < sSoundVector.size(); i++)
	{
		if(sSoundVector.at(i) == sound)
		{
			sSoundVector[i]->stop();
			sSoundVector.erase(sSoundVector.begin() + i);
			return;
		}
	}
	LOG(LogError) << "AudioManager Error - tried to unregister a sound that wasn't registered!";
}

void AudioManager::unregisterMusic(std::shared_ptr<Music> & music)
{
	getInstance();
	for(unsigned int i = 0; i < sMusicVector.size(); i++)
	{
		if(sMusicVector.at(i) == music)
		{
			//sMusicVector[i]->stop();
			sMusicVector.erase(sMusicVector.begin() + i);
			return;
		}
	}
	LOG(LogError) << "AudioManager Error - tried to unregister music that wasn't registered!";
}

void AudioManager::play()
{
	getInstance();

	//unpause audio, the mixer will figure out if samples need to be played...
	SDL_PauseAudio(0);
}

void AudioManager::stop()
{
	//stop playing all Sounds
	for(unsigned int i = 0; i < sSoundVector.size(); i++)
	{
		if(sSoundVector.at(i)->isPlaying())
		{
			sSoundVector[i]->stop();
		}
	}
	//pause audio
	SDL_PauseAudio(1);
}
