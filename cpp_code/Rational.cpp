//
//  Rational.cpp

#include "Rational.h"
#include "Polynomial.h"
#include <iostream>

template class Rational<int>;
template class Rational<float>;
template class Rational<double>;
template class Rational<Polynomial>;

template Rational<decltype(double() + int())> Rational<double>::operator+<int>(const Rational<int>& rhs) const;
template Rational<decltype(double() - int())> Rational<double>::operator-<int>(const Rational<int>& rhs) const;
template Rational<decltype(double() * int())> Rational<double>::operator*<int>(const Rational<int>& rhs) const;
template Rational<decltype(double() / int())> Rational<double>::operator/<int>(const Rational<int>& rhs) const;

// Instantiations for Rational<Polynomial> with int
template Rational<decltype(Polynomial() + int())> Rational<Polynomial>::operator+<int>(const Rational<int>& rhs) const;
template Rational<decltype(Polynomial() - int())> Rational<Polynomial>::operator-<int>(const Rational<int>& rhs) const;
template Rational<decltype(Polynomial() * int())> Rational<Polynomial>::operator*<int>(const Rational<int>& rhs) const;
template Rational<decltype(Polynomial() / int())> Rational<Polynomial>::operator/<int>(const Rational<int>& rhs) const;

// Instantiations for Rational<Polynomial> with double
template Rational<decltype(Polynomial() + double())> Rational<Polynomial>::operator+<double>(const Rational<double>& rhs) const;
template Rational<decltype(Polynomial() - double())> Rational<Polynomial>::operator-<double>(const Rational<double>& rhs) const;
template Rational<decltype(Polynomial() * double())> Rational<Polynomial>::operator*<double>(const Rational<double>& rhs) const;
template Rational<decltype(Polynomial() / double())> Rational<Polynomial>::operator/<double>(const Rational<double>& rhs) const;




template <class T>
Rational<T> &Rational<T>::operator=(const Rational &rhs)
{


    value = rhs.value;
    return *this;
}

template <class T>
Rational<T> Rational<T>::operator+(const Rational &rhs)
{
    
    T newValue;
    
    newValue = this->GetValue() + rhs.GetValue();
    Rational<T> Output(newValue);
    return Output;
}

template <class T>
Rational<T> Rational<T>::operator-(const Rational &rhs)
{
    T newValue;
    
    newValue = this->GetValue() - rhs.GetValue();
    Rational<T> Output(newValue);
    return Output;
}

template <class T>
Rational<T> Rational<T>::operator*(const Rational &rhs)
{
    T newValue;
    
    newValue = this->GetValue() * rhs.GetValue();
    Rational<T> Output(newValue);
    return Output;
}

template <class T>
Rational<T> Rational<T>::operator/(const Rational &rhs)
{
    T newValue;
    
    newValue = this->GetValue() / rhs.GetValue();
    Rational<T> Output(newValue);
    return Output;
}


// % operator for Rational<int>
Rational<int> operator%(const Rational<int> &lhs, const Rational<int> &rhs)
{
    int newValue = lhs.GetValue() % rhs.GetValue();
    return Rational<int>(newValue);
}

// % operator for Rational<Polynomial>
Rational<Polynomial> operator%(const Rational<Polynomial> &lhs, const Rational<Polynomial> &rhs)
{
    Polynomial newValue = lhs.GetValue() % rhs.GetValue();
    return Rational<Polynomial>(newValue);
}





// intertype computation

template <class T>
template <typename U>
Rational<decltype(T() + U())> Rational<T>::operator+(const Rational<U>& rhs) const {
    using ResultType = decltype(T() + U());
    ResultType newValue = this->GetValue() + rhs.GetValue();
    return Rational<ResultType>(newValue);
}

template <class T>
template <typename U>
Rational<decltype(T() - U())> Rational<T>::operator-(const Rational<U>& rhs) const {
    using ResultType = decltype(T() - U());
    ResultType newValue = this->GetValue() - rhs.GetValue();
    return Rational<ResultType>(newValue);
}

template <class T>
template <typename U>
Rational<decltype(T() * U())> Rational<T>::operator*(const Rational<U>& rhs) const {
    using ResultType = decltype(T() * U());
    ResultType newValue = this->GetValue() * rhs.GetValue();
    return Rational<ResultType>(newValue);
}

template <class T>
template <typename U>
Rational<decltype(T() / U())> Rational<T>::operator/(const Rational<U>& rhs) const {
    using ResultType = decltype(T() / U());
    ResultType newValue = this->GetValue() / rhs.GetValue();
    return Rational<ResultType>(newValue);
}
