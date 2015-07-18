#include <flatbuffers/flatbuffers.h>
#include <flatbuffers/idl.h>

#include "flatbuffers_generated.h"

#include "util.h"
#include "benchmark.h"

int main(int argc, char **argv)
{
	const uint8_t *data = (const uint8_t *)readFile("flatbuffers.flatbuffers");
	const size_t size = fileSize("flatbuffers.flatbuffers");

	COLLECT_METRIC("flatbuffers", "", "parse_time", 1000)
	{
		benchmark::doNotOptimizeAway(flatbuffers::Verifier(data, size).VerifyBuffer<Fred::WonkoFile>());
		benchmark::doNotOptimizeAway(Fred::GetWonkoFile(data));
	}

	const Fred::WonkoFile *file = Fred::GetWonkoFile(data);
	COLLECT_METRIC("flatbuffers", "", "access_time", 1000)
	{
		uint32_t value = 0;
		const auto versions = file->versions();
		for (auto it = versions->begin(); it != versions->end(); ++it)
		{
			value = std::max(value, (*it)->time());
		}
		benchmark::doNotOptimizeAway(value);
	}

	return 0;
}
