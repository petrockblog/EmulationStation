#pragma once

#include "ThemeData.h"
#include "GuiComponent.h"
#include "components/IList.h"
#include "components/ImageComponent.h"
#include "components/TextComponent.h"
#include "components/NinePatchComponent.h"
#include "components/GridTileComponent.h"
#include "Settings.h"
#include "Renderer.h"
#include "Log.h"

struct ImageGridData
{
	std::shared_ptr<TextureResource> texture;
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
	using IList<ImageGridData, T>::getTransform;
	using IList<ImageGridData, T>::mSize;
	using IList<ImageGridData, T>::mPosition;
	using IList<ImageGridData, T>::mCursor;
	using IList<ImageGridData, T>::Entry;
	using IList<ImageGridData, T>::mWindow;
	using IList<ImageGridData, T>::mScrollTier;
	using IList<ImageGridData, T>::mTotalLoadedTextures;
	using IList<ImageGridData, T>::mLoadedTextureList;
	using IList<ImageGridData, T>::mMissingBoxartTexture;

public:
	using IList<ImageGridData, T>::size;
	using IList<ImageGridData, T>::isScrolling;
	using IList<ImageGridData, T>::stopScrolling;

	ImageGridComponent(Window* window, int modGridSize = 1);
	~ImageGridComponent();

	void remove();

	void add(const std::string& name, const std::string& imagePath, const T& obj, bool loadTextureNow = true);
	
	void onSizeChanged() override;
	
	void setMargin(Eigen::Vector2f marg) { mMargin = marg; }

	void clear(bool clearall = false) override;

	bool input(InputConfig* config, Input input) override;
	void update(int deltaTime) override;
	void render(const Eigen::Affine3f& parentTrans) override;
	void applyTheme(const std::shared_ptr<ThemeData>& theme, const std::string& view, const std::string& element, unsigned int properties) override;
	void setVisible(bool visible);

	int getEntryCount();
	int getCursorIndex();
	FileData* getSelectedObject();

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

	Eigen::Vector2i getGridDimensions() const
	{
		if (mRequestedGridDimensions.x() > 0)
            return mRequestedGridDimensions;

		Eigen::Vector2f squareSize = getMaxSquareSize();
		if (!mTiles.empty()) squareSize = mTiles[0]->getSize();

		Eigen::Vector2i gridSize(mSize.x() / (squareSize.x() + getPadding().x()), mSize.y() / (squareSize.y() + getPadding().y()));
		return gridSize;
	};

	Eigen::Vector2f getPadding() const { return Eigen::Vector2f(24, 24); }
	Eigen::Vector2f getMargin() { return mMargin; }
	
	void buildImages();
	void updateImages();

	virtual void onCursorChanged(const CursorState& state);

	bool mEntriesDirty;

	Eigen::Vector2f mMargin;
	Eigen::Vector2i mRequestedGridDimensions;

	const int MAX_TEXTURES = 80;			// The maximum amount of images that can be loaded at once
	const int CURSOR_RANGE = 34;			// How many images will be loaded around the cursor [ Cursor will be center of range ]

	CursorRange mCursorRange;	
	int mCurrentLoad = 0;					// The current loaded in texture
	bool bLoading = false;					// Loading in textures in the cursor range.
	bool bUnloaded = false;					// No longer loading and just finished unloading old textures.

	int mPrevIndex = 0;
	int mPrevScrollTier = 0;
	int mCurrentDirection = MOVING_DOWN;	// Which direction the user is moving (Orientation based on grid, not index)

	std::vector< std::shared_ptr<GridTileComponent> > mTiles;

	std::shared_ptr<ThemeData> mTheme;
	bool bThemeLoaded = false;
	bool bVisible = false;
};

template<typename T>
ImageGridComponent<T>::ImageGridComponent(Window* window, int modGridSize) : IList<ImageGridData, T>(window)
{
	mEntriesDirty = true;
	setMargin(Eigen::Vector2f(24, 24));
	mRequestedGridDimensions.x() = 0;
	mRequestedGridDimensions.y() = 0;
	mMissingBoxartTexture = TextureResource::get(":/blank_game.png");
}

