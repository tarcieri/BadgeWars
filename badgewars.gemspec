# -*- encoding: utf-8 -*-
require File.expand_path('../lib/badgewars/version', __FILE__)

Gem::Specification.new do |gem|
  gem.name          = 'badgewars'
  gem.version       = BadgeWars::VERSION
  gem.authors       = ["Tony Arcieri"]
  gem.email         = ["tony.arcieri@gmail.com"]
  gem.description   = "BadgeWars: minified CoreWars, a battle of bits to the death"
  gem.summary       = "CoreWars for conference badges: A little tiny Redcode/MARS engine in a simple C library for microcontrollers"
  gem.homepage      = "http://github.com/tarcieri/BadgeWars"
  gem.licenses      = ['MIT']

  gem.files        = Dir['README.md', 'lib/**/*']
  gem.require_path = 'lib'

  gem.add_development_dependency('rake')
  gem.add_development_dependency('rspec', ['>= 2.3.0'])
end
