Для компиляции зависимостей совместно с Qt на MinGW (без msys) 
выполнить следующее:
1. Установить msys
2. Выполнить в среде msys-MinGW:

sh init.sh

3. Полученные файлы .a, .dll, .h разместить по соответствующим папкам.
4. Файлы libexpat.dll, libiconv.dll, libintl-8.dll, zlib1.dll, 
libx265.dll, libde265.dll, libjpeg.dll разместить в System32 
или в папке с .exe файлом
5. ...
6. Profit!

Для компиляции с Qt, собранным в msys, достаточно шагов 2-3, 5-6.
