#!/usr/bin/env ruby

require "json"

EXTENSION_TO_SHADER_TYPE = {
    "frag" => "FRAGMENT_SHADER",
    "vert" => "VERTEX_SHADER"
}

SHADER_MAP = {}
Dir.foreach(".") do |filename|
  m = filename.match(/^(.*)\.([^\.]+)$/)
  shader_type = nil
  if m == nil || (shader_type = EXTENSION_TO_SHADER_TYPE[m[2]]) == nil
    next
  end
  shader_name = m[1]
  SHADER_MAP[shader_name] = {
      "type" => shader_type,
      "source" => File.read(filename)
  }
end

# do really simple minification
def minify(source)
  source.gsub(/([;{}])\s*\n+\s*/, '\1')
end
SHADER_MAP.map do |_, value|
  value["source"] = minify(value["source"])
end

# output javascript
puts "module.exports = #{JSON.pretty_generate(SHADER_MAP)};"
