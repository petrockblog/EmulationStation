#pragma once

#include "ThemeData.h"
#include "GuiComponent.h"
#include "components/IList.h"
#include "components/ImageComponent.h"
#include "components/TextComponent.h"
#include "components/NinePatchComponent.h"
#include "components/GridTileComponent.h"
#include "Settings.h"
#include "Log.h"

struct ImageGridData
{
	std::shared_ptr<TextureResource> texture;
	std::shared_ptr<TextComponent> title;
};

// Keeps track of which direction the user is moving.  ( for dynamic loading )
enum UserDirection 
{
	MOVING_UP,
	MOVING_DOWN
};

// A range around the cursor's index used for loading in textures
struct CursorRange {
	int min = 0;
	int max = 12;
	int length = 0;
};

template<typename T>
class ImageGridComponent : public IList<ImageGridData, T>
{
protected:
	using IList<ImageGridData, T>::mEntries;
	using IList<ImageGridData, T>::listUpdate;
	using IList<ImageGridData, T>::listInput;
	using IList<ImageGridData, T>::listRenderTitleOverlay;
	using IList<ImageGridData, T>::listRenderFileTitle;
	using IList<ImageGridData, T>::getTransform;
	using IList<ImageGridData, T>::mSize;
	using IList<ImageGridData, T>::mCursor;
	using IList<ImageGridData, T>::Entry;
	using IList<ImageGridData, T>::mWindow;
	using IList<ImageGridData, T>::mScrollTier;
	using IList<ImageGridData, T>::mTotalLoadedTextures;
	using IList<ImageGridData, T>::mLoadedTextureList;

public:
	using IList<ImageGridData, T>::size;
	using IList<ImageGridData, T>::isScrolling;
	using IList<ImageGridData, T>::stopScrolling;

	ImageGridComponent(Window* window, int modGridSize = 1);
	~ImageGridComponent();

	void remove();

	void add(const std::string& name, const std::string& imagePath, const T& obj, bool loadTextureNow = true);
	
	void onSizeChanged() override;
	
	void setModSize(float mod);

	bool input(InputConfig* config, Input input) override;
	void update(int deltaTime) override;
	void render(const Eigen::Affine3f& parentTrans) override;
	void applyThemeToChildren(const std::shared_ptr<ThemeData>& theme);

	int getEntryCount();
	int getCursorIndex();
	FileData* getSelectedObject();

	void setAlignmentCenter();

	void reloadTextures();
	void dynamicImageLoader();
	void clearImageAt(int index);
	void updateLoadRange();
	void unloadTextures(bool unloadAll = false);

private:
	Eigen::Vector2f getSquareSize(std::shared_ptr<TextureResource> tex = nullptr) const
	{
		Eigen::Vector2f aspect(1, 1);

		if(tex)
		{
			const Eigen::Vector2i& texSize = tex->getSize();

			if(texSize.x() > texSize.y())
				aspect[0] = (float)texSize.x() / texSize.y();
			else
				aspect[1] = (float)texSize.y() / texSize.x();
		}

		return Eigen::Vector2f((156 * aspect.x()), (156 * aspect.y() ) );
	};

	Eigen::Vector2f getMaxSquareSize() const
	{
		Eigen::Vector2f squareSize(32, 32);

		// calc biggest square size
		for(auto it = mEntries.begin(); it != mEntries.end(); it++)
		{
			Eigen::Vector2f chkSize = getSquareSize(it->data.texture);
			if(chkSize.x() > squareSize.x())
				squareSize[0] = chkSize[0];
			if(chkSize.y() > squareSize.y())
				squareSize[1] = chkSize[1];
		}

		return squareSize;
	};

	Eigen::Vector2i getGridSize() const
	{
		if (mDesiredGridSize.x() > 0) return mDesiredGridSize;

		Eigen::Vector2f squareSize = getMaxSquareSize();
		if (!mTiles.empty()) squareSize = mTiles[0]->getSize();

		Eigen::Vector2i gridSize(mSize.x() / (squareSize.x() + getPadding().x()), mSize.y() / (squareSize.y() + getPadding().y()));
		return gridSize;
	};

	Eigen::Vector2f getPadding() const { return Eigen::Vector2f(24, 24); }
	Eigen::Vector2f getMargin() { return mMargin; }
	void setMargin(Eigen::Vector2f marg) { mMargin = marg; }
	
