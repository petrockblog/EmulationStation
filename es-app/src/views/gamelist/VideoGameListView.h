#pragma once

#include "views/gamelist/BasicGameListView.h"
#include "components/ScrollableContainer.h"
#include "components/RatingComponent.h"
#include "components/DateTimeComponent.h"
#include "components/VideoComponent.h"
#include "SystemData.h"

class VideoGameListView : public BasicGameListView
{
public:
	VideoGameListView(Window* window, FileData* root, SystemData* system);
	virtual ~VideoGameListView();

	virtual void onShow() override;

	virtual void onThemeChanged(const std::shared_ptr<ThemeData>& theme) override;

	virtual const char* getName() const override { return "video"; }
	virtual void launch(FileData* game) override;

protected:
	virtual void update(int deltaTime) override;
	
	virtual std::vector<HelpPrompt> getHelpPrompts() override;

private:
	void updateInfoPanel();

	void initMDLabels();
	void initMDValues();

	ImageComponent mMarquee;
	VideoComponent* mVideo;
	ImageComponent mImage;

	TextComponent mLblRating, mLblReleaseDate, mLblDeveloper, mLblPublisher, mLblGenre, mLblPlayers, mLblLastPlayed, mLblPlayCount, mLblFavourite;

	RatingComponent mRating;
	DateTimeComponent mReleaseDate;
	TextComponent mDeveloper;
	TextComponent mPublisher;
	TextComponent mGenre;
	TextComponent mPlayers;
	DateTimeComponent mLastPlayed;
	TextComponent mPlayCount;
	TextComponent mFavourite;

	std::vector<TextComponent*> getMDLabels();
	std::vector<GuiComponent*> getMDValues();

	ScrollableContainer mDescContainer;
	TextComponent mDescription;
	
	SystemData* mSystem;

	bool		mVideoPlaying;

};
