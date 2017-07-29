#include "views/gamelist/GridGameListView.h"
#include "ThemeData.h"
#include "Window.h"
#include "Log.h"
#include "views/ViewController.h"
#include "SystemData.h" 


// =========================================================================== 
// Part of this was written by Aloshi but was never finished and added to ES.  
// It would originally load all game art at ES load and could easily eat up 
// All VRAM in smaller systems, such as the raspberry pi. 
// I've added a half-baked way of loading game art into the image grid by doing 
// it only when the user is in that system's game list and it'll do it frame 
// by frame to not eat up CPU cycles for a couple seconds.  This saves VRAM 
// but big game lists can still eat it all up and will result in all kinds 
// of problems.  A solution to fix that would to probably add an option when 
// scrapping to save in smaller resolutions or have this class shrink images 
// as they're being loaded in. 
// ============================================================================ 
 
GridGameListView::GridGameListView(Window* window, FileData* root) : ISimpleGameListView(window, root), 
  mGrid(window, 1), mBackgroundImage(window), mTitle(window)  // mgrid (true) sets to gamegrid 
{ 
	mTitle.setFont(Font::get(FONT_SIZE_MEDIUM));
	mTitle.setPosition(0, mSize.y() * 0.05f);
	mTitle.setColor(0xAAAAAAFF);
	mTitle.setSize(mSize.x(), 0);

	mTitle.setAlignment(ALIGN_CENTER);
	addChild(&mTitle);
	mGrid.setPosition(0, mSize.y() * 0.15f);
	mGrid.setSize(mSize.x(), mSize.y() * 0.8f);
	addChild(&mGrid);

	mRootFolder = root;

	// Load in just the first game to keep ES from crashing if ReloadAll() is called.  
	InitGrid(root->getChildren());
}

void GridGameListView::InitGrid(const std::vector<FileData*>& files) 
{  
  auto it = files.at(0);
  mGrid.add(it->getName(), it->getThumbnailPath(), it);
  mInit = true;
} 

FileData* GridGameListView::getCursor()
{
	return mGrid.getSelected();
}

void GridGameListView::setCursor(FileData* file)
{
	if(!mGrid.setCursor(file) && mGrid.getEntryCount() > 0)
	{
		populateList(file->getParent()->getChildren());
		mGrid.setCursor(file);
		mTitle.setText(file->getName()); 

	}
}

bool GridGameListView::input(InputConfig* config, Input input)
{
	if (config->isMappedTo("left", input) || config->isMappedTo("right", input) || config->isMappedTo("up", input) || config->isMappedTo("down", input))
	{ 
    	// Quick and dirty way of changing header title without doing in update() 
    	mTitle.setText(mGrid.getSelectedName()); 
 
    	// Destroy dpad input so mGrid can use it. 
		return GuiComponent::input(config, input);
	}

	// Lefttop and righttop bumper controls in ISimpleGameListView. 
	return ISimpleGameListView::input(config, input);
}

void GridGameListView::update(int deltatime) { 
	// For Loading in game data as the user clicks on the system.  
	// Loads one game per frame, or if specified to load on frame x. 
	if (mReloading && mLoadFrame >= mLoadFrameKey){
		mLoadFrame = 0;
		if (mNextLoad < mRootFolder->getFilesRecursive(GAME).size()) {
			auto file = mRootFolder->getChildren();
			auto it = file.at(mNextLoad);

			if (mInit == false) // Dirty fix for not loading first game twice
				mGrid.add(it->getName(), it->getThumbnailPath(), it);
			mInit = false;
			mNextLoad++;
		} else
		{
			mReloading = false;
			mNeedsRefresh = false;
			mNextLoad = 0;
		}
	}

	mLoadFrame++;
}

void GridGameListView::populateList(const std::vector<FileData*>& files)
{
	// Sets the bool to load in games in update()
	if (mNeedsRefresh)
	{
	    mReloading = true;

    	// If grid has some games still in it, continue after them
    	if (mGrid.getEntryCount() > 0)
    		mNextLoad = mGrid.getEntryCount();
		mHeaderText.setColor(0xFFFFFFFF);
	}
}

void GridGameListView::launch(FileData* game)
{
	ViewController::get()->launch(game);
}

void GridGameListView::remove(FileData *game, bool deleteFile)
{
	// TODO check not sure about this one
	if (deleteFile)
		boost::filesystem::remove(game->getPath());  // actually delete the file on the filesystem 
	if (getCursor() == game)                     // Select next element in list, or prev if none 
	{ 
		std::vector<FileData*> siblings = game->getParent()->getChildren(); 
		auto gameIter = std::find(siblings.begin(), siblings.end(), game); 
		auto gamePos = std::distance(siblings.begin(), gameIter); 
		if (gameIter != siblings.end()) 
		{ 
			if ((gamePos + 1) < siblings.size()) 
				setCursor(siblings.at(gamePos + 1)); 
			else if ((gamePos - 1) > 0)
				setCursor(siblings.at(gamePos - 1)); 
		}
	}
	delete game;                                 // remove before repopulating (removes from parent) 
	onFileChanged(game, FILE_REMOVED);           // update the view, with game removed 
}

void GridGameListView::onThemeChanged(const std::shared_ptr<ThemeData>& theme)
{
	ISimpleGameListView::onThemeChanged(theme); 
 
	using namespace ThemeFlags;

	// Add custom theme stuff below here.
	mTitle.applyTheme(theme, getName(), "md_title", ALL ^ (POSITION | ThemeFlags::SIZE | TEXT));
	mGrid.applyTheme(theme, getName(), "gamegrid", ALL ^ (POSITION | ThemeFlags::SIZE | ALIGNMENT | POSITION | ORIGIN)); 
} 
 
 
void GridGameListView::onFocusGained()
{
	populateList(mRootFolder->getChildren());
}

void GridGameListView::onFocusLost()
{
	//while (mGrid.getEntryCount() > mImageCacheAmount)
	//	mGrid.remove();
	//mNeedsRefresh = true;
}

std::vector<HelpPrompt> GridGameListView::getHelpPrompts()
{
	std::vector<HelpPrompt> prompts;
	prompts.push_back(HelpPrompt("up/down/left/right", "scroll"));
	prompts.push_back(HelpPrompt("l", "change system"));
	prompts.push_back(HelpPrompt("r", "change system"));
	prompts.push_back(HelpPrompt("a", "launch"));
	prompts.push_back(HelpPrompt("b", "back"));
	prompts.push_back(HelpPrompt("select", "Settings"));

	return prompts;
}
