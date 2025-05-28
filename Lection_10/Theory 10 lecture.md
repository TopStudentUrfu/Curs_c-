# Лекция 10: Наследование

## **1. Наследование**  
- **Определение**: Отношение между двумя классами, где один (дочерний) наследует свойства и методы другого (родительского).  
- **Доступ к членам класса**:  
  - **`private` (закрытый)**:  
    Члены недоступны в дочерних классах.  
    *Пример*: В классе `Fruit` переменные `Color` и `Weight` — приватные. Класс `Apple` не может к ним обращаться напрямую.  
  - **`protected` (защищенный)**:  
    Члены доступны в базовом классе и его наследниках, но скрыты от внешнего кода.  
    *Пример*: Переменная `FruitIsGood` в `Fruit` доступна в `Apple` и `Orange`.  
  - **`public` (открытый)**:  
    Члены доступны везде, включая внешний код.  
- **Побочные эффекты**:  
  - Повторное использование кода через вызов открытых функций родительского класса.  
  - Совместимость интерфейсов: наследники сохраняют все открытые функции родителя.  

### **1. Определение базового класса `Fruit` (C++)**  

#### **Структура класса**  
- **Приватные члены**: `Color`, `Weight` (недоступны наследникам).  
- **Защищенные члены**: `FruitIsGood` (доступен наследникам).  
- **Публичные методы**:  
  - `GetColor()`: возвращает цвет (не виртуальная функция).  
  - `GetWeight()`: возвращает вес (не виртуальная, но может быть переопределена).  
  - `Eat()`: **чисто виртуальная функция** (`= 0`), делающая класс абстрактным.  

#### **Объявление класса Fruit**
```cpp
class Fruit {
private:
  int Color;
  double Weight;
protected:
  bool FruitIsGood;
public:
  Fruit(int color);
  virtual ~Fruit() {}
  int GetColor() const;
  double GetWeight() const;
  virtual void Eat() = 0;
};
```
---
#### **Реализация методов, конструктора и деструктора**
```cpp
class Fruit {

    //... прошлый код без изменения

    // Функция с реализацией
    int GetColor() const { return Color; }

    // Функция с реализацией, которая может быть переопределена в унаследованном классе
    double GetWeight() const { return Weight; }
};
Fruit::Fruit(int color) 
: Color(color),      // Инициализация Color
  Weight(1.0),       // Инициализация Weight = 1.0
  FruitIsGood(false) // Инициализация FruitIsGood = false
{
  cout << "Creating fruit..." << endl;
}

Fruit::~Fruit() {
  cout << "Deleting fruit..." << endl;
}
```
**Пояснения:**
- **Конструктор**:
  - Использует **список инициализации** для установки значений.
  - Вес всегда инициализируется как `1.0`.
  - Флаг `FruitIsGood` устанавливается в `false`.
  - Выводит сообщение о создании объекта.

- **Деструктор**:
  - Виртуальный (ключевое слово `virtual` в объявлении).
  - Выводит сообщение об удалении объекта.
  - *Важно*: Пустое тело `{}` — деструктор по умолчанию.

Полный код:
```cpp
#include <iostream>
using namespace std;

class Fruit {// Родительский (базовый) класс "Фрукт"
private: // "Закрытые" компоненты (не доступны в унаследованных классах)
    int Color;
    double Weight;
protected: // "Защищенные" компоненты (доступны в унаследованных классах, но не доступны внешнему наблюдателю)
    bool FruitIsGood;
public: // "Открытые" компоненты, определяющие интерфейс класса
    Fruit(int color); // Конструктор
    virtual ~Fruit() {} // Деструктор (объявлен виртуальным, чтобы обеспечить корректное уничтожение унаследованных классов)

    // Функция с реализацией
    int GetColor() const { return Color; }

    // Функция с реализацией, которая может быть переопределена в унаследованном классе
    double GetWeight() const { return Weight; }

    // Абстрактная функция (без реализации)
    virtual void Eat() = 0;

};
// Реализация конструктора
Fruit::Fruit(int color) : Color(color), Weight(1.0), FruitIsGood(false)
{
    cout << "Creating fruit..." << endl;
}
// Реализация деструктора
Fruit::~Fruit()
{
    cout << "Deleting fruit..." << endl;
}

int main()
{
    Fruit *newFruit;
    Fruit *newFruit2;

    return 0;
}
```

