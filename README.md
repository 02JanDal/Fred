Usage: `rake`, then open `build/results.html` in your favourite browser.

# Contributing

New benchmarks for new languages, formats or implementations are welcome as PRs. Also fixes to the framework, though that's a lower priority.

Because of the mix of languages there is no real coding standard. Try to adapt to the file you are currently editing, or files that do something similar.

# Formats

* [protobuf](https://github.com/google/protobuf)
  * [C++](https://developers.google.com/protocol-buffers/docs/cpptutorial)
  * [Java](https://developers.google.com/protocol-buffers/docs/javatutorial)
  * [Python](https://developers.google.com/protocol-buffers/docs/pythontutorial)
  * [JavaScript](https://github.com/dcodeIO/ProtoBuf.js)
  * [Ruby](https://github.com/protobuf-ruby/beefcake)
* [JSON](http://json.org/)
  * [Qt](http://doc.qt.io/qt-5/json.html)
  * [C++](https://github.com/miloyip/rapidjson)
  * [Ruby](https://github.com/ohler55/oj)
  * [Java](https://github.com/google/gson)
  * JavaScript
* [Msgpack](http://msgpack.org/)
  * [Qt](https://github.com/romixlab/qmsgpack)
  * [C++](https://github.com/msgpack/msgpack-c)
  * [Ruby](https://github.com/msgpack/msgpack-ruby)
  * [Java](https://github.com/msgpack/msgpack-java)
  * [JavaScript](https://github.com/msgpack/msgpack-javascript)
* [BSON](http://bsonspec.org/)
  * [C++](https://github.com/dwight/bson-cxx)
  * [Ruby](https://rubygems.org/gems/bson_ext)
  * [Java](https://github.com/kohanyirobert/ebson)
  * [JavaScript](https://github.com/mongodb/js-bson)
* [Avro](http://avro.apache.org/)
* [Thrift](http://thrift.apache.org/)
* [Cap'n'Proto](https://capnproto.org/)
  * [C++](https://capnproto.org/cxx.html)
  * [Ruby](https://github.com/cstrahan/capnp-ruby)
  * [Java](https://github.com/dwrensha/capnproto-java/)
  * [JavaScript](https://github.com/capnp-js/serialization)
* [FlatBuffers](http://google.github.io/flatbuffers/)

# Testers

In `testers/` each directory should contain the shell script `install`. It will be run from the testers unique build directory and should place executable (mode +x) files in the build directory. These files will be run for the actual tests. They can have the following output (per line):

Optional fields should contain a single dash (`-`) if empty. `<format>`, `<implementation>` and `<language>` should be lowercase letter-only (ie. `capnproto`).

* `value: <format> <language> <implementation> <metric id> <metric value>`
  * `<implementation>` and `<language>` are optional.
  * `<metric value>` a base unit (no prefix (bytes instead of kilobytes etc.)).
  * See below for `<metric id>`.
* `environment: <format> <language> <implementation> <key> <data...>`
  * `<format>`, `<implementation>` and `<language>` are optional.
  * Examples for `<key>`: `cpu`, `memory`, `storage`, `cpp-compiler`
  * `<data...>` spans the rest of the line and may contain spaces
* `metadata: <format> <language> <implementation> <key> <data...>`
  * `<format>`, `<implementation>` and `<language>` are optional.
  * Examples for `<key>`: `url`, `name`
  * `<data...>` spans the rest of the line and may contain spaces

Use `common/` for files shared between several testers (like protocol schemas).

## Environment variables

These environment variables will be set for the script:

* BUILDDIR (a unique directory for each tester)
* COMMONDIR (full path to the `common/` directory)
* THEINFORMATIONPATH (full path to the `the_information.json` file)
* SRCDIR (the directory of the tester)

# Tests

"the information" is what's used for all tests so they are doing a comparable amount of work. See `the_information.json` for it.

| ID | Format | Description |
| -- | ------ | ----------- |
| size | bytes | The amount of space required to store the information |
| parse_time | ms | The amount of time required to parse the file containing the information, excluding disk access time (file should be in memory) |
| access_time | ms | The amount of time required to find the newest version (based on the time), not including time needed for parsing |
