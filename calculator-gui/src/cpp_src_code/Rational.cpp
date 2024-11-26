//
//  Rational.cpp

#include "Rational.h"
#include "Polynomial.h"
#include <iostream>

template class Rational<int>;
template class Rational<float>;
template class Rational<double>;
template class Rational<Polynomial>;



template <class T>
Rational<T> &Rational<T>::operator=(const Rational &rhs)
{

    numerator = rhs.numerator;
    denominator = rhs.denominator;
    return *this;
}

template <class T>
Rational<T> Rational<T>::operator+(const Rational &rhs)
{
    
    T newNumerator;
    T newDenominator;
    
    newNumerator = (this->GetNumerator() * rhs.GetDenominator()) + (rhs.GetNumerator() * this->GetDenominator());
    newDenominator = this->GetDenominator() * rhs.GetDenominator();
    
    Rational<T> Output(newNumerator, newDenominator);
    return Output;
}

template <class T>
Rational<T> Rational<T>::operator-(const Rational &rhs)
{
    T newNumerator;
    T newDenominator;

    newNumerator = (this->GetNumerator() * rhs.GetDenominator()) - (rhs.GetNumerator() * this->GetDenominator());
    newDenominator = this->GetDenominator() * rhs.GetDenominator();

    Rational<T> Output(newNumerator, newDenominator);
    return Output;
}

template <class T>
Rational<T> Rational<T>::operator*(const Rational &rhs)
{
    T newNumerator;
    T newDenominator;
    newNumerator = this->GetNumerator() * rhs.GetNumerator() ;
    newDenominator = this->GetDenominator() * rhs.GetDenominator();
    
    Rational<T> Output(newNumerator, newDenominator);
    return Output;
}

template <class T>
Rational<T> Rational<T>::operator/(const Rational &rhs)
{
    T newNumerator = this->GetNumerator() * rhs.GetDenominator();
    T newDenominator = this->GetDenominator() * rhs.GetNumerator();
    Rational<T> Output(newNumerator, newDenominator);
    return Output;
}


// % operator for Rational<int>
Rational<int> operator%(const Rational<int> &lhs, const Rational<int> &rhs)
{
    int newNumerator = lhs.GetNumerator() % rhs.GetNumerator();
    return Rational<int>(newNumerator, lhs.GetDenominator());
}

// % operator for Rational<Polynomial>
Rational<Polynomial> operator%(const Rational<Polynomial> &lhs, const Rational<Polynomial> &rhs)
{
    Polynomial newNumerator = lhs.GetNumerator() % rhs.GetNumerator();
    return Rational<Polynomial>(newNumerator, lhs.GetDenominator());
}