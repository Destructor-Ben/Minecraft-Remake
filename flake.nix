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
          wayland-scanner
          libdecor
          libffi
        ];

        runtime-deps = with pkgs; [
          alsa-lib
          udev
          libxkbcommon
          libGL
          pkg-config
        ] ++ x11-deps ++ wayland-deps;

        compiletime-deps = with pkgs; [
          cmake
          gcc
          gdb
          perf
          valgrind
        ] ++ build-scripts;

        ld-lib-path = runtime-deps;
        pkg-config-path = lib.filter (pkg: pkg ? "dev") runtime-deps;
      in
      {
        devShells.default = pkgs.mkShell {
          nativeBuildInputs = compiletime-deps;
          buildInputs = runtime-deps;
          LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath ld-lib-path;
          PKG_CONFIG_PATH = lib.strings.join ":" (lib.map (pkg: "${pkg.dev}/lib/pkgconfig") pkg-config-path);
        };
      }
    );
}
