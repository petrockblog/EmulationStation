#pragma once

#include "GuiComponent.h"
#include "components/MenuComponent.h"
#include "components/ProgressBarComponent.h"
#include <functional>

class GuiStorageInfo : public GuiComponent
{
public:
	GuiStorageInfo(Window* window);

	bool input(InputConfig* config, Input input) override;
	void onSizeChanged() override;
	std::vector<HelpPrompt> getHelpPrompts() override;

	std::vector <std::function<void()> > pubFunc;

private:
	void addEntry(const char* name, unsigned int color, bool add_arrow, const std::function<void()>& func);

	MenuComponent mMenu;
	TextComponent mVersion;

	std::shared_ptr<ProgressBarComponent> pbar_total;

	void getRemovableInfo();

    int pval = 1;
};
