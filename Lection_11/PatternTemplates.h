#ifndef PatternTemplatesH
#define PatternTemplatesH

// Шаблонный класс "Стек"

const size_t MaxSize = 100;

template <typename T> // T - подставляемый тип данных
class StackClass
{
private:

    // Собственно контейнер, содержащий данные типа T
    T Items[MaxSize];

    // Индекс элемента стека, следующего за верхним
    size_t Top;

public:

    // Проверка того, является ли стек пустым
    bool IsEmpty() const { return (Top == 0); }

    // Определение размера стека
    size_t Size() const { return Top; }

    // Поместить в стек новый элемент
    void Push(T newObject) { Items[Top++] = newObject; }

    // Извлечь из списка верхний элемент
    T Pop() { return Items[--Top]; }

    // Получение элемента по индексу
    T GetElementByIndex(size_t index) const { return Items[index]; }

    // Конструктор
    StackClass() : Top(0) {}
};

// Шаблонный класс "Массив"

template <typename T>
class ArrayClass
{
private:

    T Items[MaxSize];
    size_t ArraySize;

public:

    void Add(T newObject) { Items[ArraySize++] = newObject; }
    size_t Size() const { return ArraySize; }

    // Первый вариант получения элемента по индексу (перегрузка оператора [])
    T operator[](size_t index) const { return Items[index]; }

    // Второй вариант получения элемента по индексу
    T GetElement(size_t index) const { return Items[index]; }

    ArrayClass() : ArraySize(0) {}
};

// Шаблонный класс, реализующий паттерн "Итератор"
// (класс абстрактный, так как содержит функции без реализации)

template<typename T> // T - подставляемый тип данных (элемент контейнера)
class Iterator
{
protected:
    Iterator() {}

public:
    virtual ~Iterator() {}
    virtual void First() = 0; // Спозиционироваться на начале контейнера
    virtual void Next() = 0;  // Перейти к следующему элементу
    virtual bool IsDone() const = 0;  // Проверить, достигнут ли конец
    virtual T GetCurrent() const = 0; // Получить текущий элемент
};

// Реализация паттерна "Итератор",
// предназначенная для обхода массива

template<typename T>
class ArrayIterator : public Iterator<T>
{
private:
    const ArrayClass<T> *ArrayContainer; // Указатель на массив
    size_t Pos; // Текущее положение в массиве

public:
    ArrayIterator(ArrayClass<T> *container)
    : ArrayContainer(container), Pos(0) {}

    void First() override { Pos = 0; }
    void Next() override { Pos++; }
    bool IsDone() const override { return (Pos >= ArrayContainer->Size()); }
    T GetCurrent() const override { return ArrayContainer->GetElement(Pos); }
};

// Реализация паттерна "Итератор",
// предназначенная для обхода стека

template<typename T>
class StackIterator : public Iterator<T>
{
private:
    const StackClass<T> *StackContainer; // Указатель на массив
    size_t Pos; // Текущее положение в массиве

public:
    StackIterator(StackClass<T> *container)
    : StackContainer(container), Pos(0) {}

    void First() override { Pos = 0; }
    void Next() override { Pos++; }
    bool IsDone() const override { return (Pos >= StackContainer->Size()); }
    T GetCurrent() const override { return StackContainer->GetElementByIndex(Pos); }
};

#endif // PatternTemplatesH
