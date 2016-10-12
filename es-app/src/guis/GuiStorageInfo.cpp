#include "EmulationStation.h"
#include "guis/GuiStorageInfo.h"
#include "Window.h"
#include "Sound.h"
#include "Log.h"
#include "Settings.h"
#include "guis/GuiMsgBox.h"
#include "guis/GuiSettings.h"
#include "guis/GuiEmulatorList.h"

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
#include "components/ProgressBarComponent.h"
#include "components/MenuComponent.h"


GuiStorageInfo::GuiStorageInfo(Window* window) : GuiComponent(window), mMenu(window, "STORAGE"), mVersion(window)
{
	// STORAGE INFO

	// PROGRESS BAR
	// TOTAL IN SIZE
	// TOTAL AVAILABLE

	// ---


	// dump stuff from pipe.
	FILE *fp;
	char path[1035];
	fp = popen("df / -B M --output=size,avail,pcent", "r");

	std::string cLine;
	while (fgets(path, sizeof(path), fp) != NULL) {
		// Grab the last line.
		cLine = path;
	}

	// format the info out and put into ints.
	std::string f1, f2, f3;
	int f = cLine.find("M");
	f1 = cLine.substr(0, f);
	int q = cLine.substr(f + 1).find("M");
	f2 = cLine.substr(f + 1, q);
	int p = cLine.find("%");
	f3 = cLine.substr(p - 3, p);

	// convert to ints
	float iSize = stof(f1);
	float iAvailable = stof(f2);
	int iPerc = stoi(f3);

	// get size in GB then put it back to string
	std::string totalSizeInGb;
	std::string totalAvailableInGb;
	std::stringstream ss, s2;
	ss << std::setprecision(4) << float(iSize / 1000);
	s2 << std::setprecision(4) << float(iAvailable / 1000);
	totalSizeInGb = ss.str();
	totalAvailableInGb = s2.str();

	// get used
	int iUsed = iSize - iAvailable;

	// Get percents for progressbars
	//float pUsed = (float(iSize / iUsed) * 10);		// Old garbage way.
	std::stringstream s3;
	s3 << iPerc;

	ComponentListRow row;

	// Get 
		
	// Create rows
	// Pbar for Total used [ This feels kinda hacky..  progressbar is not well made ]
	pbar_total = std::make_shared<ProgressBarComponent>(mWindow, "ppp");
	auto tell_perc = std::make_shared<TextComponent>(mWindow, s3.str() + "%", Font::get(FONT_SIZE_MEDIUM), 0x777777FF);
	tell_perc->setAlignment(ALIGN_RIGHT);
	int pbSize = mMenu.getSize().x() * .8f;
	pbar_total->setSize(pbSize, mMenu.getSize().y() * .09f);
	pbar_total->setPosition(0, 0);
	pbar_total->setValue(iPerc);
	pbar_total->setColor(0x9999EEFF);
	row.addElement(pbar_total, false);
	row.addElement(tell_perc, true);
	mMenu.addRow(row);

	row.elements.clear();
		
	auto tell_totalsize = std::make_shared<TextComponent>(mWindow, "TOTAL DISK SIZE", Font::get(FONT_SIZE_MEDIUM), 0x777777FF);
	auto tell_totalsize_i = std::make_shared<TextComponent>(mWindow, "" + totalSizeInGb + " GB", Font::get(FONT_SIZE_MEDIUM), 0x777777FF);
	tell_totalsize_i->setAlignment(ALIGN_RIGHT);
	row.addElement(tell_totalsize, true);
	row.addElement(tell_totalsize_i, true);
	mMenu.addRow(row);

	row.elements.clear();

	auto tell_free = std::make_shared<TextComponent>(mWindow, "TOTAL DISK FREE", Font::get(FONT_SIZE_MEDIUM), 0x777777FF);
	auto tell_free_i = std::make_shared<TextComponent>(mWindow, "" + totalAvailableInGb + " GB", Font::get(FONT_SIZE_MEDIUM), 0x777777FF);
	tell_free_i->setAlignment(ALIGN_RIGHT);
	row.addElement(tell_free, true);
	row.addElement(tell_free_i, true);
	mMenu.addRow(row);

	addEntry("STORAGE PER SYSTEM", 0x777777FF, true, [this] {
		mWindow->pushGui(new GuiEmulatorList(mWindow));
	});

	//getRemovableInfo();


	mVersion.setFont(Font::get(FONT_SIZE_SMALL));
	mVersion.setColor(0xAAAAFFFF);
	//mVersion.setText("RAW: " + strToUpper(cLine));
	mVersion.setAlignment(ALIGN_CENTER);

	addChild(&mMenu);
	addChild(&mVersion);

	setSize(mMenu.getSize());
	setPosition((Renderer::getScreenWidth() - mSize.x()) / 2, Renderer::getScreenHeight() * 0.15f);
}

