#include "GuiComponent.h"
#include "components/ComponentGrid.h"
#include "components/NinePatchComponent.h"
#include "components/TextComponent.h"
#include "components/ImageComponent.h"

class TextComponent;
class ImageComponent;

struct KeyframeVariables {
	Eigen::Vector2f size;
	Eigen::Vector2f pos;
	unsigned char opacity = 0xFF;
	unsigned int color = 0xFFFFFFFF;
};

struct CursorAnimation {
	KeyframeVariables selected;
	KeyframeVariables unselected;
	KeyframeVariables current;

	bool animateColor = false;
	bool animateOpacity = true;
	bool animateSize = false;				// Animates a size change when selected ( from theme->(gridtile_selected:size) )
	bool animateSizeFromDefault = true;		// Animates the size from the default squaresize ( Theme->(gridtile:size) will override )

	int zframe = 0;

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
