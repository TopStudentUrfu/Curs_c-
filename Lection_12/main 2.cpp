#include <iostream>
#include <vector>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include "PatternTemplates.h"

using namespace std;

// Цвет фруктов
enum class FruitColor : int
{
    Green,
    Orange,
    Red,
    Unknown
};

class Fruit // Родительский (базовый) класс "Фрукт"
{
private: // "Закрытые" компоненты (не доступны в унаследованных классах)
    FruitColor Color;
    double Weight;

protected: // "Защищенные" компоненты (доступны в унаследованных классах, но не доступны внешнему наблюдателю)
    bool FruitIsGood;

public: // "Открытые" компоненты, определяющие интерфейс класса

    Fruit(FruitColor color) : Color(color), Weight(0.0), FruitIsGood(false)
    {
        // Значение инициализируется случайным числом 0 или 1
        FruitIsGood = static_cast<bool>(rand()%2);
    }

    virtual ~Fruit() {} // Деструктор (объявлен виртуальным, чтобы обеспечить корректное уничтожение унаследованных классов)

    // Функция с реализацией
    bool IsGood() const { return FruitIsGood; }

    // Функция с реализацией
    FruitColor GetColor() const { return Color; }

    // Функция с реализацией
    double GetWeight() const { return Weight; }

    // Функция с реализацией
    virtual void Eat()
    {
        if(IsGood())
        {
            cout << "Eating GOOD fruit... ";
        }
        else
        {
            cout << "Eating BAD fruit... ";
        }
    }
};

class Apple : public Fruit // Класс-наследник "Яблоко"
{
public:
    Apple();
    ~Apple() {}

    // Переопределение функции в унаследованном классе (функция виртуальная!)
    void Eat() override;
};

// Реализация конструктора
Apple::Apple() : Fruit(FruitColor::Green)
{
}

void Apple::Eat()
{
    Fruit::Eat(); // Вызов функции, определенной в базовом классе
    cout << "Eating apple..." << endl;
}

class Kiwi : public Fruit // Класс-наследник "Киви"
{
public:
    Kiwi() : Fruit(FruitColor::Green) { }
    ~Kiwi() {}

    // Определение функции в унаследованном классе (функция виртуальная!)
    void Eat() override;
};

void Kiwi::Eat()
{
    Fruit::Eat(); // Вызов функции, определенной в базовом классе
    cout << "Eating kiwi..." << endl;
}

class Orange : public Fruit // Класс-наследник "Апельсин"
{
public:
    Orange() : Fruit(FruitColor::Orange) { }
    ~Orange() {}

    void Eat();
};

void Orange::Eat()
{
    Fruit::Eat(); // Вызов функции, определенной в базовом классе
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

// Декоратор итератора для выделения фруктов по цвету

class FruitColorDecorator : public IteratorDecorator<class Fruit*>
{
private:
    FruitColor TargetColor;

public:
    FruitColorDecorator(Iterator<Fruit*> *it, FruitColor color)
    : IteratorDecorator<Fruit*>(it), TargetColor(color) {}

    void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->GetColor() != TargetColor)
        {
            It->Next();
        }
    }

    void Next()
    {
        do
        {
            It->Next();

        } while(!It->IsDone() && It->GetCurrent()->GetColor() != TargetColor);
    }
};

// Декоратор итератора для выделения "хороших" или "плохих" фруктов

class FruitGoodDecorator : public IteratorDecorator<class Fruit*>
{
private:
    bool TargetGood;

public:
    FruitGoodDecorator(Iterator<Fruit*> *it, bool isGood)
    : IteratorDecorator<Fruit*>(it), TargetGood(isGood) {}

    void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->IsGood() != TargetGood)
        {
            It->Next();
        }
    }

    void Next()
    {
        do
        {
            It->Next();

        } while(!It->IsDone() && It->GetCurrent()->IsGood() != TargetGood);
    }
};

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

