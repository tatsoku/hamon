# hamon!

or **H**amon **SH**ell aka hsh.

<sub> 波紋 - Ripple </sub>

A homemade shell in C.

Compiled with
[clang](https://github.com/llvm/llvm-project/tree/main/clang) or
[gcc](https://gcc.gnu.org) & linked with [mold](https://github.com/rui314/mold).


<a href="https://github.com/tatsoku/hamon/blob/main/LICENSE">
    <img src="https://img.shields.io/github/license/tatsoku/hamon?style=flat-square"
    alt="License: BSD 3-Clause" />
</a>
<a href="https://securityscorecards.dev/viewer/?uri=github.com/tatsoku/hamon">
    <img src="https://api.securityscorecards.dev/projects/github.com/tatsoku/hamon/badge?style=flat-square"
    alt="OpenSSF Scorecard" />
</a>

> \[!NOTE\]
> This project is in no way functional yet!

## Usage

```sh
 ./hsh {args}
```

## Installation

Classic installation through package manager and such, won't be available for a very long time.

So you will have to build it yourself.

Oh you want to know how?

### With Just

If you want the easy version, be sure to install [just](https://github.com/casey/just)

..and run

```sh
just fix_perms
just build
```

And you'll find it in ./bin

### Without Just

Here's where it gets "difficult". It's just a tiny bit more commands.

```sh
chmod +x ./scripts/build.sh
./scripts/build.sh -c
./scripts/build.sh -l hsh
```

_et voilà!_ Now it should be located in ./bin/ aswell.

## Contributing

Contribution is simple, make sure you're running an editor that works with C.

Such as:

- neovim
- emacs
- vscode/codium
- jetbrains
- whatever!

Just don't clutter the repo with config files for your editor.

You will also need to either have a shell that supports bash scripts,
unless you want to port
[build.sh](https://github.com/tatsoku/hamon/blob/main/scripts/build.sh)
for Powershell or Batch,
which is ick, but I'll gladly accept a pr for it.

Also, one final thing, make sure you have pre-commit hooks.

_direnv users who don't use nix, i'm sorry :(_

Made with :heart: by tatsoku-org!