---

### **2. Наследование: классы `Apple` и `Orange`**  

#### **1. Класс `Apple` (публичное наследование от `Fruit`)**
```cpp
class Apple : public Fruit {
public:
    Apple();
    ~Apple();
    double GetWeight() const;  // Переопределение НЕ виртуального метода
    void Eat() override;       // Реализация виртуального метода
};
```

##### Конструктор `Apple()`
```cpp
Apple::Apple() : Fruit(1) {
    FruitIsGood = true;
    cout << "Creating apple..." << endl;
}
```
**Особенности:**
1. **Вызов конструктора базового класса**:  
   `: Fruit(1)` - вызывает конструктор `Fruit` с параметром `color=1`.  
   *Важно*: Это единственный способ инициализировать унаследованные приватные поля (`Color`, `Weight`).

2. **Доступ к защищённому полю**:  
   `FruitIsGood = true` - разрешено, так как `protected`-поля доступны в наследниках.

3. **Запрет прямого доступа к приватным полям**:  
   `Color = 1` было бы ошибкой! Приватные поля базового класса недоступны.

4. **Порядок инициализации**:  
   1. Сначала вызывается конструктор `Fruit`.  
   2. Затем выполняется тело конструктора `Apple`.

##### Деструктор `~Apple()`
```cpp
Apple::~Apple() {
    cout << "Deleting apple..." << endl;
}
```
**Особенности:**
- Автоматически вызывает деструктор `~Fruit()` после своего выполнения (благодаря виртуальному деструктору в базе).
- Порядок вызова: `~Apple()` → `~Fruit()`.

##### Переопределение метода `GetWeight()`
```cpp
double Apple::GetWeight() const {
    cout << "Apple weight: " << Fruit::GetWeight() << endl;
    return Fruit::GetWeight();
}
```
**Ключевые моменты:**
1. **Невиртуальный метод**:  
   - В базовом классе объявлен как `double GetWeight() const` (без `virtual`).  
   - Переопределение работает, но **без полиморфизма**!  
   - При вызове через указатель на `Fruit` будет использоваться реализация `Fruit::GetWeight()`.

2. **Явный вызов базовой версии**:  
   `Fruit::GetWeight()` - синтаксис для вызова метода базового класса.

##### Реализация виртуального метода `Eat()`
```cpp
void Apple::Eat() {
    cout << "Eating apple..." << endl;
}
```
**Особенности:**
- Обязательная реализация чисто виртуального метода (`=0` в базе).  
- Без этого класс `Apple` оставался бы абстрактным.  
- Ключевое слово `override` (не показано, но рекомендуется):  
  ```cpp
  void Eat() override { ... }
  ```

#### 2. **Класс `Orange` (публичное наследование от `Fruit`)**
```cpp
class Orange : public Fruit {
public:
    Orange();
    ~Orange();
    void Eat() override;  // Реализация виртуального метода
};
```

##### Конструктор `Orange()`
```cpp
Orange::Orange() : Fruit(2) {
    FruitIsGood = true;
    cout << "Creating orange..." << endl;
}
```
**Отличия от `Apple`:**
- Вызов конструктора `Fruit(2)` (цвет апельсина = 2).  
- Сообщение "Creating orange...".

##### Деструктор `~Orange()`
```cpp
Orange::~Orange() {
    cout << "Deleting orange..." << endl;
}
```
Аналогичен `Apple`, но со своим сообщением.

##### Реализация `Eat()`
```cpp
void Orange::Eat() {
    cout << "Eating orange..." << endl;
}
```
Уникальная реализация для апельсина.


