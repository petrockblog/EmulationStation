#pragma once

#include "GuiComponent.h"
#include "views/gamelist/IGameListView.h"

#include "components/NinePatchComponent.h"
#include "components/TextComponent.h"
#include "Localization.h"

class GuiFastSelect : public GuiComponent
{
public:
	GuiFastSelect(Window* window, IGameListView* gamelist);

	bool input(InputConfig* config, Input input);
	void update(int deltaTime);

//	static const std::string LETTERS = ;
	static std::string getLetters() { return _("ABCDEFGHIJKLMNOPQRSTUVWXYZ");};

private:
	void setScrollDir(int dir);
	void scroll();
	void updateGameListCursor();
	void updateGameListSort();
	void updateSortText();

	int mSortId;
	int mLetterId;

	int mScrollDir;
	int mScrollAccumulator;

	NinePatchComponent mBackground;
	TextComponent mSortText;
	TextComponent mLetterText;

	IGameListView* mGameList;
};
