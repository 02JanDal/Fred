#include "protobuf.pb.h"
#include "benchmark.h"
#include "util.h"

int main(int argc, char **argv)
{
	const char *data = readFile("protobuf.protobuf");
	const size_t size = fileSize("protobuf.protobuf");

	COLLECT_METRIC("protobuf", "", "parse_time", 1000)
	{
		benchmark::doNotOptimizeAway(WonkoFile().ParseFromArray(data, size));
	}

	WonkoFile file;
	file.ParseFromArray(data, size);

	COLLECT_METRIC("protobuf", "", "access_time", 1000)
	{
		uint32_t value = 0;
		for (const WonkoVersion &version : file.versions())
		{
			value = std::max(value, version.time());
		}
		benchmark::doNotOptimizeAway(value);
	}

	return 0;
}
