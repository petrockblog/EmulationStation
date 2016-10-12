
#include "EmulationStation.h"
#include "guis/GuiEmulatorList.h"
#include "guis/GuiSettings.h"
#include "Window.h"
#include "Sound.h"
#include "Log.h"
#include "Settings.h"
#include "guis/GuiMsgBox.h"
#include "SystemData.h"
#include <boost/filesystem.hpp>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <stdlib.h>

#include "components/ButtonComponent.h"
#include "components/SwitchComponent.h"
#include "components/SliderComponent.h"
#include "components/TextComponent.h"
#include "components/OptionListComponent.h"
#include "components/MenuComponent.h"

namespace fs = boost::filesystem;

GuiEmulatorList::GuiEmulatorList(Window* window) : GuiComponent(window), mMenu(window, "SIZE PER SYSTEM"), mVersion(window)
{
	// EMULATORS AND SYSTEMS

	// [ list of installed emulators ]
	// ...
	// ...

	// [ onclick-> ]
	//		[name of system] SETTINGS:
	
	//		ENABLE IN SYSTEMS LIST? <switch>
	//		DELETE THIS SYSTEM?	<Button>
	//		

	int installedSystems = 0;

	// Grab instance of system vector
	std::vector<SystemData*>& vSystem = SystemData::sSystemVector;

	for (int i = 0; i < vSystem.size(); i++){
		SystemData* sys = vSystem[i];
		addEntry(sys->getName().c_str(), 0x777777FF, true, [this, sys] {

			auto s = new GuiSettings(mWindow, sys->getFullName().c_str());

			//auto enable_system = std::make_shared<SwitchComponent>(mWindow);
			//enable_system->setState(true);
			//s->addWithLabel("SHOW ON SYSTEM VIEW", enable_system);
			//s->addSaveFunc([this, enable_system, sys] {
				//sys->setSystemEnabled(enable_system->getState());
				//SystemData::saveConfig();
			//});

			//auto change_name = std::make_shared<TextComponent>(mWindow);
			//change_name->setValue(sys->getFullName());
			//s->addWithLabel("CHANGE SYSTEM NAME", change_name);
			//s->addSaveFunc([this, change_name, sys] {
			//	// popup the keyboard.
			//	mWindow->pushGui(new GuiTextEditPopupKeyboard(mWindow, sys->getFullName(), change_name->getValue(), 
			//	[sys, change_name] (const std::string& newVal) {
			//		change_name->setValue(newVal); 
			//		sys->setFullName(newVal);
			//	}, false));
			//});

			/// Create item for storing emulating system's rom size
			auto system_size = std::make_shared<TextComponent>(mWindow);
			const fs::path& fpath = sys->getStartPath();
			double rSize = 0;
			int romCount = 0;
			int saveCount = 0;

			// Check the path exists (just to be safe)
			if (fs::exists(fpath)) {
				fs::recursive_directory_iterator end;

				// Go through each file in the path.
				for (fs::recursive_directory_iterator i(fpath); i != end; ++i){
					const fs::path tpath = (*i);

					// Make sure the file exists.  (This saves crash when roms are in from two locations)
					if (fs::exists(tpath)) {
						// Try and get file size.  Some roms are stored in hidden locations so this just makes sure no crashes happen.
						try {
							rSize += fs::file_size(tpath);
						}
						catch (fs::filesystem_error& e) {
							LOG(LogError) << e.what();
						}
						romCount++;

						// look for saves too
						std::string sav = tpath.filename().string();
						int ext = sav.find_last_of(".");
						ext++;
						// This should get from SystemData as some systems have differnt save formats.
						if (sav.substr(ext) == "srm" || sav.substr(ext) == "state") {
							saveCount++;
						}
					}
					
				}

				// Figure out what measurement units to use.
				std::stringstream ss;
				if (rSize > 1000000000) 		// GB
					ss << std::setprecision(4) << rSize / 1000000000 << " GB";
				else if (rSize > 1000000)
					ss << std::setprecision(4) << rSize / 1000000 << " MB";
				else if (rSize > 1000)
					ss << std::setprecision(4) << rSize / 1000 << " KB";
				else
					ss << rSize << " Bytes";

				system_size->setValue(ss.str());
				s->addWithLabel("TOTAL ROM SIZE", system_size);

			} else {
				system_size->setValue("Unknown Folder");
				s->addWithLabel("TOTAL ROM SIZE", system_size);
			}
			
			std::stringstream ss;
			ss << saveCount;
			auto system_savecount = std::make_shared<TextComponent>(mWindow);
			system_savecount->setValue(ss.str());
			s->addWithLabel("SAVEGAME FILES", system_savecount);



			mWindow->pushGui(s);
		});

		installedSystems = i + 1;
	}


	mVersion.setFont(Font::get(FONT_SIZE_SMALL));
	mVersion.setColor(0xAAAAFFFF);
	std::stringstream mVerText;
	mVerText << "SYSTEMS REPORTED: " << installedSystems;
	mVersion.setText(mVerText.str());
	mVersion.setAlignment(ALIGN_CENTER);

	addChild(&mMenu);
	addChild(&mVersion);

	setSize(mMenu.getSize());
	setPosition((Renderer::getScreenWidth() - mSize.x()) / 2, Renderer::getScreenHeight() * 0.15f);
}

void GuiEmulatorList::onSizeChanged()
{
	mVersion.setSize(mSize.x(), 0);
	mVersion.setPosition(0, mSize.y() - mVersion.getSize().y());
}

void GuiEmulatorList::addEntry(const char* name, unsigned int color, bool add_arrow, const std::function<void()>& func)
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



bool GuiEmulatorList::input(InputConfig* config, Input input)
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

std::vector<HelpPrompt> GuiEmulatorList::getHelpPrompts()
{
	std::vector<HelpPrompt> prompts;
	prompts.push_back(HelpPrompt("up/down", "choose"));
	prompts.push_back(HelpPrompt("a", "select"));
	prompts.push_back(HelpPrompt("start", "close"));
	return prompts;
}
