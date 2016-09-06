#include "GridTileComponent.h"
#include "components/TextComponent.h"
#include "components/ImageComponent.h"
#include "Log.h"
#include "AnimationUtil.h"
#include "Renderer.h"
#include <boost/filesystem.hpp>

using namespace Eigen;

GridTileComponent::GridTileComponent(Window* window, int index) : GuiComponent(window), mGrid(window, Vector2i(1, 2)), mBackground(window)
{
	mText = std::make_shared<TextComponent>(mWindow, "", Font::get(FONT_SIZE_SMALL), 0x777777FF);
	mImage = std::make_shared<ImageComponent>(mWindow);

	mGrid.setEntry(mImage, Vector2i(0, 0), false, true);
	mGrid.setEntry(mText, Vector2i(0, 1), false, true);

	// Default row heights
	mTextContainerSize.y() = .15f;
	mGrid.setRowHeightPerc(0, .85f);
	mGrid.setRowHeightPerc(1, .15f);

	backgroundPadding.x() = 0;
	backgroundPadding.y() = 0;

	addChild(&mBackground);
	addChild(&mGrid);

	mIndex = index;
}

void GridTileComponent::setSelected(bool isSelected) {
	bSelected = isSelected;
}

void GridTileComponent::onSizeChanged()
{
	if (mSize.x() == 0 || mSize.y() == 0)
		return;

	if (mAnimation.animateTextContainer)
		mGrid.setRowHeightPerc(1, mAnimation.current.textContainerSize.y());

	if (bStretchImage) {
		mImage->setSize(mSize.x(), mGrid.getRowHeight(0));
	}

	Eigen::Vector2f newGridSize;
	newGridSize.x() = mGrid.getSize().x();
	newGridSize.y() = mImage->getSize().y() + mGrid.getRowHeight(1);

	mImage->setMaxSize(mSize.x(), mGrid.getRowHeight(0));

	mGrid.setSize(mSize);
	if (mAnimation.animateBackgroundSize)
		mBackground.fitTo(mAnimation.current.backgroundSize, Vector3f(mPosition.x(), mPosition.y(), 0), backgroundPadding);
	else
		mBackground.fitTo(newGridSize, Vector3f(mPosition.x(), mPosition.y(), 0), backgroundPadding);

	mText->setSize(mSize.x(), mGrid.getRowHeight(1) * .9f);
}

void GridTileComponent::onPositionChanged() {
	mGrid.setPosition(mPosition);
}

void GridTileComponent::setImageSize(float w, float h, float enlargeScaleX, float enlargeScaleY) {
	mImage->setResize(w, h);
	if (enlargeScaleX > 0 || enlargeScaleY > 0) mImage->setResize(enlargeScaleX, enlargeScaleY);
	//mImage->setMaxSize(w, h);
	mTextContainerSize.x() = w;
	if (mAnimation.animateTextContainer) mTextContainerSize.y() = mAnimation.unselected.textContainerSize.y();
	setSize(w, h);

	//onSizeChanged();
}

void GridTileComponent::setImageResize(float w, float h) {
	mImage->setResize(w, h);
}

void GridTileComponent::setImageToFit(bool fit) {
	bStretchImage = fit;
	onSizeChanged();
}

void GridTileComponent::render(const Eigen::Affine3f& parentTrans) {
	if (bShow) renderChildren(parentTrans);
}

