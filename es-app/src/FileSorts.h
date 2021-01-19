#pragma once
#ifndef ES_APP_FILE_SORTS_H
#define ES_APP_FILE_SORTS_H

#include "FileData.h"
#include <vector>

namespace FileSorts
{
	
	enum SortType {
		NAME_ASC,
		NAME_DESC,
		NAME_IGNORE_ARTICLES_ASC,
		NAME_IGNORE_ARTICLES_DESC,
		RATING_ASC,
		RATING_DESC,
		TIMES_PLAYED_ASC,
		TIMES_PLAYED_DESC,
		LAST_PLAYED_ASC,
		LAST_PLAYED_DESC,
		NUM_PLAYERS_ASC,
		NUM_PLAYERS_DESC,
		RELEASE_DATE_ASC,
		RELEASE_DATE_DESC,
		GENRE_ASC,
		GENRE_DESC,
		DEVELOPER_ASC,
		DEVELOPER_DESC,
		PUBLISHER_ASC,
		PUBLISHER_DESC,
		SYSTEM_ASC,
		SYSTEM_DESC
	};

	bool compareName(const FileData* file1, const FileData* file2);
	bool compareNameIgnoreArticles(const FileData* file1, const FileData* file2);
	bool compareRating(const FileData* file1, const FileData* file2);
	bool compareTimesPlayed(const FileData* file1, const FileData* file2);
	bool compareLastPlayed(const FileData* file1, const FileData* file2);
	bool compareNumPlayers(const FileData* file1, const FileData* file2);
	bool compareReleaseDate(const FileData* file1, const FileData* file2);
	bool compareGenre(const FileData* file1, const FileData* file2);
	bool compareDeveloper(const FileData* file1, const FileData* file2);
	bool comparePublisher(const FileData* file1, const FileData* file2);
	bool compareSystem(const FileData* file1, const FileData* file2);

	extern const std::vector<FileData::SortType> SortTypes;
};

#endif // ES_APP_FILE_SORTS_H
