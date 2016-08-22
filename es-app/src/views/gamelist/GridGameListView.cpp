#include "views/gamelist/GridGameListView.h"
#include "ThemeData.h"
#include "Window.h"
#include "Log.h"
#include "views/ViewController.h"
#include "SystemData.h"
#include "Settings.h"

// ===========================================================================
// Part of this was written by Aloshi but was never finished and added to ES. 
// It would originally load all game art at ES load and could easily eat up
// All VRAM in smaller systems, such as the raspberry pi.
// I've changed how ImageGridComponent stores the game's art texture so
// it will only load the images for tiles in a range of the cursor and
// unload textures outside of its range depending on which direction the
// user is moving.
// ============================================================================

GridGameListView::GridGameListView(Window* window, SystemData* system) : ISimpleGameListView(window, system->getRootFolder()),
	mGrid(window, system->getGridModSize()), mBackgroundImage(window), mTitle(window)
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
	
	mSystem = system;

	populateList(system->getRootFolder()->getChildren());
}

FileData* GridGameListView::getCursor()
{
	return mGrid.getSelected();
}

void GridGameListView::setCursor(FileData* file)
{
	if(!mGrid.setCursor(file) && mGrid.getEntryCount() > 0)
	{
		//populateList(file->getParent()->getChildren());
		mGrid.setCursor(file);
		mTitle.setText(file->getName());
	}
}

bool GridGameListView::input(InputConfig* config, Input input)
{
	if (config->isMappedTo("left", input) || config->isMappedTo("right", input) || config->isMappedTo("up", input) || config->isMappedTo("down", input)) {
		// Quick and dirty way of changing header title without doing in update()
		mTitle.setText(mGrid.getSelectedName());

		// Destroy dpad input so mGrid can use it.
		return GuiComponent::input(config, input);
	}

	// Quick system change
	if (config->isMappedTo("LeftShoulder", input) || config->isMappedTo("LeftTrigger", input)) {
		if (Settings::getInstance()->getBool("QuickSystemSelect")) {
			ViewController::get()->goToPrevGameList();
			return true;
		}
	}

	if (config->isMappedTo("RightShoulder", input) || config->isMappedTo("RightTrigger", input)) {
		if (Settings::getInstance()->getBool("QuickSystemSelect")) {
			ViewController::get()->goToNextGameList();
			return true;
		}
	}

	return ISimpleGameListView::input(config, input);
}

void GridGameListView::update(int deltatime) {
	// For Loading in game art as the user clicks on the system. 
	// Loads one per frame, or if specified to load on frame x.
	if (mLoadFrame >= mLoadFrameKey -2) {
		mGrid.dynamicImageLoader();
		mLoadFrame = 0;
	}

	mLoadFrame++;

	mGrid.update(deltatime);
}

void GridGameListView::populateList(const std::vector<FileData*>& files)
{
	// If list is already populated, return.
	if (mGrid.getEntryCount() < files.size()) {
		// Load in gamelist and load in first game's art.
		int b = 0;
		for (auto it = files.begin(); it != files.end(); it++) {
			mGrid.add((*it)->getName(), (*it)->getThumbnailPath(), *it, b == 0);
			b++;
		}
	}
}

void GridGameListView::InitGrid(const std::vector<FileData*>& files) {
	auto it = files.at(0);
	mGrid.add(it->getName(), it->getThumbnailPath(), it);

}

void GridGameListView::launch(FileData* game)
{
	ViewController::get()->launch(game);
}

void GridGameListView::remove(FileData *game)
{
	boost::filesystem::remove(game->getPath());  // actually delete the file on the filesystem
	if (getCursor() == game)                     // Select next element in list, or prev if none
	{
		std::vector<FileData*> siblings = game->getParent()->getChildren();
		auto gameIter = std::find(siblings.begin(), siblings.end(), game);
		auto gamePos = std::distance(siblings.begin(), gameIter);
		if (gameIter != siblings.end())
		{
			if ((gamePos + 1) < siblings.size())
			{
				setCursor(siblings.at(gamePos + 1));
			}
			else if ((gamePos - 1) > 0) {
				setCursor(siblings.at(gamePos - 1));
			}
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

	mTitle.applyTheme(theme, getName(), "md_title", ALL);
	mGrid.applyTheme(theme, getName(), "md_grid", ALL);

	mGrid.applyThemeToChildren(theme);

}


void GridGameListView::onFocusGained() {
	mGrid.updateLoadRange();
	mGrid.reloadTextures();
}

void GridGameListView::onFocusLost() {
	for (int i = 1; i < mGrid.getEntryCount(); i++) {
		mGrid.clearImageAt(i);
	}
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
