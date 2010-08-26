require 'rake/clean'

task :default => :test

includes = %w(-Isrc)
warnings = %w(error all shadow pointer-arith cast-qual strict-prototypes strict-overflow=5)
warning_args = warnings.map { |w| "-W#{w}" }.join(' ')
compiler = "gcc #{includes} -ansi -pedantic #{warning_args}"
ar = "ar rcs"

# Object files to build
objects = Dir["src/*.c"].map { |f| f.sub(/.c$/, '.o') }

objects.each do |object|
  src = object.sub(/.o$/, '.c')
  file object => src do
    sh "#{compiler} -c -o #{object} #{src}"
  end
end

file "badgewars.a" => objects do
  sh "#{ar} badgewars.a #{objects}"
end

file "test/bw_testrunner" => "badgewars.a" do
  sh "#{compiler} -o test/bw_testrunner test/bw_testrunner.c badgewars.a"
end

task :test => "test/bw_testrunner" do
  sh "test/bw_testrunner"
end

CLEAN.include 'badgewars.a'
CLEAN.include '**/*.o'