#!/usr/bin/env ruby

require_relative 'benchmark'
require 'oj'
require 'json'
require 'yajl'
require 'yajl/version'
require 'bson'
require 'msgpack'

puts "environment: - ruby - ruby #{`ruby -v`}"
puts "environment: json ruby oj ruby-gem-version-oj #{Oj::VERSION}"
puts "environment: json ruby yajl ruby-gem-version-yajl #{Yajl::VERSION}"
puts "environment: bson ruby bson ruby-gem-version-bson #{BSON::VERSION}"
puts "environment: msgpack ruby msgpack ruby-gem-version-msgpack #{MessagePack::VERSION}"

json_data = File.read 'json.json'
bson_data = File.read 'bson.bson'
msgpack_data = File.read 'msgpack.msgpack'

json_parsed = Oj.load json_data
bson_parsed = Hash.from_bson StringIO.new bson_data
msgpack_parsed = MessagePack.unpack msgpack_data

benchmark('json', 'oj', 'parse_time') { Oj.load json_data }
benchmark('json', 'oj', 'access_time') { get_latest json_parsed }
benchmark('json', 'json', 'parse_time') { JSON.parse json_data }
benchmark('json', 'json', 'access_time') { get_latest json_parsed }
benchmark('json', 'yajl', 'parse_time') { Yajl::Parser.new.parse json_data }
benchmark('json', 'yajl', 'access_time') { get_latest json_parsed }
benchmark('bson', nil, 'parse_time') { Hash.from_bson StringIO.new bson_data }
benchmark('bson', nil, 'access_time') { get_latest bson_parsed }
benchmark('msgpack', nil, 'parse_time') { MessagePack.unpack msgpack_data }
benchmark('msgpack', nil, 'access_time') { get_latest msgpack_parsed }
