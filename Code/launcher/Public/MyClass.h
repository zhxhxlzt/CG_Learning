#pragma once

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

inline string BuildJsonFile()
{
	using namespace rapidjson;

	StringBuffer sb;
	PrettyWriter<StringBuffer> writer(sb);


	writer.StartObject();
	writer.String("name");
	writer.String("lyq");
	writer.String("age");
	writer.Int(26);
	writer.String("Info");
	writer.StartObject();
	writer.String("speed");
	writer.Double(20.2);
	writer.String("acc");
	writer.Double(1.2);
	writer.String("Att");
	writer.String("happy");
	writer.EndObject();
	writer.EndObject();
	
	string filename = "Test.json";
	ofstream fp(filename);
	fp << sb.GetString();
	fp.close();
	return filename;
}

inline string ReadFile(string filename)
{
	fstream fp(filename);
	string json = (stringstream() << fp.rdbuf()).str();
	fp.close();
	return json;
}

inline void ReadJsonFile(string filename)
{
	auto json = ReadFile(filename);
	using namespace rapidjson;
	Document doc;
	doc.Parse(json.data());
	printf("name: %s\n", doc["name"].GetString());
	printf("age: %d\n", doc["age"].GetInt());
	printf("Info:\n");
	auto& info = doc["Info"].GetObject();
	printf("\tspeed: %.2f\n", info["speed"].GetDouble());
	printf("\tacc: %.2f\n", info["acc"].GetDouble());
	printf("\tatt: %s\n", info["Att"].GetString());
}
inline void TestRapidJson()
{
	auto filename = BuildJsonFile();
	ReadJsonFile(filename);
}

#include "MetaObject.h"

class MyObj {
	META_OBJECT(MyObj, None);

public:
	META_FUNC(Hello, int, int, g)
	{
		return g + 1;
	}
	
};