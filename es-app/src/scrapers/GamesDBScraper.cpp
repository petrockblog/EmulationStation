#include "scrapers/GamesDBScraper.h"
#include "Log.h"
#include "pugixml/pugixml.hpp"
#include "MetaData.h"
#include "Settings.h"
#include "Util.h"


void thegamesdb_generate_scraper_requests(const ScraperSearchParams& params, std::queue< std::unique_ptr<ScraperRequest> >& requests, 
	std::vector<ScraperSearchResult>& results)
{
	std::string path = "thegamesdb.net/api/GetGame.php?";

	std::string cleanName = params.nameOverride;
	if(cleanName.empty())
		cleanName = params.game->getCleanName();

	path += "name=" + HttpReq::urlEncode(cleanName);

	if(params.system->getPlatformIds().empty())
	{
		// no platform specified, we're done
		requests.push(std::unique_ptr<ScraperRequest>(new TheGamesDBRequest(results, path)));
	}else{
		// go through the list, we need to split this into multiple requests 
		// because TheGamesDB API either sucks or I don't know how to use it properly...
		std::string urlBase = path;
		auto& platforms = params.system->getPlatformIds();
		for(auto platformIt = platforms.begin(); platformIt != platforms.end(); platformIt++)
		{
			path = urlBase;
			auto mapIt = gamesdb_platformid_map.find(*platformIt);
			if(mapIt != gamesdb_platformid_map.end())
			{
				path += "&platform=";
				path += HttpReq::urlEncode(mapIt->second);
			}else{
				LOG(LogWarning) << "TheGamesDB scraper warning - no support for platform " << getPlatformName(*platformIt);
			}

			requests.push(std::unique_ptr<ScraperRequest>(new TheGamesDBRequest(results, path)));
		}
	}
}

void TheGamesDBRequest::process(const std::unique_ptr<HttpReq>& req, std::vector<ScraperSearchResult>& results)
{
	assert(req->status() == HttpReq::REQ_SUCCESS);

	pugi::xml_document doc;
	pugi::xml_parse_result parseResult = doc.load(req->getContent().c_str());
	if(!parseResult)
	{
		std::stringstream ss;
		ss << "GamesDBRequest - Error parsing XML. \n\t" << parseResult.description() << "";
		std::string err = ss.str();
		setError(err);
		LOG(LogError) << err;
		return;
	}

	pugi::xml_node data = doc.child("Data");

	std::string baseImageUrl = data.child("baseImgUrl").text().get();

	pugi::xml_node game = data.child("Game");
	while(game && results.size() < MAX_SCRAPER_RESULTS)
	{
		ScraperSearchResult result;

		result.mdl.set("name", game.child("GameTitle").text().get());
		result.mdl.set("desc", game.child("Overview").text().get());

		boost::posix_time::ptime rd = string_to_ptime(game.child("ReleaseDate").text().get(), "%m/%d/%Y");
		result.mdl.setTime("releasedate", rd);

		result.mdl.set("developer", game.child("Developer").text().get());
		result.mdl.set("publisher", game.child("Publisher").text().get());
		result.mdl.set("genre", game.child("Genres").first_child().text().get());
		result.mdl.set("players", game.child("Players").text().get());

		if(Settings::getInstance()->getBool("ScrapeRatings") && game.child("Rating"))
		{
			float ratingVal = (game.child("Rating").text().as_int() / 10.0f);
			std::stringstream ss;
			ss << ratingVal;
			result.mdl.set("rating", ss.str());
		}

		pugi::xml_node images = game.child("Images");

		if(images)
		{
			pugi::xml_node art = images.find_child_by_attribute("boxart", "side", "front");

			if(art)
			{
				result.thumbnailUrl = baseImageUrl + art.attribute("thumb").as_string();
				result.imageUrl = baseImageUrl + art.text().get();
			}
		}

		results.push_back(result);
		game = game.next_sibling("Game");
	}
}
