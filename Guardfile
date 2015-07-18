guard :coffeescript, input: 'assets/logic.coffee', output: 'build/assets/logic.js'

guard :shell do
  watch %r{scripts/.*} do |file|
    puts 'rebuilding html'
    `rake html`
  end
  watch %r{build/(assets/)?.*\.(html|css|js|png|jpg)} do
    puts 'asking browser to reload'
    `CUR_WID=$(xdotool getwindowfocus); xdotool windowactivate $(xdotool search --name Fred); xdotool key F5; xdotool windowactivate $CUR_WID`
  end
  watch %r{assets/.*\.(coffee|scss)$} do |file|
    puts 'rebuilding ' + file.first
    `rake build/#{file.first.sub('coffee', 'js')}`
  end
end
