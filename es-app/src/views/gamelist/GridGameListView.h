#pragma once

#include "views/gamelist/ISimpleGameListView.h"
#include "components/ImageGridComponent.h"
#include "components/ImageComponent.h"
#include "components/TextComponent.h" 
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
 
	int mImageCacheAmount = 6; 

	ImageGridComponent<FileData*> mGrid;

	TextComponent mTitle; 

	ImageComponent mBackgroundImage;

	FileData* mRootFolder;
 
	//SystemData* mSystem; 
 
	bool mNeedsRefresh = true; 
	bool mReloading = true; 
	int mNextLoad = 0; 
	int mLoadFrame = 0; 
	int mLoadFrameKey = 4; 
};
