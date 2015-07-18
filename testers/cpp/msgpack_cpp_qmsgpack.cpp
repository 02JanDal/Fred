#include <QFile>
#include <qmsgpack/msgpack.h>

#include "benchmark.h"
#include "util.h"

static inline int getLatest(const QVariant &parsed)
{
  const QVariantList versions = parsed.toMap()["versions"].toList();
  int value = 0;
  for (const QVariant &version : versions)
  {
    value = std::max(value, version.toMap()["time"].toInt());
  }
  return value;
}

int main(int argc, char **argv)
{
  QFile file("msgpack.msgpack");
  file.open(QFile::ReadOnly);
  const QByteArray data = file.readAll();

  COLLECT_METRIC("msgpack", "qmsgpack", "parse_time", 1000)
  {
    benchmark::doNotOptimizeAway(MsgPack::unpack(data));
  }

  const QVariant parsed = MsgPack::unpack(data);
  COLLECT_METRIC("msgpack", "qmsgpack", "access_time", 1000)
  {
    benchmark::doNotOptimizeAway(getLatest(parsed));
  }

  return 0;
}