void GridTileComponent::update(int deltaTime) {
	bool bAnimateChange = false;
	if (bSelected) {
		if (mAnimation.frame < mAnimation.maxFrame) {
			mAnimation.current.opacity += (mAnimation.selected.opacity - mAnimation.unselected.opacity) / mAnimation.maxFrame;
			mAnimation.current.color = getColorTween(mAnimation.unselected.color, mAnimation.selected.color, mAnimation.frame, mAnimation.maxFrame - 1);
			mAnimation.current.size += (mAnimation.selected.size - mAnimation.unselected.size) / mAnimation.maxFrame;
			mAnimation.current.backgroundSize.y() += (mAnimation.selected.backgroundSize.y()) / mAnimation.maxFrame;
			mAnimation.current.textContainerSize.y() += (mAnimation.selected.textContainerSize.y() - mAnimation.unselected.textContainerSize.y()) / mAnimation.maxFrame;
			mAnimation.current.pos -= ((mAnimation.selected.size - mAnimation.unselected.size) / mAnimation.maxFrame) / 2;
			mAnimation.current.backgroundColor = getColorTween(mAnimation.unselected.backgroundColor, mAnimation.selected.backgroundColor, mAnimation.frame, mAnimation.maxFrame - 1);
			mAnimation.current.textColor = getColorTween(mAnimation.unselected.textColor, mAnimation.selected.textColor, mAnimation.frame, mAnimation.maxFrame - 1);
			mAnimation.zframe = 1;
			mAnimation.frame++;
			bAnimateChange = true;
		}
	}
	else {
		if (mAnimation.frame > 0) {
			mAnimation.current.opacity -= (mAnimation.selected.opacity - mAnimation.unselected.opacity) / mAnimation.maxFrame;
			mAnimation.current.color = getColorTween(mAnimation.selected.color, mAnimation.unselected.color, mAnimation.frame - 1, mAnimation.maxFrame, true);
			mAnimation.current.size -= (mAnimation.selected.size - mAnimation.unselected.size) / mAnimation.maxFrame;
			mAnimation.current.backgroundSize -= (mAnimation.selected.backgroundSize) / mAnimation.maxFrame;
			mAnimation.current.pos += ((mAnimation.selected.size - mAnimation.unselected.size) / mAnimation.maxFrame) / 2;
			mAnimation.current.textContainerSize.y() -= (mAnimation.selected.textContainerSize.y() - mAnimation.unselected.textContainerSize.y()) / mAnimation.maxFrame;
			mAnimation.current.backgroundColor = getColorTween(mAnimation.selected.backgroundColor, mAnimation.unselected.backgroundColor, mAnimation.frame - 1, mAnimation.maxFrame, true);
			mAnimation.current.textColor = getColorTween(mAnimation.selected.textColor, mAnimation.unselected.textColor, mAnimation.frame - 1, mAnimation.maxFrame, true);
			mAnimation.zframe = 0;
			mAnimation.frame--;
			bAnimateChange = true;
		}
	}

	if (!bAnimateChange) return;

	//if (mAnimation.animateOpacity) setOpacity(mAnimation.current.opacity);
	if (mAnimation.animateTextContainer) mGrid.setRowHeightPerc(1, mAnimation.current.textContainerSize.y());
	if (mAnimation.animateBackgroundColor) {
		mBackground.setCenterColor(mAnimation.current.backgroundColor);
		mBackground.setEdgeColor(mAnimation.current.backgroundColor);
	}
	if (mAnimation.animateColor) mImage->setColorShift(mAnimation.current.color);  // <- Doesn't work.
	if (mAnimation.animateSize) {
		setSize(mAnimation.current.size);
		setPosition(mAnimation.current.pos.x(), mAnimation.current.pos.y(), mAnimation.zframe);
	}
	if (mAnimation.animateTextColor) mText->setColor(mAnimation.current.textColor);
}

