#pragma once
#ifndef ES_CORE_COMPONENTS_METADATA_COMPONENT_H
#define ES_CORE_COMPONENTS_METADATA_COMPONENT_H

#include "GuiComponent.h"
#include "ThemeData.h"
#include "FileData.h"

class IGameListExtra
{
protected:
	std::string mThemingName;

public:
	IGameListExtra(const std::string& name) : mThemingName(name) {};

	//virtual void update();
	virtual void updateInfo(FileData* file) = 0;
	virtual void applyTheme(const std::shared_ptr<ThemeData>& theme, const std::string& view) = 0;
	virtual GuiComponent* getComponent() = 0;
};

#endif // ES_CORE_COMPONENTS_METADATA_COMPONENT_H
