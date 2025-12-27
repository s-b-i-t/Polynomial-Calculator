// RationalComplex.cpp
#include "RationalComplex.h"
#include <algorithm>
#include <cmath>
#include <vector>

RationalComplex::RationalComplex()
    : numerator(ComplexPoly()),
      // Denominator = 1 (real polynomial 1, imag 0)
      denominator(ComplexPoly(Polynomial(std::vector<double>{1.0}))) {
    // already canonical
}

RationalComplex::RationalComplex(const ComplexPoly &num): 
    numerator(num),
    denominator(ComplexPoly(Polynomial(std::vector<double>{1.0})))
    {
    normalize();
}

RationalComplex::RationalComplex(const ComplexPoly &num, const ComplexPoly &den) : 
    numerator(num), 
    denominator(den) 
    {
    normalize();
}

bool RationalComplex::isZero(const ComplexPoly &p, double eps) {
    const std::vector<double> &re = p.getListCoeffsIn();
    const std::vector<double> &im = p.getcomplexCoeffsList();
    std::size_t n = std::max(re.size(), im.size());

    for (std::size_t i = 0; i < n; ++i) {
        double r = (i < re.size() ? re[i] : 0.0);
        double c = (i < im.size() ? im[i] : 0.0);
        if (std::fabs(r) > eps || std::fabs(c) > eps) {
            return false;
        }
    }
    return true;
}

void RationalComplex::normalize() {
    // Denominator cannot be zero
    if (isZero(denominator)) {
        throw std::runtime_error("RationalComplex: zero denominator");
    }

    // If numerator is zero, normalize to 0 / 1
    if (isZero(numerator)) {
        numerator = ComplexPoly(); // 0
        denominator = ComplexPoly(Polynomial(std::vector<double>{1.0}));
        return;
    }

    // For now: no further normalization (no polynomial GCD / monic scaling).
    // You can add that later if you want to simplify P/Q more aggressively.
}

RationalComplex RationalComplex::operator+(const RationalComplex &rhs) const {
    // (a/b) + (c/d) = (a*d + c*b) / (b*d)
    ComplexPoly newNum = numerator * rhs.denominator +
                         rhs.numerator * denominator;
    ComplexPoly newDen = denominator * rhs.denominator;
    return RationalComplex(newNum, newDen);
}

RationalComplex RationalComplex::operator-(const RationalComplex &rhs) const {
    // (a/b) - (c/d) = (a*d - c*b) / (b*d)
    ComplexPoly newNum = numerator * rhs.denominator -
                         rhs.numerator * denominator;
    ComplexPoly newDen = denominator * rhs.denominator;
    return RationalComplex(newNum, newDen);
}

RationalComplex RationalComplex::operator*(const RationalComplex &rhs) const {
    // (a/b) * (c/d) = (a*c) / (b*d)
    ComplexPoly newNum = numerator * rhs.numerator;
    ComplexPoly newDen = denominator * rhs.denominator;
    return RationalComplex(newNum, newDen);
}

RationalComplex RationalComplex::operator/(const RationalComplex &rhs) const {
    // (a/b) / (c/d) = (a*d) / (b*c)
    if (isZero(rhs.numerator)) {
        throw std::runtime_error("RationalComplex: division by zero rational");
    }

    ComplexPoly newNum = numerator * rhs.denominator;
    ComplexPoly newDen = denominator * rhs.numerator;
    return RationalComplex(newNum, newDen);
}

RationalComplex &RationalComplex::operator+=(const RationalComplex &rhs) {
    *this = *this + rhs;
    return *this;
}

RationalComplex &RationalComplex::operator-=(const RationalComplex &rhs) {
    *this = *this - rhs;
    return *this;
}

RationalComplex &RationalComplex::operator*=(const RationalComplex &rhs) {
    *this = *this * rhs;
    return *this;
}

RationalComplex &RationalComplex::operator/=(const RationalComplex &rhs) {
    *this = *this / rhs;
    return *this;
}
