require 'mkmf'

$LIBS << '../badgewars.a'

dir_config 'badgewars'
create_makefile 'badgewars'