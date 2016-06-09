#include "scrapers/SselphScraper.h"
#include "scrapers/GamesDBScraper.h"
#include "HttpReq.h"
#include "Log.h"
#include "pugixml/pugixml.hpp"
#include "Settings.h"
#include "Util.h"


void sselph_generate_scraper_requests(const ScraperSearchParams& params, std::queue< std::unique_ptr<ScraperRequest> >& requests,
	std::vector<ScraperSearchResult>& results)
{
	std::string path = "http://localhost:55129/getgame?";

	path += "path=" + HttpReq::urlEncode(params.game->getPath().string());
	requests.push(std::unique_ptr<ScraperRequest>(new SselphRequest(results, path)));
}

void sselph_generate_fallback_scraper_requests(const ScraperSearchParams& params, std::queue< std::unique_ptr<ScraperRequest> >& requests,
	std::vector<ScraperSearchResult>& results)
{
	std::string path = "http://localhost:55129/getgame?";

	path += "path=" + HttpReq::urlEncode(params.game->getPath().string());
	
	std::string cleanName = params.nameOverride;
	if(cleanName.empty())
		cleanName = params.game->getCleanName();

	path += "&name=" + HttpReq::urlEncode(cleanName);

	if(params.system->getPlatformIds().empty())
	{
		requests.push(std::unique_ptr<ScraperRequest>(new SselphRequest(results, path)));
	}else{
		auto& platforms = params.system->getPlatformIds();
		for(auto platformIt = platforms.begin(); platformIt != platforms.end(); platformIt++)
		{
			auto mapIt = gamesdb_platformid_map.find(*platformIt);
			if(mapIt != gamesdb_platformid_map.end())
			{
				path += "&platform=";
				path += HttpReq::urlEncode(mapIt->second);
			}else{
				LOG(LogWarning) << "TheGamesDB scraper warning - no support for platform " << getPlatformName(*platformIt);
			}
				
		}
	}
	requests.push(std::unique_ptr<ScraperRequest>(new SselphRequest(results, path)));
}

void SselphRequest::process(const std::unique_ptr<HttpReq>& req, std::vector<ScraperSearchResult>& results)
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

	pugi::xml_node game = doc.child("game");
	while(game && results.size() < MAX_SCRAPER_RESULTS)
	{
		ScraperSearchResult result;
		result.crcMatched = game.child("CRCMatch").text().as_bool();
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
	
		std::string image = game.child("Image").text().get();
		if(!image.empty())
		{
			result.thumbnailUrl = image;
			result.imageUrl = image;
		}
		results.push_back(result);
		game = game.next_sibling("game");
	}
}
