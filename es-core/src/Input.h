#pragma once
#ifndef ES_CORE_INPUT_H
#define ES_CORE_INPUT_H

#include <string>

struct Input
{
public:

	enum Device
	{
		DEVICE_KEYBOARD = -1,
		DEVICE_CEC      = -2
	};

	enum Type
	{
		TYPE_AXIS,
		TYPE_BUTTON,
		TYPE_HAT,
		TYPE_KEY,
		TYPE_CEC_BUTTON,
		TYPE_COUNT
	};

	Input(const int _device = DEVICE_KEYBOARD, const Type _type = TYPE_COUNT, const int _id = -1, const int _value = -999, const bool _configured = false);

	std::string asString();

	Type type;
	int  device;
	int  id;
	int  value;
	bool configured;

}; // Input

#endif // ES_CORE_INPUT_H
