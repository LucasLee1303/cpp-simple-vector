// вставьте сюда ваш код для класса ArrayPtr
// внесите в него изменения, 
// которые позволят реализовать move-семантику
#pragma once

#include <cstdlib>
#include <algorithm>

template <typename Type>
class ArrayPtr {
public:
    // Инициализирует ArrayPtr нулевым указателем
    ArrayPtr() = default;

    // Создаёт в куче массив из size элементов типа Type.
    // Если size == 0, поле raw_ptr_ должно быть равно nullptr
    explicit ArrayPtr(size_t size) {
        if (size == 0) raw_ptr_ = nullptr;
        else raw_ptr_ = new Type[size];
    }

    // Конструктор из сырого указателя, хранящего адрес массива в куче либо nullptr
    explicit ArrayPtr(Type* raw_ptr) noexcept
        : raw_ptr_(raw_ptr)
    {
    }

    // Запрещаем копирование
    ArrayPtr(const ArrayPtr&) = delete;

    ~ArrayPtr() {
        delete[] raw_ptr_;
    }

    // Запрещаем присваивание
    ArrayPtr& operator=(const ArrayPtr&) = delete;
    
    //Конструктор из временного объекта
    ArrayPtr(ArrayPtr<Type>&& raw) {
     raw_ptr_ = std::swap(raw.raw_ptr_, nullptr);
    }

    //Присвоение временного объекта
    ArrayPtr<Type>& operator=(ArrayPtr<Type>&& raw) {
        if (this != &raw) {
            std::swap(this, raw);
        }
        return *(this);
    }

    // Прекращает владением массивом в памяти, возвращает значение адреса массива
    // После вызова метода указатель на массив должен обнулиться
    [[nodiscard]] Type* Release() noexcept {
        Type* tmp = raw_ptr_;
        raw_ptr_ = nullptr;
        return tmp;
    }

    // Возвращает ссылку на элемент массива с индексом index
    Type& operator[](size_t index) noexcept {
        return raw_ptr_[index];
    }

    // Возвращает константную ссылку на элемент массива с индексом index
    const Type& operator[](size_t index) const noexcept {
        return raw_ptr_[index];
    }

    // Возвращает true, если указатель ненулевой, и false в противном случае
    explicit operator bool() const {
        return raw_ptr_ == nullptr ? false : true;
    }

    // Возвращает значение сырого указателя, хранящего адрес начала массива
    Type* Get() const noexcept {
        return &raw_ptr_[0];
    }

    // Обменивается значениям указателя на массив с объектом other
    void swap(ArrayPtr& other) noexcept {
        std::swap(other.raw_ptr_, raw_ptr_);
    }

private:
    Type* raw_ptr_ = nullptr;
};
