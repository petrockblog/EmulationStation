#include "EmulationStation.h"
#include "guis/GuiSystemSettings.h"
#include "EmulationStation.h"
#include "guis/GuiMenu.h"
#include "Window.h"
#include "Sound.h"
#include "Log.h"
#include "Settings.h"
#include "guis/GuiMsgBox.h"
#include "guis/GuiSettings.h"
#include "guis/GuiScraperStart.h"
#include "guis/GuiDetectDevice.h"
#include "views/ViewController.h"

#include "components/ButtonComponent.h"
#include "components/SwitchComponent.h"
#include "components/SliderComponent.h"
#include "components/TextComponent.h"
#include "components/OptionListComponent.h"
#include "components/MenuComponent.h"
#include "VolumeControl.h"
#include "components/ProgressBarComponent.h"

GuiSystemSettings::GuiSystemSettings(Window* window) : GuiComponent(window), mMenu(window, "SETTINGS"), mVersion(window)
{

	// SOUND SETTINGS >
	// DISPLAY SETTINGS >
	// UI SETTINGS >
	// OTHER SETTINGS >

addEntry("SOUND SETTINGS", 0x777777FF, true, 
		[this] {
			auto s = new GuiSettings(mWindow, "SOUND SETTINGS");

			// volume
			auto volume = std::make_shared<SliderComponent>(mWindow, 0.f, 100.f, 1.f, "%");
			volume->setValue((float)VolumeControl::getInstance()->getVolume());
			s->addWithLabel("SYSTEM VOLUME", volume);
			s->addSaveFunc([volume] { VolumeControl::getInstance()->setVolume((int)round(volume->getValue())); });
			
			// disable sounds
			auto sounds_enabled = std::make_shared<SwitchComponent>(mWindow);
			sounds_enabled->setState(Settings::getInstance()->getBool("EnableSounds"));
			s->addWithLabel("ENABLE SOUNDS", sounds_enabled);
			s->addSaveFunc([sounds_enabled] { Settings::getInstance()->setBool("EnableSounds", sounds_enabled->getState()); });

			Window* window = mWindow;

			ComponentListRow row;
			row.makeAcceptInputHandler([window] {
				window->pushGui(new GuiMsgBox(window, "REALLY DISABLE BACKGROUND MUSIC?", "YES", 
				[] { 
					system("/home/pi/RetroPie/music/disablebgmusic.sh >/dev/null 2>&1");
					
				}, "NO", nullptr));
			});
			row.addElement(std::make_shared<TextComponent>(window, "DISABLE BACKGROUND MUSIC", Font::get(FONT_SIZE_MEDIUM), 0x777777FF), true);
			s->addRow(row);

			row.elements.clear();
			row.makeAcceptInputHandler([window] {
				window->pushGui(new GuiMsgBox(window, "REALLY ENABLE BACKGROUND MUSIC?", "YES", 
				[] { 
					system("/home/pi/RetroPie/music/enablebgmusic.sh >/dev/null 2>&1");

				}, "NO", nullptr));
			});
			row.addElement(std::make_shared<TextComponent>(window, "ENABLE BACKGROUND MUSIC", Font::get(FONT_SIZE_MEDIUM), 0x777777FF), true);

			s->addRow(row);

			mWindow->pushGui(s);
	});

addEntry("DISPLAY SETTINGS", 0x777777FF, true,
		[this] {
			auto s = new GuiSettings(mWindow, "DISPLAY SETTINGS");
			
			Window* window = mWindow;

			ComponentListRow row;
			row.makeAcceptInputHandler([window] {
				window->pushGui(new GuiMsgBox(window, "REALLY SET RESOLUTION TO 480i AND RESTART?", "YES", 
				[] { 
					system("sudo h3disp -m 0 >/dev/null 2>&1 && sudo reboot >/dev/null 2>&1");
					
				}, "NO", nullptr));
			});
			row.addElement(std::make_shared<TextComponent>(window, "480i", Font::get(FONT_SIZE_MEDIUM), 0x777777FF), true);
			s->addRow(row);

			row.elements.clear();
			row.makeAcceptInputHandler([window] {
				window->pushGui(new GuiMsgBox(window, "REALLY SET RESOLUTION TO 576i AND RESTART?", "YES", 
				[] { 
					system("sudo h3disp -m 1 >/dev/null 2>&1 && sudo reboot >/dev/null 2>&1");

				}, "NO", nullptr));
			});
			row.addElement(std::make_shared<TextComponent>(window, "576i", Font::get(FONT_SIZE_MEDIUM), 0x777777FF), true);
			s->addRow(row);

			row.elements.clear();
			row.makeAcceptInputHandler([window] {
				window->pushGui(new GuiMsgBox(window, "REALLY SET RESOLUTION TO 720P 50HZ AND RESTART?", "YES",
				[window] {
					system("sudo h3disp -m 4 >/dev/null 2>&1 && sudo reboot >/dev/null 2>&1");
				}, "NO", nullptr));
			});
			row.addElement(std::make_shared<TextComponent>(window, "720P 50HZ", Font::get(FONT_SIZE_MEDIUM), 0x777777FF), true);
			s->addRow(row);
			row.elements.clear();
			row.makeAcceptInputHandler([window] {
				window->pushGui(new GuiMsgBox(window, "REALLY SET RESOLUTION TO 720P 60HZ AND RESTART?", "YES",
				[window] {
					system("sudo h3disp -m 5 >/dev/null 2>&1 && sudo reboot >/dev/null 2>&1");
				}, "NO", nullptr));
			});
			row.addElement(std::make_shared<TextComponent>(window, "720P 60HZ", Font::get(FONT_SIZE_MEDIUM), 0x777777FF), true);
			s->addRow(row);

			row.elements.clear();
			row.makeAcceptInputHandler([window] {
				window->pushGui(new GuiMsgBox(window, "REALLY SET RESOLUTION TO 1080P 50HZ AND RESTART?", "YES", 
				[] { 
					system("sudo h3disp -m 9 >/dev/null 2>&1 && sudo reboot >/dev/null 2>&1");
				}, "NO", nullptr));
			});
			row.addElement(std::make_shared<TextComponent>(window, "1080P 50HZ", Font::get(FONT_SIZE_MEDIUM), 0x777777FF), true);
			s->addRow(row);
			row.elements.clear();
			row.makeAcceptInputHandler([window] {
				window->pushGui(new GuiMsgBox(window, "REALLY SET RESOLUTION TO 1080P 60HZ AND RESTART?", "YES", 
				[] { 
					system("sudo h3disp -m 10 >/dev/null 2>&1 && sudo reboot >/dev/null 2>&1");
				}, "NO", nullptr));
			});
			row.addElement(std::make_shared<TextComponent>(window, "1080P 60HZ", Font::get(FONT_SIZE_MEDIUM), 0x777777FF), true);
			s->addRow(row);
			row.elements.clear();
			row.makeAcceptInputHandler([window] {
				window->pushGui(new GuiMsgBox(window, "REALLY SET RESOLUTION TO 1080i 50HZ AND RESTART?", "YES", 
				[] { 
					system("sudo h3disp -m 6 >/dev/null 2>&1 && sudo reboot >/dev/null 2>&1");

				}, "NO", nullptr));
			});
			row.addElement(std::make_shared<TextComponent>(window, "1080i 50HZ", Font::get(FONT_SIZE_MEDIUM), 0x777777FF), true);
			s->addRow(row);
			row.elements.clear();
			row.makeAcceptInputHandler([window] {
				window->pushGui(new GuiMsgBox(window, "REALLY SET RESOLUTION TO 1080i 60HZ AND RESTART?", "YES", 
				[] { 
					system("sudo h3disp -m 7 >/dev/null 2>&1 && sudo reboot >/dev/null 2>&1");

				}, "NO", nullptr));
			});
			row.addElement(std::make_shared<TextComponent>(window, "1080i 60HZ", Font::get(FONT_SIZE_MEDIUM), 0x777777FF), true);
			s->addRow(row);

			row.elements.clear();
			row.makeAcceptInputHandler([window] {
				window->pushGui(new GuiMsgBox(window, "REALLY SET RESOLUTION TO 1024x768 AND RESTART?", "YES", 
				[] { 
					system("sudo h3disp -m 32 >/dev/null 2>&1 && sudo reboot >/dev/null 2>&1");
				}, "NO", nullptr));
			});
			row.addElement(std::make_shared<TextComponent>(window, "1024x768", Font::get(FONT_SIZE_MEDIUM), 0x777777FF), true);
			s->addRow(row);

			row.elements.clear();
			row.makeAcceptInputHandler([window] {
				window->pushGui(new GuiMsgBox(window, "REALLY SET RESOLUTION TO 1280x1024 AND RESTART?", "YES", 
				[] { 
					system("sudo h3disp -m 33 >/dev/null 2>&1 && sudo reboot >/dev/null 2>&1");

				}, "NO", nullptr));
			});
			row.addElement(std::make_shared<TextComponent>(window, "1280x1024", Font::get(FONT_SIZE_MEDIUM), 0x777777FF), true);
			s->addRow(row);

			row.elements.clear();
			row.makeAcceptInputHandler([window] {
				window->pushGui(new GuiMsgBox(window, "REALLY SET RESOLUTION TO 1360x768  AND RESTART?", "YES", 
				[] { 
					system("sudo h3disp -m 34 >/dev/null 2>&1 && sudo reboot >/dev/null 2>&1");

				}, "NO", nullptr));
			});
			row.addElement(std::make_shared<TextComponent>(window, "1360x768 ", Font::get(FONT_SIZE_MEDIUM), 0x777777FF), true);
			s->addRow(row);

			row.elements.clear();
			row.makeAcceptInputHandler([window] {
				window->pushGui(new GuiMsgBox(window, "REALLY SET RESOLUTION TO 1440x900  AND RESTART?", "YES", 
				[] { 
					system("sudo h3disp -m 35 >/dev/null 2>&1 && sudo reboot >/dev/null 2>&1");

				}, "NO", nullptr));
			});
			row.addElement(std::make_shared<TextComponent>(window, "1440x900 ", Font::get(FONT_SIZE_MEDIUM), 0x777777FF), true);
			s->addRow(row);

			mWindow->pushGui(s);
	});

addEntry("UI SETTINGS", 0x777777FF, true,
		[this] {
			auto s = new GuiSettings(mWindow, "UI SETTINGS");

			// screensaver time
			auto screensaver_time = std::make_shared<SliderComponent>(mWindow, 0.f, 30.f, 1.f, "m");
			screensaver_time->setValue((float)(Settings::getInstance()->getInt("ScreenSaverTime") / (1000 * 60)));
			s->addWithLabel("SCREENSAVER AFTER", screensaver_time);
			s->addSaveFunc([screensaver_time] { Settings::getInstance()->setInt("ScreenSaverTime", (int)round(screensaver_time->getValue()) * (1000 * 60)); });

			// screensaver behavior
			auto screensaver_behavior = std::make_shared< OptionListComponent<std::string> >(mWindow, "TRANSITION STYLE", false);
			std::vector<std::string> screensavers;
			screensavers.push_back("dim");
			screensavers.push_back("black");
			for(auto it = screensavers.begin(); it != screensavers.end(); it++)
				screensaver_behavior->add(*it, *it, Settings::getInstance()->getString("ScreenSaverBehavior") == *it);
			s->addWithLabel("SCREENSAVER BEHAVIOR", screensaver_behavior);
			s->addSaveFunc([screensaver_behavior] { Settings::getInstance()->setString("ScreenSaverBehavior", screensaver_behavior->getSelected()); });

			// framerate
			auto framerate = std::make_shared<SwitchComponent>(mWindow);
			framerate->setState(Settings::getInstance()->getBool("DrawFramerate"));
			s->addWithLabel("SHOW FRAMERATE", framerate);
			s->addSaveFunc([framerate] { Settings::getInstance()->setBool("DrawFramerate", framerate->getState()); });

			// show help
			auto show_help = std::make_shared<SwitchComponent>(mWindow);
			show_help->setState(Settings::getInstance()->getBool("ShowHelpPrompts"));
			s->addWithLabel("ON-SCREEN HELP", show_help);
			s->addSaveFunc([show_help] { Settings::getInstance()->setBool("ShowHelpPrompts", show_help->getState()); });

			// quick system select (left/right in game list view)
			auto quick_sys_select = std::make_shared<SwitchComponent>(mWindow);
			quick_sys_select->setState(Settings::getInstance()->getBool("QuickSystemSelect"));
			s->addWithLabel("QUICK SYSTEM SELECT", quick_sys_select);
			s->addSaveFunc([quick_sys_select] { Settings::getInstance()->setBool("QuickSystemSelect", quick_sys_select->getState()); });

			// transition style
			auto transition_style = std::make_shared< OptionListComponent<std::string> >(mWindow, "TRANSITION STYLE", false);
			std::vector<std::string> transitions;
			transitions.push_back("fade");
			transitions.push_back("slide");
			for(auto it = transitions.begin(); it != transitions.end(); it++)
				transition_style->add(*it, *it, Settings::getInstance()->getString("TransitionStyle") == *it);
			s->addWithLabel("TRANSITION STYLE", transition_style);
			s->addSaveFunc([transition_style] { Settings::getInstance()->setString("TransitionStyle", transition_style->getSelected()); });

			// theme set
			auto themeSets = ThemeData::getThemeSets();

			if(!themeSets.empty())
			{
				auto selectedSet = themeSets.find(Settings::getInstance()->getString("ThemeSet"));
				if(selectedSet == themeSets.end())
					selectedSet = themeSets.begin();

				auto theme_set = std::make_shared< OptionListComponent<std::string> >(mWindow, "THEME SET", false);
				for(auto it = themeSets.begin(); it != themeSets.end(); it++)
					theme_set->add(it->first, it->first, it == selectedSet);
				s->addWithLabel("THEME SET", theme_set);

				Window* window = mWindow;
				s->addSaveFunc([window, theme_set] 
				{
					bool needReload = false;
					if(Settings::getInstance()->getString("ThemeSet") != theme_set->getSelected())
						needReload = true;

					Settings::getInstance()->setString("ThemeSet", theme_set->getSelected());

					if(needReload)
						ViewController::get()->reloadAll(); // TODO - replace this with some sort of signal-based implementation
				});
			}

			mWindow->pushGui(s);
	});


	addEntry("OTHER SETTINGS", 0x777777FF, true,
		[this] {
			auto s = new GuiSettings(mWindow, "OTHER SETTINGS");

			// gamelists
			auto save_gamelists = std::make_shared<SwitchComponent>(mWindow);
			save_gamelists->setState(Settings::getInstance()->getBool("SaveGamelistsOnExit"));
			s->addWithLabel("SAVE METADATA ON EXIT", save_gamelists);
			s->addSaveFunc([save_gamelists] { Settings::getInstance()->setBool("SaveGamelistsOnExit", save_gamelists->getState()); });

			auto parse_gamelists = std::make_shared<SwitchComponent>(mWindow);
			parse_gamelists->setState(Settings::getInstance()->getBool("ParseGamelistOnly"));
			s->addWithLabel("PARSE GAMESLISTS ONLY", parse_gamelists);
			s->addSaveFunc([parse_gamelists] { Settings::getInstance()->setBool("ParseGamelistOnly", parse_gamelists->getState()); });

			// maximum vram
			auto max_vram = std::make_shared<SliderComponent>(mWindow, 0.f, 1000.f, 10.f, "Mb");
			max_vram->setValue((float)(Settings::getInstance()->getInt("MaxVRAM")));
			s->addWithLabel("VRAM LIMIT", max_vram);
			s->addSaveFunc([max_vram] { Settings::getInstance()->setInt("MaxVRAM", (int)round(max_vram->getValue())); });

			mWindow->pushGui(s);
	});

	mVersion.setFont(Font::get(FONT_SIZE_SMALL));
	mVersion.setColor(0xAAAAFFFF);
	mVersion.setText("BUILD " + strToUpper(PROGRAM_VERSION_STRING));
	mVersion.setAlignment(ALIGN_CENTER);

	addChild(&mMenu);
	addChild(&mVersion);

	setSize(mMenu.getSize());
	setPosition((Renderer::getScreenWidth() - mSize.x()) / 2, Renderer::getScreenHeight() * 0.15f);
}

