#include <msgpack.hpp>

#include "benchmark.h"
#include "util.h"

static inline int getLatest(const msgpack::unpacked &msg)
{
  msgpack::object root = msg.get();
  std::map<std::string, msgpack::object> rootParts;
  root.convert(&rootParts);

  std::vector<std::map<std::string, msgpack::object>> versions;
  rootParts["versions"].convert(&versions);

  int value = 0;
  for (auto &version : versions)
  {
    int time;
    version["time"].convert(&time);
    value = std::max(value, time);
  }
  return value;
}

int main(int argc, char **argv)
{
  const char *data = readFile("msgpack.msgpack");
  const size_t size = fileSize("msgpack.msgpack");

  COLLECT_METRIC("msgpack", "msgpack-c", "parse_time", 1000)
  {
    msgpack::unpacked msg;
    msgpack::unpack(&msg, data, size);
    benchmark::doNotOptimizeAway(msg);
  }

  msgpack::unpacked msg;
  msgpack::unpack(&msg, data, size);
  COLLECT_METRIC("msgpack", "msgpack-c", "access_time", 1000)
  {
    benchmark::doNotOptimizeAway(getLatest(msg));
  }

  return 0;
}
