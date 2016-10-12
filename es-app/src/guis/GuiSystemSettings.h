#pragma once

#include "GuiComponent.h"
#include "components/MenuComponent.h"
#include <functional>

class GuiSystemSettings : public GuiComponent
{
public:
	GuiSystemSettings(Window* window);

	bool input(InputConfig* config, Input input) override;
	void onSizeChanged() override;
	std::vector<HelpPrompt> getHelpPrompts() override;

	std::vector <std::function<void()> > pubFunc;

private:
	void addEntry(const char* name, unsigned int color, bool add_arrow, const std::function<void()>& func);

	MenuComponent mMenu;
	TextComponent mVersion;
};
