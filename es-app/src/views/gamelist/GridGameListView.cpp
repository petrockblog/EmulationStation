#include "views/gamelist/GridGameListView.h"
#include "animations/LambdaAnimation.h"

// ===========================================================================
// Part of this was written by Aloshi but was never finished and added to ES. 
// It would originally load all game art at ES load and could easily eat up
// All VRAM in smaller systems, such as the raspberry pi.
// I've changed how ImageGridComponent stores the game's art texture so
// it will only load the images for tiles in a range of the cursor and
// unload textures outside of its range depending on which direction the
// user is moving.
// ============================================================================

GridGameListView::GridGameListView(Window* window, FileData* root) : ISimpleGameListView(window, root),
	mGrid(window, 6), mBackgroundImage(window), mTitle(window),
	mDescContainer(window), mDescription(window),
	mImage(window),
	
	mLblRating(window), mLblReleaseDate(window), mLblDeveloper(window), mLblPublisher(window),
	mLblGenre(window), mLblPlayers(window), mLblLastPlayed(window), mLblPlayCount(window),

	mRating(window), mReleaseDate(window), mDeveloper(window), mPublisher(window),
	mGenre(window), mPlayers(window), mLastPlayed(window), mPlayCount(window)
{
	mTitle.setFont(Font::get(FONT_SIZE_MEDIUM));
	mTitle.setPosition(0, mSize.y() * 0.05f);
	mTitle.setColor(0xAAAAAAFF);
	mTitle.setSize(mSize.x(), 0);
	mTitle.setAlignment(ALIGN_CENTER);
	addChild(&mTitle);

	mGrid.setPosition(24, mSize.y() * 0.15f);
	mGrid.setSize(mSize.x() * 0.8f, mSize.y() * 0.8f);
	mGrid.setMargin(Eigen::Vector2f(Renderer::getScreenWidth() * .02f, Renderer::getScreenHeight() * .05f));
	addChild(&mGrid);
	
	mRoot = root;

	populateList(mRoot->getChildren());

	// --- DETAILED LIST METADATA ---
	const float padding = 0.01f;

	// metadata labels + values
	mLblRating.setText("Rating: ");
	addChild(&mLblRating);
	addChild(&mRating);
	mLblReleaseDate.setText("Released: ");
	addChild(&mLblReleaseDate);
	addChild(&mReleaseDate);
	mLblDeveloper.setText("Developer: ");
	addChild(&mLblDeveloper);
	addChild(&mDeveloper);
	mLblPublisher.setText("Publisher: ");
	addChild(&mLblPublisher);
	addChild(&mPublisher);
	mLblGenre.setText("Genre: ");
	addChild(&mLblGenre);
	addChild(&mGenre);
	mLblPlayers.setText("Players: ");
	addChild(&mLblPlayers);
	addChild(&mPlayers);
	mLblLastPlayed.setText("Last played: ");
	addChild(&mLblLastPlayed);
	mLastPlayed.setDisplayMode(DateTimeComponent::DISP_RELATIVE_TO_NOW);
	addChild(&mLastPlayed);
	mLblPlayCount.setText("Times played: ");
	addChild(&mLblPlayCount);
	addChild(&mPlayCount);

	mDescContainer.setPosition(mSize.x() * padding, mSize.y() * 0.65f);
	mDescContainer.setSize(mSize.x() * (0.50f - 2 * padding), mSize.y() - mDescContainer.getPosition().y());
	mDescContainer.setAutoScroll(true);
	addChild(&mDescContainer);

	mDescription.setFont(Font::get(FONT_SIZE_SMALL));
	mDescription.setSize(mDescContainer.getSize().x(), 0);
	mDescContainer.addChild(&mDescription);

	initMDLabels();
	initMDValues();
	updateInfoPanel();
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
	if (config->isMappedTo("left", input) || config->isMappedTo("right", input)
	    || config->isMappedTo("up", input) || config->isMappedTo("down", input)) {
		// Quick and dirty way of changing header title without doing in update()
		mTitle.setText(mGrid.getSelectedName());
		updateInfoPanel();

		// Destroy dpad input so mGrid can use it.
		return GuiComponent::input(config, input);
	}

	return ISimpleGameListView::input(config, input);
}

void GridGameListView::update(int deltatime) {
	if (!bFocused)
		return;

	// For Loading in game art as the user clicks on the system. 
	// Loads one per frame, or if specified to load on frame x.
	if (mLoadFrame >= mLoadFrameKey) {
		mGrid.dynamicImageLoader();
		mLoadFrame = 0;
	}

	mDescContainer.update(deltatime);
	mLoadFrame++;

	mGrid.update(deltatime);

	if (mSystemQuickChangeCoolDown < 30)
		mSystemQuickChangeCoolDown++;
}

