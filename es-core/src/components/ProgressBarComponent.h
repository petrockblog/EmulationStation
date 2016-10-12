#pragma once

#include "components/ImageComponent.h"
#include "GuiComponent.h"
#include "components/NinePatchComponent.h"
#include "Util.h"
#include <string>



class ProgressBarComponent : public GuiComponent
{
public:
	ProgressBarComponent(Window* window, std::string name = "pbar");

	void setValue(int val);
	int getValue() { return mValue; }

	void onSizeChanged() override;

	void setColor(unsigned int color, unsigned int backcolor = 0xCCCCCCFF);

	// Overrides
	void update(int deltatime) override;
	void render(const Eigen::Affine3f& parentTrans) override;
	

	void setUpdateFrame(int updateframe) { mFrameCount = 0; mUpdateFrame = updateframe; }


private:
	void setupBar();

	bool mEnable = true;		// Controled from UI SETTINGS (Show Network)
	bool mShowName = true;
	std::string mName;			// Name of progress bar.

	int mValue = 0;
	int mIncValue = 0;			// The value that adds to the mValue (for animation purpose.)

	// Keep a frame count to update on specific key frames
	int mFrameCount = 0;
	int mUpdateFrame = 60;		// Which frame to update on.

	// progressbar filler
	NinePatchComponent mFill;
	NinePatchComponent mBar;

	bool bRenderFill = false;




};
