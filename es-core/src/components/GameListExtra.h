#pragma once
#ifndef EMULATIONSTATION_ALL_PUBLISHERCOMPONENT_H
#define EMULATIONSTATION_ALL_PUBLISHERCOMPONENT_H

#include "components/RatingComponent.h"
#include "IGameListExtra.h"
#include "TextComponent.h"

template <typename T>
class GameListExtra : public IGameListExtra
{
protected:
	const std::shared_ptr<Font> defaultFont = Font::get(FONT_SIZE_SMALL);

	T mComponent;
	MetaDataDecl mMetadataDecl;
	bool mIsGame;

public:
	GameListExtra(Window *window, const std::string& themingName, const MetaDataDecl& metadataDecl, bool isGame);

	virtual void updateInfo(FileData* file) override;
	virtual void applyTheme(const std::shared_ptr<ThemeData>& theme, const std::string& view) override;
	virtual T* getComponent() override;
};

template <typename T>
GameListExtra<T>::GameListExtra(Window *window, const std::string& themingName, const MetaDataDecl& metadataDecl, bool isGame) :
IGameListExtra(themingName), mComponent(window), mMetadataDecl(metadataDecl), mIsGame(isGame)
{
	//mComponent.setFont(defaultFont);
	mComponent.setDefaultZIndex(40);
}

template <typename T>
void GameListExtra<T>::updateInfo(FileData* file)
{
	if(file != NULL)
	{
		if(file->getType() == GAME || !mIsGame)
		{
			mComponent.setValue(mMetadataDecl.type == MetaDataType::MD_PATH ?
								file->metadata.get(mMetadataDecl.key) :
								file->metadata.get(mMetadataDecl.key));

			//mComponent.setColor(0xFFFFFFFF);

		}
	}

}

template <typename T>
void GameListExtra<T>::applyTheme(const std::shared_ptr<ThemeData>& theme, const std::string& view)
{
	mComponent.applyTheme(theme, view, mThemingName, ThemeFlags::ALL ^ ThemeFlags::TEXT);
}

template <typename T>
T* GameListExtra<T>::getComponent()
{
	return &mComponent;
}

#endif //EMULATIONSTATION_ALL_PUBLISHERCOMPONENT_H
