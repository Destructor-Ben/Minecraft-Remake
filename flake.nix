{
  description = "Minecraft Remake";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
        lib = nixpkgs.lib;

        build-scripts-folder = ./scripts;
        build-script-files = lib.filterAttrs (n: v: v == "regular") (builtins.readDir build-scripts-folder);
        build-scripts = lib.mapAttrsToList (n: v:
          pkgs.writeShellScriptBin (lib.removeSuffix ".sh" n) (builtins.readFile "${build-scripts-folder}/${n}")
        )
        build-script-files;

        # TODO: see what is/isn't needed as deps

        x11-deps = with pkgs; [
          libx11
          libxcursor
          libxi
          libxrandr
          libxinerama
        ];

        wayland-deps = with pkgs; [
          wayland
          wayland-protocols
          libdecor
        ];

        runtime-deps = with pkgs; [
          alsa-lib
          udev
          libxkbcommon
          vulkan-loader
          vulkan-validation-layers
        ] ++ x11-deps ++ wayland-deps;

        compiletime-deps = with pkgs; [
          gcc
          cmake
          gdb
        ] ++ build-scripts;

        ld-lib-path = with pkgs; [
          libGL
        ];
      in
      {
        devShells.default = pkgs.mkShell {
          nativeBuildInputs = compiletime-deps;
          buildInputs = runtime-deps;
          LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath ld-lib-path;
        };
      }
    );
}
