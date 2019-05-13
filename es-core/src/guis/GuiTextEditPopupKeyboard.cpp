#include "guis/GuiTextEditPopupKeyboard.h"
#include "components/MenuComponent.h"
#include "Log.h"
#include <locale>

GuiTextEditPopupKeyboard::GuiTextEditPopupKeyboard(Window* window, const std::string& title, const std::string& initValue,
	const std::function<void(const std::string&)>& okCallback, bool multiLine, const char* acceptBtnText)
	: GuiComponent(window), mBackground(window, ":/frame.png"), mGrid(window, Vector2i(1, 7)), mMultiLine(multiLine)
{
	addChild(&mBackground);
	addChild(&mGrid);

	mTitle = std::make_shared<TextComponent>(mWindow, title, Font::get(FONT_SIZE_LARGE), 0x555555FF, ALIGN_CENTER);
	mKeyboardGrid = std::make_shared<ComponentGrid>(mWindow, Vector2i(10, 5));

	mText = std::make_shared<TextEditComponent>(mWindow);
	mText->setValue(initValue);

	if (!multiLine)
		mText->setCursor(initValue.size());

	// Header
	mGrid.setEntry(mTitle, Vector2i(0, 0), false, true);

	// Text edit add
	mGrid.setEntry(mText, Vector2i(0, 1), true, false, Vector2i(1, 1), GridFlags::BORDER_TOP | GridFlags::BORDER_BOTTOM);


	// Keyboard
	// Case for if multiline is enabled, then don't create the keyboard.
	if (!mMultiLine) {

		// Locale for shifting upper/lower case
		std::locale loc;

		// Digit Row & Special Chara.
		for (int k = 0; k < 10; k++) {
			// Create string for button display name.
			std::string strName = "";
			strName += numRow[k];
			strName += " ";
			strName += numRowUp[k];

			// Init button and store in Vector
			digitButtons.push_back(std::make_shared<ButtonComponent>
				(mWindow, strName, numRow[k], [this, k, loc] {
				mText->startEditing();
				if (mShift) mText->textInput(numRowUp[k]);
				else mText->textInput(numRow[k]);
				mText->stopEditing();
			}));

			// Send just created button into mGrid
			mKeyboardGrid->setEntry(digitButtons[k], Vector2i(k, 0), true, false);
		}

		// Top row
		for (int k = 0; k < 10; k++) {
			kButtons.push_back(std::make_shared<ButtonComponent>
				(mWindow, topRowUp[k], topRowUp[k], [this, k, loc] {
				mText->startEditing();
				if (mShift) mText->textInput(topRowUp[k]);
				else mText->textInput(topRow[k]);
				mText->stopEditing();
			}));

			// Send just created button into mGrid
			mKeyboardGrid->setEntry(kButtons[k], Vector2i(k, 1), true, false);
		}

		// Home Row
		for (int k = 0; k < 10; k++) {
			auto key = homeRowUp[k];
			if (k == 9) key = homeRow[k];
			hButtons.push_back(std::make_shared<ButtonComponent>
				(mWindow, key, key, [this, k] {
				mText->startEditing();
				if (mShift) mText->textInput(homeRowUp[k]);
				else mText->textInput(homeRow[k]);
				mText->stopEditing();
			}));

			// Send just created button into mGrid
			mKeyboardGrid->setEntry(hButtons[k], Vector2i(k, 2), true, false);
		}

		// Special case for shift key
		bButtons.push_back(std::make_shared<ButtonComponent>(mWindow, "SHIFT", "SHIFTS FOR UPPER,LOWER, AND SPECIAL", [this, okCallback] {
			okCallback(mText->getValue());
			if (mShift) mShift = false;
			else mShift = true;
			shiftKeys();
		}));

		// Bottom row [Z - M]
		for (int k = 0; k < 7; k++) {
			bButtons.push_back(std::make_shared<ButtonComponent>
				(mWindow, bottomRowUp[k], bottomRowUp[k], [this, k, loc] {
				mText->startEditing();
				if (mShift) mText->textInput(bottomRowUp[k]);
				else mText->textInput(bottomRow[k]);
				mText->stopEditing();
			}));
		}

		// Add in the last two manualy because they're special chara [,< and .>]
		for (int k = 7; k < 9; k++) {
			bButtons.push_back(std::make_shared<ButtonComponent>(mWindow, bottomRow[7], bottomRow[7], [this, k] {
				mText->startEditing();
				if (mShift) mText->textInput(bottomRowUp[k]);
				else mText->textInput(bottomRow[k]);
				mText->stopEditing();
			}));
		}

		// Do a sererate for loop because shift key makes it weird
		for (int k = 0; k < 10; k++) {
			mKeyboardGrid->setEntry(bButtons[k], Vector2i(k, 3), true, false);
		}

		// END KEYBOARD IF
	}


	// Accept/Cancel buttons
	buttons.push_back(std::make_shared<ButtonComponent>(mWindow, acceptBtnText, acceptBtnText, [this, okCallback] { okCallback(mText->getValue()); delete this; }));
	buttons.push_back(std::make_shared<ButtonComponent>(mWindow, "CANCEL", "discard changes", [this] { delete this; }));
	buttons.push_back(std::make_shared<ButtonComponent>(mWindow, "SPACE", "SPACE", [this] {
		mText->startEditing();
		mText->textInput(" ");
		mText->stopEditing();
	}));
	buttons.push_back(std::make_shared<ButtonComponent>(mWindow, "DEL", "DELETE A CHAR", [this] {
		mText->startEditing();
		mText->textInput("\b");
		mText->stopEditing();
	}));

	// Add a/c buttons
	mKeyboardGrid->setEntry(buttons[0], Vector2i(3, 4), true, false, Vector2i(1, 1));
	mKeyboardGrid->setEntry(buttons[2], Vector2i(4, 4), true, false, Vector2i(1, 1));
	mKeyboardGrid->setEntry(buttons[3], Vector2i(5, 4), true, false, Vector2i(1, 1));
	mKeyboardGrid->setEntry(buttons[1], Vector2i(6, 4), true, false, Vector2i(1, 1));

	//buttons[1]->setSize(buttons[1]->getSize().x(), buttons[0]->getSize().y());

	mGrid.setEntry(mKeyboardGrid, Vector2i(0, 2), true, true, Vector2i(2, 5));

	// Determine size from text size
	float textHeight = mText->getFont()->getHeight();
	if (multiLine)
		textHeight *= 6;
	mText->setSize(0, textHeight);

	// If multiline, set all diminsions back to default, else draw size for keyboard.
	if (mMultiLine) {
		setSize(Renderer::getScreenWidth() * 0.5f, mTitle->getFont()->getHeight() + textHeight + mKeyboardGrid->getSize().y() + 40);
		setPosition((Renderer::getScreenWidth() - mSize.x()) / 2, (Renderer::getScreenHeight() - mSize.y()) / 2);
	}
	else {
		// Set size based on ScreenHieght * .08f by the amount of keyboard rows there are.
		setSize(Renderer::getScreenWidth() * 0.75f, mTitle->getFont()->getHeight() + textHeight + 60 + (Renderer::getScreenHeight() * 0.085f) * 5);
		setPosition((Renderer::getScreenWidth() - mSize.x()) / 2, (Renderer::getScreenHeight() - mSize.y()) / 2);
	}
}


