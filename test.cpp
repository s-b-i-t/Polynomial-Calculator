#include <iostream>
#include <cassert>
#include <vector>
#include "ECPolynomial.h"

void testPolynomialAddition(const ECPolynomial& poly1, const ECPolynomial& poly2) {
    ECPolynomial result = poly1 + poly2;
    assert(result.GetCoeff(0) == 4);
    assert(result.GetCoeff(1) == 6);
    assert(result.GetCoeff(2) == 8);
    std::cout << "Passed Addition" << std::endl;
}




void testPolynomialSubtraction(const ECPolynomial& poly1, const ECPolynomial& poly2) {
    ECPolynomial result = poly1 - poly2;
    assert(result.GetCoeff(0) == -2);
    assert(result.GetCoeff(1) == -2);
    assert(result.GetCoeff(2) == -2);
    std::cout << "Passed Subtraction" << std::endl;
}




void testPolynomialMultiplication(const ECPolynomial& poly1, const ECPolynomial& poly2) {
    ECPolynomial result = poly1 * poly2;

    // poly1.Dump();
    // poly2.Dump();
    // result.Dump();


    assert(result.GetCoeff(0) == 3);   // 3
    assert(result.GetCoeff(1) == 10);  // 10x
    assert(result.GetCoeff(2) == 22);   // 22x^2
    assert(result.GetCoeff(3) == 22);   // 22x^3
    assert(result.GetCoeff(4) == 15);   // 15x^4
    std::cout << "Passed Multiplication" << std::endl;
}




void testPolynomialScaling(const ECPolynomial& poly) {
    ECPolynomial result = poly.Scale(2);
    assert(result.GetCoeff(0) == 2);
    assert(result.GetCoeff(1) == 4);
    assert(result.GetCoeff(2) == 6);
    std::cout << "Passed Scaling" << std::endl;
}




void testPolynomialDegree(const ECPolynomial& poly) {
    assert(poly.GetDegree() == 2);
    std::cout << "Passed GetDegree" << std::endl;
}




void testPolynomialDump(const ECPolynomial& poly) {
    std::cout << "Expected: 1 + 2x^1 + 3x^2" << std::endl;
    std::cout << "Actual: ";
    poly.Dump();
    std::cout << "Passed Dump Function" << std::endl;
}




void testPolynomialDivision(const ECPolynomial& poly1, const ECPolynomial& poly2) {
    ECPolynomial result = poly1 / poly2;
    assert(result.GetDegree() == poly1.GetDegree());
    std::cout << "Passed Division" << std::endl;
}




void testPolynomialRemainder(const ECPolynomial& poly1, const ECPolynomial& poly2) {
    ECPolynomial remainder = poly1 % poly2;
    assert(remainder.GetDegree() == poly1.GetDegree());
    std::cout << "Passed Remainder" << std::endl;
}




int main() {
    ECPolynomial poly1({1, 2, 3}); // 3x^2 + 2x + 1
    ECPolynomial poly2({3, 4, 5}); // 5x^2 + 4x + 3

    ECPolynomial poly3({1, 0, -1}); // -x^2 + 1
    ECPolynomial poly4({1, 1});     // x + 1



    testPolynomialAddition(poly1, poly2);
    testPolynomialSubtraction(poly1, poly2);
    testPolynomialMultiplication(poly1, poly2);
    testPolynomialScaling(poly1);
    testPolynomialDegree(poly1);
    testPolynomialDump(poly1);
    testPolynomialDivision(poly3, poly4);
    testPolynomialRemainder(poly3, poly4);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
