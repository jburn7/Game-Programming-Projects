#include "jsonData.h"

JSONData *JSONData::msInstance = NULL;

JSONData *JSONData::getInstance()
{
	assert(msInstance != NULL);
	return msInstance;
}

void JSONData::init(const char *str)
{
	msInstance = new JSONData(str);
}

void JSONData::cleanup()
{
	if(msInstance)
	{
		delete msInstance;
		msInstance = NULL;
	}
	msInstance->mDoc.Clear();
}

void JSONData::save()
{
	rapidjson::GenericStringBuffer<rapidjson::UTF8<>> buffer;
	rapidjson::PrettyWriter<rapidjson::GenericStringBuffer<rapidjson::UTF8<>>> writer(buffer);
	mSaveDoc.Accept(writer);
	const char* str = buffer.GetString();

	std::ofstream out(mDoc["game"]["saveFile"].GetString());
	out << str;
	out.close();
}

rapidjson::Document &JSONData::getJSON()
{
	return mDoc;
}

rapidjson::Document &JSONData::getSaveDoc()
{
	return mSaveDoc;
}

rapidjson::Document &JSONData::getLoadedSaveJSON()
{
	return mSaveJSON;
}

JSONData::JSONData(const char *str)
{
	//open str
	//read all contents into a char *
	//parse *that* char pointer
	std::ifstream fin(str);
	std::string line = "";
	std::stringstream json;
	while(std::getline(fin, line))
	{
		json << line;
	}
	mDoc.Parse(json.str().c_str());
	fin.close();

	mSaveDoc.SetObject();

	std::ifstream saveData(mDoc["game"]["saveFile"].GetString());
	line = "";
	std::stringstream saveJson;
	while(std::getline(saveData, line))
	{
		saveJson << line;
	}
	mSaveJSON.Parse(saveJson.str().c_str());
	saveData.close();
}

JSONData::~JSONData()
{
}
