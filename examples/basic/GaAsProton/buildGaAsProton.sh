# Change into local

rm -rf build
mkdir build && cd build

cmake -DGEANT4_INSTALL_DATA=ON -DCMAKE_BUILD_TYPE=Release ../

cmake --build . --config Release
