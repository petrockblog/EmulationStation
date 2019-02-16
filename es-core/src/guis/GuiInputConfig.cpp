#include "guis/GuiInputConfig.h"

#include "components/ButtonComponent.h"
#include "components/MenuComponent.h"
#include "guis/GuiMsgBox.h"
#include "InputManager.h"
#include "Log.h"
#include "Window.h"
#include "Locale.h"

struct InputConfigStructure
{
	const char* name;
	const bool  skippable;
	const char* dispName;
	const char* icon;
};

#define pgettext_noop(context, id) id

static const int inputCount = 25;
static const InputConfigStructure GUI_INPUT_CONFIG_LIST[inputCount] =
{
	{ "Up",               false, pgettext_noop("button", "D-PAD UP"),           ":/help/dpad_up.svg" },
	{ "Down",             false, pgettext_noop("button", "D-PAD DOWN"),         ":/help/dpad_down.svg" },
	{ "Left",             false, pgettext_noop("button", "D-PAD LEFT"),         ":/help/dpad_left.svg" },
	{ "Right",            false, pgettext_noop("button", "D-PAD RIGHT"),        ":/help/dpad_right.svg" },
	{ "Start",            true,  pgettext_noop("button", "START"),              ":/help/button_start.svg" },
	{ "Select",           true,  pgettext_noop("button", "SELECT"),             ":/help/button_select.svg" },
	{ "A",                false, pgettext_noop("button", "BUTTON A / EAST"),    ":/help/buttons_east.svg" },		// FIXME symbol changed
	{ "B",                true,  pgettext_noop("button", "BUTTON B / SOUTH"),   ":/help/buttons_south.svg" },		// FIXME
	{ "X",                true,  pgettext_noop("button", "BUTTON X / NORTH"),   ":/help/buttons_north.svg" },		// FIXME
	{ "Y",                true,  pgettext_noop("button", "BUTTON Y / WEST"),    ":/help/buttons_west.svg" },		// FIXME
	{ "LeftShoulder",     true,  pgettext_noop("button", "LEFT SHOULDER"),      ":/help/button_l.svg" },
	{ "RightShoulder",    true,  pgettext_noop("button", "RIGHT SHOULDER"),     ":/help/button_r.svg" },
	{ "LeftTrigger",      true,  pgettext_noop("button", "LEFT TRIGGER"),       ":/help/button_lt.svg" },
	{ "RightTrigger",     true,  pgettext_noop("button", "RIGHT TRIGGER"),      ":/help/button_rt.svg" },
	{ "LeftThumb",        true,  pgettext_noop("button", "LEFT THUMB"),         ":/help/analog_thumb.svg" },
	{ "RightThumb",       true,  pgettext_noop("button", "RIGHT THUMB"),        ":/help/analog_thumb.svg" },
	{ "LeftAnalogUp",     true,  pgettext_noop("button", "LEFT ANALOG UP"),     ":/help/analog_up.svg" },
	{ "LeftAnalogDown",   true,  pgettext_noop("button", "LEFT ANALOG DOWN"),   ":/help/analog_down.svg" },
	{ "LeftAnalogLeft",   true,  pgettext_noop("button", "LEFT ANALOG LEFT"),   ":/help/analog_left.svg" },
	{ "LeftAnalogRight",  true,  pgettext_noop("button", "LEFT ANALOG RIGHT"),  ":/help/analog_right.svg" },
	{ "RightAnalogUp",    true,  pgettext_noop("button", "RIGHT ANALOG UP"),    ":/help/analog_up.svg" },
	{ "RightAnalogDown",  true,  pgettext_noop("button", "RIGHT ANALOG DOWN"),  ":/help/analog_down.svg" },
	{ "RightAnalogLeft",  true,  pgettext_noop("button", "RIGHT ANALOG LEFT"),  ":/help/analog_left.svg" },
	{ "RightAnalogRight", true,  pgettext_noop("button", "RIGHT ANALOG RIGHT"), ":/help/analog_right.svg" },
	{ "HotKeyEnable",     true,  pgettext_noop("button", "HOTKEY ENABLE"),      ":/help/button_hotkey.svg" }
};