	void buildImages();
	void updateImages();

	virtual void onCursorChanged(const CursorState& state);

	bool mEntriesDirty;
	bool mGameGrid = true;

	int mTotalEntrys = 0;					// How many entries are loaded in from gamelist

	float mGridMod = 1;						// What size the tiles will be multiplied by (1 = .1)
	Eigen::Vector2f mMargin;
	Eigen::Vector2i mDesiredGridSize;

	const int MAX_TEXTURES = 80;			// The maximum amount of images that can be loaded at once
	const int CURSOR_RANGE = 32;			// How many images will be loaded around the cursor [ Cursor will be center of range ]

	CursorRange mCursorRange;	
	int mCurrentLoad = 0;					// The current loaded in texture
	bool bLoading = false;					// Loading in textures in the cursor range.
	bool bUnloaded = false;					// No longer loading and just finished unloading old textures.

	int mCursorUpdateDelay = 0;				// Holds how many frames have passed [ for waiting to update load range ]

	int mPrevIndex = 0;
	int mPrevScrollTier = 0;
	int mCurrentDirection = MOVING_DOWN;	// Which direction the user is moving (Orientation based on grid, not index)
	int mAlignment = ALIGN_LEFT;
	
	std::vector<TextComponent> mTitles;
	std::vector< std::shared_ptr<GridTileComponent> > mTiles;

	std::shared_ptr<ThemeData> mTheme;
	bool bThemeLoaded = false;
};

template<typename T>
ImageGridComponent<T>::ImageGridComponent(Window* window, int modGridSize) : IList<ImageGridData, T>(window)
{
	mEntriesDirty = true;
	setMargin(Eigen::Vector2f(24, 24));
	mDesiredGridSize.x() = 0;
	mDesiredGridSize.y() = 0;
	mGridMod = modGridSize;
}

template<typename T>
ImageGridComponent<T>::~ImageGridComponent() {
	mTiles.clear();
}

template<typename T>
void ImageGridComponent<T>::setAlignmentCenter() {
	mAlignment = ALIGN_CENTER;
}

template<typename T>
int ImageGridComponent<T>::getEntryCount() {
	return mTotalEntrys;
}

template<typename T>
int ImageGridComponent<T>::getCursorIndex() {
	return static_cast<IList< ImageGridData, T >*>(this)->getCursorIndex();
}

template<typename T>
void ImageGridComponent<T>::remove() {
	static_cast<IList< ImageGridData, T >*>(this)->pop_back();

	mEntriesDirty = true;
	mTotalEntrys--;
}

template<typename T>
void ImageGridComponent<T>::add(const std::string& name, const std::string& imagePath, const T& obj, bool loadTextureNow)
{
	typename IList<ImageGridData, T>::Entry entry;
	entry.name = name;
	entry.object = obj;
	entry.strdata = imagePath;
	if (loadTextureNow) entry.data.texture = ResourceManager::getInstance()->fileExists(imagePath) ? TextureResource::get(imagePath) : TextureResource::get(":/blank_game.png");
	else entry.data.texture = TextureResource::get(":/frame.png");
	entry.data.title = std::make_shared<TextComponent>(mWindow, name, Font::get(FONT_SIZE_MEDIUM), 0xAAAAAAFF);
	static_cast<IList< ImageGridData, T >*>(this)->add(entry);
	mEntriesDirty = true;
	mTotalEntrys++;
}

template<typename T>
FileData* ImageGridComponent<T>::getSelectedObject() {
	return mEntries.at(getCursorIndex()).object;
}

template<typename T>
void ImageGridComponent<T>::unloadTextures(bool unloadAll) {
	// Phase 1: Go through and unload textures based on list of indexes
	if (mLoadedTextureList.size() > 0) {
		for (auto i = mLoadedTextureList.begin(); i != mLoadedTextureList.end(); i++) {
			// Ignore textures currently in range of cursor
			if (!unloadAll)
				if ((*i) > mCursorRange.min && (*i) < mCursorRange.max) continue;
			clearImageAt((*i));
			i = mLoadedTextureList.erase(i);
		}
	}

	// Phase 2: If there are still too many textures.  Just remove all
	if (mTotalLoadedTextures > MAX_TEXTURES) {
		// If there are too many textures loaded, the user is likely out of
		// Cursor range, so just delete every image.
		for (int i = 1; i < mTotalEntrys - 1; i++)
			clearImageAt(i);
		mLoadedTextureList.clear();
	}

	bLoading = true;		// Reload images now.
}

