//
//  Rational.cpp
//
//
//  Created by Yufeng Wu on 1/7/23.
//

#include "Rational.h"

#include <iostream>

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