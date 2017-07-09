#include "guis/GuiSlideshowScreensaverOptions.h"
#include "Window.h"
#include "Settings.h"
#include "views/ViewController.h"

#include "components/ButtonComponent.h"
#include "components/SwitchComponent.h"
#include "components/SliderComponent.h"
#include "components/TextComponent.h"
//#include "components/TextEditComponent.h"
#include "components/OptionListComponent.h"
#include "components/MenuComponent.h"
#include "guis/GuiMsgBox.h"

GuiSlideshowScreensaverOptions::GuiSlideshowScreensaverOptions(Window* window, const char* title) : GuiScreensaverOptions(window, title)
{
	auto sss_image_sec = std::make_shared<SliderComponent>(mWindow, 5.0f, 45.0f, 1.0f, "s");
	sss_image_sec->setValue(Settings::getInstance()->getFloat("SlideshowScreenSaverImageSec"));
	addWithLabel("IMAGE TIME (SEC)", sss_image_sec);
	addSaveFunc([sss_image_sec] {
	    Settings::getInstance()->setFloat("SlideshowScreenSaverImageSec", sss_image_sec->getValue());
	  });

	/*
	auto sss_image_dir = std::make_shared<TextEditComponent>(mWindow);
	sss_image_dir->setValue(Settings::getInstance()->getString("SlideshowScreenSaverImageDir"));
	addWithLabel("SLIDESHOW SCREENSAVER IMAGE PATH", sss_image_dir);
	addSaveFunc([sss_image_dir] {
	    Settings::getInstance()->setString("SlideshowScreenSaverImageDir", sss_image_dir->getValue());
	  });
	*/

	auto sss_image_dir = std::make_shared<TextComponent>(mWindow);
	sss_image_dir->setValue(Settings::getInstance()->getString("SlideshowScreenSaverImageDir"));
	addWithLabel("IMAGE DIR", sss_image_dir);

	auto sss_recurse = std::make_shared<SwitchComponent>(mWindow);
	sss_recurse->setState(Settings::getInstance()->getBool("SlideshowScreenSaverRecurse"));
	addWithLabel("IMAGE DIR RECURSIVE", sss_recurse);
	addSaveFunc([sss_recurse] {
	    Settings::getInstance()->setBool("SlideshowScreenSaverRecurse", sss_recurse->getState());
	  });

	auto sss_image_filter = std::make_shared<TextComponent>(mWindow);
	sss_image_filter->setValue(Settings::getInstance()->getString("SlideshowScreenSaverImageFilter"));
	addWithLabel("IMAGE FILTER", sss_image_filter);

 	auto sss_stretch = std::make_shared<SwitchComponent>(mWindow);
	sss_stretch->setState(Settings::getInstance()->getBool("SlideshowScreenSaverStretch"));
	addWithLabel("STRETCH IMAGES", sss_stretch);
	addSaveFunc([sss_stretch] {
	    Settings::getInstance()->setBool("SlideshowScreenSaverStretch", sss_stretch->getState());
	  });

	auto sss_bg_audio_file = std::make_shared<TextComponent>(mWindow);
	sss_bg_audio_file->setValue(Settings::getInstance()->getString("SlideshowScreenSaverBackgroundAudioFile"));
	addWithLabel("BACKGROUND AUDIO", sss_bg_audio_file);
}

GuiSlideshowScreensaverOptions::~GuiSlideshowScreensaverOptions()
{
}
