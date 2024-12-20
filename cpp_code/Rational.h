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
    Rational() : numerator(T(0)), denominator(T(1)) {}
    
    Rational(const T &numeratorIn, const T &denominatorIn = T(1)) : numerator(numeratorIn), denominator(denominatorIn) {}

    
    // copy constructor
    Rational(const Rational<T> &object) : numerator(object.numerator), denominator(object.denominator) {}
 
    // assignment operator
    Rational<T>& operator=(const Rational &rhs); 
    
    Rational<T> operator+(const Rational &rhs); 
    
    Rational<T> operator-(const Rational &rhs); 

    Rational<T> operator*(const Rational &rhs); 

    Rational<T> operator/(const Rational &rhs);     
    



    // access numerator and denominator
    const T &GetNumerator() const {return numerator;}
    const T &GetDenominator() const {return denominator;}
    
private:

    T numerator;
    T denominator;
};


#endif /* Rational_h */
