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
	Eigen::Vector2f backgroundSize;
	Eigen::Vector2f textContainerSize;
	unsigned char opacity = 0xFF;
	unsigned int color = 0xFFFFFFFF;
	unsigned int backgroundColor = 0xFFFFFFFF;
	unsigned int textColor = 0xFFFFFFFF;
};

struct CursorAnimation {
	KeyframeVariables selected;
	KeyframeVariables unselected;
	KeyframeVariables current;

	bool animateColor = false;
	bool animateOpacity = true;
	bool animateSize = false;				// Animates a size change when selected ( from theme->(gridtile_selected:size) )
	bool animateSizeFromDefault = true;		// Animates the size from the default squaresize ( Theme->(gridtile:size) will override )
	bool animateBackgroundSize = false;
	bool animateTextContainer = false;
	bool animateBackgroundColor = false;
	bool animateTextColor = false;

	int zframe = 0;

	int maxFrame = 8;
	int frame = 0;
};

class GridTileComponent : public GuiComponent
{
public:
	GridTileComponent(Window* window, int index);

	void render(const Eigen::Affine3f& parentTrans) override;
	void update(int deltaTime) override;
	void onPositionChanged() override;

	void setSelected(bool isSelected);
	void hide();
	void show();

	int getIndex() { return mIndex; }
	bool isSelected() { return bSelected; }

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
	int mIndex = 0;

	std::shared_ptr<TextComponent> mText;
	std::shared_ptr<ImageComponent> mImage;
	NinePatchComponent mBackground;
	ComponentGrid mGrid;

	Eigen::Vector2f backgroundPadding;
	bool bThemeBackground = false;
	bool bStretchImage = false;
	bool bShow = true;

	Eigen::Vector2f mTextContainerSize;

	CursorAnimation mAnimation;
	bool bSelected = false;

};
