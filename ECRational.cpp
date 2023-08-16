//
//  ECRational.cpp
//
//
//  Created by Yufeng Wu on 1/7/23.
//

#include "ECRational.h"

#include <iostream>

template <class T>

ECRational<T> &ECRational<T>::operator=(const ECRational &rhs)
{

    numerator = rhs.numerator;
    denominator = rhs.denominator;
    return *this;
}

template <class T>
ECRational<T> ECRational<T>::operator+(const ECRational &rhs)
{
    
    T newNumerator;
    T newDenominator;
    
    newNumerator = (this->GetNumerator() * rhs.GetDenominator()) + (rhs.GetNumerator() * this->GetDenominator());
    newDenominator = this->GetDenominator() * rhs.GetDenominator();
    
    ECRational<T> Output(newNumerator, newDenominator);
    return Output;
}

template <class T>
ECRational<T> ECRational<T>::operator-(const ECRational &rhs)
{
    T newNumerator;
    T newDenominator;

    newNumerator = (this->GetNumerator() * rhs.GetDenominator()) - (rhs.GetNumerator() * this->GetDenominator());
    newDenominator = this->GetDenominator() * rhs.GetDenominator();

    ECRational<T> Output(newNumerator, newDenominator);
    return Output;
}

template <class T>
ECRational<T> ECRational<T>::operator*(const ECRational &rhs)
{
    T newNumerator;
    T newDenominator;
    newNumerator = this->GetNumerator() * rhs.GetNumerator() ;
    newDenominator = this->GetDenominator() * rhs.GetDenominator();
    
    ECRational<T> Output(newNumerator, newDenominator);
    return Output;
}

template <class T>
ECRational<T> ECRational<T>::operator/(const ECRational &rhs)
{
    T newNumerator = this->GetNumerator() * rhs.GetDenominator();
    T newDenominator = this->GetDenominator() * rhs.GetNumerator();
    ECRational<T> Output(newNumerator, newDenominator);
    return Output;
}