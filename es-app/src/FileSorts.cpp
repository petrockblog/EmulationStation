#include "FileSorts.h"

namespace FileSorts
{
	const FileData::SortType typesArr[] = {
		FileData::SortType(&compareFileName, true, "filename, ascending"),
		FileData::SortType(&compareFileName, false, "filename, descending"),

		FileData::SortType(&compareRating, true, "rating, ascending"),
		FileData::SortType(&compareRating, false, "rating, descending"),

		FileData::SortType(&compareTimesPlayed, true, "times played, ascending"),
		FileData::SortType(&compareTimesPlayed, false, "times played, descending"),

		FileData::SortType(&compareLastPlayed, true, "last played, ascending"),
		FileData::SortType(&compareLastPlayed, false, "last played, descending"),

		FileData::SortType(&compareNumPlayers, true, "number players, ascending"),
		FileData::SortType(&compareNumPlayers, false, "number players, descending"),

		FileData::SortType(&compareReleaseDate, true, "release date, ascending"),
		FileData::SortType(&compareReleaseDate, false, "release date, descending"),

		FileData::SortType(&compareGenre, true, "genre, ascending"),
		FileData::SortType(&compareGenre, false, "genre, descending"),

		FileData::SortType(&compareDeveloper, true, "developer, ascending"),
		FileData::SortType(&compareDeveloper, false, "developer, descending"),

		FileData::SortType(&comparePublisher, true, "publisher, ascending"),
		FileData::SortType(&comparePublisher, false, "publisher, descending")
	};

	const std::vector<FileData::SortType> SortTypes(typesArr, typesArr + sizeof(typesArr)/sizeof(typesArr[0]));

	//returns if file1 should come before file2
	bool compareFileName(const FileData* file1, const FileData* file2)
	{
		std::string name1 = file1->getName();
		std::string name2 = file2->getName();

		//min of name1/name2 .length()s
		unsigned int count = name1.length() > name2.length() ? name2.length() : name1.length();
		for(unsigned int i = 0; i < count; i++)
		{
			if(toupper(name1[i]) != toupper(name2[i]))
			{
				return toupper(name1[i]) < toupper(name2[i]);
			}
		}

		return name1.length() < name2.length();
	}

	bool compareRating(const FileData* file1, const FileData* file2)
	{
		//only games have rating metadata
		if(file1->metadata.getType() == GAME_METADATA && file2->metadata.getType() == GAME_METADATA)
		{
			return file1->metadata.getFloat("rating") < file2->metadata.getFloat("rating");
		}

		return false;
	}

	bool compareTimesPlayed(const FileData* file1, const FileData* file2)
	{
		//only games have playcount metadata
		if(file1->metadata.getType() == GAME_METADATA && file2->metadata.getType() == GAME_METADATA)
		{
			return (file1)->metadata.getInt("playcount") < (file2)->metadata.getInt("playcount");
		}

		return false;
	}

	bool compareLastPlayed(const FileData* file1, const FileData* file2)
	{
		//only games have lastplayed metadata
		if(file1->metadata.getType() == GAME_METADATA && file2->metadata.getType() == GAME_METADATA)
		{
			return (file1)->metadata.getTime("lastplayed") < (file2)->metadata.getTime("lastplayed");
		}

		return false;
	}

	bool compareNumPlayers(const FileData* file1, const FileData* file2)
	{
		//only games have players metadata
		if (file1->metadata.getType() == GAME_METADATA && file2->metadata.getType() == GAME_METADATA)
		{
			return (file1)->metadata.getInt("players") < (file2)->metadata.getInt("players");
		}

		return false;
	}

	bool compareReleaseDate(const FileData* file1, const FileData* file2)
	{
		//only games have releasedate metadata
		if (file1->metadata.getType() == GAME_METADATA && file2->metadata.getType() == GAME_METADATA)
		{
			return (file1)->metadata.getTime("releasedate") < (file2)->metadata.getTime("releasedate");
		}

		return false;
	}

	bool compareGenre(const FileData* file1, const FileData* file2)
	{
		std::string genre1 = file1->metadata.get("genre");
		std::string genre2 = file2->metadata.get("genre");

		//min of genre1/genre2 .length()s
		unsigned int count = genre1.length() > genre2.length() ? genre2.length() : genre1.length();
		for (unsigned int i = 0; i < count; i++)
		{
			if (toupper(genre1[i]) != toupper(genre2[i]))
			{
				return toupper(genre1[i]) < toupper(genre2[i]);
			}
		}

		return genre1.length() < genre2.length();
	}

	bool compareDeveloper(const FileData* file1, const FileData* file2)
	{
		std::string developer1 = file1->metadata.get("developer");
		std::string developer2 = file2->metadata.get("developer");

		//min of developer1/developer2 .length()s
		unsigned int count = developer1.length() > developer2.length() ? developer2.length() : developer1.length();
		for (unsigned int i = 0; i < count; i++)
		{
			if (toupper(developer1[i]) != toupper(developer2[i]))
			{
				return toupper(developer1[i]) < toupper(developer2[i]);
			}
		}

		return developer1.length() < developer2.length();
	}

	bool comparePublisher(const FileData* file1, const FileData* file2)
	{
		std::string publisher1 = file1->metadata.get("publisher");
		std::string publisher2 = file2->metadata.get("publisher");

		//min of publisher1/publisher2 .length()s
		unsigned int count = publisher1.length() > publisher2.length() ? publisher2.length() : publisher1.length();
		for (unsigned int i = 0; i < count; i++)
		{
			if (toupper(publisher1[i]) != toupper(publisher2[i]))
			{
				return toupper(publisher1[i]) < toupper(publisher2[i]);
			}
		}

		return publisher1.length() < publisher2.length();
	}
};
