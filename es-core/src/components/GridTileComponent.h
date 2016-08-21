#include "GuiComponent.h"
#include "components/ComponentGrid.h"
#include "components/NinePatchComponent.h"
#include "components/TextComponent.h"
#include "components/ImageComponent.h"

class TextComponent;
class ImageComponent;

struct CursorAnimation {
	struct Selected {
		Eigen::Vector2f size;
		unsigned char opacity = 0xFF;
		unsigned int color = 0xFFFFFFFF;
	}; Selected selected;
	
	struct Unselected {
		Eigen::Vector2f size;
		unsigned char opacity = 0xAA;
		unsigned int color = 0xEEEEEEAA;
	}; Unselected unselected;

	struct Current {
		Eigen::Vector2f size;
		unsigned char opacity = 0xAA;
		unsigned int color = 0xEEEEEEAA;
	}; Current current;

	bool animateColor = false;
	bool animateOpacity = true;
	bool animateSize = true;

	int maxFrame = 8;
	int frame = 0;
};

class GridTileComponent : public GuiComponent
{
public:
	GridTileComponent(Window* window);

	void render(const Eigen::Affine3f& parentTrans) override;
	void update(int deltaTime) override;
	void onPositionChanged() override;

	void setSelected(bool isSelected);

	// Used to give custom theme control to all elements and for slight animation puroposes.
	void setTheme(const std::shared_ptr<ThemeData>& theme);

	void setImage(const std::shared_ptr<TextureResource>& texture);
	void setImage(std::string path, bool tile = false);
	void setImageSize(float w, float h, bool fit = false);
	void setImageToFit(bool fit);

	void setText(const std::string& text);
	void setTextColor(unsigned int color);
	void setTextAlignment(Alignment align);
	void setUppercase(bool uppercase);

	void setBackgroundPath(const std::string& path);

	void onSizeChanged() override;

private:
	std::shared_ptr<TextComponent> mText;
	std::shared_ptr<ImageComponent> mImage;
	NinePatchComponent mBackground;
	ComponentGrid mGrid;

	bool bThemeBackground = false;
	bool bStretchImage = false;

	CursorAnimation mAnimation;
	bool bSelected = false;

};
