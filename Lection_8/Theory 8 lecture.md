# Лекция 8: Использование библиотек(пример SQLite). Рассмотрение кода-примера

---

## **1. Подключение к базе данных**  
**Цель:** Установить соединение с SQLite-базой, обработать возможные ошибки.

```cpp
#include <sqlite3.h>
#include <iostream>
#include <ctime>
#include <iomanip> // Для форматирования времени

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");
    sqlite3* db = nullptr; // Указатель на базу данных

    // Открытие базы 
    int result = sqlite3_open16("History.db", &db);
    
    if (result != SQLITE_OK) {
        wcout << "Ошибка подключения: " << endl;
        sqlite3_close(db); // Важно закрыть даже при ошибке!
        return 1;
    }
    // ... Действия с базой данных ...
    sqlite3_close(db);
    return 0;
```

**Пояснения:**
- `sqlite3_open()` – открывает базу или создает новую, если она не существует.
- Всегда проверяйте код возврата (`SQLITE_OK` = успех).
- `sqlite3_errmsg()` возвращает текстовое описание ошибки.
- Закрывайте базу через `sqlite3_close()` даже при ошибках, чтобы избежать утечек памяти.

---

## **2. Выполнение SQL-запросов**  
**Цель:** Показать выполнение простых запросов и обработку ошибок.

```cpp
    char* errMsg = nullptr;
    int execResult = sqlite3_exec(db, "SELECT count(*) FROM urls;", nullptr, nullptr, &errMsg);
    // urls - имя таблицы (запрос выводит количество строк в таблицы urls)
    
    if (execResult != SQLITE_OK) {
        wcout << L"Ошибка создания таблицы: " << errMsg << endl;
        sqlite3_free(errMsg); // Освобождаем сообщение об ошибке
        sqlite3_close(db);
        return 1;
    }
```

**Пояснения:**
- `sqlite3_exec()` подходит для запросов без возвращаемых данных (CREATE, INSERT, UPDATE).
- `errMsg` динамически выделяется SQLite – освобождайте его через `sqlite3_free()`.

---

## **3. Подготовленные выражения**  
**Цель:** Выполнение запросов с параметрами 

```cpp
    sqlite3_stmt* stmt = nullptr; // указатель на откомпелированную программу
    int execResult2 = sqlite3_prepare_v2(db, "SELECT id, title, url, last_visit FROM urls;", -1, &stmt, nullptr);
    if (execResult2 != SQLITE_OK) {
        wcout << L"Ошибка подготовки запроса: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 1;
    }
```

**Пояснения:**
- `sqlite3_prepare_v2()` "компилирует" SQL-запрос в байт-код.
- Всегда проверяйте результат перед использованием `stmt`.

---

## **4. Чтение данных**  
**Цель:** Построчное извлечение данных из результата запроса.

```cpp
    while (true) {
        stepResult = sqlite3_step(stmt); // Переход к следующей строке
        
        if (stepResult == SQLITE_DONE) break; // Все строки обработаны
        if (stepResult != SQLITE_ROW) {
            cerr << "Ошибка чтения: " << sqlite3_errmsg(db) << endl;
            break;
        }
        
        // Извлечение данных из текущей строки
        int64_t id = sqlite3_column_int64(stmt, 0); // Столбец 0 – id
        
        // Обработка TEXT-полей (может быть NULL)
        wchar_t *title = sqlite3_column_text16(stmt, 1);
        wcout << L"Title: " << title << endl;
        
        const wchar_t *url = (const wchar_t)sqlite3_column_text16(stmt, 2);
        wcout << L"URL: " << url << endl;
        
        int64_t database_time = sqlite3_column_int64(stmt, 3);
        wcout << L"Database time: " << database_time << endl;
        
   }
```

**Пояснения:**
- `sqlite3_step()` возвращает `SQLITE_ROW` при наличии данных, `SQLITE_DONE` при завершении.
- Используйте функции `sqlite3_column_XXX()` в соответствии с типами столбцов.
- Всегда проверяйте указатели на `NULL` для текстовых полей.

---

## **5. Преобразование времени**  
**Цель:** Конвертация Unix-времени в читаемый формат.

