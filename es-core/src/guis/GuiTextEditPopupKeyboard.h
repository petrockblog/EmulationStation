#include "GuiComponent.h"

#include "components/NinePatchComponent.h"
#include "components/ButtonComponent.h"
#include "components/ComponentGrid.h"
#include "components/TextEditComponent.h"
#include "components/TextComponent.h"

class GuiTextEditPopupKeyboard : public GuiComponent
{
public:
	GuiTextEditPopupKeyboard(Window* window, const std::string& title, const std::string& initValue,
		const std::function<void(const std::string&)>& okCallback, bool multiLine, const char* acceptBtnText = "OK");

	bool input(InputConfig* config, Input input);
	void update(int deltatime) override;
	void onSizeChanged();
	std::vector<HelpPrompt> getHelpPrompts() override;

private:
	void shiftKeys();

	NinePatchComponent mBackground;
	ComponentGrid mGrid;

	// Vectors for button rows
	std::vector< std::shared_ptr<ButtonComponent> > buttons;
	std::vector< std::shared_ptr<ButtonComponent> > kButtons;
	std::vector< std::shared_ptr<ButtonComponent> > hButtons;
	std::vector< std::shared_ptr<ButtonComponent> > bButtons;
	std::vector< std::shared_ptr<ButtonComponent> > digitButtons;

	std::shared_ptr<TextComponent> mTitle;
	std::shared_ptr<TextEditComponent> mText;
	std::shared_ptr<ComponentGrid> mKeyboardGrid;
	std::shared_ptr<ComponentGrid> mButtonGrid;
	std::shared_ptr<ComponentGrid> mNewGrid;

	// Define keyboard key rows.
	const char* numRow[10] = { "1","2","3","4","5","6","7","8","9","0" };
	const char* numRowUp[10] = { "!", "@", "#", "$", "%", "^", "&", "*", "(", ")" };
	const char* topRow[10] = { "q","w","e","r","t","y","u","i","o","p" };
	const char* topRowUp[10] = { "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P" };		// Just so I don't have to deal with toupper
	const char* homeRow[10] = { "a","s","d","f","g","h","j","k","l",";" };
	const char* homeRowUp[10] = { "A", "S", "D", "F", "G", "H", "J", "K", "L", ":" };
	const char* bottomRow[9] = { "z","x","c","v","b","n","m",",","." };						// Shift is handled in the constructor
	const char* bottomRowUp[9] = { "Z", "X", "C", "V", "B", "N", "M", "<", ">" };

	int mxIndex = 0;		// Stores the X index and makes every grid the same.

	bool mMultiLine;
	bool mShift = false;
	bool mShiftChange = false;
};
