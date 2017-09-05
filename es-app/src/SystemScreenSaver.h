#pragma once

#include "Window.h"
#ifdef _RPI_
#include "interface/vchi/vchi.h"
#include "bcm_host.h"
#endif

class VideoComponent;

// Screensaver implementation for main window
class SystemScreenSaver : public Window::ScreenSaver
{
public:
	SystemScreenSaver(Window* window);
	virtual ~SystemScreenSaver();

	virtual void startScreenSaver();
	virtual void stopScreenSaver();
	virtual void nextVideo();
	virtual void renderScreenSaver();
	virtual bool allowSleep();
	virtual void update(int deltaTime);
	virtual bool isScreenSaverActive();

	virtual FileData* getCurrentGame();
	virtual void launchGame();

private:
	void	countVideos();
	void	pickRandomVideo(std::string& path);

	void input(InputConfig* config, Input input);

	enum STATE {
		STATE_INACTIVE,
		STATE_FADE_OUT_WINDOW,
		STATE_FADE_IN_VIDEO,
		STATE_SCREENSAVER_ACTIVE
	};

private:
	bool			mCounted;
	unsigned long	mVideoCount;
	VideoComponent* mVideoScreensaver;
	Window*			mWindow;
	STATE			mState;
	float			mOpacity;
	int				mTimer;
	FileData*		mCurrentGame;
	std::string		mGameName;
	std::string		mSystemName;
	int 			mVideoChangeTime;
	bool 			mVchiInit;
  	VCHI_INSTANCE_T 	mVchiInstance;
	VCHI_CONNECTION_T 	*mVchiConnection = nullptr;

};