template<typename T>
void ImageGridComponent<T>::reloadTextures() {
	if (mEntries.size() > 0) {
		bLoading = true;
		mCurrentLoad = mCursorRange.min;
	}
}

template<typename T>
void ImageGridComponent<T>::dynamicImageLoader() {
	if (mTotalLoadedTextures > MAX_TEXTURES) unloadTextures();

	// if cursor is getting close to the ends of range; update
	if (getCursorIndex() > mCursorRange.max - 5 && getCursorIndex() < mCursorRange.min + 5) updateLoadRange();

	// Update range if user is out of range and no loading/unloading is being performed.
	if (bLoading == false && bUnloaded && getCursorIndex() > mCursorRange.max || getCursorIndex() < mCursorRange.min)
		updateLoadRange();

	// Load in Texture per cycle.
	if (bLoading && mScrollTier < 1) {
		// Make sure index is in range.
		if (mCurrentLoad < getEntryCount() && mCurrentLoad > 0) {
			if (mTotalLoadedTextures > MAX_TEXTURES) unloadTextures();
			static_cast<IList <ImageGridData, T >*>(this)->loadTexture(mCurrentLoad);
		}

		// update images as they load in.
		updateImages();

		if (mCurrentLoad < mCursorRange.max) mCurrentLoad++;
		else bLoading = false;
	}
	else
	{
		if (bUnloaded) return;

		// Unload images that are out of range in the opposite direction the user is going
		switch (mCurrentDirection) {
		case MOVING_DOWN:
			if (mCursorRange.min > 0) {
				for (auto i = mLoadedTextureList.begin(); i != mLoadedTextureList.end(); i++) {
					if ((*i) >= mCursorRange.min) break;
					clearImageAt((*i));
					i = mLoadedTextureList.erase(i);
				}
			}
			break;

		case MOVING_UP:
			if (mCursorRange.max < mTotalEntrys) {
				for (auto i = mLoadedTextureList.begin(); i != mLoadedTextureList.end(); i++) {
					if ((*i) <= mCursorRange.max) break;
					clearImageAt((*i));
					i = mLoadedTextureList.erase(i);
				}
			}
			break;
		}

		bUnloaded = true;
	}

	// update and load after user exits out of fast scroll
	if (mPrevScrollTier > 0 && mScrollTier == 0) {
		bLoading = false;
		updateLoadRange();
	}

	mPrevScrollTier = mScrollTier;
}

template<typename T>
void ImageGridComponent<T>::updateLoadRange() {
	// Only update range if not loading and not within last built range
	if (bLoading) return;

	// Create a range based on cursor position
	int cursor = getCursorIndex();

	// return if index hasn't changed and range is setup
	if (cursor == mPrevIndex && mCursorRange.length > 0) return;

	// Get minimum [ will stay at 0 until user moves past 12. ]
	int rmin = cursor - int(CURSOR_RANGE / 2);
	if (rmin < 0) rmin += rmin * -1;

	// get max [ will try to be just the viewable area based on mod size ]
	int rmax = cursor + int(CURSOR_RANGE / 2);
	if (rmax > mTotalEntrys) rmax = mTotalEntrys - 1;

	// if there is only one game, set range 0-0
	if (mTotalEntrys == 1) rmax = 0;

	mCursorRange.min = rmin;
	mCursorRange.max = rmax;
	mCursorRange.length = rmax - rmin;

	mCurrentLoad = mCursorRange.min;
	bLoading = true;

	// Determin user's direction
	if (mPrevIndex > cursor) mCurrentDirection = MOVING_UP;
	else mCurrentDirection = MOVING_DOWN;
	bUnloaded = false;
	mPrevIndex = cursor;

}

template<typename T>
void ImageGridComponent<T>::clearImageAt(int index) {
	static_cast<IList <ImageGridData, T >*>(this)->clearImage(index);
}

