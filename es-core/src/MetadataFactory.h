#ifndef EMULATIONSTATION_ALL_METADATAFACTORY_H
#define EMULATIONSTATION_ALL_METADATAFACTORY_H

#include <iostream>
#include "components/GameListExtra.h"
#include "components/GameListExtraRating.h"
#include "components/RatingComponent.h"
#include "components/DateTimeComponent.h"
#include "components/IGameListExtra.h"

class MetadataFactory
{
public:
	static IGameListExtra* create(Window* window, std::string const &themingName)
	{
		// Remove "md_" TODO care out_of_range exception
		std::string name = themingName.substr(3, themingName.length() - 3);

		// TODO check folder first then game
		std::vector<MetaDataDecl> gameMetadata = getMDDByType(MetaDataListType::GAME_METADATA);
		for (auto it = gameMetadata.begin(); it != gameMetadata.end(); it++)
		{
			if ((*it).key == name)
				return create2(window, themingName, (*it), true);
		}

		std::cout << themingName << " : no match !" << std::endl;
		return NULL;
	}

	static IGameListExtra* create2(Window* window, std::string const &themingName, MetaDataDecl metadataDecl, bool isGame)
	{
		switch (metadataDecl.type)
		{
			case MD_RATING:
				return new GameListExtraRating(window, themingName, metadataDecl, isGame);
			case MD_DATE:
			case MD_TIME: // TODO DisplayMod = DISP_RELATIVE_TO_NOW
				return new GameListExtra<DateTimeComponent>(window, themingName, metadataDecl, isGame);
			case MD_STRING:
			case MD_INT:
			case MD_FLOAT:
			case MD_BOOL:
			case MD_MULTILINE_STRING:
			case MD_PATH:
			default:
				return new GameListExtra<TextComponent>(window, themingName, metadataDecl, isGame);
		}
	}
};

#endif //EMULATIONSTATION_ALL_METADATAFACTORY_H