Пример полного кода:  
```cpp
class Apple : public Fruit // Класс-наследник "Яблоко"
{
public:
    Apple();
    ~Apple();

    // Переопределение функции в унаследованном классе (функция не виртуальная!)
    double GetWeight() const;

    // Определение функции в унаследованном классе (функция виртуальная!)
    void Eat();
};
// Реализация конструктора
Apple::Apple() : Fruit(1) //
{
    // Color = 1; // Не заработает, так как нет доступа к Color

    FruitIsGood = true; // Инициализация FruitIsGood через присваивание

    cout << "Creating apple..." << endl;
}

// Реализация деструктора
Apple::~Apple()
{
    cout << "Deleting apple..." << endl;
}

double Apple::GetWeight() const
{
    // Дополнительный функционал, которого не было в родительском классе
    cout << "Apple weight: " << Fruit::GetWeight() << endl;

    // Вызываем реализацию из родительского класса
    return Fruit::GetWeight();
}

void Apple::Eat()
{
    cout << "Eating apple..." << endl;
}


class Orange : public Fruit
{
public:
    Orange();
    ~Orange();

    void Eat();
};

Orange::Orange() : Fruit(2) //
{
    FruitIsGood = true; // Инициализация FruitIsGood через присваивание

    cout << "Creating orange..." << endl;
}

Orange::~Orange()
{
    cout << "Deleting orange..." << endl;
}

void Orange::Eat()
{
    cout << "Eating orange..." << endl;
}
//Пример использования:
int main{
  // Создание яблока как яблока
    Apple newApple1;
  // Вызов функции, реализация которой выполнена в родительском классе
    cout << "Apple color is: " << newApple1.GetColor() << endl;

  // Вызов функции, реализация которой была переопределена в унаследованном классе
    cout << "Calling GetWeight()..." << endl;
    double appleWeight1 = newApple1.GetWeight();
    cout << "Output?" << endl;

  // Вызов виртуальной функции
    newApple1.Eat();

    cout << endl;
}
```
- Protected - доступен родительскому классу наследнику, но недоступная извне
**Побочные эффекты**:  
  - Повторное использование кода через вызов открытых функций родительского класса.  
  - Совместимость интерфейсов: наследники сохраняют все открытые функции родителя.  


---

## **2. Работа с объектами и полиморфизм**  
**Полиморфизм** - это возможность использовать объекты дочерних классов через интерфейс базового класса. В нашем коде это реализовано следующим образом:

#### Условия полиморфизма:
1. **Совместный интерфейс**:  
   Классы `Apple` и `Orange` наследуют все публичные методы `Fruit`, включая `GetColor()`, `GetWeight()` и `Eat()`.

2. **Реализация через указатели/ссылки**:  
   В коде используются указатели на базовый класс:
   ```cpp
   Fruit *newFruit1 = nullptr; // Пустой указатель

   // Инициализация указателя экземпляром унаследованного класса
   newFruit1 = new Apple; // Создание экземпляра класса (создание яблока как фрукта)
   
   Fruit *newFruit2 = new Orange; // Указатель на Orange как на Fruit

   // Создание экземпляра класса с созданием ссылки
   Apple tempApple;
   Fruit& newFruit3 = tempApple;
   ```

#### Динамическое создание объектов:
```cpp

Fruit* newFruit1 = new Apple();  // Создание яблока
cout << "Apple color: " << newFruit1->GetColor() << endl;// Использование интерфейса родительского класса

Fruit* newFruit2 = new Orange(); // Создание апельсина
cout << "Orange color: " << newFruit2->GetColor() << endl; // Использование интерфейса родительского класса

// Вызов функции, реализация которой была переопределена в унаследованном классе
  cout << "Calling GetWeight()..." << endl;
  double appleWeight2 = newFruit1->GetWeight(); // В унаследованном классе предусмотрен вывод в консоль
  double appleWeight3 = newFruit3.GetWeight(); // В унаследованном классе предусмотрен вывод в консоль
```
#### Использование виртуальных методов:
Полиморфное поведение демонстрируется при вызове виртуального метода `Eat()`:
```cpp
newFruit1->Eat();  // Вызывается Apple::Eat()
newFruit2->Eat();  // Вызывается Orange::Eat()
newFruit3.Eat();   // Вызывается Apple::Eat()
```