template<typename T>
bool ImageGridComponent<T>::input(InputConfig* config, Input input)
{
	if(input.value != 0)
	{
		Eigen::Vector2i dir = Eigen::Vector2i::Zero();
		if(config->isMappedTo("up", input))
			dir[1] = -1;
		else if(config->isMappedTo("down", input))
			dir[1] = 1;
		else if(config->isMappedTo("left", input))
			dir[0] = -1;
		else if(config->isMappedTo("right", input))
			dir[0] = 1;

		if(dir != Eigen::Vector2i::Zero())
		{
			listInput(dir.x() + dir.y() * getGridSize().x());
			return true;
		}
	}else{
		if(config->isMappedTo("up", input) || config->isMappedTo("down", input) || config->isMappedTo("left", input) || config->isMappedTo("right", input))
		{
			stopScrolling();
		}
	}

	return GuiComponent::input(config, input);
}

template<typename T>
void ImageGridComponent<T>::update(int deltaTime)
{
	listUpdate(deltaTime);

	for (auto t = mTiles.begin(); t != mTiles.end(); t++) {
		(*t)->update(deltaTime);
	}
}

template<typename T>
void ImageGridComponent<T>::setModSize(float mod) {
	mGridMod = mod;
	mEntriesDirty = true;
}

template<typename T>
void ImageGridComponent<T>::render(const Eigen::Affine3f& parentTrans)
{
	Eigen::Affine3f trans = getTransform() * parentTrans;

	if(mEntriesDirty)
	{
		buildImages();
		updateImages();
		mEntriesDirty = false;
	}

	int i = 0;
	std::shared_ptr<GridTileComponent> pTile;
	bool bSelected = false;
	for (auto ti = mTiles.begin(); ti != mTiles.end(); ti++) {
		if (i > getEntryCount() - 1) break;
		if ((*ti)->isSelected()) {
			i++;
			pTile = (*ti);
			bSelected = true;
		}
		else {
			i++;
			(*ti)->render(trans);
		}
	}

	if (bSelected) {
		pTile->render(trans);
	}


	listRenderTitleOverlay(trans);

	GuiComponent::renderChildren(trans);
}

template<typename T>
void ImageGridComponent<T>::applyThemeToChildren(const std::shared_ptr<ThemeData>& theme) {
	Eigen::Vector2f screen = Eigen::Vector2f((float)Renderer::getScreenWidth(), (float)Renderer::getScreenHeight());
	using namespace ThemeFlags;

	for (int i = 0; i < mTitles.size(); i++) {
		mTitles[i].applyTheme(theme, "grid", "mdGameTitle", ALL);
	}

	// Keep theme data pointer.
	mTheme = theme;
	bThemeLoaded = true;

	// Get hacked-in theme stuff
	// Margin:
	auto elem = theme->getElement("grid", "md_grid_margin", "container");
	if (elem) {
		if (elem->has("size")) 
			setMargin(elem->get<Eigen::Vector2f>("size").cwiseProduct(screen));
	} 

	// Grid Size (Columns and Rows)
	// Get the element acording to what size the user specified
	elem = theme->getElement("grid", "gridRowsAndColumns", "container");
	if (mGridMod == 0) elem = theme->getElement("grid", "gridRowsAndColumns-small", "container");
	if (mGridMod == 2) elem = theme->getElement("grid", "gridRowsAndColumns-big", "container");
	if (elem) {
		if (elem->has("size")) {
			Eigen::Vector2f DesiredGridSize = elem->get<Eigen::Vector2f>("size");
			mDesiredGridSize.x() = (int)DesiredGridSize.x();
			mDesiredGridSize.y() = (int)DesiredGridSize.y();
		}
	}
}

template<typename T>
void ImageGridComponent<T>::onCursorChanged(const CursorState& state)
{
	updateImages();
	updateLoadRange();
}

template<typename T>
void ImageGridComponent<T>::onSizeChanged()
{
	buildImages();
	updateImages();
}

