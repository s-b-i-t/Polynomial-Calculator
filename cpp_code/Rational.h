//  Rational.h
// works for Polynomial, Complex, int, double, float
// does not simplify ratio eg 2/4 stays 2/4 not 1/2 or 3/1 stays 3/1 not 3

#ifndef Rational_h
#define Rational_h
#include "Polynomial.h"

template <class T>
class Rational
{
public:
    Rational() : value(T(0))  {}
    
    Rational(const T &valueIn) : value(valueIn) {}
    
    // copy constructor
    Rational(const Rational<T> &object) : value(object.value) {}


    // assignment operator
    Rational<T>& operator=(const Rational &rhs); 
    
    Rational<T> operator+(const Rational &rhs); 
    
    Rational<T> operator-(const Rational &rhs); 

    Rational<T> operator*(const Rational &rhs); 

    Rational<T> operator/(const Rational &rhs); 



    // intertype computation

    template <typename U>
    Rational<decltype(T() + U())> operator+(const Rational<U>& rhs) const;

    template <typename U>
    Rational<decltype(T() - U())> operator-(const Rational<U>& rhs) const;

    template <typename U>
    Rational<decltype(T() * U())> operator*(const Rational<U>& rhs) const;

    template <typename U>
    Rational<decltype(T() / U())> operator/(const Rational<U>& rhs) const;


    
    // access VALUE
    const T &GetValue() const {return value;}
    
private:

    T value;
};

    // modulo only supported for int and polynomial so outside class definition
    Rational<int> operator%(const Rational<int> &lhs, const Rational<int> &rhs);

    Rational<Polynomial> operator%(const Rational<Polynomial> &lhs, const Rational<Polynomial> &rhs);

#endif /* Rational_h */
