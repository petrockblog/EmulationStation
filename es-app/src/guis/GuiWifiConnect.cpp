#include "EmulationStation.h"
#include "guis/GuiWifiConnect.h"
#include "Window.h"
#include "Log.h"
#include "Settings.h"
#include "guis/GuiMsgBox.h"
#include "guis/GuiSettings.h"

#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <string>

#include "components/ButtonComponent.h"
#include "components/SwitchComponent.h"
#include "components/SliderComponent.h"
#include "components/TextComponent.h"
#include "components/OptionListComponent.h"
#include "components/MenuComponent.h"
#include "guis/GuiTextEditPopup.h"

GuiWifiConnect::GuiWifiConnect(Window* window, std::string wifiName, bool encrypted) : GuiComponent(window), mMenu(window, "Connect to wifi"), mVersion(window)
{
	// CONNECT TO WIFI

	// ENTER PASSWORD >
	// GET FULL DETAILS >
	// CONNECT >

	// --------------

	std::string windowName = "Connect to -> " + wifiName;
	mMenu.setTitle(windowName.c_str(), Font::get(FONT_SIZE_MEDIUM));

	auto ed = std::make_shared<TextComponent>(mWindow, "", Font::get(FONT_SIZE_SMALL, FONT_PATH_LIGHT), 0x777777FF, ALIGN_RIGHT);


	/// ======  ENTER PASSWORD  ======
	// if network is ecrypted:
	if (encrypted) {
		addEntry("ENTER PASSWORD", 0x777777FF, true, [this, wifiName, ed] {
			// create callback function to store password from keyboard popup
			auto updateVal = [ed](const std::string& newVal) { ed->setValue(newVal); }; // ok callback (apply new value to ed)

			// popup the keyboard.
			mWindow->pushGui(new GuiTextEditPopup(mWindow, wifiName + " PASSWORD", ed->getValue(), updateVal, false));
		});
	}
	else {
		// When network is NOT encrpyted and OPEN:
		addEntry("ADD PASSWORD [NETOWRK IS OPEN]", 0x777777FF, true, [this, wifiName, ed] {
			// create callback function to store password from keyboard popup
			auto updateVal = [ed](const std::string& newVal) { ed->setValue(newVal); }; // ok callback (apply new value to ed)

			// popup the keyboard.
			mWindow->pushGui(new GuiTextEditPopup(mWindow, wifiName + " PASSWORD", ed->getValue(), updateVal, false));
		});
	}


	/// ===== FULL DETAILS =====
	addEntry("GET FULL DETAILS", 0x777777FF, true, 
		[this, wifiName] {
			auto s = new GuiSettings(mWindow, "FULL DETAILS");

			// scan only this ssid
			FILE *fp;
			char path[1035];
			std::string command = "sudo iwlist wlan0 scanning essid \"" + wifiName + "\" | grep 'Channel:\\|IEEE\\|Signal'";
			fp = popen(command.c_str(), "r");

			// Sort through scanned essid
			std::string currentLine;
			std::size_t found = 0;
			std::string wChannel, wIEEE, wSignal;

			while (fgets(path, sizeof(path), fp) != NULL) {
				currentLine = path;

				found = currentLine.find("Channel:");
				if (found != std::string::npos) {
					std::string channel = currentLine;
					channel = channel.substr(found + 8) + "";
					int trim = channel.find("\n");
					channel = channel.substr(0, trim - 1);
					wChannel = channel;
				}

				found = currentLine.find("Quality");
				if (found != std::string::npos) {
					std::string wQuality = currentLine;
					wQuality = wQuality.substr(found);
					int trim = wQuality.find("\n");
					wQuality = wQuality.substr(0, trim - 1);		// trim out \n
					wSignal = wQuality;
				}

				found = currentLine.find("IEEE");
				if (found != std::string::npos) {
					wIEEE = currentLine.substr(found);
					int endr = wIEEE.find("\n");
					wIEEE = wIEEE.substr(0, endr - 1);
				}

			}

			// ESSID
			auto show_ssid = std::make_shared<TextComponent>(mWindow, wifiName, Font::get(FONT_SIZE_MEDIUM), 0x777777FF);
			s->addWithLabel("Network Name", show_ssid);

			auto show_channel = std::make_shared<TextComponent>(mWindow, wChannel, Font::get(FONT_SIZE_MEDIUM), 0x777777FF);
			s->addWithLabel("Channel", show_channel);

			auto show_encryption = std::make_shared<TextComponent>(mWindow, wIEEE, Font::get(FONT_SIZE_MEDIUM), 0x777777FF);
			s->addWithLabel("Encryption", show_encryption);

			auto show_quality = std::make_shared<TextComponent>(mWindow, wSignal, Font::get(FONT_SIZE_MEDIUM), 0x777777FF);
			s->addWithLabel("Signal", show_quality);

			mWindow->pushGui(s);
	});

	addEntry("CONNECT", 0x777777FF, true, [this, ed, wifiName, encrypted] {
		// make sure the password is at least 8 characters long
		if (ed->getValue().length() < 8 && encrypted) {
			mWindow->pushGui(new GuiMsgBox(mWindow, "Password is not long enough!  Must be at least 8 characters long.", "Ok", nullptr));
			return;
		}
		mWindow->pushGui(new GuiMsgBox(mWindow, "Network: " + wifiName + "\n Password: " + ed->getValue(), "Connect", [this, wifiName, ed] {
			// Quick and dirty just send the info to wificonnect
			std::string cmdStr = "sudo " + getHomePath() + "/.emulationstation/app/wifi/./wificonnect --ssid '" + wifiName + "' --password " + ed->getValue();
			const char* cmd = cmdStr.c_str();

			// Make sure wificonnect exists
			std::string path = getHomePath() + "/.emulationstation/app/wifi/wificonnect";
			if (boost::filesystem::exists(path)) {
				system(cmd);
				mConnected = true;
			} else {
				mWindow->pushGui(new GuiMsgBox(mWindow, "wificonnect is missing.  This is used to send wifi info to wpa_supplicant.  This should be in ~/.emulationstation/app/wifi", "OK", nullptr));
				LOG(LogError) << "WifiConnect: Couldn't find wificonnect in " << path << " folder";
			}


			//std::ofstream oFile;
			//oFile.open("~/.emulationstation/networks.lst", std::ofstream::out | std::ofstream::app);
			//oFile << wifiName;
			//oFile.close();

			// If wpa_supplicant.conf couldn't be opened.
			//mWindow->pushGui(new GuiMsgBox(mWindow, "Could not open /etc/wpa_supplicant/wpa_supplicant.conf file.  This could be permission issues.", "Ok", nullptr, "Cancel", nullptr));

		},

			"Cancel", nullptr));
	});


	mVersion.setFont(Font::get(FONT_SIZE_SMALL));
	mVersion.setColor(0x0044FFFF);
	mVersion.setText("GUIWIFI");
	mVersion.setAlignment(ALIGN_CENTER);

	addChild(&mMenu);
	addChild(&mVersion);

	setSize(mMenu.getSize());
	setPosition((Renderer::getScreenWidth() - mSize.x()) / 2, Renderer::getScreenHeight() * 0.15f);
}

void GuiWifiConnect::onSizeChanged()
{
	mVersion.setSize(mSize.x(), 0);
	mVersion.setPosition(0, mSize.y() - mVersion.getSize().y());
}

void GuiWifiConnect::addEntry(const char* name, unsigned int color, bool add_arrow, const std::function<void()>& func)
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

bool GuiWifiConnect::input(InputConfig* config, Input input)
{
	if(GuiComponent::input(config, input))
		return true;

	if((config->isMappedTo("b", input) || config->isMappedTo("start", input)) && input.value != 0)
	{
		delete this;
		return true;
	}

	if (mConnected) {
		delete this;
		return true;
	}

	return false;
}

std::vector<HelpPrompt> GuiWifiConnect::getHelpPrompts()
{
	std::vector<HelpPrompt> prompts;
	prompts.push_back(HelpPrompt("up/down", "choose"));
	prompts.push_back(HelpPrompt("a", "select"));
	prompts.push_back(HelpPrompt("start", "close"));
	return prompts;
}
