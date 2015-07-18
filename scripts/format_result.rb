#!/usr/bin/env ruby

require 'json'
require 'active_support/core_ext/string/inflections'
require_relative 'base'

if ARGV.length != 2
  puts "Usage: #{__FILE__} <input> <output>"
  exit -1
end
result_rows = JSON.parse File.read(ARGV[0]), symbolize_names: true

$general_logger.info 'Got a total of ' + result_rows[:values].length.to_s + ' values'
$general_logger.info 'Generating output...'
input = File.read(File.expand_path __FILE__ + '/../results.html')
input = input.sub('JSON_CONTENT', JSON.generate(result_rows)).sub('BENCHMARK_RUN_TIME', Time.new.utc.strftime('%Y-%m-%d %H:%M:%S %Z'))
File.write(ARGV[1], input)
$general_logger.info 'Done. You may now open '.yellow + 'results.html'.yellow.bold + ' in your favourite browser.'.yellow
