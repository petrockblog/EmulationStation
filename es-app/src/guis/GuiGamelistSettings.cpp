#include "EmulationStation.h"
#include "GuiGamelistSettings.h"
#include "Window.h"
#include "Sound.h"
#include "Log.h"
#include "Settings.h"
#include "guis/GuiMsgBox.h"
#include "guis/GuiSettings.h"
#include "views/ViewController.h"
#include "SystemData.h"

#include <iostream>
#include <fstream>
#include <string>

#include "components/ButtonComponent.h"
#include "components/SwitchComponent.h"
#include "components/SliderComponent.h"
#include "components/TextComponent.h"
#include "components/OptionListComponent.h"
#include "components/MenuComponent.h"

GuiGamelistSettings::GuiGamelistSettings(Window* window, SystemData* system) : GuiComponent(window), 
	mMenu(window, " SYSTEM SETTINGS"), mVersion(window)
{
	// %SystemName% Settings

	// Enable on system view	[on/off]
	// System view				[optionlist]
	// Grid size				[slider]

	mSystem = system;

	// Enable on System View =========================================
	//row.elements.clear();
	systemEnable_switch = std::make_shared<SwitchComponent>(mWindow);
	bool systemEnableState = mSystem->getSystemEnabled();
	systemEnable_switch->setState(systemEnableState);
	mMenu.addWithLabel("ENABLE ON SYSTEM VIEW", systemEnable_switch);

	// VIEW MODE OPTION LIST ==========================================
	// Get what view mode this system is set to
	std::string vMode = mSystem->getSystemViewMode();
	mViewList = std::make_shared<ViewList>(mWindow, "VIEW MODE", false);

	// Create list of view modes
	mViewList->add("DEFAULT", "DEFAULT", "DEFAULT" == vMode);
	mViewList->add("GRID VIEW", "GRID VIEW", "GRID VIEW" == vMode);
	mViewList->add("DETAILED VIEW", "DETAILED VIEW", "DETAILED VIEW" == vMode);
	mViewList->add("SIMPLE VIEW", "SIMPLE VIEW", "SIMPLE VIEW" == vMode);

	mMenu.addWithLabel("VIEW MODE", mViewList);

	// GameGrid Tile Size ==============================================
	int gridsize = (int)system->getGridModSize();
	if (gridsize > 2) gridsize = 2;
	gamegrid_size = std::make_shared<ViewList>(mWindow, "GRID SIZE", false);

	gamegrid_size->add("SMALL", "SMALL", 0 == gridsize);
	gamegrid_size->add("NORMAL", "NORMAL", 1 == gridsize);
	gamegrid_size->add("BIG", "BIG", 2 == gridsize);

	mMenu.addWithLabel("GRID SIZE", gamegrid_size);

	// VERSION AND FOOTER TEXT ==========================================
	mVersion.setFont(Font::get(FONT_SIZE_SMALL));
	mVersion.setColor(0x0044FFFF);
	mVersion.setText("Additional settings in Menu->System->Emulators");
	mVersion.setAlignment(ALIGN_CENTER);
	mVersion.setPosition(0, mMenu.getSize().y() + 8);

	// Buttons
	mMenu.addButton("SAVE & APPLY", "SAVES CHANGES TO SYSTEM AND RELOADS", [this] { mSaveLevel = 2; delete this; });
	mMenu.addButton("SAVE", "SAVES CHANGES TO SYSTEM [NEEDS RESTART FOR CHANGES TO TAKE PLACE", [this] { mSaveLevel = 1; delete this; });
	mMenu.addButton("CLOSE", "DISCARDS CHANGES AND DOES NOTHING", [this] { mSaveLevel = 0; delete this; });

	addChild(&mMenu);
	//addChild(&mVersion);

	setSize(mMenu.getSize());
	setPosition((Renderer::getScreenWidth() - mSize.x()) / 2, Renderer::getScreenHeight() * 0.15f);
}

GuiGamelistSettings::~GuiGamelistSettings() {
	if (mSaveLevel > 0) {
		// Save Things
		mSystem->SystemData::setSystemViewMode(mViewList->getSelected());
		mSystem->setSystemEnabled(systemEnable_switch->getState());

		// grab gridsize and convert to a simple int
		int newsize = 1;
		if (gamegrid_size->getSelected() == "SMALL") newsize = 0;
		if (gamegrid_size->getSelected() == "BIG") newsize = 2;

		mSystem->setGridModSize(newsize);

		int saveinfo = SystemData::saveConfig();

		// Reload if user hit save&apply
		if (mSaveLevel > 1) {
			ViewController::get()->reloadAll();
			ViewController::get()->goToGameList(mSystem);
		}
	}
}

//void GuiGamelistSettings::onSizeChanged()
//{
//	mVersion.setSize(mSize.x(), 0);
//	mVersion.setPosition(0, mSize.y() - mVersion.getSize().y());
//}

bool GuiGamelistSettings::input(InputConfig* config, Input input)
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

std::vector<HelpPrompt> GuiGamelistSettings::getHelpPrompts()
{
	std::vector<HelpPrompt> prompts;
	prompts.push_back(HelpPrompt("up/down", "choose"));
	prompts.push_back(HelpPrompt("a", "select"));
	prompts.push_back(HelpPrompt("start", "close"));
	return prompts;
}
