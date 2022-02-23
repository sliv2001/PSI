Для компиляции exiv2 совместно с Qt выполнить следующее:
1. Установить msys
2. Выполнить:

pacman -S mingw-w64-x86_64-binutils mingw-w64-x86_64-cmake mingw-w64-x86_64-curl mingw-w64-x86_64-expat mingw-w64-x86_64-gcc mingw-w64-x86_64-gettext mingw-w64-x86_64-gtest mingw-w64-x86_64-libiconv mingw-w64-x86_64-make mingw-w64-x86_64-zlib
mkdir -p ~/gnu/github/exiv2 && cd       ~/gnu/github/exiv2 && git clone https://github.com/exiv2/exiv2 && cd exiv2 && mkdir build && cd build && cmake -G "MSYS Makefiles" -DCMAKE_CXX_FLAGS=-Wno-deprecated -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DEXIV2_BUILD_SAMPLES=ON -DEXIV2_ENABLE_NLS=ON -DEXIV2_ENABLE_WEBREADY=ON -DEXIV2_ENABLE_BMFF=ON -DEXIV2_BUILD_UNIT_TESTS=ON .. && cmake --build . --parallel

3. Полученные файлы .a, .dll, .h разместить по соответствующим папкам.
4. Файлы libexpat.dll, libiconv.dll, libintl-8.dll, zlib1.dll разместить в System32 или в папке с .exe файлом
5. ...
6. Profit!