require 'rake/clean'

task :default => :test

includes = %w(-Isrc)
warnings = %w(error all shadow pointer-arith cast-qual strict-prototypes strict-overflow=5)
warning_args = warnings.map { |w| "-W#{w}" }.join(' ')
compiler = "gcc #{includes} -ansi -pedantic #{warning_args}"
ar = "ar rcs"

lib_sources = Dir["src/*.c"] - %w(src/bwserver.c)
objects = lib_sources.map { |f| f.sub(/.c$/, '.o') }

objects.each do |object|
  src = object.sub(/.o$/, '.c')
  file object => src do
    sh "#{compiler} -c -o #{object} #{src}"
  end
end

file "badgewars.a" => objects do
  sh "#{ar} badgewars.a #{objects.join(' ')}"
end

file "bwserver" => "badgewars.a" do
  sh "#{compiler} -o bwserver src/bwserver.c badgewars.a"
end

task :start_bwserver => "bwserver" do
  sh "./bwserver"
end

task :test => :start_bwserver

CLEAN.include 'badgewars.a'
CLEAN.include '**/*.o'