void GridGameListView::populateList(const std::vector<FileData*>& files)
{
	// If list is already populated, return.
	if (mGrid.getEntryCount() != files.size()) {
		mGrid.clear(true);
		// Load in gamelist and load in first game's art.
		int b = 0;
		for (auto it = files.begin(); it != files.end(); it++) {
			mGrid.add((*it)->getName(), (*it)->getThumbnailPath(), *it, b == 0);
			b++;
		}

		// Reload textures after folder change
		mGrid.reloadTextures();
		mLoadFrame = mLoadFrameKey;
	}
}

void GridGameListView::launch(FileData* game)
{
	ViewController::get()->launch(game);
}

void GridGameListView::remove(FileData *game, bool deleteFile)
{
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
	mTitle.applyTheme(theme, getName(), "md_title", ALL);
	mGrid.applyTheme(theme, getName(), "md_grid", ALL);
	mGrid.applyThemeToChildren(theme);

	// ---  DETAILED METADATA THEME ---
	mImage.applyTheme(theme, getName(), "md_image", POSITION | ThemeFlags::SIZE | Z_INDEX);

	initMDLabels();
	std::vector<TextComponent*> labels = getMDLabels();
	assert(labels.size() == 8);
	const char* lblElements[8] = {
		"md_lbl_rating", "md_lbl_releasedate", "md_lbl_developer", "md_lbl_publisher",
		"md_lbl_genre", "md_lbl_players", "md_lbl_lastplayed", "md_lbl_playcount"
	};

	for (unsigned int i = 0; i < labels.size(); i++)
		labels[i]->applyTheme(theme, getName(), lblElements[i], ALL);

	initMDValues();
	std::vector<GuiComponent*> values = getMDValues();
	assert(values.size() == 8);
	const char* valElements[8] = {
		"md_rating", "md_releasedate", "md_developer", "md_publisher",
		"md_genre", "md_players", "md_lastplayed", "md_playcount"
	};

	for (unsigned int i = 0; i < values.size(); i++)
		values[i]->applyTheme(theme, getName(), valElements[i], ALL ^ ThemeFlags::TEXT);

	mDescContainer.applyTheme(theme, getName(), "md_description", POSITION | ThemeFlags::SIZE | Z_INDEX);
	mDescription.setSize(mDescContainer.getSize().x(), 0);
	mDescription.applyTheme(theme, getName(), "md_description", ALL ^ (POSITION | ThemeFlags::SIZE | ThemeFlags::ORIGIN | TEXT));

	sortChildren();
}

void GridGameListView::updateInfoPanel()
{
	FileData* file = (mGrid.size() == 0 || mGrid.isScrolling()) ? NULL : mGrid.getSelectedObject();

	bool fadingOut;
	if (file == NULL)
	{
		//mImage.setImage("");
		//mDescription.setText("");
		fadingOut = true;
	}
	else {
		mImage.setImage(file->metadata.get("image"));
		mDescription.setText(file->metadata.get("desc"));
		mDescContainer.reset();

		if (file->getType() == GAME)
		{
			mRating.setValue(file->metadata.get("rating"));
			mReleaseDate.setValue(file->metadata.get("releasedate"));
			mDeveloper.setValue(file->metadata.get("developer"));
			mPublisher.setValue(file->metadata.get("publisher"));
			mGenre.setValue(file->metadata.get("genre"));
			mPlayers.setValue(file->metadata.get("players"));
			mLastPlayed.setValue(file->metadata.get("lastplayed"));
			mPlayCount.setValue(file->metadata.get("playcount"));
		}

		fadingOut = false;
	}

	std::vector<GuiComponent*> comps = getMDValues();
	comps.push_back(&mImage);
	comps.push_back(&mDescription);
	std::vector<TextComponent*> labels = getMDLabels();
	comps.insert(comps.end(), labels.begin(), labels.end());

	for (auto it = comps.begin(); it != comps.end(); it++)
	{
		GuiComponent* comp = *it;
		// an animation is playing
		//   then animate if reverse != fadingOut
		// an animation is not playing
		//   then animate if opacity != our target opacity
		if ((comp->isAnimationPlaying(0) && comp->isAnimationReversed(0) != fadingOut) ||
			(!comp->isAnimationPlaying(0) && comp->getOpacity() != (fadingOut ? 0 : 255)))
		{
			auto func = [comp](float t)
			{
				comp->setOpacity((unsigned char)(lerp<float>(0.0f, 1.0f, t) * 255));
			};
			comp->setAnimation(new LambdaAnimation(func, 150), 0, nullptr, fadingOut);
		}
	}
}

