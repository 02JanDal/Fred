require 'benchmark'

def benchmark(format, implementation, id, iterations = 1000, &blk)
  time = Benchmark.realtime { iterations.times { yield } }
  seconds = time.to_f / iterations
  puts "value: #{format} ruby #{implementation || '-'} #{id} #{seconds}"
end

def get_latest(data)
  data['versions'].map { |v| v['time'].to_i }.max
end

def get_latest_methodcall(data)
  data.versions.map { |v| v.time.to_i }.max
end
