#pragma once

#include "views/gamelist/ISimpleGameListView.h"
#include "components/ImageGridComponent.h"
#include "components/ImageComponent.h"
#include "components/TextComponent.h"
#include "components/ScrollableContainer.h"
#include "components/RatingComponent.h"
#include "components/DateTimeComponent.h"
#include "SystemData.h"
#include <stack>

class GridGameListView : public ISimpleGameListView
{
public:
	GridGameListView(Window* window, FileData* root);

	virtual FileData* getCursor() override;
	virtual void setCursor(FileData*) override;

	virtual bool input(InputConfig* config, Input input) override;
	virtual void update(int deltatime) override;

	virtual const char* getName() const override { return "grid"; }

	virtual void onThemeChanged(const std::shared_ptr<ThemeData>& theme);

	virtual std::vector<HelpPrompt> getHelpPrompts() override;

	virtual void onFocusGained() override;
	virtual void onFocusLost() override;

protected:
	virtual void populateList(const std::vector<FileData*>& files) override;
	virtual void launch(FileData* game) override;
	virtual void remove(FileData* game, bool deleteFile) override;

	ImageGridComponent<FileData*> mGrid;

	TextComponent mGameTitle;

	ImageComponent mBackgroundImage;

	FileData* mRoot;

	int mLoadFrame = 0;
	int mLoadFrameKey = 1;

private:
	void getDefaultElements();
	void updateInfoPanel();
	void initMDLabels();
	void initMDValues();

	bool bFocused = false;

	ScrollableContainer mDescContainer;
	TextComponent mDescription;

	ImageComponent mImage;

	TextComponent mLblRating, mLblReleaseDate, mLblDeveloper, mLblPublisher, mLblGenre, mLblPlayers, mLblLastPlayed, mLblPlayCount;

	RatingComponent mRating;
	DateTimeComponent mReleaseDate;
	TextComponent mDeveloper;
	TextComponent mPublisher;
	TextComponent mGenre;
	TextComponent mPlayers;
	DateTimeComponent mLastPlayed;
	TextComponent mPlayCount;

	std::vector<TextComponent*> getMDLabels();
	std::vector<GuiComponent*> getMDValues();
};
