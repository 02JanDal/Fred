#!/usr/bin/env ruby

require_relative 'benchmark'
require 'capn_proto'
require 'pry'

module WonkoFile extend CapnProto::SchemaLoader
  load_schema('capnproto.capnp')
end

data = File.read 'capnproto.capnproto'

data_parsed = WonkoFile::WonkoFile.make_from_bytes data

puts "environment: capnproto ruby - ruby-gem-version-capn_proto #{CapnProto::VERSION}"
benchmark('capnproto', nil, 'parse_time') { WonkoFile::WonkoFile.make_from_bytes data }
benchmark('capnproto', nil, 'access_time') { get_latest data_parsed }
