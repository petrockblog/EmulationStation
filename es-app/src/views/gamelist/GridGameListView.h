#pragma once

#include "views/gamelist/ISimpleGameListView.h"
#include "views/gamelist/DetailedGameListView.h"
#include "components/ImageGridComponent.h"
#include "components/ImageComponent.h"
#include "components/TextComponent.h"
#include "SystemData.h"
#include <stack>

class GridGameListView : public ISimpleGameListView
{
public:
	GridGameListView(Window* window, SystemData* system);

	//virtual void onThemeChanged(const std::shared_ptr<ThemeData>& theme) override;

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
	virtual void remove(FileData* game) override;

	void InitGrid(const std::vector<FileData*>& files);

	int mImageCacheAmount = 6;

	ImageGridComponent<FileData*> mGrid;

	TextComponent mTitle;

	ImageComponent mBackgroundImage;

	SystemData* mSystem;

	int mNextLoad = 0;
	int mLoadFrame = 0;
	int mLoadFrameKey = 2;
};
