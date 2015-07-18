#include "capnproto.capnp.h"
#include <capnp/message.h>
#include <capnp/serialize.h>
#include <capnp/serialize-packed.h>

#include "util.h"
#include "benchmark.h"

void run(const char *filename, const char *implementation)
{
	const char *data = readFile(filename);
	const size_t size = fileSize(filename);

	COLLECT_METRIC("capnproto", implementation, "parse_time", 1000)
	{
		kj::ArrayInputStream istream(kj::ArrayPtr<const kj::byte>((const kj::byte *)data, size));
		capnp::MallocMessageBuilder builder;
		capnp::readMessageCopy(istream, builder);
		benchmark::doNotOptimizeAway(builder.getRoot<WonkoFile>());
	}

	kj::ArrayInputStream istream(kj::ArrayPtr<const kj::byte>((const kj::byte *)data, size));
	capnp::MallocMessageBuilder builder;
	capnp::readMessageCopy(istream, builder);
	auto file = builder.getRoot<WonkoFile>();

	COLLECT_METRIC("capnproto", implementation, "access_time", 1000)
	{
		uint32_t value = 0;
		auto versions = file.getVersions();
		for (auto version : versions)
		{
			value = std::max(value, version.getTime());
		}
		benchmark::doNotOptimizeAway(value);
	}
}

int main(int argc, char **argv)
{
	// non-packed
	{
		const char *data = readFile("capnproto.capnproto");
		const size_t size = fileSize("capnproto.capnproto");

		COLLECT_METRIC("capnproto", "", "parse_time", 1000)
		{
			kj::ArrayInputStream istream(kj::ArrayPtr<const kj::byte>((const kj::byte *)data, size));
			capnp::MallocMessageBuilder builder;
			capnp::readMessageCopy(istream, builder);
			benchmark::doNotOptimizeAway(builder.getRoot<WonkoFile>());
		}

		kj::ArrayInputStream istream(kj::ArrayPtr<const kj::byte>((const kj::byte *)data, size));
		capnp::MallocMessageBuilder builder;
		capnp::readMessageCopy(istream, builder);
		auto file = builder.getRoot<WonkoFile>();

		COLLECT_METRIC("capnproto", "", "access_time", 1000)
		{
			uint32_t value = 0;
			auto versions = file.getVersions();
			for (auto version : versions)
			{
				value = std::max(value, version.getTime());
			}
			benchmark::doNotOptimizeAway(value);
		}
	}

	// packed
	{
		const char *data = readFile("capnproto_packed.capnproto_packed");
		const size_t size = fileSize("capnproto_packed.capnproto_packed");

		COLLECT_METRIC("capnproto", "packed", "parse_time", 1000)
		{
			kj::ArrayInputStream istream(kj::ArrayPtr<const kj::byte>((const kj::byte *)data, size));
			capnp::PackedMessageReader reader(istream);
			benchmark::doNotOptimizeAway(reader.getRoot<WonkoFile>());
		}

		kj::ArrayInputStream istream(kj::ArrayPtr<const kj::byte>((const kj::byte *)data, size));
		capnp::PackedMessageReader reader(istream);
		auto file = reader.getRoot<WonkoFile>();

		COLLECT_METRIC("capnproto", "packed", "access_time", 1000)
		{
			uint32_t value = 0;
			auto versions = file.getVersions();
			for (auto version : versions)
			{
				value = std::max(value, version.getTime());
			}
			benchmark::doNotOptimizeAway(value);
		}
	}

	return 0;
}