**Ключевое наблюдение**:  
Метод `GetWeight()` не является виртуальным, поэтому при вызове через указатель на базовый класс всегда вызывается реализация из `Fruit`:
```cpp
newFruit1->GetWeight(); // Всегда Fruit::GetWeight()
```
---
Код:
```cpp
#include <iostream>

using namespace std;


class Fruit // Родительский (базовый) класс "Фрукт"
{
private: // "Закрытые" компоненты (не доступны в унаследованных классах)
  int Color;
  double Weight;

protected: // "Защищенные" компоненты (доступны в унаследованных классах, но не доступны внешнему наблюдателю)
  bool FruitIsGood;

public: // "Открытые" компоненты, определяющие интерфейс класса

  Fruit(int color); // Конструктор
  virtual ~Fruit(); // Деструктор (объявлен виртуальным, чтобы обеспечить корректное уничтожение унаследованных классов)

  // Функция с реализацией
  int GetColor() const { return Color; }

  // Функция с реализацией, которая может быть переопределена в унаследованном классе
  // (можно попробовать сделать ее виртуальной и посмотреть, как будет вести себя программа)
  double GetWeight() const { return Weight; }

  // Абстрактная функция (без реализации)
  virtual void Eat() = 0;
};

// Реализация конструктора
Fruit::Fruit(int color) : Color(color), Weight(1.0), FruitIsGood(false)
{
  cout << "Creating fruit..." << endl;
}

// Реализация деструктора
Fruit::~Fruit()
{
  cout << "Deleting fruit..." << endl;
}

class Apple : public Fruit // Класс-наследник "Яблоко"
{
public:
  Apple();
  ~Apple();

  // Переопределение функции в унаследованном классе (функция не виртуальная!)
  double GetWeight() const;

  // Определение функции в унаследованном классе (функция виртуальная!)
  void Eat();
};

// Реализация конструктора
Apple::Apple() : Fruit(1) //
{
  // Color = 1; // Не заработает, так как нет доступа к Color

  FruitIsGood = true; // Инициализация FruitIsGood через присваивание

  cout << "Creating apple..." << endl;
}

// Реализация деструктора
Apple::~Apple()
{
  cout << "Deleting apple..." << endl;
}

double Apple::GetWeight() const
{
  // Дополнительный функционал, которого не было в родительском классе
  cout << "Apple weight: " << Fruit::GetWeight() << endl;

  // Вызываем реализацию из родительского класса
  return Fruit::GetWeight();
}

void Apple::Eat()
{
  cout << "Eating apple..." << endl;
}

class Orange : public Fruit
{
public:
  Orange();
  ~Orange();

  void Eat();
};

Orange::Orange() : Fruit(2) //
{
  FruitIsGood = true; // Инициализация FruitIsGood через присваивание

  cout << "Creating orange..." << endl;
}

Orange::~Orange()
{
  cout << "Deleting orange..." << endl;
}

void Orange::Eat()
{
  cout << "Eating orange..." << endl;
}

int main()
{
  // Fruit newAbstractFruit; // Не заработает, так как класс абстрактный

  // Создание яблока как яблока
  Apple newApple1;

  // Вызов функции, реализация которой выполнена в родительском классе
  cout << "Apple color is: " << newApple1.GetColor() << endl;

  // Вызов функции, реализация которой была переопределена в унаследованном классе
  cout << "Calling GetWeight()..." << endl;
  double appleWeight1 = newApple1.GetWeight();
  cout << "Output?" << endl;

  // Вызов виртуальной функции
  newApple1.Eat();

  cout << endl;

  // Пример полиморфизма

  Fruit *newFruit1 = nullptr; // Пустой указатель

  // Инициализация указателя экземпляром унаследованного класса
  newFruit1 = new Apple; // Создание экземпляра класса (создание яблока как фрукта)

  // Объявление с одновременной инициализацией (более правильный вариант, чем предыдущий!)
  Fruit *newFruit2 = new Orange;

  // Создание экземпляра класса с созданием ссылки
  Apple tempApple;
  Fruit& newFruit3 = tempApple;

  cout << endl;

  // Использование интерфейса родительского класса
  cout << "Fruit color is: " << newFruit1->GetColor() << endl;
  cout << "Fruit color is: " << newFruit3.GetColor() << endl;

  // Вызов функции, реализация которой была переопределена в унаследованном классе
  cout << "Calling GetWeight()..." << endl;
  double appleWeight2 = newFruit1->GetWeight(); // В унаследованном классе предусмотрен вывод в консоль
  double appleWeight3 = newFruit3.GetWeight(); // В унаследованном классе предусмотрен вывод в консоль
  cout << "Output?" << endl;

  // Вызов виртуальной функции
  newFruit1->Eat();
  newFruit2->Eat();
  newFruit3.Eat();

  cout << endl;

  delete newFruit1;
  delete newFruit2;

  // newFruit3 удалится автоматически, потому что создан статически, а не динамически

  return 0;
}

```
**Вывод:**
```
Creating fruit...
Creating apple...
Apple color is: 1
Calling GetWeight()...
Apple weight: 1
Output?
Eating apple...

Creating fruit...
Creating apple...
Creating fruit...
Creating orange...
Creating fruit...
Creating apple...

Fruit color is: 1
Fruit color is: 1
Calling GetWeight()...
Output?
Eating apple...
Eating orange...
Eating apple...

Deleting apple...
Deleting fruit...
Deleting orange...
Deleting fruit...
Deleting apple...
Deleting fruit...
Deleting apple...
Deleting fruit...
```
---
## **3. Виртуальные функции и абстрактные классы**  
- **Виртуальные функции**:  
  - **Чисто виртуальная функция**: `virtual void eat() = 0;` (не имеет реализации в базовом классе).  
  - Позволяют переопределение в наследниках:
  ```cpp
  // В классе Apple
  void Eat() override {
      cout << "Eating apple..." << endl;
  }
  
  // В классе Orange
  void Eat() override {
      cout << "Eating orange..." << endl;
  }
  ```

