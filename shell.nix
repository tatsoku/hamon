{ pkgs ? import <nixpkgs> {}}:

pkgs.mkShell {
  allowUnfree = true;
  name = "hamon!";
  packages = with pkgs; [
    zsh
    clang-tools
    clang
    mold
    pre-commit
    bash-language-server
    valgrind
  ];
  shellHook = ''
  '';
}