//MasterVolUp and MasterVolDown are also hooked up, but do not appear on this screen.
//If you want, you can manually add them to es_input.cfg.

#define HOLD_TO_SKIP_MS 1000

GuiInputConfig::GuiInputConfig(Window* window, InputConfig* target, bool reconfigureAll, const std::function<void()>& okCallback) : GuiComponent(window), 
	mBackground(window, ":/frame.png"), mGrid(window, Vector2i(1, 7)), 
	mTargetConfig(target), mHoldingInput(false), mBusyAnim(window)
{
	LOG(LogInfo) << "Configuring device " << target->getDeviceId() << " (" << target->getDeviceName() << ").";

	if(reconfigureAll)
		target->clear();

	mConfiguringAll = reconfigureAll;
	mConfiguringRow = mConfiguringAll;

	addChild(&mBackground);
	addChild(&mGrid);

	// 0 is a spacer row
	mGrid.setEntry(std::make_shared<GuiComponent>(mWindow), Vector2i(0, 0), false);

	mTitle = std::make_shared<TextComponent>(mWindow, _("CONFIGURING"), Font::get(FONT_SIZE_LARGE), 0x555555FF, ALIGN_CENTER);
	mGrid.setEntry(mTitle, Vector2i(0, 1), false, true);
	
	std::string deviceName;
	if(target->getDeviceId() == DEVICE_KEYBOARD)
		deviceName = _("KEYBOARD");
	else if(target->getDeviceId() == DEVICE_CEC)
		deviceName = _("CEC");
	else
		deviceName = (boost::locale::format(_("GAMEPAD {1}")) % (target->getDeviceId() + 1)).str();
	mSubtitle1 = std::make_shared<TextComponent>(mWindow, Utils::String::toUpper(deviceName), Font::get(FONT_SIZE_MEDIUM), 0x555555FF, ALIGN_CENTER);
	mGrid.setEntry(mSubtitle1, Vector2i(0, 2), false, true);

	mSubtitle2 = std::make_shared<TextComponent>(mWindow, _("HOLD ANY BUTTON TO SKIP"), Font::get(FONT_SIZE_SMALL), 0x999999FF, ALIGN_CENTER);
	mGrid.setEntry(mSubtitle2, Vector2i(0, 3), false, true);

	// 4 is a spacer row

	mList = std::make_shared<ComponentList>(mWindow);
	mGrid.setEntry(mList, Vector2i(0, 5), true, true);
	for(int i = 0; i < inputCount; i++)
	{
		ComponentListRow row;
		
		// icon
		auto icon = std::make_shared<ImageComponent>(mWindow);
		icon->setImage(GUI_INPUT_CONFIG_LIST[i].icon);
		icon->setColorShift(0x777777FF);
		icon->setResize(0, Font::get(FONT_SIZE_MEDIUM)->getLetterHeight() * 1.25f);
		row.addElement(icon, false);

		// spacer between icon and text
		auto spacer = std::make_shared<GuiComponent>(mWindow);
		spacer->setSize(16, 0);
		row.addElement(spacer, false);

		auto text = std::make_shared<TextComponent>(mWindow, boost::locale::pgettext("button", GUI_INPUT_CONFIG_LIST[i].dispName), Font::get(FONT_SIZE_MEDIUM), 0x777777FF);
		row.addElement(text, true);

		auto mapping = std::make_shared<TextComponent>(mWindow, _("-NOT DEFINED-"), Font::get(FONT_SIZE_MEDIUM, FONT_PATH_LIGHT), 0x999999FF, ALIGN_RIGHT);
		setNotDefined(mapping); // overrides text and color set above
		row.addElement(mapping, true);
		mMappings.push_back(mapping);

		row.input_handler = [this, i, mapping](InputConfig* config, Input input) -> bool
		{
			// ignore input not from our target device
			if(config != mTargetConfig)
				return false;

			// if we're not configuring, start configuring when A is pressed
			if(!mConfiguringRow)
			{
				if(config->isMappedTo("a", input) && input.value)
				{
					mList->stopScrolling();
					mConfiguringRow = true;
					setPress(mapping);
					return true;
				}
				
				// we're not configuring and they didn't press A to start, so ignore this
				return false;
			}

			// we are configuring
			if(input.value != 0)
			{
				// input down
				// if we're already holding something, ignore this, otherwise plan to map this input
				if(mHoldingInput)
					return true;

				mHoldingInput = true;
				mHeldInput = input;
				mHeldTime = 0;
				mHeldInputId = i;

				return true;
			}else{
				// input up
				// make sure we were holding something and we let go of what we were previously holding
				if(!mHoldingInput || mHeldInput.device != input.device || mHeldInput.id != input.id || mHeldInput.type != input.type)
					return true;

				mHoldingInput = false;

				if(assign(mHeldInput, i))
					rowDone(); // if successful, move cursor/stop configuring - if not, we'll just try again

				return true;
			}
		};

		mList->addRow(row);
	}

	// only show "HOLD TO SKIP" if this input is skippable
	mList->setCursorChangedCallback([this](CursorState /*state*/) {
		bool skippable = GUI_INPUT_CONFIG_LIST[mList->getCursorId()].skippable;
		mSubtitle2->setOpacity(skippable * 255);
	});

	// make the first one say "PRESS ANYTHING" if we're re-configuring everything
	if(mConfiguringAll)
		setPress(mMappings.front());

	// buttons
	std::vector< std::shared_ptr<ButtonComponent> > buttons;
	std::function<void()> okFunction = [this, okCallback] {
		InputManager::getInstance()->writeDeviceConfig(mTargetConfig); // save
		if(okCallback)
			okCallback();
		delete this; 
	};
	buttons.push_back(std::make_shared<ButtonComponent>(mWindow, _("OK"), _("OK"), [this, okFunction] {
		// check if the hotkey enable button is set. if not prompt the user to use select or nothing.
		Input input;
		if (!mTargetConfig->getInputByName("HotKeyEnable", &input)) {
			mWindow->pushGui(new GuiMsgBox(mWindow,
				_("YOU DIDN'T CHOOSE A HOTKEY ENABLE BUTTON. THIS IS REQUIRED FOR EXITING GAMES WITH A CONTROLLER. DO YOU WANT TO USE THE SELECT BUTTON DEFAULT ? PLEASE ANSWER YES TO USE SELECT OR NO TO NOT SET A HOTKEY ENABLE BUTTON."),
				_("YES"), [this, okFunction] {
					Input input;
					mTargetConfig->getInputByName("Select", &input);
					mTargetConfig->mapInput("HotKeyEnable", input);
					okFunction();
					},
				_("NO"), [this, okFunction] {
					// for a disabled hotkey enable button, set to a key with id 0,
					// so the input configuration script can be backwards compatible.
					mTargetConfig->mapInput("HotKeyEnable", Input(DEVICE_KEYBOARD, TYPE_KEY, 0, 1, true));
					okFunction();
				}
			));
		} else {
			okFunction();
		}
	}));
	mButtonGrid = makeButtonGrid(mWindow, buttons);
	mGrid.setEntry(mButtonGrid, Vector2i(0, 6), true, false);

	setSize(Renderer::getScreenWidth() * 0.6f, Renderer::getScreenHeight() * 0.75f);
	setPosition((Renderer::getScreenWidth() - mSize.x()) / 2, (Renderer::getScreenHeight() - mSize.y()) / 2);
}

