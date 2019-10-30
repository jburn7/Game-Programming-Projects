#pragma once
#include "document.h"
#include "rapidjson.h"
#include "prettywriter.h"
#include <fstream>
#include <string>
#include <sstream>

//the data should be pretty small, so one class holding all game data should be manageable
class JSONData
{
public:
	static JSONData *getInstance();
	static void init(const char *str);
	static void cleanup();

	void save();

	rapidjson::Document &getJSON();
	//returns a blank save doc which will write
	rapidjson::Document &getSaveDoc();
	//returns the json from the save file
	rapidjson::Document &getLoadedSaveJSON();
private:
	JSONData(const char *str);
	~JSONData();

	static JSONData *msInstance;

	rapidjson::Document mDoc;
    rapidjson::Document mSaveDoc; //remains empty until the game saves
	rapidjson::Document mSaveJSON; //loads up all json from the save file
};