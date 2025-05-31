# Лекция 14: MSYS2
# Установка среды разработки MSYS2

1. Дистрибутив загружается с заголовочной страницы официального сайта [MSYS2](https://www.msys2.org/). Для установки `MSYS2` требуется 64-битная версия Microsoft Windows 10 или более новая ОС. Обычно установка выполняется в каталог `X:\msys64`, где `X` - буква диска.

2. Для запуска процедуры установки пакетов расширения запустить `mingw64.exe`. 

3. Чтобы установить все пакеты из текстового файла `pkglist.txt` в `MSYS2`, выполнить команду `pacman` вместе с `xargs`:

```bash
xargs -a pkglist.txt pacman -S --needed
```

Здесь:
- `xargs` читает имена пакетов из файла `pkglist.txt`.
- `pacman -S` устанавливает указанные пакеты.
- `--needed` позволяет избежать повторной установки пакетов, которые уже установлены.

Файл `pkglist.txt` должен содержать названия всех подлежащих установке пакетов, по одному на строку. Предлагаемый **минимальный** набор пакетов для установки:
```
base
base-devel
doxygen
filesystem
git
msys2-runtime
mingw-w64-x86_64-binutils
mingw-w64-x86_64-cmake
mingw-w64-x86_64-crt-git
mingw-w64-x86_64-gcc
mingw-w64-x86_64-gcc-ada
mingw-w64-x86_64-gcc-fortran
mingw-w64-x86_64-gcc-libgfortran
mingw-w64-x86_64-gcc-libs
mingw-w64-x86_64-gcc-objc
mingw-w64-x86_64-gdb
mingw-w64-x86_64-gdb-multiarch
mingw-w64-x86_64-headers-git
mingw-w64-x86_64-iconv
mingw-w64-x86_64-icu
mingw-w64-x86_64-libmangle-git
mingw-w64-x86_64-libxml2
mingw-w64-x86_64-libxml2-docs
mingw-w64-x86_64-make
mingw-w64-x86_64-pkgconf
mingw-w64-x86_64-python-pip
mingw-w64-x86_64-python-pip-tools
mingw-w64-x86_64-tools-git
mingw-w64-x86_64-winpthreads-git
mingw-w64-x86_64-winstorecompat-git
mingw-w64-gtest
mingw-w64-catch
mingw-w64-crypto++
```

4. Для установки одного пакета:

    `pacman -S <имя_пакета>`

5. При необходимости обновить пакеты:

    `pacman -Suy`

4. Полученный в результате каталог `msys64` с установленными пакетами в дальнейшем можно переносить с компьютера на компьютер без необходимости повторной установки и выхода в Интернет.

# Установка редактора Visual Studio Code

1. Дистрибутив загружается со страницы официального сайта [Download Visual Studio Code](https://code.visualstudio.com/Download).

2. Установить программу, например, в каталог `X:\VSCode`, где `X` - буква диска.

3. Запустить `VSCode` и установить следующие пакеты расширений:

   - `C/C++`
   - `CMake Tools`
   - `C/C++ Themes`
   - `C/C++ Extension Pack`
   - `C/C++ Snippets`
   - `Header Guard Generator`
   - `EPITECH C/C++ Headers`
   - `Prettier - Code formatter`
   - `PlantUML`
   - `Doxygen Documentation Generator`
   - `GitLens — Git supercharged`

    Установку можно выполнять в режиме он-лайн или с использованием предварительно загруженных VSIX-файлов.

# Настройка CMake

1. Сразу после установки `CMake Tools` будет предложено выбрать компилятор, который будет использоваться для сборки программ. Необходимо указать компилятор, расположенный в каталоге `X:\msys64\mingw64\bin\`. Если компилятор не будет найден, надо зайти в настройки `CMake Tools` и в разделе `Cmake: Additional Compiler Search Dirs` добавить путь `X:\msys64\mingw64\bin\`. 

2. Далее в настройках `CMake Tools` необходимо найти `Cmake: Cmake Path` и указать там путь до исполняемого файла `X:\msys64\mingw64\bin\cmake.exe`

3. Нажать `Ctrl`+`Shift`+`P`, выбрать `CMake: Select a Kit` и указать компилятор, расположенный в каталоге `X:\msys64\mingw64\bin\`.

# Установка библиотек Boost

1. Загрузить последнюю версию библиотек со страницы официального сайта [Boost C++ Libraries](https://www.boost.org/) в варианте архива **.7z**

2. Распаковать полученный архив **с использованием архиватора 7-Zip последней версии** в корневой каталог диска, например, так: `X:\boost_1_88_0`

3. Запустить `X:\msys64\mingw64.exe`, перейти в каталог, куда был распакован Boost: `cd /x/boost_1_88_0/`

4. Проверить, что компилятор доступен: `./gcc --version`

5. Выполнить сборку установщика с использованием компилятора `GCC`, установленного в `MSYS2`: `./bootstrap.sh gcc` Это занимает некоторое время..

6. Выполнить установку Boost: `./b2 toolset=gcc` Это длительная (до 30 минут) и ресурсоемкая операция! При выполнении ее на ноутбуке убедитесь, что компьютер подключен к розетке.

# Создание тестового проекта и его сборка при помощи CMake

1. Открыть каталог, в котором будет располагаться проект (предварительно каталог надо создать).
   
2. Создать файлы `main.cpp` и `main.h`

3. Переключиться в файл `main.h`, нажать `Ctrl`+`Shift`+`P`, ввести `Generate Header Guard` и нажать `Enter` для генерации ограждающей конструкции, препятствующей повторному подключению заголовочного файла (используется расширение `Header Guard Generator`).

4. В файле `main.cpp` написать простую программу:

```C++
#include <iostream>
using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");    
    wcout << L"Всем привет!" << endl;
}
```

5. Создать файл `CMakeLists.txt` со следующим содержимым

```CMake
cmake_minimum_required(VERSION 3.10) # Требования к минимальной версии CMake
project(HelloCMake LANGUAGES CXX) # Название проекта и используемый язык (C++)
set(CMAKE_CXX_STANDARD 17) # Старнадрт языка (C++ 17)

add_executable(
    HelloCMake # "Цель" сборки (название исполняемого файла)
    "main.cpp" # Файл исходного кода (пока один)
)
```

6. Выполнить конфигурацию проекта: `Ctrl`+`Shift`+`P` `CMake: Configure`

7. Выполнить сборку: `Ctrl`+`Shift`+`P` `CMake: Build` или `F7`

8. Убедиться, что в подкаталоге `build` каталога с проектом будет создан файл `HelloCMake.exe`. Запустить его!

```
Всем привет!
```

9.  Добавить к проекту файлы `second.cpp` и `second.h` со следующим содержимым:
    
```C++

#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdint>
#include <cryptopp/sha.h>

void print_something()
{
    std::wcout << L"Какой-то русский текст" << std::endl;
}

void calculate_and_print_digest()
{
    std::vector<uint8_t> myVector {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    uint8_t digest[CryptoPP::SHA1::DIGESTSIZE];
    CryptoPP::SHA1 hash;
    hash.CalculateDigest(digest, myVector.data(), myVector.size());

    for(auto i : digest)
    {
        std::cout << std::hex << std::setw(2) << int(i);
    }

    std::cout << std::endl;

}

```

10. В файле `main.cpp` добавить:

```C++
#include "second.h"
// ...
print_something();
calculate_and_print_digest();
```

11. Попытаться выполнить очистку `CMake: Clean`, а затем сборку `CMake: Build`. Сборка не выполнится, поскольку к проекту не добавлен файл `second.cpp`. Его нужно добавить новой строкой в раздел `add_executable` файла `CMakeLists.txt`.

12. Чтобы подключить к проекту библиотеку `Crypto++`, используемую для расчета хеш-сумм, надо добавить к файлу `CMakeLists.txt` следующие строки:

```CMake
# ...

add_executable(
    HelloCMake # "Цель" сборки (название исполняемого файла)
    "main.cpp"
    "second.cpp"
)

find_library(CRYPTOPP_LIB NAMES libcryptopp.a) # Найти библиотеку Crypto++

if(NOT CRYPTOPP_LIB) # Убедиться, что она нашлась
    message(FATAL_ERROR "Crypto++ library not found!")
endif()

# Подключить библиотеку к проекту
target_link_libraries(HelloCMake PRIVATE ${CRYPTOPP_LIB})

```

12. Выполнить очистку `CMake: Clean`, а затем сборку `CMake: Build`. Убедиться, что программа запускается. Если нужно запустить проект, можно нажать `Shift`+`F5`

## Задание:
**Лабораторная работа № 6 (MSYS2 + Visual Studio Code + Boost + CMake)**

Надо развернуть на своем компьютере среду разработки по схеме, которую мы изучили на прошлой лекции (руководство опубликовано). В этой среде надо собрать программу, которую вы получили в результате выполнения всех предыдущих лабораторных работ, включая работу № 5. Программу надо разделить на несколько файлов исходных кодов (*.cpp), а также использовать в ней любую стороннюю библиотеку, например Crypto++ (`mingw-w64-crypto++`) или SQLite (`mingw-w64-sqlite3`). В качестве результата хочу увидеть:
1. Скриншоты развернутой среды разработки с вашей программой, в том числе показывающие, что программа работает в терминале VSCode, а также то, что вы можете выполнять ее отладку с установкой точек останова (breakpoint).
2. Файл CMakeLists.txt вашего проекта.
3. Текстовое описание того, зачем в вашей программе используется сторонняя библиотека.
4. Исходный код того места программы, где используется библиотека (можно совместить с п. 3, если в коде оставите нормальный комментарий).
5. Base Package: mingw-w64-crypto++ - MSYS2 Packages
 