void GridTileComponent::setTheme(const std::shared_ptr<ThemeData>& theme) {
	using namespace ThemeFlags;

	// Set animation settings to default if that theme element isn't defined.
	mAnimation.unselected.backgroundSize = mSize;
	mAnimation.current.backgroundSize = mSize;
	mAnimation.unselected.size = getSize();
	mAnimation.unselected.pos.x() = getPosition().x();
	mAnimation.unselected.pos.y() = getPosition().y();
	mAnimation.current.size = mAnimation.unselected.size;
	mAnimation.current.pos = mAnimation.unselected.pos;
	mAnimation.current.opacity = 0xAA;
	mAnimation.unselected.opacity = 0xAA;

	// Apply themedata to normal objects
	mBackground.applyTheme(theme, "grid", "gridtile_background", ALL);
	mText->applyTheme(theme, "grid", "gridtile_text", ALL);

	Eigen::Vector2f screen = Eigen::Vector2f((float)Renderer::getScreenWidth(), (float)Renderer::getScreenHeight());

	// Apply theme data to Struct: GridTile when selected:
	const ThemeData::ThemeElement* elem = theme->getElement("grid", "gridtile_selected", "image");
	if (elem) {
		if (elem->has("color")) {
			mAnimation.selected.color = elem->get<unsigned int>("color");
			//mAnimation.selected.color = addAlpha(mAnimation.selected.color, 0xFF);
			mAnimation.animateColor = true;
		}
		if (elem->has("size")) {
			mAnimation.selected.size = elem->get<Eigen::Vector2f>("size").cwiseProduct(screen);
			mAnimation.animateSize = true;
		}
	}

	// Apply theme data to Struct: GridTile when NOT selected:
	elem = theme->getElement("grid", "gridtile", "image");
	if (elem) {
		if (elem->has("color")) {
			mAnimation.unselected.color = elem->get<unsigned int>("color");
			//mAnimation.unselected.color = addAlpha(mAnimation.current.color, 0xAA);
			mAnimation.current.color = mAnimation.unselected.color;
			mImage->setColorShift(mAnimation.current.color);
		}
		if (elem->has("size")) {
			mAnimation.unselected.size = elem->get<Eigen::Vector2f>("size").cwiseProduct(screen);
			mAnimation.current.size = mAnimation.unselected.size;
			mAnimation.current.pos.x() = getPosition().x();
			mAnimation.current.pos.y() = getPosition().y();
			mAnimation.animateSizeFromDefault = false;
		}
	}

	// Text Container size (selected)
	elem = theme->getElement("grid", "gridtile_textRow_selected", "container");
	if (elem) {
		if (elem->has("size")) {
			mAnimation.selected.textContainerSize = elem->get<Eigen::Vector2f>("size");
			mAnimation.unselected.textContainerSize = mTextContainerSize;
			mAnimation.current.textContainerSize = mTextContainerSize;
			mAnimation.animateTextContainer = true;
		}
	}
	// Text Container size
	elem = theme->getElement("grid", "gridtile_textRow", "container");
	if (elem) {
		if (elem->has("size")) {
			mAnimation.unselected.textContainerSize = elem->get<Eigen::Vector2f>("size");
			mAnimation.current.textContainerSize = mAnimation.unselected.textContainerSize;
			LOG(LogError) << "Setting row height to : " << mAnimation.current.textContainerSize.y();
			mGrid.setRowHeightPerc(1, mAnimation.current.textContainerSize.y());
		}
	}

	// Apply theme data to struct: Gridtile_background_selected:
	elem = theme->getElement("grid", "gridtile_background_selected", "ninepatch");
	if (elem) {
		if (elem->has("size")) {
			mAnimation.selected.backgroundSize = elem->get<Eigen::Vector2f>("size").cwiseProduct(screen);
			mAnimation.animateBackgroundSize = true;
		}
	}

	if (mAnimation.animateColor) {
		mAnimation.current.opacity = getAlpha(mAnimation.unselected.color);
		mAnimation.unselected.opacity = mAnimation.current.opacity;
	}

	// Setup size for if themer didn't specify a size on unselected
	if (mAnimation.animateSizeFromDefault) {
		mAnimation.current.size = getSize();
		mAnimation.unselected.size = getSize();
		mAnimation.selected.size += getSize();
		mAnimation.current.pos.x() = getPosition().x();
		mAnimation.current.pos.y() = getPosition().y();
		mAnimation.current.backgroundSize = mBackground.getSize();
	}

	// BACKGROUND :: SELECTED
	elem = theme->getElement("grid", "gridtile_background_selected", "ninepatch");
	if (!elem) elem = theme->getElement("grid", "gridtile_background_selected", "image");
	if (elem) {
		if (elem->has("color")) {
			mAnimation.current.backgroundColor = elem->get<unsigned int>("color");
			mAnimation.selected.backgroundColor = mAnimation.current.backgroundColor;
			mAnimation.animateBackgroundColor = true;
		}
	}

	// BACKGROUND :: UNSELECTED
	elem = theme->getElement("grid", "gridtile_background", "ninepatch");
	if (!elem) elem = theme->getElement("grid", "gridtile_background", "image");
	if (elem) {
		if (elem->has("path")) {
			setBackgroundPath(elem->get<std::string>("path"));
			bThemeBackground = true;
		}
		if (elem->has("color")) {
			mAnimation.unselected.backgroundColor = (elem->get<unsigned int>("color"));
			mAnimation.current.backgroundColor = mAnimation.unselected.backgroundColor;
			mBackground.setEdgeColor(elem->get<unsigned int>("color"));
			mBackground.setCenterColor(elem->get<unsigned int>("color"));
		}
		if (elem->has("color-center"))
			mBackground.setCenterColor(elem->get<unsigned int>("color-center"));
		if (elem->has("color-edge"))
			mBackground.setEdgeColor(elem->get<unsigned int>("color-edge"));
		if (elem->has("padding")) {
			backgroundPadding = elem->get<Eigen::Vector2f>("padding").cwiseProduct(screen);
		}
	}

	// TEXT :: SELECTED
	elem = theme->getElement("grid", "gridtile_text_selected", "text");
	if (elem) {
		if (elem->has("color")) {
			mAnimation.current.textColor = elem->get<unsigned int>("color");
			mAnimation.selected.textColor = mAnimation.current.textColor;
			mAnimation.unselected.textColor = mAnimation.current.textColor;
			mAnimation.animateTextColor = true;
		}
	}

	elem = theme->getElement("grid", "gridtile_text", "text");
	if (elem) {
		if (elem->has("color")) {
			mAnimation.current.textColor = elem->get<unsigned int>("color");
			mAnimation.unselected.textColor = mAnimation.current.textColor;
		}
	}

}

void GridTileComponent::hide() {
	bShow = false;
}

void GridTileComponent::show() {
	bShow = true;
}

// --- SETS FOR CHILDREN ---
void GridTileComponent::setImage(std::string path, bool tile) {
	mImage->setImage(path, tile);
}

void GridTileComponent::setImage(const std::shared_ptr<TextureResource>& texture) {
	mImage->setImage(texture);
}

void GridTileComponent::setText(const std::string& text) {
	mText->setText(text);
}

void GridTileComponent::setTextColor(unsigned int color) {
	mText->setColor(color);
}

void GridTileComponent::setTextAlignment(Alignment align) {
	mText->setAlignment(align);
}

void GridTileComponent::setUppercase(bool uppercase) {
	mText->setUppercase(uppercase);
}

void GridTileComponent::setBackgroundPath(const std::string& path) {
	if (boost::filesystem::exists(path) || ResourceManager::getInstance()->fileExists(path)) {
		if (!bThemeBackground) mBackground.setImagePath(path);
	}
	else {
		mBackground.setImagePath(":/frame.png");
		return;
	}
}