void GuiTextEditPopupKeyboard::onSizeChanged()
{
	mBackground.fitTo(mSize, Vector3f::Zero(), Vector2f(-32, -32));

	mText->setSize(mSize.x() - 40, mText->getSize().y());

	// update grid
	mGrid.setRowHeightPerc(0, mTitle->getFont()->getHeight() / mSize.y());
	mGrid.setRowHeightPerc(2, mKeyboardGrid->getSize().y() / mSize.y());

	mGrid.setSize(mSize);
}

bool GuiTextEditPopupKeyboard::input(InputConfig* config, Input input)
{
	if (GuiComponent::input(config, input))
		return true;

	// pressing back when not text editing closes us
	if (config->isMappedTo("b", input) && input.value)
	{
		delete this;
		return true;
	}

	// For deleting a chara (Left Top Button)
	if (config->isMappedTo("leftshoulder", input) && input.value) {
		mText->startEditing();
		mText->textInput("\b");
		mText->stopEditing();
	}

	// For Adding a space (Right Top Button)
	if (config->isMappedTo("rightshoulder", input) && input.value) {
		mText->startEditing();
		mText->textInput(" ");
	}

	// For Shifting (X)
	if (config->isMappedTo("x", input) && input.value) {
		if (mShift) mShift = false;
		else mShift = true;
		shiftKeys();
	}

	

	return false;
}

void GuiTextEditPopupKeyboard::update(int deltatime) {

}

// Shifts the keys when user hits the shift button.
void GuiTextEditPopupKeyboard::shiftKeys() {
	if (mShift) {
		// FOR SHIFTING UP
		// Change Shift button color
		//bButtons[0]->setColorShift(0xEBFD00AA);		// <- Would rather use this but buttoncomponent doesn't support in this branch yet.
		bButtons[0]->setText("[SHIFT]", "SHIFTS FOR UPPER,LOWER, AND SPECIAL");
		// Change Special chara
		hButtons[9]->setText(":", ":");
		bButtons[8]->setText("<", "<");
		bButtons[9]->setText(">", ">");
	} else {
		// UNSHIFTING
		// Remove button color
		//bButtons[0]->removeColorShift();
		// Change Special chara
		bButtons[0]->setText("SHIFT", "SHIFTS FOR UPPER,LOWER, AND SPECIAL");
		hButtons[9]->setText(";", ";");
		bButtons[8]->setText(",", ",");
		bButtons[9]->setText(".", ".");
	}

}

std::vector<HelpPrompt> GuiTextEditPopupKeyboard::getHelpPrompts()
{
	std::vector<HelpPrompt> prompts = mGrid.getHelpPrompts();
	prompts.push_back(HelpPrompt("x", "SHIFT"));
	prompts.push_back(HelpPrompt("b", "back"));
	prompts.push_back(HelpPrompt("r", "SPACE"));
	prompts.push_back(HelpPrompt("l", "DELETE"));
	return prompts;
}

