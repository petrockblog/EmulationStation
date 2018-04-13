#pragma once
#ifndef EMULATIONSTATION_ALL_GAMELISTEXTRARATING_H
#define EMULATIONSTATION_ALL_GAMELISTEXTRARATING_H

#include "GameListExtra.h"

class GameListExtraRating : public GameListExtra<RatingComponent>
{
public:
	GameListExtraRating(Window *window, const std::string& themingName, const MetaDataDecl& metadataDecl, bool isGame);

	void updateInfo(FileData* file) override;
	void applyTheme(const std::shared_ptr<ThemeData>& theme, const std::string& view) override;
	RatingComponent* getComponent() override;
};

GameListExtraRating::GameListExtraRating(Window *window, const std::string& themingName, const MetaDataDecl& metadataDecl, bool isGame) :
		GameListExtra(window, themingName, metadataDecl, isGame)
{
	mComponent.setSize(defaultFont->getHeight() * 5.0f, defaultFont->getHeight());
	mComponent.setDefaultZIndex(40);
}

void GameListExtraRating::updateInfo(FileData* file)
{
	if(file != NULL)
	{
		// Shouldn't be based on isStatistic to know if it's "lastplayed" or "playcount"
		if (mIsGame || !mMetadataDecl.isStatistic)
		{
			mComponent.setValue(mMetadataDecl.type == MetaDataType::MD_PATH ?
								file->metadata.get(mMetadataDecl.displayName) :
								file->metadata.get(mMetadataDecl.displayName));

			mComponent.setColorShift(0xFFFFFFFF);
		}
	}

}

void GameListExtraRating::applyTheme(const std::shared_ptr<ThemeData>& theme, const std::string& view)
{
	mComponent.applyTheme(theme, view, mThemingName, ThemeFlags::ALL ^ ThemeFlags::TEXT);
}

RatingComponent* GameListExtraRating::getComponent()
{
	return &mComponent;
}

#endif //EMULATIONSTATION_ALL_GAMELISTEXTRARATING_H