void GuiInputConfig::onSizeChanged()
{
	mBackground.fitTo(mSize, Vector3f::Zero(), Vector2f(-32, -32));

	// update grid
	mGrid.setSize(mSize);

	//mGrid.setRowHeightPerc(0, 0.025f);
	mGrid.setRowHeightPerc(1, mTitle->getFont()->getHeight()*0.75f / mSize.y());
	mGrid.setRowHeightPerc(2, mSubtitle1->getFont()->getHeight() / mSize.y());
	mGrid.setRowHeightPerc(3, mSubtitle2->getFont()->getHeight() / mSize.y());
	//mGrid.setRowHeightPerc(4, 0.03f);
	mGrid.setRowHeightPerc(5, (mList->getRowHeight(0) * 5 + 2) / mSize.y());
	mGrid.setRowHeightPerc(6, mButtonGrid->getSize().y() / mSize.y());

	mBusyAnim.setSize(mSize);
}

void GuiInputConfig::update(int deltaTime)
{
	if(mConfiguringRow && mHoldingInput && GUI_INPUT_CONFIG_LIST[mHeldInputId].skippable)
	{
		int prevSec = mHeldTime / 1000;
		mHeldTime += deltaTime;
		int curSec = mHeldTime / 1000;

		if(mHeldTime >= HOLD_TO_SKIP_MS)
		{
			setNotDefined(mMappings.at(mHeldInputId));
			clearAssignment(mHeldInputId);
			mHoldingInput = false;
			rowDone();
		}else{
			if(prevSec != curSec)
			{
				// crossed the second boundary, update text
				const auto& text = mMappings.at(mHeldInputId);
				const int delay =  HOLD_TO_SKIP_MS/1000 - curSec;
				std::string message = (boost::locale::format(ngettext("HOLD FOR {1}S TO SKIP", "HOLD FOR {1}S TO SKIP", delay)) % delay).str();
				text->setText(message);
				text->setColor(0x777777FF);
			}
		}
	}
}