// Функция, позволяющая "съесть" только хорошие фрукты
// (демонстрация решения проблемы "в лоб")
void EatEmAllGood(Iterator<Fruit*> *it)
{
    for(it->First(); !it->IsDone(); it->Next())
    {
        Fruit *currentFruit = it->GetCurrent();
        if(!currentFruit->IsGood()) continue;

        currentFruit->Eat();
    }
}

// Функция, позволяющая "съесть" только зеленые фрукты
// (демонстрация решения проблемы "в лоб")
void EatEmAllGreen(Iterator<Fruit*> *it)
{
    for(it->First(); !it->IsDone(); it->Next())
    {
        Fruit *currentFruit = it->GetCurrent();
        if(currentFruit->GetColor() != FruitColor::Green) continue;

        currentFruit->Eat();
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");

    size_t N = 30;

    // Массив фруктов

    ArrayClass<Fruit*> fruitArray;
    for(size_t i=0; i<N; i++)
    {
        int fruit_num = rand()%3+1; // Число от 1 до 3 (случайный фрукт)
        FruitType fruit_type = static_cast<FruitType>(fruit_num);
        Fruit *newFruit = CreateFruit(fruit_type);
        fruitArray.Add(newFruit);
    }

    wcout << L"Размер массива фруктов: " << fruitArray.Size() << endl;

    // Связанный список фруктов (для демонстрации адаптера)

    list<Fruit*> fruitVector; // а еще можно vector, forward_list, ...
    for(size_t i=0; i<N; i++)
    {
        int fruit_num = rand()%3+1; // Число от 1 до 3 (случайный фрукт)
        FruitType fruit_type = static_cast<FruitType>(fruit_num);
        Fruit *newFruit = CreateFruit(fruit_type);
        fruitVector.push_back(newFruit); // Добавить новый фрукт в конец списка
    }

    wcout << L"Размер списка фруктов: " << fruitVector.size() << endl;

    // Обход в простом цикле
    cout << endl << "Eating all in a simple loop:" << endl;
    for(size_t i=0; i<fruitArray.Size(); i++)
    {
        Fruit *currentFruit = fruitArray[i];
        currentFruit->Eat();
    }

    // Обход всех элементов при помощи итератора
    cout << endl << "Eating all using iterator:" << endl;
    Iterator<Fruit*> *allIt = fruitArray.GetIterator();
    EatEmAll(allIt);
    delete allIt;

    // Обход всех хороших фруктов
    cout << endl << "Eating all good using iterator:" << endl;
    Iterator<Fruit*> *goodIt = new FruitGoodDecorator(fruitArray.GetIterator(), true);
    EatEmAll(goodIt);
    delete goodIt;

    // Обход всех оранжевых фруктов
    cout << endl << "Eating all orange using iterator:" << endl;
    Iterator<Fruit*> *orangeIt = new FruitColorDecorator(fruitArray.GetIterator(), FruitColor::Orange);
    EatEmAll(orangeIt);
    delete orangeIt;

    // Обход всех хороших оранжевых фруктов
    cout << endl << "Eating all good orange using iterator:" << endl;
    Iterator<Fruit*> *goodOrangeIt =
        new FruitGoodDecorator(new FruitColorDecorator(fruitArray.GetIterator(), FruitColor::Orange), true);
    EatEmAll(goodOrangeIt);
    delete goodOrangeIt;

    // Демонстрация работы адаптера
    cout << endl << "Eating all good orange using adapted iterator (another container):" << endl;
    Iterator<Fruit*> *adaptedIt = new ConstIteratorAdapter<std::list<Fruit*>, Fruit*>(&fruitVector);
    Iterator<Fruit*> *adaptedOrangeIt = new FruitGoodDecorator(new FruitColorDecorator(adaptedIt, FruitColor::Orange), true);
    EatEmAll(adaptedOrangeIt);
    delete adaptedOrangeIt;
    // adaptedIt удалять не надо, так как он удаляется внутри декоратора adaptedOrangeIt

    return 0;
}
