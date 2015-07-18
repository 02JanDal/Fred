#!/usr/bin/env ruby

require 'pry'
require 'json'
require 'rake'
require 'rake/contrib/sshpublisher'
require 'coffee_script'
require_relative 'scripts/base'

Dir.chdir File.expand_path File.dirname __FILE__

builddir_base = File.expand_path 'build'
commondir = File.expand_path 'common'
def get_testers(builddir_base, commondir)
  theinformationpath = File.expand_path 'the_information.json'

  # get a list of testers
  testers = Dir['testers/*'].map do |t|
    {
      id: File.basename(t),
      path: File.expand_path(t),
      builddir: builddir_base + '/' + File.basename(t),
      install_cmd: File.expand_path(t) + '/install',
      env: {
            'BUILDDIR' => builddir_base + '/' + File.basename(t),
            'COMMONDIR' => commondir,
            'THEINFORMATIONPATH' => theinformationpath,
            'SRCDIR' => File.expand_path(t)
           }
    }
  end

  $general_logger.info 'These testers were found: ' + testers.map { |t| t[:id] }.map(&:to_s).join(', ')

  testers
end

class DirectoryTask < Rake::FileTask
  def out_of_date?(stamp)
    @prerequisites.any? { |n| application[n, @scope].timestamp > timestamp }
  end

  def timestamp
    if File.exist? name
      if File.directory? name
        Dir.glob(name.to_s + '/**/*', File::FNM_DOTMATCH).reject { |e| ['.', '..'].include? e }.map { |e| File.exists?(e) ? File.mtime(e) : Rake::EARLY }.max
      else
        File.mtime name.to_s
      end
    else
      Rake::LATE
    end
  end
end

testers = get_testers builddir_base, commondir

# register tasks
testers.each do |tester|
  DirectoryTask.define_task tester[:builddir] => [File.expand_path('common'), tester[:path]] do
    Logging.logger[tester[:id].to_s].info 'Running setup...'
    FileUtils.mkdir_p tester[:builddir]
    if not system(tester[:env], tester[:install_cmd], chdir: tester[:builddir])
      fail 'Setup for a tester failed'
    end
    FileUtils.touch tester[:builddir] + '/.timestamp'
  end
  file "#{builddir_base}/results.#{tester[:id]}.json" => [File.expand_path('common'), tester[:builddir]] do
    raw_result_rows = []
    Logging.logger[tester[:id].to_s].info 'Running...'
    output_file = "#{builddir_base}/output.#{tester[:id]}.txt"
    Dir[tester[:builddir] + '/*'].select { |f| File.executable?(f) && File.file?(f) }.each do |file|
      Logging.logger[tester[:id].to_s].info '  Running ' + file + '...'
      Process.wait spawn tester[:env], file, out: output_file, err: :out, chdir: tester[:builddir]
      if File.exists? output_file
        raw_result_rows |= File.read(output_file).lines
        File.delete output_file
      else
        fail 'A tester failed'
      end
    end

    categorized_rows = RawRow.parse raw_result_rows

    final_result = {}
    final_result[:values] = categorized_rows[:value].map do |row|
      {
        format: row.required(0),
        language: row.optional(1),
        implementation: row.optional(2),
        id: row.required(3),
        value: row.required(4, Float)
      }
    end
    final_result[:metadata] = categorized_rows[:metadata].map do |row|
      {
        format: row.optional(0),
        language: row.optional(1),
        implementation: row.optional(2),
        key: row.required(3),
        data: row.required(4, :rest)
      }
    end
    final_result[:environment] = categorized_rows[:environment].map do |row|
      {
        format: row.optional(0),
        language: row.optional(1),
        implementation: row.optional(2),
        key: row.required(3),
        data: row.required(4, :rest)
      }
    end

    File.write "#{builddir_base}/results.#{tester[:id]}.json", JSON.pretty_generate(final_result)
  end
end

results_json_filename = "#{builddir_base}/results.json"

file results_json_filename => testers.map { |t| "#{builddir_base}/results.#{t[:id]}.json" } do
  joined_result = Hash.new { |h, k| h[k] = [] }
  testers.map do |t|
    JSON.parse(File.read "#{builddir_base}/results.#{t[:id]}.json").each do |key, value|
      joined_result[key] += value.map { |hash| hash.merge({ tester: t[:id] }) }
    end
  end
  File.write results_json_filename, JSON.pretty_generate(joined_result)
end

desc 'Runs the setup for all testers'
task :setup => testers.map { |t| t[:builddir] }

desc 'Runs all testers and collects the result'
task :run => results_json_filename

file 'build/assets/logic.js' => ['assets/logic.coffee'] do
  $general_logger.info 'Building logic.js'
  Dir.mkdir builddir_base + '/assets'
  File.write "#{builddir_base}/assets/logic.js", CoffeeScript.compile(File.read 'assets/logic.coffee')
end

file 'build/assets/style.css' => ['assets/style.scss'] do
end

desc 'Generates the HTML output'
task :html => [results_json_filename, 'build/assets/logic.js', 'build/assets/style.css'] do
  system '/usr/bin/env', 'ruby', 'scripts/format_result.rb', results_json_filename, "build/results.html"
end
task :default => :html

desc 'Uploads the HTML to the server specified in REMOTE_HOST'
task :upload => :html do
  fail 'You need to specify REMOTE_HOST' unless ENV.key? 'REMOTE_HOST'
  ['results.html','assets/'].each do |file|
    sh 'scp', '-qr', builddir_base + '/' + file, "#{ENV['REMOTE_HOST']}:#{ENV['REMOTE_DIR'] || '~/www/Fred'}/#{file}"
  end
end
