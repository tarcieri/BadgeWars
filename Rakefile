require 'rake'
require 'rake/clean'

task :default => ['badgewars.a', :ext]

#
# BadgeWars core library
#

includes = %w(-Isrc)
warnings = %w(error all shadow pointer-arith cast-qual strict-prototypes strict-overflow=5)
warning_args = warnings.map { |w| "-W#{w}" }.join(' ')
compiler = "gcc #{includes} -ansi -pedantic #{warning_args}"
ar = "ar rcs"

lib_sources = Dir["src/*.c"] - %w(src/bwserver.c)
objects = lib_sources.map { |f| f.sub(/.c$/, '.o') }

rule '.o' => '.c' do |t|
  sh "#{compiler} -c -o #{t.name} #{t.source}"
end

file "badgewars.a" => objects do
  sh "#{ar} badgewars.a #{objects.join(' ')}"
end

#
# BadgeWars Ruby extension
#

ext_so = "ext/badgewars.#{Config::CONFIG['DLEXT']}"
ext_files = FileList[
  'ext/*.c',
  'ext/*.h',
  'ext/extconf.rb',
  'ext/Makefile'
]

file 'ext/Makefile' => 'ext/extconf.rb' do
  Dir.chdir('ext') { ruby 'extconf.rb' }
end

file ext_so => ext_files do
  Dir.chdir('ext') { sh 'make' }
end

task :ext => ext_so

CLEAN.include 'badgewars.a'
CLEAN.include '**/*.o'