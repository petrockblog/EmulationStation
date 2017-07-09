#include "guis/GuiSlideshowScreensaverOptions.h"
#include "Window.h"
#include "Settings.h"
#include "views/ViewController.h"

#include "components/ButtonComponent.h"
#include "components/SwitchComponent.h"
#include "components/SliderComponent.h"
#include "components/TextComponent.h"
#include "components/OptionListComponent.h"
#include "components/MenuComponent.h"
#include "guis/GuiMsgBox.h"
#include "guis/GuiTextEditPopup.h"

GuiSlideshowScreensaverOptions::GuiSlideshowScreensaverOptions(Window* window, const char* title) : GuiScreensaverOptions(window, title)
{
	ComponentListRow row;

	auto sss_image_sec = std::make_shared<SliderComponent>(mWindow, 5.0f, 45.0f, 1.0f, "s");
	sss_image_sec->setValue(Settings::getInstance()->getFloat("SlideshowScreenSaverImageSec"));
	addWithLabel(row, "IMAGE TIME (SEC)", sss_image_sec);
	addSaveFunc([sss_image_sec] {
	    Settings::getInstance()->setFloat("SlideshowScreenSaverImageSec", sss_image_sec->getValue());
	  });

	auto sss_stretch = std::make_shared<SwitchComponent>(mWindow);
	sss_stretch->setState(Settings::getInstance()->getBool("SlideshowScreenSaverStretch"));
	addWithLabel(row, "STRETCH IMAGES", sss_stretch);
	addSaveFunc([sss_stretch] {
	    Settings::getInstance()->setBool("SlideshowScreenSaverStretch", sss_stretch->getState());
	  });

	auto sss_bg_audio_file = std::make_shared<TextComponent>(mWindow);
	addEditableTextComponent(row, "BACKGROUND AUDIO", sss_bg_audio_file, Settings::getInstance()->getString("SlideshowScreenSaverBackgroundAudioFile"));
	addSaveFunc([sss_bg_audio_file] {
	    Settings::getInstance()->setString("SlideshowScreenSaverBackgroundAudioFile", sss_bg_audio_file->getValue());
	  });

	// By default, we will show images from the game list
	// This overrides that behavior and uses a custom directory specified in the options below

	auto sss_custom_images = std::make_shared<SwitchComponent>(mWindow);
	sss_custom_images->setState(Settings::getInstance()->getBool("SlideshowScreenSaverCustomImages"));
	addWithLabel(row, "USE CUSTOM IMAGES", sss_custom_images);
	addSaveFunc([sss_custom_images] {
	    Settings::getInstance()->setBool("SlideshowScreenSaverCustomImages", sss_custom_images->getState());
	  });

	auto sss_image_dir = std::make_shared<TextComponent>(mWindow);
	addEditableTextComponent(row, "CUSTOM IMAGE DIR", sss_image_dir, Settings::getInstance()->getString("SlideshowScreenSaverImageDir"));
	addSaveFunc([sss_image_dir] {
	    Settings::getInstance()->setString("SlideshowScreenSaverImageDir", sss_image_dir->getValue());
	  });

	auto sss_recurse = std::make_shared<SwitchComponent>(mWindow);
	sss_recurse->setState(Settings::getInstance()->getBool("SlideshowScreenSaverRecurse"));
	addWithLabel(row, "CUSTOM IMAGE DIR RECURSIVE", sss_recurse);
	addSaveFunc([sss_recurse] {
	    Settings::getInstance()->setBool("SlideshowScreenSaverRecurse", sss_recurse->getState());
	  });

	auto sss_image_filter = std::make_shared<TextComponent>(mWindow);
	addEditableTextComponent(row, "CUSTOM IMAGE FILTER", sss_image_filter, Settings::getInstance()->getString("SlideshowScreenSaverImageFilter"));
	addSaveFunc([sss_image_filter] {
	    Settings::getInstance()->setString("SlideshowScreenSaverImageFilter", sss_image_filter->getValue());
	  });
}

GuiSlideshowScreensaverOptions::~GuiSlideshowScreensaverOptions()
{
}

void GuiSlideshowScreensaverOptions::addWithLabel(ComponentListRow row, const std::string label, std::shared_ptr<GuiComponent> component)
{
	row.elements.clear();

	auto lbl = std::make_shared<TextComponent>(mWindow, strToUpper(label), Font::get(FONT_SIZE_SMALL), 0x777777FF);
	row.addElement(lbl, true); // label

	row.addElement(component, false, true);

	addRow(row);
}

void GuiSlideshowScreensaverOptions::addEditableTextComponent(ComponentListRow row, const std::string label, std::shared_ptr<GuiComponent> ed, std::string value)
{
	row.elements.clear();

	auto lbl = std::make_shared<TextComponent>(mWindow, strToUpper(label), Font::get(FONT_SIZE_SMALL), 0x777777FF);
	row.addElement(lbl, true); // label

	//std::shared_ptr<GuiComponent> ed = std::make_shared<TextComponent>(window, "", Font::get(FONT_SIZE_SMALL, FONT_PATH_LIGHT), 0x777777FF, ALIGN_RIGHT);
	row.addElement(ed, true);

	auto spacer = std::make_shared<GuiComponent>(mWindow);
	spacer->setSize(Renderer::getScreenWidth() * 0.005f, 0);
	row.addElement(spacer, false);

	auto bracket = std::make_shared<ImageComponent>(mWindow);
	bracket->setImage(":/arrow.svg");
	bracket->setResize(Eigen::Vector2f(0, lbl->getFont()->getLetterHeight()));
	row.addElement(bracket, false);

	auto updateVal = [ed](const std::string& newVal) { ed->setValue(newVal); }; // ok callback (apply new value to ed)
	row.makeAcceptInputHandler([this, label, ed, updateVal] {
		mWindow->pushGui(new GuiTextEditPopup(mWindow, label, ed->getValue(), updateVal, false));
	});

	assert(ed);
	addRow(row);
	ed->setValue(value);
}