```cpp
    #include <ctime>//библиотека для работы со временем

    int64_t database_time = sqlite3_column_int64(stmt, 3);
    time_t last_visit_time = (database_time / 1000000ULL) - 11644473600ULL;
    tm timeinfo;
    gmtime_s(&timeinfo, &last_visit_time);
    //выводим время как строку:
    cout << "GMT Time: " << put_time(&timeinfo, "%d.%m.%Y %H:%M:%S %z") << endl;
```

**Пояснения:**
- SQLite хранит время как целое число (Unix-время).
- `gmtime()` преобразует время в структуру `tm` (UTC).

---

## **6. Завершение работы**  
**Цель:** Корректное освобождение ресурсов.

```cpp
    sqlite3_finalize(stmt); // Уничтожение подготовленного выражения
    sqlite3_close(db); // Закрытие базы данных
    
    return 0;
}
```

**Пояснения:**
- `sqlite3_finalize()` освобождает память, занятую `stmt`.
- Всегда закрывайте базу после работы.

---

### **Полный код**
```cpp
#include <sqlite3.h>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <locale.h>

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian"); // Для вывода русских символов

    // 1. Подключение к базе данных
    sqlite3* db = nullptr;
    int result = sqlite3_open16(L"History.db", &db);
    
    if (result != SQLITE_OK) {
        wcout << L"Ошибка подключения: " << endl;
        sqlite3_close(db);
        return 1;
    }

    // 2. Создание таблицы (пример)
      
    char* errMsg = nullptr;
    int execResult = sqlite3_exec(db, "SELECT count(*) FROM urls;", nullptr, nullptr, &errMsg);
    if (execResult != SQLITE_OK) {
        wcout << L"Ошибка создания таблицы: " << errMsg << endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }

    // 3. Подготовка запроса
    sqlite3_stmt* stmt = nullptr;
    int execResult2 = sqlite3_prepare_v2(db, "SELECT id, title, url, last_visit FROM urls;", -1, &stmt, nullptr);
    if (execResult2 != SQLITE_OK) {
        wcout << L"Ошибка подготовки запроса: " << sqlite3_errmsg16(db) << endl;
        sqlite3_close(db);
        return 1;
    }

    // 4. Чтение данных
    int stepResult;
    while (true) {
        stepResult = sqlite3_step(stmt);
        if (stepResult == SQLITE_DONE) break;
        if (stepResult != SQLITE_ROW) {
            wcout << L"Ошибка чтения: " << sqlite3_errmsg16(db) << endl;
            break;
        }

        // Извлечение данных
        int64_t id = sqlite3_column_int64(stmt, 0); // Столбец 0 – id
        
        // Обработка TEXT-полей (может быть NULL)
        wchar_t *title = sqlite3_column_text16(stmt, 1);
        wcout << L"Title: " << title << endl;
        
        const wchar_t *url = (const wchar_t)sqlite3_column_text16(stmt, 2);
        wcout << L"URL: " << url << endl;
        
        int64_t database_time = sqlite3_column_int64(stmt, 3);

        // 5. Преобразование времени
        time_t last_visit_time = (database_time / 1000000ULL) - 11644473600ULL;
        tm timeinfo;
        gmtime_s(&timeinfo, &last_visit_time);
        cout << "GMT Time: " << put_time(&timeinfo, "%d.%m.%Y %H:%M:%S %z") << endl;
    }

    // 6. Завершение работы
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

```
## Задание:
Надо написать программу, которая читает содержимое истории посещений вашего любимого веб-браузера на базе движка Chromium (но можно и Firefox, если хотите). В веб-браузерах на базе движка Chromium (на примере Yandex Browser) это файл \Users\*\AppData\Local\Yandex\YandexBrowser\User Data\Default\History, в Firefox - файл \Users\*\AppData\Roaming\Mozilla\Firefox\Profiles\*\places.sqlite. Программа должна читать историю посещений и выводить ее в консоль в виде списка. Обязательно выводить идентификатор записи, URL посещенного сайта и заголовок страницы. Еще надо предусмотреть возможность удалить любую запись по заданному идентификатору (SQL-запросом DELETE FROM...). В принципе, я все рассказал и показал на лекции, но если что, то на сайте sqlite.org есть вся необходимая документация. Всем, кто хочет достичь "продвинутого" уровня, необходимо разобраться с технологией bind-инга полей SQL-запроса применительно к решению задачи удаления записей (запрос должен готовиться один раз при запуске программы, а затем идентификаторы должны подставляться в него при помощи соответствующей функции sqlite3_bind_...).

---
