
# Лекция 12: Паттерн проектирование "Декоратор" и "Адаптер". 

## Паттерн проектирования - "Декоратор"

#### Задача
Требуется реализовать различные стратегии обхода контейнера с фруктами:
1. **Все фрукты** - без фильтрации
2. **Только хорошие** - пропускать испорченные
3. **Только зеленые** - пропускать незеленые
4. **Только хорошие зеленые** - комбинация условий

**Прямая реализация приводит к дублированию кода и нарушению принципа открытости/закрытости.**

### Небольшие изменения кода перед работой:

**Для удобства в Fruit сделали реализацию функции Eat:**

- в Fruit сделали реализацию функции Eat:
```cpp
    virtual void Eat() 
	{
		if(IsGood()){
			cout << Good! << endl;
		}
		else{
			cout << Bad! << endl;
		}
		
	};
```
---
- Добавили enum class цветов
```cpp
enum class FruitColor : int 
{
	Green,
	Orange,
	Red,
	Unknown
};
```
---
- Реализация конструктора яблока и говорим что оно зелёное, так же для киви и апельсина
```cpp
// Реализация конструктора
Apple::Apple() : Fruit(FruitColor::Green){}

Kiwi::Kiwi() : Fruit(FruitColor::Green;){}

Orange::Orange() : Fruit(FruitColor::Orange){}
```
---
- Также вызвали функцию Eat из родительского класса в унаследованных:
```cpp
void Apple::Eat()
{
    Fruit::Eat(); // Вызов функции, определенной в базовом классе
    cout << "Eating apple..." << endl;
}
void Kiwi::Eat()
{
    Fruit::Eat(); // Вызов функции, определенной в базовом классе
    cout << "Eating kiwi..." << endl;
}

void Orange::Eat()
{
    Fruit::Eat(); // Вызов функции, определенной в базовом классе
    cout << "Eating orange..." << endl;
}
```
---
### Решение 1: Модификация функций обхода

**1. Базовая функция обхода (все фрукты):**
```cpp
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
```

**2. Функция только для хороших фруктов:**
```cpp
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
```

**3. Функция только для зеленых фруктов:**
```cpp
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
```

**4. Комбинированная функция:**
```cpp
void EatEmAllGoodGreen(Iterator<Fruit*> *it) {
    for(it->First(); !it->IsDone(); it->Next()) {
        Fruit *currentFruit = it->GetCurrent();
        if(!currentFruit->IsGood()) continue;
        if(currentFruit->GetColor() != FruitColor::Green) continue;
        currentFruit->Eat();
    }
}
```
---
## Решение: Паттерн "Декоратор"
Декоратор - структурный паттерн, который:
1. Динамически добавляет новую функциональность объектам
2. Обеспечивает гибкую альтернативу наследованию
3. Сохраняет интерфейс исходного класса

> Декоратор-класс оборачивающийся вокруг класса и меняеющий его свойства, его интрефейс полность совпадает 
Например: функция first, если хотим сделать так чтобы вывовдило только зелёные, то прыгать пока не появится первый зелёный
		  
### Реализация декоратора для итераторов
#### 1. Базовый класс декоратора
```cpp
template<typename T> 
class IteratorDecorator : public Iterator<T>
{
protected:
	Iterator<T> *It;(то что сделалаи называется инкапсюляция(вложили внутрь))
public:
	IteratorDecorator(Iterator<T> *it) : It(it) {} // конструктор
	virtual ~IteratorDecorator() (delete It; )
	virtual void First() { It->First(); }
	virtual void Next() { It->Next(); }
	virtual bool IsDone() const { return It->IsDone(); }
	virtual T GetCurrent() const { return It->GetCurrent(); }
	
};
```
#### 2. Конкретные декораторы
**Декоратор по цвету:**
```cpp
class FruitColorDecorator : public  IteratorDecorator<class Fruit*> 
{
private: 
	FruitColor TargetColor;
public:
	FruitColorDecorator(Iterator<Fruit*> *it, FruitColor color) //написали конструктор
	: TargetColor(color), IteratorDecorator<Fruit*>(it) {}
	
	void First() override 
	{
		It->First(); сначала вызываем фукнцию ферст итератора
		while(!It->IsDone() && It->GetCurrent()->GetColor() != TargetColor)
		{
			It->Next();
		}
	}
	
	void Next() override 
	{
		do
		{
			It->Next();
			
		}while(!It->IsDone() && It->GetCurrent()->GetColor() != TargetColor):
	}
};
```

**Декоратор по качеству:**
```cpp
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
```


### Различные стратегии обхода

**Все фрукты:**
```cpp
Iterator<Fruit*> *baseIt = new ArrayIterator<Fruit*>(&fruitArray);
EatEmAll(baseIt);
delete baseIt;
```

**Только хорошие фрукты:**
```cpp
Iterator<Fruit*> *goodIt = new FruitGoodDecorator(
    new ArrayIterator<Fruit*>(&fruitArray), 
    true
);
EatEmAll(goodIt);
delete goodIt;
```

**Только зеленые фрукты:**
```cpp
Iterator<Fruit*> *greenIt = new FruitColorDecorator(
    new ArrayIterator<Fruit*>(&fruitArray),
    FruitColor::Green
);
EatEmAll(greenIt);
delete greenIt;
```

