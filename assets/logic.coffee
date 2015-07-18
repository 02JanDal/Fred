class HumanizerClass
  constructor: ->
    @conversionSteps = []
    @add (str) -> str.charAt(0).toUpperCase() + str.slice(1)
    @add '-', ' '
    @add '_', ' '
  add: (func_or_match, null_or_subst) ->
    if null_or_subst == undefined
      @conversionSteps.push func_or_match
    else
      func_or_match = new RegExp func_or_match, 'gi' unless func_or_match instanceof RegExp
      @conversionSteps.push (str) -> str.replace func_or_match, null_or_subst
    return @
  human: (string) ->
    str = string
    _.each @conversionSteps, (func) -> str = func str
    return str
window.Humanizer = new HumanizerClass

class FormatterClass
  constructor: ->
    @formatters = {}
    @typeMatchers = []
    @staticFormatters = {}
    @staticLabels = {}
  add: (type, func) ->
    @formatters[type] = func;
    return @
  addMatcher: (match, type) ->
    @typeMatchers.push { match: match, type: type }
    return @
  addStatic: (type, func) ->
    @staticFormatters[type] = func;
    return @
  addStaticLabel: (type, label) ->
    @staticLabels[type] = label;
    return @
  format: (type, value) ->
    return '' if value == null
    return if @formatters[type] then @formatters[type](value) else value;
  formatStatic: (type, value) ->
    return 0 if value == null
    return if @staticFormatters[type] then @staticFormatters[type](value) else value;
  staticLabel: (type) -> return @staticLabels[type]
  type: (string) ->
    return (_.find(@typeMatchers, (matcher) -> string.match(matcher.match) != null) || {type:null}).type;
window.Formatter = new FormatterClass

Backbone.sync = (method, model, options) ->
  return if method == "create" || method == "update" || method == "delete"
  options.success(data) if model instanceof BenchmarkCollection

class RenderingView extends Backbone.View
  initialize: -> @render()
  render: ->
    @$el.html(_.result(this, 'template')(_.result(this, 'templateData') || null))
    return @
  template: -> return _.template($(_.result(this, 'templateId')).html())
  templateData: -> return if @model == undefined then {} else @model.toJSON()
window.RenderingView = RenderingView