- **Абстрактный класс**:  
Класс `Fruit` является абстрактным:
- Содержит чисто виртуальную функцию `Eat()`
- Нельзя создать экземпляр:
  ```cpp
  // Fruit newAbstractFruit; // Ошибка компиляции!
  ```
- Определяет интерфейс для всех фруктов:
  - `GetColor()`
  - `GetWeight()`
  - `Eat()`
---

## **4. Деструктор**  
- **Назначение**: Освобождение ресурсов (память, файлы и т.д.).  
- **Правило**: Деструктор базового класса **должен быть виртуальным**, чтобы корректно удалять объекты наследников через указатель на родителя.  
Проблема:
```cpp
Fruit* p = new Apple();
delete p; // Вызовется только деструктор Fruit!
```

Решение:
```cpp
class Fruit {
public:
  virtual ~Fruit() {
    cout << "Fruit destroyed" << endl;
  } // Виртуальный деструктор
};

```
```cpp
class Apple {
public:
  virtual ~Apple() {
    cout << "Apple destroyed" << endl;
  } 
};

```
---

## **5. Паттерны проектирования**  
- **Определение**:  
  - Типовые решения для часто встречающихся задач в объектно-ориентированном программировании.  
  - Включают: описание задачи, мотивацию, решение, результат, рекомендации.  
- **Особенности**:  
  - **Не являются библиотеками** (это концепции, а не готовый код).  
- **Преимущества**:  
  - Повышение выразительности и краткости кода.  
  - Абстрагирование от деталей реализации.  
  - Единая терминология для разработчиков.  
> Но об этом поподробнее поговорим на следующих лекциях


## **Задание**  
Придумать семейство объектов:  
1. **Родительский объект**:  
   - Минимум 3 функции (+ конструктор, + деструктор).  
   - из них 2 функции — виртуальные.  
2. **Наследники**: 3 объекта, переопределяющие виртуальные методы.  
3. **Программа-реализацию работы**:  
   - Создать экземпляры наследников.  
   - Использовать указатель/ссылку на родительский класс для демонстрации полиморфизма.  
   - Показать работу виртуальных функций и корректное удаление объектов.  
