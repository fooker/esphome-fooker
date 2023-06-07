{ pkgs ? import (builtins.fetchTarball {
  url = "https://github.com/nixos/nixpkgs/archive/f4aa6afa5f934ece2d1eb3157e392d056be01617.tar.gz";
  sha256 = "0iqa0ysv5d6gpzmkikn2s8scz2sy2nk8j9h9p123yfkxg6lf9g59";
}) { } }:

let
in pkgs.mkShell {
  buildInputs = with pkgs; [
    bash
    git
    esphome
  ];
}

