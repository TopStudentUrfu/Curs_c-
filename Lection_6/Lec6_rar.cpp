
#include <iostream>
#include <fstream>
#include <vector>

#include <cstdint> // Это для использования обозначений целых чисел с указанием их размера

using namespace std;

// Установить 1-байтовое выравнивание
#pragma pack(push, 1)

// Объявить структуру
struct rar_header
{
    uint16_t header_crc;    // 2-байтовое целое без знака
    uint8_t header_type;    // 1-байтовое целое без знака
    uint16_t header_flags;
    uint16_t header_size;

    // Дальше просто демонстрация
    // char padding[8]; // Пропуск 8 байтов (просто чтобы показать)
    // int32_t anotherInt; // Еще целое 4-байтовое число со знаком
};

// Восстановить выравнивание
#pragma pack(pop)

int main()
{
    // Открыть файл в двоичном режиме
    ifstream rar_file(L"t:\\Source\\Example.rar", ios::binary);

    // Убедиться, что открылся
    if(rar_file.is_open())
    {
        // Определить размер файла
        rar_file.seekg(0, ios::end);
        int fileSize = rar_file.tellg();
        cout << "File size: " << fileSize << endl;

        // Снова спозиционироваться в начало файла
        rar_file.seekg(0, ios::beg);

        // Выделить память
        vector<char> rar_data(fileSize, 0);

        // Считать данные
        rar_file.read(rar_data.data(), fileSize);

        // Инициализировать указатель на структуру
        // (по смещению 7, потому что надо пропустить сигнатуру длиной 7 байт)
        rar_header* p_header = reinterpret_cast<rar_header*>(&rar_data[7]);

        // rar_header* - тип переменной
        // p_header - имя переменной
        // &rar_data[7] - указатель на байт массива с индексом 7
        // reinterpret_cast<rar_header*> - приведение типа к rar_header*

        // Вывести два поля этой структуры

        // Вывод в шестнадцатеричном представлении (hex)
        cout << "Header type: 0x" << hex << int(p_header->header_type) << endl;

        // Вывод в десятичном представлении (dec)
        cout << "Header size: " << dec << int(p_header->header_size) << endl;
    }

    return 0;
}
