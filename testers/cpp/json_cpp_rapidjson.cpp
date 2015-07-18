#include <rapidjson/document.h>
#include <rapidjson/reader.h>
#include <fstream>

#include "benchmark.h"
#include "util.h"

static inline rapidjson::Document parse(const char *data)
{
	rapidjson::Document doc;
	doc.Parse(data);
	return doc;
}

static inline int getLatest(const rapidjson::Document &doc)
{
	const rapidjson::Value &versions = doc["versions"];
	int value = 0;
	for (auto it = versions.Begin(); it != versions.End(); ++it)
	{
		value = std::max(value, (*it)["time"].GetInt());
	}
	return value;
}

int main(int argc, char **argv)
{
	const char *data = readFile("json.json");
	COLLECT_METRIC("json", "rapidjson", "parse_time", 1000)
	{
		benchmark::doNotOptimizeAway(parse(data));
	}
	const rapidjson::Document doc = parse(data);
	COLLECT_METRIC("json", "rapidjson", "access_time", 10000)
	{
		benchmark::doNotOptimizeAway(getLatest(doc));
	}

	return 0;
}
