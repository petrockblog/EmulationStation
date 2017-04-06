#include "GuiComponent.h"
#include "components/MenuComponent.h"
#include "components/OptionListComponent.h"
#include "components/SwitchComponent.h"
#include "components/SliderComponent.h"
#include "FileSorts.h"

class IGameListView;

class GuiGamelistSettings : public GuiComponent
{
public:
	GuiGamelistSettings(Window* window, SystemData* system);
	virtual ~GuiGamelistSettings();

	//void update(int deltatime) override;
	virtual bool input(InputConfig* config, Input input) override;
	virtual std::vector<HelpPrompt> getHelpPrompts() override;

private:
	MenuComponent mMenu;
	TextComponent mVersion;

	typedef OptionListComponent<std::string> ViewList;
	std::shared_ptr<ViewList> mViewList;

	std::shared_ptr<SwitchComponent> systemEnable_switch;

	std::shared_ptr<SliderComponent> gamegrid_tilesize;
	
	SystemData* mSystem;

	int mSaveLevel = 0;
};