**Только хорошие зеленые:**
```cpp
Iterator<Fruit*> *comboIt = new FruitColorDecorator(
    new FruitColorDecorator(
        new ArrayIterator<Fruit*>(&fruitArray),
        true
    ),
    FruitColor::Green
);
EatEmAll(comboIt);
delete comboIt;
```

### Преимущества паттерна "Декоратор"

1. **Гибкость комбинирования**:
   - Возможность создавать цепочки декораторов
   - Комбинации создаются динамически в runtime

2. **Соблюдение OCP**:
   - Новые фильтры добавляются без изменения существующего кода
   - Не требуется модифицировать базовый итератор

3. **Единая точка изменения**:
   - Логика фильтрации инкапсулирована в одном месте

4. **Прозрачность**:
   - Клиентский код работает с декоратором как с обычным итератором
   - Нет необходимости в специальной обработке

---
## Паттерн "Адаптер"

#### Проблема
В коде используются два типа контейнеров:
1. Кастомный `ArrayClass`
2. Стандартный `list` из STL

У них разные интерфейсы для итерации:
```cpp
// Кастомный контейнер
Iterator<Fruit*>* it = fruitArray.GetIterator();

// STL контейнер
auto it = fruitVector.begin();
```

Как использовать единый интерфейс `Iterator` для работы с обоими типами контейнеров?

### Решение: Паттерн "Адаптер"
Адаптер - структурный паттерн, который:
1. Преобразует интерфейс одного класса в интерфейс другого
2. Позволяет работать с разными классами через единый интерфейс
3. Решает проблему несовместимости интерфейсов

### Реализация адаптера для STL-контейнеров

```cpp
template<typename ContainerType, typename ItemType>
class ConstIteratorAdapter : public Iterator<ItemType>
{
protected:
    ContainerType *Container;
    typename ContainerType::const_iterator It;

public:
    ConstIteratorAdapter(ContainerType *container)
    : Container(container)
    {
        It = Container->begin();
    }

    virtual ~ConstIteratorAdapter() {}
    
    virtual void First() override { 
        It = Container->begin(); 
    }
    
    virtual void Next() override { 
        It++; 
    }
    
    virtual bool IsDone() const override { 
        return (It == Container->end()); 
    }
    
    virtual ItemType GetCurrent() const override { 
        return *It; 
    }
};
```

#### Ключевые компоненты:

1. **Шаблонные параметры**:
   - `ContainerType`: Тип STL-контейнера (list, vector и т.д.)
   - `ItemType`: Тип элементов в контейнере (Fruit*)

2. **Хранение контейнера**:
   ```cpp
   ContainerType *Container;  // Указатель на адаптируемый контейнер
   ```
   - Не владеет контейнером, только ссылается на него

3. **STL-итератор**:
   ```cpp
   typename ContainerType::const_iterator It;
   ```
   - Использует встроенный итератор контейнера
   - `const_iterator` гарантирует неизменяемость элементов

4. **Реализация интерфейса Iterator**:
   - `First()`: Сбрасывает итератор в начало (`begin()`)
   - `Next()`: Перемещает на следующий элемент (`operator++`)
   - `IsDone()`: Проверяет достижение конца (`end()`)
   - `GetCurrent()`: Возвращает текущий элемент (`operator*`)

### Использование адаптера в коде

#### 1. Создание STL-контейнера
```cpp
list<Fruit*> fruitVector; // а еще можно vector, forward_list, ...
for(size_t i=0; i<N; i++)
{
	int fruit_num = rand()%3+1; // Число от 1 до 3 (случайный фрукт)
	FruitType fruit_type = static_cast<FruitType>(fruit_num);
	Fruit *newFruit = CreateFruit(fruit_type);
	fruitVector.push_back(newFruit); // Добавить новый фрукт в конец списка
}
```

#### 2. Создание адаптера
```cpp
Iterator<Fruit*> *adaptedIt = 
    new ConstIteratorAdapter<std::list<Fruit*>, Fruit*>(&fruitVector);
```

#### 3. Использование с декораторами
```cpp
Iterator<Fruit*> *adaptedOrangeIt = 
    new FruitGoodDecorator(
        new FruitColorDecorator(adaptedIt, FruitColor::Orange), 
        true
    );

EatEmAll(adaptedOrangeIt);
```

#### 4. Освобождение ресурсов
```cpp
delete adaptedOrangeIt; // Автоматически удаляет адаптер и декораторы
```

### Преимущества паттерна "Адаптер"

1. **Интеграция legacy-кода**:
   - Позволяет использовать старые компоненты с новой системой

2. **Работа со сторонними библиотеками**:
   - Адаптация интерфейсов библиотек (STL, Boost и т.д.)

3. **Единообразие интерфейсов**:
   - Упрощает систему, скрывая разнородные реализации

4. **Соблюдение OCP**:
   - Новые типы контейнеров добавляются без изменения клиентского кода

## Задание:
1. Реализовать создание итератора во всех разработанных ранее контейнерах при помощи функции GetIterator, предварительно объединив все контейнеры общим интерфейсом (т. е. унаследовать их от одного общего класса с виртуальной функцией GetIterator).
2. Реализовать минимум 3 декоратора для итераторов, продемонстрировать их работоспособность.
3. Реализовать адаптер для итератора в стиле STL, продемонстрировать его работоспособность.
