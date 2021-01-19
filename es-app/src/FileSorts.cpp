#include "FileSorts.h"

#include "utils/StringUtil.h"

namespace FileSorts
{

	const FileData::SortType typesArr[] = {
		FileData::SortType(NAME_ASC, &compareName, true, "filename, ascending"),
		FileData::SortType(NAME_DESC, &compareName, false, "filename, descending"),

		FileData::SortType(NAME_IGNORE_ARTICLES_ASC, &compareNameIgnoreArticles, true, "filename (ignore articles), ascending"),
		FileData::SortType(NAME_IGNORE_ARTICLES_DESC, &compareNameIgnoreArticles, false, "filename (ignore articles), descending"),

		FileData::SortType(RATING_ASC, &compareRating, true, "rating, ascending"),
		FileData::SortType(RATING_DESC, &compareRating, false, "rating, descending"),

		FileData::SortType(TIMES_PLAYED_ASC, &compareTimesPlayed, true, "times played, ascending"),
		FileData::SortType(TIMES_PLAYED_DESC, &compareTimesPlayed, false, "times played, descending"),

		FileData::SortType(LAST_PLAYED_ASC, &compareLastPlayed, true, "last played, ascending"),
		FileData::SortType(LAST_PLAYED_DESC, &compareLastPlayed, false, "last played, descending"),

		FileData::SortType(NUM_PLAYERS_ASC, &compareNumPlayers, true, "number players, ascending"),
		FileData::SortType(NUM_PLAYERS_DESC, &compareNumPlayers, false, "number players, descending"),

		FileData::SortType(RELEASE_DATE_ASC, &compareReleaseDate, true, "release date, ascending"),
		FileData::SortType(RELEASE_DATE_DESC, &compareReleaseDate, false, "release date, descending"),

		FileData::SortType(GENRE_ASC, &compareGenre, true, "genre, ascending"),
		FileData::SortType(GENRE_DESC, &compareGenre, false, "genre, descending"),

		FileData::SortType(DEVELOPER_ASC, &compareDeveloper, true, "developer, ascending"),
		FileData::SortType(DEVELOPER_DESC, &compareDeveloper, false, "developer, descending"),

		FileData::SortType(PUBLISHER_ASC, &comparePublisher, true, "publisher, ascending"),
		FileData::SortType(PUBLISHER_DESC, &comparePublisher, false, "publisher, descending"),

		FileData::SortType(SYSTEM_ASC, &compareSystem, true, "system, ascending"),
		FileData::SortType(SYSTEM_DESC, &compareSystem, false, "system, descending")
	};

	const std::vector<FileData::SortType> SortTypes(typesArr, typesArr + sizeof(typesArr)/sizeof(typesArr[0]));

	//returns if file1 should come before file2
	bool compareName(const FileData* file1, const FileData* file2)
	{
		// we compare the actual metadata name, as collection files have the system appended which messes up the order
		std::string name1 = Utils::String::toUpper(file1->metadata.get("sortname"));
		std::string name2 = Utils::String::toUpper(file2->metadata.get("sortname"));
		if(name1.empty()){
			name1 = Utils::String::toUpper(file1->metadata.get("name"));
		}
		if(name2.empty()){
			name2 = Utils::String::toUpper(file2->metadata.get("name"));
		}
		return name1.compare(name2) < 0;
	}

	//returns if file1 should come before file2, ignores articles ("the", "a") while sorting
	bool compareNameIgnoreArticles(const FileData* file1, const FileData* file2)
	{
		// we compare the actual metadata name, as collection files have the system appended which messes up the order
		std::string name1 = Utils::String::toUpper(file1->metadata.get("sortname"));
		std::string name2 = Utils::String::toUpper(file2->metadata.get("sortname"));
		if(name1.empty()){
			name1 = Utils::String::toUpper(file1->metadata.get("name"));
		}
		if(name2.empty()){
			name2 = Utils::String::toUpper(file2->metadata.get("name"));
		}

		//Filter out any common leading English articles
		//(This can probably be optimized via a loop and external lookup of some kind)
		if (Utils::String::startsWith(name1, "A ")) {
			name1 = Utils::String::replace(name1, "A ", "");
		}
		else if (Utils::String::startsWith(name1, "AN ")) {
			name1 = Utils::String::replace(name1, "AN ", "");
		}
		else if (Utils::String::startsWith(name1, "THE ")) {
			name1 = Utils::String::replace(name1, "THE ", "");
		}

		if (Utils::String::startsWith(name2, "A ")) {
			name2 = Utils::String::replace(name2, "A ", "");
		}
		else if (Utils::String::startsWith(name2, "AN ")) {
			name2 = Utils::String::replace(name2, "AN ", "");
		}
		else if (Utils::String::startsWith(name2, "THE ")) {
			name2 = Utils::String::replace(name2, "THE ", "");
		}

		return name1.compare(name2) < 0;
	}

	bool compareRating(const FileData* file1, const FileData* file2)
	{
		return file1->metadata.getFloat("rating") < file2->metadata.getFloat("rating");
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
		// since it's stored as an ISO string (YYYYMMDDTHHMMSS), we can compare as a string
		// as it's a lot faster than the time casts and then time comparisons
		return (file1)->metadata.get("lastplayed") < (file2)->metadata.get("lastplayed");
	}

	bool compareNumPlayers(const FileData* file1, const FileData* file2)
	{
		return (file1)->metadata.getInt("players") < (file2)->metadata.getInt("players");
	}

	bool compareReleaseDate(const FileData* file1, const FileData* file2)
	{
		// since it's stored as an ISO string (YYYYMMDDTHHMMSS), we can compare as a string
		// as it's a lot faster than the time casts and then time comparisons
		return (file1)->metadata.get("releasedate") < (file2)->metadata.get("releasedate");
	}

	bool compareGenre(const FileData* file1, const FileData* file2)
	{
		std::string genre1 = Utils::String::toUpper(file1->metadata.get("genre"));
		std::string genre2 = Utils::String::toUpper(file2->metadata.get("genre"));
		return genre1.compare(genre2) < 0;
	}

	bool compareDeveloper(const FileData* file1, const FileData* file2)
	{
		std::string developer1 = Utils::String::toUpper(file1->metadata.get("developer"));
		std::string developer2 = Utils::String::toUpper(file2->metadata.get("developer"));
		return developer1.compare(developer2) < 0;
	}

	bool comparePublisher(const FileData* file1, const FileData* file2)
	{
		std::string publisher1 = Utils::String::toUpper(file1->metadata.get("publisher"));
		std::string publisher2 = Utils::String::toUpper(file2->metadata.get("publisher"));
		return publisher1.compare(publisher2) < 0;
	}

	bool compareSystem(const FileData* file1, const FileData* file2)
	{
		std::string system1 = Utils::String::toUpper(file1->getSystemName());
		std::string system2 = Utils::String::toUpper(file2->getSystemName());
		return system1.compare(system2) < 0;
	}
};
