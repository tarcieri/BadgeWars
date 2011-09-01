require 'rake'
require 'rake/clean'

Dir['tasks/**/*.rake'].each { |task| load task }

task :default => :spec

#
# BadgeWars core library
#

includes = File.expand_path("../src", __FILE__)
warnings = %w(error all shadow pointer-arith cast-qual strict-prototypes strict-overflow=5)
warning_args = warnings.map { |w| "-W#{w}" }.join(' ')
compiler = "gcc -I#{includes} -ansi -pedantic #{warning_args}"
ar = "ar rcs"
library = 'badgewars.a'

lib_sources = Dir["src/*.c"] - %w(src/bwserver.c)
objects = lib_sources.map { |f| f.sub(/.c$/, '.o') }

rule '.o' => '.c' do |t|
  sh "#{compiler} -c -o #{t.name} #{t.source}"
end

file library => objects do
  sh "#{ar} #{library} #{objects.join(' ')}"
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

file ext_so => [library, *ext_files] do
  Dir.chdir('ext') { sh 'make' }
end

task :ext => ext_so

#
# RSpec
#

begin
  require 'rspec/core/rake_task'

  desc "Run the BadgeWars RSpecs"
  RSpec::Core::RakeTask.new
rescue LoadError
  task :spec do
    puts "Run 'gem install rspec' to be able to run the BadgeWars specs"
  end
end

Rake::Task[:spec].enhance [:ext]

#
# Clean
#

CLEAN.include library
CLEAN.include ext_so
CLEAN.include '**/*.o'