{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
    utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, utils }:
    let
      supportedSystems = [ "x86_64-linux" "aarch64-linux" ];
    in
    utils.lib.eachSystem supportedSystems (system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
      {
        devShell = with pkgs; mkShell {
          packages = [
            clang-tools
            gnumake
            cmake
            gcc
            gcc-arm-embedded
            libusb
            ninja
            openocd
            python3
          ];
        };
      });
}
