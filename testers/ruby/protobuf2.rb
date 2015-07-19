#!/usr/bin/env ruby

require_relative 'benchmark'
require 'protocol_buffers'
require 'protocol_buffers/version'
require 'pry'

require_relative 'protocolbuffers/protobuf.pb'

data = File.read 'protobuf.protobuf'

parsed = WonkoFile.parse data

puts "environment: protobuf ruby protobuf ruby-gem-version-protocol_buffers #{ProtocolBuffers::VERSION}"
benchmark('protobuf', 'protocol_buffers', 'parse_time', 100) { WonkoFile.parse data }
benchmark('protobuf', 'protocol_buffers', 'access_time') { get_latest_methodcall parsed }
