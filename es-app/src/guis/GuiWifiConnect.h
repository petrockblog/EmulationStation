#pragma once

#include "GuiComponent.h"
#include "components/MenuComponent.h"
#include <functional>

class GuiWifiConnect : public GuiComponent
{
public:
	GuiWifiConnect(Window* window, std::string wifiName, bool encrypted);

	bool input(InputConfig* config, Input input) override;
	void onSizeChanged() override;
	std::vector<HelpPrompt> getHelpPrompts() override;

private:
	void addEntry(const char* name, unsigned int color, bool add_arrow, const std::function<void()>& func);

	MenuComponent mMenu;
	TextComponent mVersion;

	bool mConnected = false;

	const char* mPassword;
};
