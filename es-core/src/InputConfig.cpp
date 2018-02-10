#include "InputConfig.h"

#include "utils/StringUtil.h"
#include "Input.h"
#include "Log.h"
#include <pugixml/src/pugixml.hpp>

static std::string inputTypeToString(const InputType _type)
{
	switch(_type)
	{
		case TYPE_AXIS:       return "axis";
		case TYPE_BUTTON:     return "button";
		case TYPE_HAT:        return "hat";
		case TYPE_KEY:        return "key";
		case TYPE_CEC_BUTTON: return "cec-button";
		default:              return "error";
	}

} // inputTypeToString

static InputType stringToInputType(const std::string& _type)
{
	if(     _type == "axis")       return TYPE_AXIS;
	else if(_type == "button")     return TYPE_BUTTON;
	else if(_type == "hat")        return TYPE_HAT;
	else if(_type == "key")        return TYPE_KEY;
	else if(_type == "cec-button") return TYPE_CEC_BUTTON;
	else                           return TYPE_COUNT;

} // stringToInputType

InputConfig::InputConfig(const int _deviceId, const std::string& _deviceName, const std::string& _deviceGUID)
: mDeviceId(_deviceId)
, mDeviceName(_deviceName)
, mDeviceGUID(_deviceGUID)
{

} // InputConfig

void InputConfig::clear()
{
	mNameMap.clear();

} // clear

void InputConfig::mapInput(const std::string& _name, const Input& _input)
{
	mNameMap[Utils::String::toLower(_name)] = _input;

} // mapInput

void InputConfig::unmapInput(const std::string& _name)
{
	inputMap::const_iterator it = mNameMap.find(Utils::String::toLower(_name));

	if(it != mNameMap.cend())
		mNameMap.erase(it);

} // unmapInput

bool InputConfig::isMappedTo(const Input& _compare, const Input& _input)
{
	if(  _compare.configured               &&
		(_compare.device == _input.device) &&
		(_compare.type   == _input.type)   &&
		(_compare.id     == _input.id))
	{
		if(     _compare.type == TYPE_HAT)  return ((_input.value == 0) || (_input.value &  _compare.value));
		else if(_compare.type == TYPE_AXIS) return ((_input.value == 0) || (_input.value == _compare.value));
		else                                return true;
	}

	return false;

} // isMappedTo

bool InputConfig::isMappedTo(const std::string& _name, const Input& _input)
{
	Input compare;

	if(!getInputByName(_name, &compare))
		return false;
	
	return isMappedTo(compare, _input);

} // isMappedTo

bool InputConfig::getInputByName(const std::string& _name, Input* _result)
{
	inputMap::const_iterator it = mNameMap.find(Utils::String::toLower(_name));

	if(it != mNameMap.cend())
	{
		*_result = it->second;
		return true;
	}

	return false;

} // getInputByName

InputConfig::stringVector InputConfig::getMappedTo(const Input& _input)
{
	stringVector maps;

	for(inputMap::const_iterator it = mNameMap.cbegin(); it != mNameMap.cend(); ++it)
	{
		if(isMappedTo(it->second, _input))
			maps.push_back(it->first);
	}

	return maps;

} // getMappedTo

void InputConfig::loadFromXML(pugi::xml_node& _node)
{
	clear();

	for(pugi::xml_node input = _node.child("input"); input; input = input.next_sibling("input"))
	{
		const std::string name     = input.attribute("name").as_string();
		const std::string type     = input.attribute("type").as_string();
		const int         id       = input.attribute("id").as_int();
		const int         value    = input.attribute("value").as_int();
		const InputType   typeEnum = stringToInputType(type);

		if(typeEnum == TYPE_COUNT)
		{
			LOG(LogError) << "ERROR: InputConfig type \"" << type << "\" is invalid! Skipping input \"" << name << "\".\n";
			continue;
		}

		if(value == 0)
			LOG(LogWarning) << "WARNING: InputConfig value is 0 for " << type << " " << id << "!\n";

		mNameMap[Utils::String::toLower(name)] = Input(mDeviceId, typeEnum, id, value, true);
	}

} // loadFromXML

void InputConfig::writeToXML(pugi::xml_node& _parent)
{
	pugi::xml_node config = _parent.append_child("inputConfig");

	if(mDeviceId == DEVICE_KEYBOARD)
	{
		config.append_attribute("type")       = "keyboard";
		config.append_attribute("deviceName") = "Keyboard";
	}
	else if(mDeviceId == DEVICE_CEC)
	{
		config.append_attribute("type")       = "cec";
		config.append_attribute("deviceName") = "cec";
	}
	else
	{
		config.append_attribute("type")       = "joystick";
		config.append_attribute("deviceName") = mDeviceName.c_str();
	}

//////////////////////////////////////////////////////////////////////////

	config.append_attribute("deviceGUID") = mDeviceGUID.c_str();

	for(inputMap::const_iterator it = mNameMap.cbegin(); it != mNameMap.cend(); ++it)
	{
		if(!it->second.configured)
			continue;

		pugi::xml_node input = config.append_child("input");

		input.append_attribute("name")  = it->first.c_str();
		input.append_attribute("type")  = inputTypeToString(it->second.type).c_str();
		input.append_attribute("id")    = it->second.id;
		input.append_attribute("value") = it->second.value;
	}

} // writeToXML
