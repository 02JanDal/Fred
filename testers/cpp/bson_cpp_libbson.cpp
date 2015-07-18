#include <bson.h>

#include "benchmark.h"
#include "util.h"

const bson_t *parse(const char *filename)
{
  bool is_eof;
  bson_error_t error;
  bson_reader_t *reader = bson_reader_new_from_file(filename, &error);
  const bson_t *doc = bson_reader_read(reader, &is_eof);
  bson_reader_destroy(reader);
  return doc;
}
int getLatest(const bson_t *doc)
{
  bson_iter_t docIt, versionsIt, versionIt;
  bson_iter_init(&docIt, doc);
  bson_iter_find(&docIt, "versions");
  bson_iter_recurse(&docIt, &versionsIt);
  int value = 0;
  while (bson_iter_next(&versionsIt))
  {
    bson_iter_recurse(&versionsIt, &versionIt);
    bson_iter_find(&versionIt, "time");
    value = std::max(value, bson_iter_int32(&versionIt));
  }
  return value;
}

int main(int argc, char **argv)
{
  COLLECT_METRIC("bson", "libbson", "parse_time", 10000)
  {
    benchmark::doNotOptimizeAway(parse("bson.bson"));
  }
  const bson_t *doc = parse("bson.bson");
  COLLECT_METRIC("bson", "libbson", "access_time", 10000)
  {
    benchmark::doNotOptimizeAway(getLatest(doc));
  }
  return 0;
}
