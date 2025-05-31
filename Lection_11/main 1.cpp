#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "PatternTemplates.h"

using namespace std;


class Fruit // Родительский (базовый) класс "Фрукт"
{
private: // "Закрытые" компоненты (не доступны в унаследованных классах)
    int Color;
    double Weight;

protected: // "Защищенные" компоненты (доступны в унаследованных классах, но не доступны внешнему наблюдателю)
    bool FruitIsGood;

public: // "Открытые" компоненты, определяющие интерфейс класса

    Fruit(int color) : Color(color), Weight(0.0), FruitIsGood(false) {} // Конструктор
    virtual ~Fruit() {} // Деструктор (объявлен виртуальным, чтобы обеспечить корректное уничтожение унаследованных классов)

    // Функция с реализацией
    int GetColor() const { return Color; }

    // Функция с реализацией
    double GetWeight() const { return Weight; }

    // Абстрактная функция (без реализации)
    virtual void Eat() = 0;
};

class Apple : public Fruit // Класс-наследник "Яблоко"
{
public:
    Apple();
    ~Apple() {}

    // Определение функции в унаследованном классе (функция виртуальная!)
    void Eat() override;
};

// Реализация конструктора
Apple::Apple() : Fruit(1)
{
    FruitIsGood = true; // Инициализация FruitIsGood через присваивание
}

void Apple::Eat()
{
    cout << "Eating apple..." << endl;
}

class Kiwi : public Fruit // Класс-наследник "Киви"
{
public:
    Kiwi() : Fruit(1) { FruitIsGood = true; }
    ~Kiwi() {}

    // Определение функции в унаследованном классе (функция виртуальная!)
    void Eat() override;
};

void Kiwi::Eat()
{
    cout << "Eating kiwi..." << endl;
}

class Orange : public Fruit // Класс-наследник "Апельсин"
{
public:
    Orange() : Fruit(2) { FruitIsGood = true; }
    ~Orange() {}

    void Eat();
};

void Orange::Eat()
{
    cout << "Eating orange..." << endl;
}

// Реализация паттерна "Фабричный метод" для создания фруктов

enum class FruitType : int
{
    Apple = 1,
    Orange = 2,
    Kiwi = 3,

    Undefined = 0 // На всякий случай
};

Fruit *CreateFruit(FruitType type)
{
    Fruit *newFruit = nullptr;

    if(type == FruitType::Apple)
    {
        newFruit = new Apple;
    }
    else if(type == FruitType::Orange)
    {
        newFruit = new Orange;
    }
    else if(type == FruitType::Kiwi)
    {
        newFruit = new Kiwi;
    }

    return newFruit;
}

// Функция, позволяющая "съесть" любые фрукты из любого контейнера
// вне зависимости от его внутреннего устройства
void EatEmAll(Iterator<Fruit*> *it)
{
    for(it->First(); !it->IsDone(); it->Next())
    {
        Fruit *currentFruit = it->GetCurrent();
        currentFruit->Eat();
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");

    wcout << L"Какой фрукт создать (1 - яблоко, 2 - апельсин, 3 - киви)?" << endl;
    FruitType type = FruitType::Undefined;
    int ii;
    cin >> ii;
    type = static_cast<FruitType>(ii);

    // "Сложный" способ создания объектов

    Fruit *newFruit = nullptr;
    if(type == FruitType::Apple)
    {
        newFruit = new Apple;
    }
    else if(type == FruitType::Orange)
    {
        newFruit = new Orange;
    }
    else if(type == FruitType::Kiwi)
    {
        newFruit = new Kiwi;
    }

    // Демонстрация использования общего интерфейса фруктов
    newFruit->Eat();
    delete newFruit;

    // Создание фруктов при помощи фабричного метода
    // (гораздо короче и универсальнее, не упоминаются конкретные классы-наследники)

    Fruit *newFruit2 = CreateFruit(type);
    newFruit2->Eat();
    delete newFruit2;

    cout << endl << endl;

    // Использование шаблонных классов-контейнеров и итераторов для их обхода

    size_t N = 0;
    wcout << L"Введите количество фруктов: ";
    cin >> N;

    cout << endl;

    // 1. Стек конкретных объектов (апельсинов)

    StackClass<Orange> orangeStack;
    for(size_t i=0; i<N; i++)
    {
        Orange newOrange;
        orangeStack.Push(newOrange); // Добавляем апельсин в контейнер
    }

    wcout << L"Размер стека апельсинов: " << orangeStack.Size() << endl;

    Iterator<Orange> *it = new StackIterator(&orangeStack);
    for(it->First(); !it->IsDone(); it->Next())
    {
        Orange currentFruit = it->GetCurrent();
        currentFruit.Eat();
    }

    cout << endl << endl;

    // 2. Стек фруктов

    StackClass<Fruit*> fruitStack;
    for(size_t i=0; i<N; i++)
    {
        int fruit_num = rand()%3+1; // Число от 1 до 3 (случайный фрукт)
        FruitType fruit_type = static_cast<FruitType>(fruit_num);
        Fruit *newFruit = CreateFruit(fruit_type);
        fruitStack.Push(newFruit);
    }

    wcout << L"Размер стека фруктов: " << fruitStack.Size() << endl;

    Iterator<Fruit*> *it2 = new StackIterator<Fruit*>(&fruitStack);
    EatEmAll(it2);
    delete it2;

    cout << endl << endl;

    // 3. Массив фруктов

    ArrayClass<Fruit*> fruitArray;
    for(size_t i=0; i<10; i++)
    {
        int fruit_num = rand()%3+1; // Число от 1 до 3 (случайный фрукт)
        FruitType fruit_type = static_cast<FruitType>(fruit_num);
        Fruit *newFruit = CreateFruit(fruit_type);
        fruitArray.Add(newFruit);
    }

    wcout << L"Размер массива фруктов: " << fruitStack.Size() << endl;

    /*

    // Обход в простом цикле
    for(size_t i=0; i<fruitArray.Size(); i++)
    {
        Fruit *currentFruit = fruitArray[i];
        currentFruit->Eat();
    }

    */

    Iterator<Fruit*> *it3 = new ArrayIterator<Fruit*>(&fruitArray);
    EatEmAll(it3);
    delete it3;

    return 0;
}
