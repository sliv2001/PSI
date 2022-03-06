pacman -S mingw-w64-x86_64-binutils \
mingw-w64-x86_64-cmake mingw-w64-x86_64-curl mingw-w64-x86_64-expat \
mingw-w64-x86_64-gcc mingw-w64-x86_64-gettext mingw-w64-x86_64-gtest \
mingw-w64-x86_64-libiconv mingw-w64-x86_64-make mingw-w64-x86_64-zlib \
autoconf autogen automake autotools mingw-w64-x86_64-x265 \
mingw-w64-x86_64-libde265 mingw-w64-x86_64-libjpeg-turbo \
--noconfirm --needed


#Compiling exiv2

cd ./exiv2
mkdir build
cd build
cmake -G "MSYS Makefiles" \
      -DCMAKE_CXX_FLAGS=-Wno-deprecated \
      -DCMAKE_BUILD_TYPE=Release \
      -DBUILD_SHARED_LIBS=ON \
      -DEXIV2_BUILD_SAMPLES=ON \
      -DEXIV2_ENABLE_NLS=ON \
      -DEXIV2_ENABLE_WEBREADY=ON \
      -DEXIV2_ENABLE_BMFF=ON \
      -DEXIV2_BUILD_UNIT_TESTS=OFF \
      ..
cmake --build . --parallel
cd ../../


#Compiling libheif

cd ./libheif
./autogen.sh
./configure --enable-shared --enable-dependency-tracking --disable-examples
make -j
cd ..
