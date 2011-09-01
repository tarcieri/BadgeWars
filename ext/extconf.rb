require 'mkmf'

$defs << " -I" << File.expand_path("../../src", __FILE__)
$LIBS << File.expand_path("../../badgewars.a", __FILE__)

dir_config 'badgewars_ext'
create_makefile 'badgewars_ext'