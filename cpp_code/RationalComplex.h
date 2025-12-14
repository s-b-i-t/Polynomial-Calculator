// RationalComplex.h
#ifndef RATIONAL_COMPLEX_H
#define RATIONAL_COMPLEX_H

#include "Complex.h"
#include "Polynomial.h"
#include <stdexcept>

// Rational function with ComplexPoly numerator and denominator:
//   (numerator(x)) / (denominator(x))
class RationalComplex {
public:
    // Default: 0 / 1
    RationalComplex();

    // Construct from a numerator only: num / 1
    explicit RationalComplex(const ComplexPoly &num);

    // Construct from numerator and denominator
    RationalComplex(const ComplexPoly &num, const ComplexPoly &den);

    const ComplexPoly &GetNumerator() const { return numerator; }
    const ComplexPoly &GetDenominator() const { return denominator; }

    // Arithmetic with other rationals
    RationalComplex operator+(const RationalComplex &rhs) const;
    RationalComplex operator-(const RationalComplex &rhs) const;
    RationalComplex operator*(const RationalComplex &rhs) const;
    RationalComplex operator/(const RationalComplex &rhs) const;

    RationalComplex &operator+=(const RationalComplex &rhs);
    RationalComplex &operator-=(const RationalComplex &rhs);
    RationalComplex &operator*=(const RationalComplex &rhs);
    RationalComplex &operator/=(const RationalComplex &rhs);

private:
    void normalize();
    static bool isZero(const ComplexPoly &p, double eps = 1e-9);

    ComplexPoly numerator;
    ComplexPoly denominator;
};

// Optional helpers if you want them later:

// Promote ComplexPoly -> RationalComplex (den = 1)
inline RationalComplex make_rational(const ComplexPoly &p) {
    return RationalComplex(p);
}

// Promote Polynomial -> RationalComplex via ComplexPoly
inline RationalComplex make_rational(const Polynomial &p) {
    ComplexPoly cp(p);
    return RationalComplex(cp);
}

#endif // RATIONAL_COMPLEX_H
