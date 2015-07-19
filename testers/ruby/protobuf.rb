#!/usr/bin/env ruby

ENV['PB_NO_NETWORKING'] = '1'

require_relative 'benchmark'
require 'beefcake'
require 'beefcake/version'
require 'protobuf'
require 'protobuf/version'
require 'pry'

require_relative 'beefcake/protobuf.pb'
require_relative 'protobuf/protobuf.pb'

data = File.read 'protobuf.protobuf'

beefcake_parsed = Beefcake::WonkoFile.decode data
protobuf_parsed = WonkoFile.decode data

puts "environment: protobuf ruby beefcake ruby-gem-version-beefcake #{Beefcake::VERSION}"
benchmark('protobuf', 'beefcake', 'parse_time', 100) { Beefcake::WonkoFile.decode data }
benchmark('protobuf', 'beefcake', 'access_time') { get_latest_methodcall beefcake_parsed }

puts "environment: protobuf ruby protobuf ruby-gem-version-protobuf #{Protobuf::VERSION}"
benchmark('protobuf', 'protobuf', 'parse_time', 100) { WonkoFile.decode data }
benchmark('protobuf', 'protobuf', 'access_time') { get_latest_methodcall protobuf_parsed }
