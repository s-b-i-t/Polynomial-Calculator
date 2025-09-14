//
//  Rational.cpp

#include "Rational.h"
#include "Polynomial.h"
#include <iostream>
#include <complex>

using std::complex;

template class Rational<Polynomial>;
template class Rational<complex<double>>;
// template class 


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

