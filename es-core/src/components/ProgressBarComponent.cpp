#include "components/ProgressBarComponent.h"
#include "Window.h"
#include "Renderer.h"
#include "Settings.h"
#include "Log.h"
#include "resources/Font.h"

// MARGIN CONST PERCENTAGES
#define MARGIN_X -0.1f
#define MARGIN_Y -0.2f

// Constructor
ProgressBarComponent::ProgressBarComponent(Window* window, std::string name) : GuiComponent(window),
	mBar(window, ":/frame.png"), mFill(window, ":/frame.png") {

	setSize(360, Font::get(FONT_SIZE_MEDIUM)->getLetterHeight() - 6);
	mFill.setCenterColor(0xFF4444CC);
	mFill.setEdgeColor(0xFF4444CC);
	mBar.setCenterColor(0xCCCCCCFF);
	mBar.setEdgeColor(0xCCCCCCFF);

	setupBar();

	addChild(&mBar);
	addChild(&mFill);
}


void ProgressBarComponent::setupBar() {
	float sizeX = mSize.x();
	float sizeY = mSize.y();
	float posX = mPosition.x();
	float posY = mPosition.y();

	// Setup margin percents
	float mX = MARGIN_X * sizeX;
	float mY = MARGIN_Y * sizeY;

	// Stretch bar to desired fit.
	mBar.fitTo(Eigen::Vector2f(sizeX, sizeY), Eigen::Vector3f(posX, posY, 1), Eigen::Vector2f(mX, mY));
}

void ProgressBarComponent::onSizeChanged() {
	setupBar();
	setValue(mValue);
}


void ProgressBarComponent::setValue(int val) {
	float mX = MARGIN_X * mSize.x();
	float mY = (MARGIN_Y - .05f) * mSize.y();
	
	// Try and correct X Padding to make up for percentage fit
	//mX *= (val * .015f);

	// fit value in percentage to the bar.  (attempts to do it inside the bar and based on screen resolution)
	float size = mSize.x() - 85;
	mFill.fitTo(Eigen::Vector2f((size * (val *.01f)) + (size * .09f), mSize.y()), Eigen::Vector3f(mPosition.x(), mPosition.y(), 1), 
		Eigen::Vector2f(mX, mY));
	mValue = val;

	if (val < 1) bRenderFill = false;
	else bRenderFill = true;

    setupBar();
}

void ProgressBarComponent::setColor(unsigned int color, unsigned int backcolor) {
	mFill.setCenterColor(color);
	mFill.setEdgeColor(color);
	mBar.setCenterColor(backcolor);
	mBar.setEdgeColor(backcolor);
}


// ~ Overrides ~
/// UPDATE
void ProgressBarComponent::update(int deltatime) {

}

/// RENDER
void ProgressBarComponent::render(const Eigen::Affine3f& parentTrans) {
	Eigen::Affine3f trans = parentTrans * getTransform();

	mBar.render(trans);
	if (bRenderFill) mFill.render(trans);
}