// move cursor to the next thing if we're configuring all, 
// or come out of "configure mode" if we were only configuring one row
void GuiInputConfig::rowDone()
{
	if(mConfiguringAll)
	{
		if(!mList->moveCursor(1)) // try to move to the next one
		{
			// at bottom of list, done
			mConfiguringAll = false;
			mConfiguringRow = false;
			mGrid.moveCursor(Vector2i(0, 1));
		}else{
			// on another one
			setPress(mMappings.at(mList->getCursorId()));
		}
	}else{
		// only configuring one row, so stop
		mConfiguringRow = false;
	}
}

void GuiInputConfig::setPress(const std::shared_ptr<TextComponent>& text)
{
	text->setText(_("PRESS ANYTHING"));
	text->setColor(0x656565FF);
}

void GuiInputConfig::setNotDefined(const std::shared_ptr<TextComponent>& text)
{
	text->setText(_("-NOT DEFINED-"));
	text->setColor(0x999999FF);
}

void GuiInputConfig::setAssignedTo(const std::shared_ptr<TextComponent>& text, Input input)
{
	text->setText(Utils::String::toUpper(input.string()));
	text->setColor(0x777777FF);
}

void GuiInputConfig::error(const std::shared_ptr<TextComponent>& text, const std::string& /*msg*/)
{
	text->setText(_("ALREADY TAKEN"));
	text->setColor(0x656565FF);
}

bool GuiInputConfig::assign(Input input, int inputId)
{
	// input is from InputConfig* mTargetConfig

	// if this input is mapped to something other than "nothing" or the current row, error
	// (if it's the same as what it was before, allow it)
	if(mTargetConfig->getMappedTo(input).size() > 0 && !mTargetConfig->isMappedTo(GUI_INPUT_CONFIG_LIST[inputId].name, input) && strcmp(GUI_INPUT_CONFIG_LIST[inputId].name, "HotKeyEnable") != 0)
	{
		error(mMappings.at(inputId), "Already mapped!");
		return false;
	}

	setAssignedTo(mMappings.at(inputId), input);
	
	input.configured = true;
	mTargetConfig->mapInput(GUI_INPUT_CONFIG_LIST[inputId].name, input);

	LOG(LogInfo) << "  Mapping [" << input.string() << "] -> " << GUI_INPUT_CONFIG_LIST[inputId].name;

	return true;
}

void GuiInputConfig::clearAssignment(int inputId)
{
	mTargetConfig->unmapInput(GUI_INPUT_CONFIG_LIST[inputId].name);
}
