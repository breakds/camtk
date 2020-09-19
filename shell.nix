let pkgs = import <nixpkgs> {};

    customizedOpencv4 = pkgs.opencv4.override {
      enableGtk3 = true;
      enableFfmpeg = true;
      enableGStreamer = true;
    };

    libpopl = pkgs.callPackage ./nix/popl.nix {};

in pkgs.mkShell rec {
  name = "osm";
  buildInputs = with pkgs; [
    cmake
    cmakeCurses
    pkgconfig

    spdlog
    libyamlcpp
    customizedOpencv4
    libpopl
  ];
  shellHook = ''
    export CC=clang
    export CXX=clang++
  '';
}
