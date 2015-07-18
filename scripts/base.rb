require 'colorize'
require 'logging'
require 'shellwords'
require 'active_support/core_ext/string/inflections'

ActiveSupport::Inflector.inflections :en do |inflect|
  inflect.acronym 'JSON'
  inflect.acronym 'BSON'
  inflect.human 'json', 'JSON'
  inflect.human 'bson', 'BSON'
  inflect.human /cpp/i, 'C++'
  inflect.human /rapidjson/i, 'RapidJSON'
  inflect.human 'libbson', 'libbson'
end

class Array
  def categorize(&block)
    output = Hash.new { |h, k| h[k] = [] }
    each { |i| output[yield i] << i }
    output
  end
end

class RawRow
  def self.parse(rows)
    Array(rows).map do |r|
      r.chomp # clean
    end.map do |r|
      r.scan(/^([^:]*): (.*)$/).first # split type/rest
    end.map do |r|
      RawRow.new(r[0].to_sym, r[1]).split # convert to special type
    end.flatten.categorize do |r|
      r.type # categorize by type
    end
  end

  attr_reader :type, :raw, :fields

  def initialize(type, raw)
    @type, @raw = type, raw
    if raw.is_a? String
      @fields = Shellwords.split(raw).map do |field|
        (field == '-' || field == ' ' || field == '') ? nil : field
      end
    else
      @fields = raw
    end
  end

  def split
    output = []
    @fields.each_with_index do |field, index|
      next if field.nil?
      if field.start_with?('{') && field.end_with?('}')
        field[1...(field.length - 1)].split(',').each do |splitted|
          fields = @fields.dup
          fields[index] = splitted
          output << RawRow.new(@type, fields).split
        end
      end
    end
    output << self if output.empty?
    output
  end

  def required(*args)
    get(*(args | [:required]))
  end

  def optional(*args)
    get(*args)
  end

  private

  def get(index, *args)
    type = args.find { |arg| arg.is_a? Class } || String
    data = args.include?(:rest) ? @fields[index..-1].join(' ') : @fields[index]
    fail 'No value provided for a required field in "' + @raw + '"' if data.nil? && args.include?(:required)

    case type.class
    when String.class
      data.to_s
    when Float.class
      data.to_f
    when Symbol.class
      data.to_sym
    when Integer.class
      data.to_i
    else
      fail 'Unknown type provided'
    end
  end
end

Logging.color_scheme 'bright', levels: { debug: :gray, info: :green, warn: :yellow, error: :red, fatal: [:white, :on_red] }, date: :blue, logger: :cyan
Logging.appenders.stdout 'stdout', layout: Logging.layouts.pattern(pattern: '[%d] %-5l %c: %m\n', color_scheme: 'bright'), level: :debug
Logging.logger.root.add_appenders 'stdout'

$general_logger = Logging.logger['General']
