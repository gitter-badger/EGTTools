#!/bin/bash

set -e
set -x

# OpenMP is not present on macOS by default
if [[ "$RUNNER_OS" == "macOS" ]]; then
    # Make sure to use a libomp version binary compatible with the oldest
    # supported version of the macos SDK as libomp will be vendored into the
    # scikit-learn wheels for macos. The list of bottles can be found at:
    # https://formulae.brew.sh/api/formula/libomp.json. Currently, the oldest
    # supported macos version is: High Sierra / 10.13. When upgrading this, be
    # sure to update the MACOSX_DEPLOYMENT_TARGET environment variable in
    # wheels.yml accordingly.
    if [[ "$BUILD_ARCH" == "macosx_x86_64"  ]]; then
      wget https://homebrew.bintray.com/bottles/libomp-11.0.0.high_sierra.bottle.tar.gz
      brew install libomp-11.0.0.high_sierra.bottle.tar.gz
      export MACOSX_DEPLOYMENT_TARGET=10.13
      export CIBW_ENVIRONMENT="$CIBW_ENVIRONMENT MACOSX_DEPLOYMENT_TARGET=10.13"
    else
      export MACOSX_DEPLOYMENT_TARGET=10.15
      export CIBW_ENVIRONMENT="$CIBW_ENVIRONMENT MACOSX_DEPLOYMENT_TARGET=10.15"
      brew install libomp
    fi
    export CC=/usr/bin/clang
    export CXX=/usr/bin/clang++
    export CPPFLAGS="$CPPFLAGS -Xpreprocessor -fopenmp"
    export CFLAGS="$CFLAGS -I/usr/local/opt/libomp/include"
    export CXXFLAGS="$CXXFLAGS -I/usr/local/opt/libomp/include"
    export LDFLAGS="$LDFLAGS -Wl,-rpath,/usr/local/opt/libomp/lib -L/usr/local/opt/libomp/lib -lomp"

    brew install eigen
    brew install openblas

    export LDFLAGS="$LDFLAGS -L/usr/local/opt/openblas/lib"
    export CPPFLAGS="$CPPFLAGS -I/usr/local/opt/openblas/include"
fi

# The version of the built dependencies are specified
# in the pyproject.toml file, while the tests are run
# against the most recent version of the dependencies

python -m pip install cibuildwheel twine
python -m cibuildwheel --output-dir wheelhouse