// create and position imagecomponents (mImages)
template<typename T>
void ImageGridComponent<T>::buildImages()
{
	mTiles.clear();

	Eigen::Vector2i gridSize = getGridSize();
	Eigen::Vector2f squareSize = getMaxSquareSize();
	Eigen::Vector2f padding = getPadding();

	// attempt to center within our size
	Eigen::Vector2f totalSize(gridSize.x() * (squareSize.x() + padding.x()), gridSize.y() * (squareSize.y() + padding.y()));
	Eigen::Vector2f offset(mSize.x() - totalSize.x(), mSize.y() - totalSize.y());
	offset /= 2;

	// Adjust offset to 0 if set to align left.
	if (mAlignment == ALIGN_LEFT) {
		offset.x() = 0;
		offset.y() = 0;
	}

	// Setup gridsize either by default or from theme
	if (mDesiredGridSize.x() == 0) {
		mDesiredGridSize.x() = 4;
		mDesiredGridSize.y() = 2;
	}

	// Get distance between tile points.
	float tileDistanceX = (mSize.x() / mDesiredGridSize.x()) - getMargin().x();
	float tileDistanceY = (mSize.y() / mDesiredGridSize.y()) - getMargin().y();
	float smallestDistance = tileDistanceX;
	bool smallestIsX = true;
	if (tileDistanceY < tileDistanceX) {
		smallestDistance = tileDistanceY;
		smallestIsX = false;
	}

	float imgExpandPerc = smallestDistance * .01;
	float absSmallestX = std::abs(squareSize.x() - smallestDistance);
	float absSmallestY = std::abs(squareSize.y() - smallestDistance);

	if (smallestDistance < squareSize.x() && smallestDistance < squareSize.y()) {
		// IF TILES ARE ALREADY TOO BIG, SHRINK THEM:
		if (smallestIsX) {
			// Stretch image to x and add percentage to y based on aspect ratio
			squareSize.y() -= absSmallestY * squareSize.y() / squareSize.x();
			squareSize.x() -= absSmallestX;
		}
		else {
			// Stretch image to y then add to x based on aspect ratio
			squareSize.x() -= absSmallestX * squareSize.x() / squareSize.y();
			squareSize.y() -= absSmallestY;
		}
	} else {
		// IF TILES ARE NOT BIG ENOUGH, EXPAND THEM:
		if (smallestIsX) {
			// Stretch image to x and add percentage to y based on aspect ratio
			squareSize.y() += absSmallestY * squareSize.y() / squareSize.x();
			squareSize.x() += absSmallestX;
		}
		else {
			// Stretch image to y then add to x based on aspect ratio
			squareSize.x() += absSmallestX * squareSize.x() / squareSize.y();
			squareSize.y() += absSmallestY;
		}

	}

	for(int y = 0; y < gridSize.y(); y++)
	{
		for(int x = 0; x < gridSize.x(); x++)
		{
			// Create tiles
			auto tile = std::make_shared<GridTileComponent>(mWindow, y * gridSize.x() + x);
			tile->setImageSize(squareSize.x(), squareSize.y());
			Eigen::Vector2f newSquareSize = tile->getSize();	// Get new size because a square is built arount the image.
			tile->setPosition(((tileDistanceX + padding.x()) * (x + 0.0f)) + offset.x() + (x * getMargin().x()), 
				(tileDistanceY + padding.y()) * (y + 0.0f) + offset.y() + (y * getMargin().y()));

			if (bThemeLoaded) tile->setTheme(mTheme);
			
			mTiles.push_back(tile);

		}
	}
}

template<typename T>
void ImageGridComponent<T>::updateImages()
{
	if (mTiles.empty())
		buildImages();

	Eigen::Vector2i gridSize = getGridSize();

	int cursorRow = mCursor / gridSize.x();
	int cursorCol = mCursor % gridSize.x();

	int start = (cursorRow - (gridSize.y() / 2)) * gridSize.x();

	//if we're at the end put the row as close as we can and no higher
	if(start + (gridSize.x() * gridSize.y()) >= (int)mEntries.size())
		start = gridSize.x() * ((int)mEntries.size()/gridSize.x() - gridSize.y() + 1);

	if(start < 0)
		start = 0;

	unsigned int i = (unsigned int)start;
	for(unsigned int img = 0; img < mTiles.size(); img++)
	{
		// SET IMAGE FROM TEXUTRE
		auto tile = mTiles.at(img);
		if(i >= (unsigned int)size())
		{
			tile->setSelected(false);
			tile->hide();
			break;
		}

		tile->show();

		Eigen::Vector2f squareSize = getSquareSize(mEntries.at(i).data.texture);
		if(i == mCursor)
		{
			tile->setSelected(true);
			tile->show();
		}else{
			tile->setSelected(false);
		}

		tile->setBackgroundPath(":/frame.png");
		tile->setImage(mEntries.at(i).data.texture);
		tile->setText(mEntries.at(i).name);
		tile->setImageToFit(true);

		i++;
	}
}