void GuiStorageInfo::onSizeChanged()
{
	mVersion.setSize(mSize.x(), 0);
	mVersion.setPosition(0, mSize.y() - mVersion.getSize().y());
}

void GuiStorageInfo::addEntry(const char* name, unsigned int color, bool add_arrow, const std::function<void()>& func)
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


void GuiStorageInfo::getRemovableInfo() {
	FILE *fp;
	char path[1035];

	// try and get jumpdrives
	fp = popen("lsblk -d --output name,model,size,mountpoint | grep \"sd\\|NAME\"", "r");

	std::string deviceLine;
	int iName = 0, iModel = 0, iSize = 0, iMp = 0;
	std::vector< std::vector<std::string> > devices;
	int totDevices = 0;

	while (fgets(path, sizeof(path), fp) != NULL) {
		deviceLine = path;
		LOG(LogError) << totDevices << " : " << deviceLine;
		// Get column sizes
		if (deviceLine.find("NAME") != std::string::npos) {
			iName = deviceLine.find("NAME");
			iModel = deviceLine.find("MODEL");
			iSize = deviceLine.find("SIZE");
			iMp = deviceLine.find("MOUNTPOINT");
			LOG(LogError) << iName << " " << iModel << " " << iSize;
			continue;
		}

		// Get line info
		if (deviceLine.find("sd") != std::string::npos && iSize > 0) {
			// GET NAME
			LOG(LogError) << "Getting strings";
			std::string name = deviceLine.substr(iName, iModel);
			std::string model = deviceLine.substr(iModel, iSize);
			std::string size = deviceLine.substr(iSize, iMp);
			std::string mp = deviceLine.substr(iMp);

			LOG(LogError) << "Setting Vectors " << name << " " << model << " " << size << " " << mp;
			devices[totDevices].push_back(name);
			devices[totDevices].push_back(model);
			devices[totDevices].push_back(size);
			if (mp.length() < 1) mp = " ";
			devices[totDevices].push_back(mp);

			

			totDevices++;

		}
	}

	// create componentlistrow vector
	for (int i = 0; i < totDevices; i++) {
		ComponentListRow r;
		r.elements.clear();
		auto dev_name = std::make_shared<TextComponent>(mWindow);
		dev_name->setValue(devices[i][0]);
		r.addElement(dev_name, true);
		auto dev_model = std::make_shared<TextComponent>(mWindow);
		dev_model->setValue(devices[i][1]);
		r.addElement(dev_model, true);
		auto dev_size = std::make_shared<TextComponent>(mWindow);
		dev_size->setValue(devices[i][2]);
		r.addElement(dev_size, true);
		auto dev_mp = std::make_shared<TextComponent>(mWindow);
		dev_mp->setValue(devices[i][3]);
		r.addElement(dev_mp, true);
		mMenu.addRow(r);

	}


}


bool GuiStorageInfo::input(InputConfig* config, Input input)
{
	if(GuiComponent::input(config, input))
		return true;

	if((config->isMappedTo("b", input) || config->isMappedTo("start", input)) && input.value != 0)
	{
		delete this;
		return true;
	}

	if(config->isMappedTo("right", input)){
		//pbar_total->setValue(pval);
		pval++;
		if (pval > 100) pval = 0;
	}

	return false;
}

std::vector<HelpPrompt> GuiStorageInfo::getHelpPrompts()
{
	std::vector<HelpPrompt> prompts;
	prompts.push_back(HelpPrompt("up/down", "choose"));
	prompts.push_back(HelpPrompt("a", "select"));
	prompts.push_back(HelpPrompt("start", "close"));
	return prompts;
}