template<typename T>
ImageGridComponent<T>::~ImageGridComponent() {
	mTiles.clear();
}

template<typename T>
void ImageGridComponent<T>::clear(bool clearall) {
	unloadTextures(clearall);
	mEntriesDirty = true;
	mCurrentDirection = MOVING_DOWN;

	mPrevIndex = 0;
	bLoading = false;
	bUnloaded = false;
	mCurrentLoad = 0;

	IList<ImageGridData, T>::clear();
	mTiles.clear();
}

template<typename T>
int ImageGridComponent<T>::getEntryCount() {
	return mEntries.size();
}

template<typename T>
int ImageGridComponent<T>::getCursorIndex() {
	return static_cast<IList< ImageGridData, T >*>(this)->getCursorIndex();
}

template<typename T>
void ImageGridComponent<T>::remove() {
	static_cast<IList< ImageGridData, T >*>(this)->pop_back();

	mEntriesDirty = true;
}

template<typename T>
void ImageGridComponent<T>::add(const std::string& name, const std::string& imagePath, const T& obj, bool loadTextureNow)
{
	typename IList<ImageGridData, T>::Entry entry;
	entry.name = name;
	entry.object = obj;
	entry.strdata = imagePath;
	if (obj->getType() == 2) entry.data.texture = TextureResource::get(":/folder.png");
	else {
		if (loadTextureNow) entry.data.texture = ResourceManager::getInstance()->fileExists(imagePath) ? TextureResource::get(imagePath) : TextureResource::get(":/blank_game.png");
		else entry.data.texture = TextureResource::get(":/frame.png");
	}

	static_cast<IList< ImageGridData, T >*>(this)->add(entry);
	mEntriesDirty = true;
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
			if ((*i) > mEntries.size()) continue;
			clearImageAt((*i));
			i = mLoadedTextureList.erase(i);
		}
	}

	// Phase 2: If there are still too many textures.  Just remove all
	if (mLoadedTextureList.size() > MAX_TEXTURES || unloadAll) {
		// If there are too many textures loaded, the user is likely out of
		// Cursor range, so just delete every image.
		for (int i = 1; i < getEntryCount() - 1; i++)
			clearImageAt(i);
		mLoadedTextureList.clear();
	}

	if (unloadAll) mTiles.clear();

	bLoading = true;		// Reload images now.
	bUnloaded = false;
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
	if (mLoadedTextureList.size() > MAX_TEXTURES) unloadTextures();

	// if cursor is getting close to the ends of range; update
	if (getCursorIndex() > mCursorRange.max - 5 && getCursorIndex() < mCursorRange.min + 5) updateLoadRange();

	// Update range if user is out of range and no loading/unloading is being performed.
	if (bLoading == false && bUnloaded && getCursorIndex() > mCursorRange.max || getCursorIndex() < mCursorRange.min)
		updateLoadRange();

	// Load in Texture per cycle.
	if (bLoading && mScrollTier < 1) {
		// Make sure index is in range.
		if (mCurrentLoad < getEntryCount() && mCurrentLoad > 0) {
			if (mLoadedTextureList.size() > MAX_TEXTURES) unloadTextures();
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
			if (mCursorRange.max < getEntryCount()) {
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

	int cursorRange = CURSOR_RANGE;

	// Create a range based on cursor position
	int cursor = getCursorIndex();

	// return if index hasn't changed and range is setup
	if (cursor == mPrevIndex && mCursorRange.length > 0) return;

	// Get minimum [ will stay at 0 until user moves past 12. ]
	int rmin = cursor - int(cursorRange / 2);
	if (rmin < 0) rmin += rmin * -1;

	// get max [ will try to be just the viewable area based on mod size ]
	int rmax = 0;
	if (rmin < cursorRange / 2)
		rmax = cursor + int(cursorRange);
	else rmax = cursor + int(cursorRange / 2);
	if (rmax > getEntryCount()) rmax = getEntryCount() - 1;

	// if there is only one game, set range 0-0
	if (getEntryCount() == 1) rmax = 0;

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
			listInput(dir.x() + dir.y() * getGridDimensions().x());
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
void ImageGridComponent<T>::setVisible(bool visible) {
	bVisible = visible;
}

template<typename T>
void ImageGridComponent<T>::render(const Eigen::Affine3f& parentTrans)
{
	// If this grid isn't in focus, return and do not render
	if (!bVisible) return;

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
		// Keep the selected tile and render it later (so it draws on top others)
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
void ImageGridComponent<T>::applyTheme(const std::shared_ptr<ThemeData>& theme, const std::string& view, const std::string& element, unsigned int properties)
{
	Eigen::Vector2f screen = Eigen::Vector2f((float)Renderer::getScreenWidth(), (float)Renderer::getScreenHeight());
	using namespace ThemeFlags;

	// Keep theme data pointer.
	mTheme = theme;
	bThemeLoaded = true;

	// Get hacked-in theme stuff
	// Margin:
	auto elem = theme->getElement(view, "md_grid_margin", "container");
	if (elem) {
		if (elem->has("size")) 
			setMargin(elem->get<Eigen::Vector2f>("size").cwiseProduct(screen));
	} 

	// Grid Size (Columns and Rows)
	elem = theme->getElement(view, "gridRowsAndColumns", "container");
	if (elem) {
		if (elem->has("size")) {
			Eigen::Vector2f RequestedGridDimensions = elem->get<Eigen::Vector2f>("size");
			mRequestedGridDimensions.x() = (int)RequestedGridDimensions.x();
			mRequestedGridDimensions.y() = (int)RequestedGridDimensions.y();
		}
	}

	// Change default missing boxart icon
	elem = theme->getElement(view, "missing_boxart", "image");
	if (elem) {
		std::string path = elem->get<std::string>("path");
		if (ResourceManager::getInstance()->fileExists(path))
			mMissingBoxartTexture = TextureResource::get(path);
		else
			LOG(LogWarning) << "Could not replace MissingBoxart, check path: " << path;
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

	Eigen::Vector2i gridSize = getGridDimensions();
	Eigen::Vector2f squareSize = getMaxSquareSize();

	// Setup gridsize either by default or from theme
	if (mRequestedGridDimensions.x() == 0) {
		mRequestedGridDimensions.x() = 4;
		mRequestedGridDimensions.y() = 2;
	}

	// Get distance between tile points.
	float tileDistanceX = (mSize.x() / mRequestedGridDimensions.x()) - getMargin().x();
	float tileDistanceY = (mSize.y() / mRequestedGridDimensions.y()) - getMargin().y();

	// Scale image by aspect ratio.
	float ratioW = tileDistanceX / squareSize.x();
	float ratioH = tileDistanceY / squareSize.y();
	float ratio = ratioW < ratioH ? ratioW : ratioH;
	squareSize.x() *= ratio;
	squareSize.y() *= ratio;
	
	// The margin to add to have all tiles be evenly spaced and be flush with sides.
	float realMargin = mSize.x() - (squareSize.x() * mRequestedGridDimensions.x());
	realMargin /= mRequestedGridDimensions.x() - 1;
	float realMarginY = mSize.y() - (squareSize.y() * mRequestedGridDimensions.y());
	realMarginY /= mRequestedGridDimensions.y() - 1;

	// Layout tile size and position
	float tdy = 0;
	for(int y = 0; y < gridSize.y(); y++)
	{
		float tdx = 0;
		for(int x = 0; x < gridSize.x(); x++)
		{
			// Create tiles
			auto tile = std::make_shared<GridTileComponent>(mWindow, y * gridSize.x() + x);
			tile->setImageSize(squareSize.x(), squareSize.y());

			tile->setPosition(tdx, tdy);

			if (bThemeLoaded) tile->setTheme(mTheme);

			mTiles.push_back(tile);

			// Increase X position
			tdx += squareSize.x() + realMargin;
		}

		// Increase y position
		tdy += squareSize.y() + realMarginY;
	}

}

template<typename T>
void ImageGridComponent<T>::updateImages()
{
	if (mTiles.empty())
		buildImages();

	Eigen::Vector2i gridSize = getGridDimensions();

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
			continue;
		}

		tile->show();

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