void GridGameListView::initMDLabels()
{
	using namespace Eigen;

	std::vector<TextComponent*> components = getMDLabels();

	const unsigned int colCount = 2;
	const unsigned int rowCount = components.size() / 2;

	Vector3f start(mSize.x() * 0.01f, mSize.y() * 0.625f, 0.0f);

	const float colSize = (mSize.x() * 0.48f) / colCount;
	const float rowPadding = 0.01f * mSize.y();

	for (unsigned int i = 0; i < components.size(); i++)
	{
		const unsigned int row = i % rowCount;
		Vector3f pos(0.0f, 0.0f, 0.0f);
		if (row == 0)
			pos = start + Vector3f(colSize * (i / rowCount), 0, 0);
		else {
			// work from the last component
			GuiComponent* lc = components[i - 1];
			pos = lc->getPosition() + Vector3f(0, lc->getSize().y() + rowPadding, 0);
		}

		components[i]->setFont(Font::get(FONT_SIZE_SMALL));
		components[i]->setPosition(pos);
		components[i]->setDefaultZIndex(40);
	}
}

void GridGameListView::initMDValues()
{
	using namespace Eigen;

	std::vector<TextComponent*> labels = getMDLabels();
	std::vector<GuiComponent*> values = getMDValues();

	std::shared_ptr<Font> defaultFont = Font::get(FONT_SIZE_SMALL);
	mRating.setSize(defaultFont->getHeight() * 5.0f, (float)defaultFont->getHeight());
	mReleaseDate.setFont(defaultFont);
	mDeveloper.setFont(defaultFont);
	mPublisher.setFont(defaultFont);
	mGenre.setFont(defaultFont);
	mPlayers.setFont(defaultFont);
	mLastPlayed.setFont(defaultFont);
	mPlayCount.setFont(defaultFont);

	float bottom = 0.0f;

	const float colSize = (mSize.x() * 0.48f) / 2;
	for (unsigned int i = 0; i < labels.size(); i++)
	{
		const float heightDiff = (labels[i]->getSize().y() - values[i]->getSize().y()) / 2;
		values[i]->setPosition(labels[i]->getPosition() + Vector3f(labels[i]->getSize().x(), heightDiff, 0));
		values[i]->setSize(colSize - labels[i]->getSize().x(), values[i]->getSize().y());
		values[i]->setDefaultZIndex(40);

		float testBot = values[i]->getPosition().y() + values[i]->getSize().y();
		if (testBot > bottom)
			bottom = testBot;
	}

	mDescContainer.setPosition(mDescContainer.getPosition().x(), bottom + mSize.y() * 0.01f);
	mDescContainer.setSize(mDescContainer.getSize().x(), mSize.y() - mDescContainer.getPosition().y());
}

std::vector<TextComponent*> GridGameListView::getMDLabels()
{
	std::vector<TextComponent*> ret;
	ret.push_back(&mLblRating);
	ret.push_back(&mLblReleaseDate);
	ret.push_back(&mLblDeveloper);
	ret.push_back(&mLblPublisher);
	ret.push_back(&mLblGenre);
	ret.push_back(&mLblPlayers);
	ret.push_back(&mLblLastPlayed);
	ret.push_back(&mLblPlayCount);
	return ret;
}

std::vector<GuiComponent*> GridGameListView::getMDValues()
{
	std::vector<GuiComponent*> ret;
	ret.push_back(&mRating);
	ret.push_back(&mReleaseDate);
	ret.push_back(&mDeveloper);
	ret.push_back(&mPublisher);
	ret.push_back(&mGenre);
	ret.push_back(&mPlayers);
	ret.push_back(&mLastPlayed);
	ret.push_back(&mPlayCount);
	return ret;
}

void GridGameListView::onFocusGained() {
	mGrid.updateLoadRange();
	mGrid.reloadTextures();
	mGrid.setVisible(true);
	bFocused = true;
	mSystemQuickChangeCoolDown = 0;
}

void GridGameListView::onFocusLost() {
	if (mGrid.getEntryCount() > 0)
		mGrid.unloadTextures(true);
	mGrid.setVisible(false);
	bFocused = false;
	mSystemQuickChangeCoolDown = 0;
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
