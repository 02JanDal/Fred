#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "benchmark.h"

static inline int getLatest(const QJsonDocument &doc)
{
	const QJsonArray versions = doc.object().value("versions").toArray();
	int time = 0;
	for (const QJsonValue &value : versions)
	{
		time = std::max(time, value.toObject().value("time").toInt());
	}
	return time;
}

int main(int argc, char **argv)
{
	QFile file("json.json");
	file.open(QFile::ReadOnly);
	const QByteArray data = file.readAll();
	COLLECT_METRIC("json", "qt", "parse_time", 1000)
	{
		benchmark::doNotOptimizeAway(QJsonDocument::fromJson(data));
	}
	const QJsonDocument parsed = QJsonDocument::fromJson(data);
	COLLECT_METRIC("json", "qt", "access_time", 10000)
	{
		benchmark::doNotOptimizeAway(getLatest(parsed));
	}

	return 0;
}
