# Компилирование программы.
# Для запуска установите: sudo apt-get install gcc-multilib mingw-w64 g++
# Создание выходной папки.
mkdir -p ./bin
# Описание команд:
# i686-w64-mingw32-gcc, x86_64-w64-mingw32-gcc, gcc - инструменты для компиляции (win32, win64, linux)
# -finput-charset=utf-8 -fexec-charset=ibm866 - сохраняет .exe файл со строками кодировки CP866. Так работают русские буквы.
# -m32 или -m64 - платформа x86 или x64
# -std=c11 - стандарт языка C 2011 года.
# -o <файл> - путь до выходного файла.
# ParetoInterface.c - главный файл компиляции.
echo Win32
i686-w64-mingw32-gcc -finput-charset=utf-8 -fexec-charset=ibm866 -O0 -m32 -std=c11 -o ./bin/pareto32.exe ParetoInterface.c
echo Win64
x86_64-w64-mingw32-gcc -finput-charset=utf-8 -fexec-charset=ibm866 -O0 -m64 -std=c11 -o ./bin/pareto64.exe ParetoInterface.c
echo Linux32
gcc -O0 -m32 -std=c11 -o ./bin/pareto32 ParetoInterface.c
chmod a+x ./bin/pareto32
echo Linux64
gcc -O0 -m64 -std=c11 -o ./bin/pareto64 ParetoInterface.c
chmod a+x ./bin/pareto64
