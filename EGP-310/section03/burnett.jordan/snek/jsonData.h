#pragma once
#include "document.h"
#include "rapidjson.h"
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

	rapidjson::Document &getJSON();
private:
	JSONData(const char *str);
	~JSONData();

	static JSONData *msInstance;

	rapidjson::Document mDoc;
};