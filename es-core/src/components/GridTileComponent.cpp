#include "GridTileComponent.h"
#include "components/TextComponent.h"
#include "components/ImageComponent.h"
#include "Log.h"
#include "Renderer.h"

using namespace Eigen;

GridTileComponent::GridTileComponent(Window* window) : GuiComponent(window), mGrid(window, Vector2i(1, 2)), mBackground(window)
{
	mText = std::make_shared<TextComponent>(mWindow, "", Font::get(FONT_SIZE_SMALL), 0x777777FF);
	mImage = std::make_shared<ImageComponent>(mWindow);

	mGrid.setEntry(mImage, Vector2i(0, 0), false, true);
	mGrid.setEntry(mText, Vector2i(0, 1), false, true);

	addChild(&mBackground);
	addChild(&mGrid);
}

void GridTileComponent::setSelected(bool isSelected) {
	bSelected = isSelected;
}

void GridTileComponent::onSizeChanged()
{
	mGrid.setSize(mSize);
	mBackground.fitTo(mSize, Vector3f(mPosition.x(), mPosition.y(), 0));
	
	mText->setPosition(0, mSize.y() * .85f);
	mText->setSize(mSize.x(), mSize.y());


	if(mSize.x() == 0 || mSize.y() == 0)
		return;

	const float middleSpacerWidth = 0.01f * Renderer::getScreenWidth();
	const float textHeight = mText->getFont()->getLetterHeight();
	mText->setSize(0, textHeight);
	const float textWidth = mText->getSize().x() + 4;

	//mGrid.setColWidthPerc(0, mSize.x()); // animation is square
	//mGrid.setColWidthPerc(2, middleSpacerWidth / mSize.x());
	//mGrid.setColWidthPerc(3, textWidth / mSize.x());

	mGrid.setRowHeightPerc(0, .9f);
	mImage->setMaxSize(mSize.x(), mGrid.getRowHeight(0));

	if (bStretchImage) {
		mImage->setSize(mSize.x(), mGrid.getRowHeight(0));
	}
	
	//mBackground.fitTo();
}

void GridTileComponent::onPositionChanged() {
	mGrid.setPosition(mPosition);
}

void GridTileComponent::setImageSize(float w, float h, bool fit) {
	mImage->setSize(w, h);
	mImage->setMaxSize(w, h);
	setSize(w, h + mText->getFont()->getLetterHeight() );

	//onSizeChanged();
}

void GridTileComponent::setImageToFit(bool fit) {
	bStretchImage = fit;
	onSizeChanged();
}

void GridTileComponent::render(const Eigen::Affine3f& parentTrans) {
	renderChildren(parentTrans);
}

void GridTileComponent::update(int deltaTime) {
	if (bSelected) {
		if (mAnimation.frame < mAnimation.maxFrame) {
			mAnimation.current.opacity += (mAnimation.selected.opacity - mAnimation.unselected.opacity) / mAnimation.maxFrame;
			mAnimation.current.color += (mAnimation.selected.color - mAnimation.unselected.color) / mAnimation.maxFrame;
			mAnimation.current.size += (mAnimation.selected.size - mAnimation.unselected.size) / mAnimation.maxFrame;
			mAnimation.current.pos -= ((mAnimation.selected.size - mAnimation.unselected.size) / mAnimation.maxFrame) / 2;
			mAnimation.zframe = 1;
			mAnimation.frame++;
		}
	}
	else {
		if (mAnimation.frame > 0) {
			mAnimation.current.opacity -= (mAnimation.selected.opacity - mAnimation.unselected.opacity) / mAnimation.maxFrame;
			mAnimation.current.color -= (mAnimation.selected.color - mAnimation.unselected.color) / mAnimation.maxFrame;
			mAnimation.current.size -= (mAnimation.selected.size - mAnimation.unselected.size) / mAnimation.maxFrame;
			mAnimation.current.pos += ((mAnimation.selected.size - mAnimation.unselected.size) / mAnimation.maxFrame) / 2;
			mAnimation.zframe = 0;
			mAnimation.frame--;
		}
	}

	if (mAnimation.animateOpacity) setOpacity(mAnimation.current.opacity);
	if (mAnimation.animateColor) mImage->setColorShift(mAnimation.current.color);
	if (mAnimation.animateSize) {
		setSize(mAnimation.current.size);
		setPosition(mAnimation.current.pos.x(), mAnimation.current.pos.y(), mAnimation.zframe);
	}
}

void GridTileComponent::setTheme(const std::shared_ptr<ThemeData>& theme) {
	using namespace ThemeFlags;

	// Apply themedata to normal objects
	mBackground.applyTheme(theme, "grid", "gridtile_background", ALL);
	mText->applyTheme(theme, "grid", "gridtile_text", ALL);

	Eigen::Vector2f screen = Eigen::Vector2f((float)Renderer::getScreenWidth(), (float)Renderer::getScreenHeight());

	// Apply theme data to Struct: GridTile when selected:
	const ThemeData::ThemeElement* elem = theme->getElement("grid", "gridtile_selected", "image");
	if (elem) {
		if (elem->has("color")) {
			mAnimation.selected.color = elem->get<unsigned int>("color");
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
			//mAnimation.animateColor = true;
		}
		if (elem->has("size")) {
			mAnimation.unselected.size = elem->get<Eigen::Vector2f>("size").cwiseProduct(screen);
			mAnimation.current.size = mAnimation.unselected.size;
			mAnimation.current.pos.x() = getPosition().x();
			mAnimation.current.pos.y() = getPosition().y();
			mAnimation.animateSizeFromDefault = false;
		}
	}

	mAnimation.current.opacity = 0xAA;
	mAnimation.unselected.opacity = 0xAA;

	// Setup size for if themer didn't specify a size on unselected
	if (mAnimation.animateSizeFromDefault) {
		mAnimation.current.size = getSize();
		mAnimation.unselected.size = getSize();
		mAnimation.selected.size += getSize();
		mAnimation.current.pos.x() = getPosition().x();
		mAnimation.current.pos.y() = getPosition().y();
	}

	// Catch Ninepatch image path change
	elem = theme->getElement("grid", "gridtile_background", "ninepatch");
	if (elem->has("path")) {
		mBackground.setImagePath(elem->get<std::string>("path"));
		bThemeBackground = true;
	}

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
	//if (!bThemeBackground) mBackground.setImagePath(path);
}