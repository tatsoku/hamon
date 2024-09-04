default:
  just --list

@fix_perms:
  chmod +x scripts/build.sh

@compile extra_args="": fix_perms
  scripts/build.sh -c {{extra_args}}

@link name="hsh": fix_perms
  scripts/build.sh -l {{name}}

@test extra_arg1="" extra_arg2 ="": fix_perms
  scripts/build.sh -t {{extra_arg1}} {{extra_arg2}}

@setup-testing: fix_perms
  scripts/build.sh -st

@build name="hsh": fix_perms
  just compile {{name}}
  just link {{name}}

@run args="" name="hsh": fix_perms
  just compile {{name}}
  just link {{name}}
  ./bin/{{name}} {{args}}

@debug args="" name="hsh": fix_perms
  just compile --debug
  just link {{name}}
  valgrind ./bin/{{name}} {{args}}

@clear_cores: fix_perms
  scripts/build.sh -vg

@install name="hsh": fix_perms
  just build {{name}}
  sudo mv bin/{{name}} /usr/bin/
