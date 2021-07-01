#pragma once
#ifndef ES_APP_SYSTEM_SCREEN_SAVER_H
#define ES_APP_SYSTEM_SCREEN_SAVER_H

#include "Window.h"
#include <random>
#include <thread>

class ImageComponent;
class Sound;
class VideoComponent;

// Screensaver implementation for main window
class SystemScreenSaver : public Window::ScreenSaver
{
public:
	SystemScreenSaver(Window* window);
	virtual ~SystemScreenSaver();

	virtual void startScreenSaver();
	virtual void stopScreenSaver();
	virtual void nextMediaItem();
	virtual void renderScreenSaver();
	virtual bool allowSleep();
	virtual void update(int deltaTime);
	virtual bool isScreenSaverActive();

	virtual FileData* getCurrentGame();
	virtual void launchGame();

private:
	void pickGameListNode(const char *nodeName, std::string& path);
	void pickRandomVideo(std::string& path);
	void pickRandomGameListImage(std::string& path);
	void pickRandomCustomImage(std::string& path);
	std::vector<std::string> getCustomImageFiles(const std::string &imageDir);
	std::vector<FileData*> getAllGamelistNodes();
	void backgroundIndexing();

	void input(InputConfig* config, Input input);

	enum STATE {
		STATE_INACTIVE,
		STATE_FADE_OUT_WINDOW,
		STATE_FADE_IN_VIDEO,
		STATE_SCREENSAVER_ACTIVE
	};

private:
	VideoComponent*		mVideoScreensaver;
	ImageComponent*		mImageScreensaver;
	Window*			mWindow;
	STATE			mState;
	float			mOpacity;
	int			mTimer;
	FileData*		mCurrentGame;
	int 			mSwapTimeout;
	std::shared_ptr<Sound>	mBackgroundAudio;
	bool			mStopBackgroundAudio;
	std::vector<FileData*> 		mAllFiles;
	std::vector<std::string>	mCustomImageFiles;
	int 				mAllFilesSize;
	std::ranlux48			mUrng;
	std::thread*				mThread;
	bool 						mExit;
};

#endif // ES_APP_SYSTEM_SCREEN_SAVER_H
