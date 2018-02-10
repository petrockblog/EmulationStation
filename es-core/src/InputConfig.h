#pragma once
#ifndef ES_CORE_INPUT_CONFIG_H
#define ES_CORE_INPUT_CONFIG_H

#include <map>
#include <string>
#include <vector>

namespace pugi { class xml_node; }
struct Input;

class InputConfig
{
public:

	typedef std::vector<std::string> stringVector;

	InputConfig(const int _deviceId, const std::string& _deviceName, const std::string& _deviceGUID);

	void         clear         ();
	void         mapInput      (const std::string& _name, const Input& _input);
	void         unmapInput    (const std::string& _name);
	bool         isMappedTo    (const Input& _compare, const Input& _input);
	bool         isMappedTo    (const std::string& _name, const Input& _input);
	bool         getInputByName(const std::string& _name, Input* _result);
	stringVector getMappedTo   (const Input& _input);
	void         loadFromXML   (pugi::xml_node& _root);
	void         writeToXML    (pugi::xml_node& _parent);

	inline const std::string& getDeviceName      () const { return mDeviceName; }
	inline const std::string& getDeviceGUIDString() const { return mDeviceGUID; }
	inline int                getDeviceId        () const { return mDeviceId; }
	inline bool               isConfigured       () const { return mNameMap.size() > 0; }

private:

	typedef std::map<std::string, Input> inputMap;

	inputMap    mNameMap;
	std::string mDeviceName;
	std::string mDeviceGUID;
	int         mDeviceId;

}; // InputConfig

#endif // ES_CORE_INPUT_CONFIG_H
