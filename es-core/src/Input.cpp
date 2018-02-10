#include "Input.h"

#include "CECInput.h"
#include <SDL_joystick.h>
#include <SDL_keyboard.h>
#include <sstream>

static std::string getHatDir(int val)
{
	if(     val & SDL_HAT_UP)    return "up";
	else if(val & SDL_HAT_DOWN)  return "down";
	else if(val & SDL_HAT_LEFT)  return "left";
	else if(val & SDL_HAT_RIGHT) return "right";
	else                         return "neutral?";

} // getHatDir

Input::Input(const int _device, const Type _type, const int _id, const int _value, const bool _configured)
: mType      (_type)
, mDevice    (_device)
, mId        (_id)
, mValue     (_value)
, mConfigured(_configured)
{

} // Input

std::string Input::asString()
{
	std::stringstream stream;

	switch(mType)
	{
		case TYPE_BUTTON:     { stream << "Button "     << mId;                              } break;
		case TYPE_AXIS:       { stream << "Axis "       << mId << (mValue > 0 ? "+" : "-");  } break;
		case TYPE_HAT:        { stream << "Hat "        << mId << " " << getHatDir(mValue);  } break;
		case TYPE_KEY:        { stream << "Key "        << SDL_GetKeyName((SDL_Keycode)mId); } break;
		case TYPE_CEC_BUTTON: { stream << "CEC-Button " << CECInput::getKeyCodeString(mId);  } break;
		default:              { stream << "Input to string error";                           }
	}

	return stream.str();

} // asString