void GuiSystemSettings::onSizeChanged()
{
	mVersion.setSize(mSize.x(), 0);
	mVersion.setPosition(0, mSize.y() - mVersion.getSize().y());
}

void GuiSystemSettings::addEntry(const char* name, unsigned int color, bool add_arrow, const std::function<void()>& func)
{
	std::shared_ptr<Font> font = Font::get(FONT_SIZE_MEDIUM);
	
	// populate the list
	ComponentListRow row;
	row.addElement(std::make_shared<TextComponent>(mWindow, name, font, color), true);

	if(add_arrow)
	{
		std::shared_ptr<ImageComponent> bracket = makeArrow(mWindow);
		row.addElement(bracket, false);
	}
	
	row.makeAcceptInputHandler(func);

	mMenu.addRow(row);
}

bool GuiSystemSettings::input(InputConfig* config, Input input)
{
	if(GuiComponent::input(config, input))
		return true;

	if((config->isMappedTo("b", input) || config->isMappedTo("start", input)) && input.value != 0)
	{
		delete this;
		return true;
	}

	return false;
}

std::vector<HelpPrompt> GuiSystemSettings::getHelpPrompts()
{
	std::vector<HelpPrompt> prompts;
	prompts.push_back(HelpPrompt("up/down", "choose"));
	prompts.push_back(HelpPrompt("a", "select"));
	prompts.push_back(HelpPrompt("start", "close"));
	return prompts;
}
