{ stdenv, cmake, fetchFromGitHub }:

stdenv.mkDerivation rec {
  pname = "popl";
  version = "1.2";
  
  src = fetchFromGitHub {
    owner = "badaix";
    repo = "popl";
    sha256 = "0sygi3gmfl9hm1hcmx5x2zpf2cj94wl99897drnnadmgwx0cmkad";
    rev = "85d8f9990c3f8723de69d67e38deb902c4fa398f";
  };

  nativeBuildInputs = [ cmake ];

  meta = with stdenv.lib; {
    homepage = "https://github.com/badaix/popl";
    description = ''
      popl is a C++ command line arguments parser that supports the
      same set of options as GNU's getopt and thus closely follows the
      POSIX guidelines for the command-line options of a program
    '';
    platforms = with platforms; linux ++ darwin;
    licencse = licenses.mit;
  };
}
