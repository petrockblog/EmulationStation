#pragma once
#ifndef ES_APP_VIEWS_GAME_LIST_GRID_GAME_LIST_VIEW_H
#define ES_APP_VIEWS_GAME_LIST_GRID_GAME_LIST_VIEW_H

#include "components/DateTimeComponent.h"
#include "components/RatingComponent.h"
#include "components/ScrollableContainer.h"
#include "components/ImageGridComponent.h"
#include "views/gamelist/ISimpleGameListView.h"
#include "components/GameListExtra.h"

class GridGameListView : public ISimpleGameListView
{
public:
	GridGameListView(Window* window, FileData* root);

	virtual void onThemeChanged(const std::shared_ptr<ThemeData>& theme) override;

	virtual FileData* getCursor() override;
	virtual void setCursor(FileData*) override;

	virtual bool input(InputConfig* config, Input input) override;

	virtual const char* getName() const override { return "grid"; }

	virtual std::vector<HelpPrompt> getHelpPrompts() override;
	virtual void launch(FileData* game) override;

protected:
	virtual std::string getQuickSystemSelectRightButton() override;
	virtual std::string getQuickSystemSelectLeftButton() override;
	virtual void populateList(const std::vector<FileData*>& files) override;
	virtual void remove(FileData* game, bool deleteFile) override;
	virtual void addPlaceholder();

	ImageGridComponent<FileData*> mGrid;

private:
	void updateInfoPanel();

	void initMDLabels();
	void initMDValues();

	bool addMetadataElement(const std::string& name);

	TextComponent mLblRating, mLblReleaseDate, mLblDeveloper, mLblPublisher, mLblGenre, mLblPlayers, mLblLastPlayed, mLblPlayCount;

	/*
	RatingComponent mRating;
	DateTimeComponent mReleaseDate;
	TextComponent mDeveloper;
	TextComponent mPublisher;
	TextComponent mGenre;
	TextComponent mPlayers;
	DateTimeComponent mLastPlayed;
	TextComponent mPlayCount;
	*/

	std::vector<TextComponent*> getMDLabels();

	ScrollableContainer mDescContainer;
	TextComponent mDescription;

	std::vector<IGameListExtra*> mExtras;
	Window* mWindow;
};

#endif // ES_APP_VIEWS_GAME_LIST_GRID_GAME_LIST_VIEW_H
