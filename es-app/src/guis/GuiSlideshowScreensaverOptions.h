#ifndef _GUI_SLIDESHOW_SCREENSAVER_OPTIONS_H_
#define _GUI_SLIDESHOW_SCREENSAVER_OPTIONS_H_

#include "components/MenuComponent.h"
#include "GuiScreensaverOptions.h"

class GuiSlideshowScreensaverOptions : public GuiScreensaverOptions
{
public:
	GuiSlideshowScreensaverOptions(Window* window, const char* title);
	virtual ~GuiSlideshowScreensaverOptions();
};

#endif // _GUI_SLIDESHOW_SCREENSAVER_OPTIONS_H_
