#include <iostream>
#include <cassert>
#include <vector>
#include "Polynomial.h"
#include "PolynomialDefinitions.h"
#include "Rational.h"

void testPolynomialAddition() {
    Polynomial result; 
    
    result = poly1 + poly2; // 0 + 1 -> {1}

    
    assert (result.GetCoeff(0) == 1);

    result = poly1 + poly3; // 0 + 3x -> {0, 3}
    assert(result.GetCoeff(0) == 0 && result.GetCoeff(1) == 3);
    
    std::cout << "Passed Addition" << std::endl;

}

void testPolynomialSubtraction() {
    Polynomial result;

    result = poly1 - poly2; // 0 - 1 -> {-1}
    assert(result.GetCoeff(0) == -1);

    result = poly3 - poly4; // 3x - 7x -> {0, -4}
    assert(result.GetCoeff(0) == 0 && result.GetCoeff(1) == -4);

    result = poly5 - poly6; // 2 + 4x - 1 - 5x^2 -> {1, 4, -5}

    assert(result.GetCoeff(0) == 1 && result.GetCoeff(1) == 4 && result.GetCoeff(2) == -5);

    std::cout << "Passed Subtraction" << std::endl;
}


void testPolynomialMultiplication() {
    Polynomial result;

    result = poly2 * poly3; // 1 * 3x -> {0, 3}
    assert(result.GetCoeff(0) == 0 && result.GetCoeff(1) == 3);

    // testing to just scale via multiplication (works good)
    Polynomial poly1000({5});
    result = poly1000 * poly5 ; // 5 * (2 + 4x) -> {10, 20}
    assert(result.GetCoeff(0) == 10 && result.GetCoeff(1) == 20);
    Polynomial poly1001({7});
    result = poly1000 * poly1001; // 5 * 7 -> {35}
    assert(result.GetCoeff(0) == 35);





    result = poly4 * poly5; // 7x * (2 + 4x) -> {0, 14, 28}
    assert(result.GetCoeff(0) == 0 && result.GetCoeff(1) == 14 && result.GetCoeff(2) == 28);

    result = poly6 * poly7; // (1 + 5x^2) * (3 + 2x^2) -> {3, 0, 17, 0, 10}
    assert(result.GetCoeff(0) == 3 && result.GetCoeff(1) == 0 && result.GetCoeff(2) == 17 && result.GetCoeff(3) == 0 && result.GetCoeff(4) == 10);

    result = poly8 * poly9; // (1 + 2x + 3x^2) * (4 + 5x + 6x^2) -> {4, 13, 28, 27, 18}
    assert(result.GetCoeff(0) == 4 && result.GetCoeff(1) == 13 && result.GetCoeff(2) == 28 && result.GetCoeff(3) == 27 && result.GetCoeff(4) == 18);

    std::cout << "Passed Multiplication" << std::endl;
}

void testPolynomialDivision() {
    Polynomial result;

    result = poly10 / poly2; // (12 + 32x + 4x^2) / 1 -> {12, 32, 4}
    assert(result.GetCoeff(0) == 12 && result.GetCoeff(1) == 32 && result.GetCoeff(2) == 4);
    
    result = poly12 / poly3; // (5 + 6x + 3x^3) / 3x -> {2, 0, 1}
    assert(result.GetCoeff(0) == 2 && result.GetCoeff(1) == 0  && result.GetCoeff(2) == 1);

    std::cout << "Passed Division" << std::endl;
}


void testPolynomialRemainder() {
    Polynomial remainder;

    remainder = poly10 % poly2; // (12 + 32x + 4x^2) % 1 -> {0}
    assert(remainder.GetDegree() == 0 && remainder.GetCoeff(0) == 0);

    remainder = poly12 % poly3; // (5 + 6x + 3x^3) % 3x -> {5}
    assert(remainder.GetDegree() == 0 && remainder.GetCoeff(0) == 5);

    remainder = poly13 % poly7; // (1 + 2x + 3x^2 + 5x^4) % (3 + 2x^2) -> {7.75, 2}
    assert(remainder.GetDegree() == 1 && remainder.GetCoeff(0) == 7.75 && remainder.GetCoeff(1) == 2);

    std::cout << "Passed Remainder" << std::endl;
}

void testRational() {
    Rational<int> r1(2);    // 1/1
    Rational<int> r2(1);    // 2/1
    Rational<int> r3;       // 0/1

    r3 = r1 % r2;
    std::cout << r3.GetVal().GetCoeff(0) << std::endl;
    Rational<Polynomial> r4(poly13);
    Rational<Polynomial> r5(poly7);
    Rational<Polynomial> r6(5);
    
    r6 = r4 % r5;
    // std::cout << r6.GetNumerator().GetCoeff(0) << std::endl;


}


int main() {

    testPolynomialAddition();
    testPolynomialSubtraction();
    testPolynomialMultiplication();
    testPolynomialDivision();
    testPolynomialRemainder();
    testRational();
     std::cout << "All tests passed" << std::endl;
    return 0;

}
