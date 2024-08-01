default:
  just --list

@fix_perms:
  chmod +x scripts/build.sh
  chmod +x scripts/clear_vgcores.sh

@compile-debug name="hsh":
  scripts/build.sh -c {{name}} --debug

@compile name="hsh":
  scripts/build.sh -c {{name}}

@link name="hsh":
  scripts/build.sh -l {{name}}

@build name="hsh":
  just compile {{name}}
  just link {{name}}

@run args="" name="hsh":
  just compile {{name}}
  just link {{name}}
  ./bin/{{name}} {{args}}

@debug args="" name="hsh":
  just compile-debug {{name}}
  just link {{name}}
  valgrind ./bin/{{name}} {{args}}

@clear_cores:
  scripts/build.sh -vg

@install name="hsh":
  just build {{name}}
  sudo mv bin/{{name}} /usr/bin/
