# hShell

> Pronounced: _/ɧɛlː/_

A homemade shell in C.

Compiled with [clang](https://github.com/llvm/llvm-project/tree/main/clang) & linked with [mold](https://github.com/rui314/mold).

![[LICENSE](https://github.com/h4rldev/hshell/blob/main/LICENSE)](https://img.shields.io/github/license/h4rldev/hshell?style=flat-square)
![[OpenSSF Scorecard](https://securityscorecards.dev/viewer/?uri=github.com/h4rldev/hshell)](https://api.securityscorecards.dev/projects/github.com/h4rldev/hshell/badge?style=flat-square)

> \[!NOTE\]
> This project is in no way functional yet!

## Installation

> Classic installation through package manager and such, won't be available for a very long time.

> So you will have to build it yourself.

> Oh you want to know how?

### With Just

> If you want the easy version, be sure to install [just](https://github.com/casey/just)

..and run

```sh
just fix_perms
just build
```

> And you'll find it in ./bin

### Without Just

> Here's we're it gets "difficult". It's just a tiny bit more commands.

```sh
chmod +x ./scripts/build.sh
./scripts/build.sh -c
./scripts/build.sh -l hsh
```

> _et voilà!_ Now it should be located in ./bin/ aswell.

## Usage

```sh
 ./hsh {args}
```

> Made with :heart: by h4rl
