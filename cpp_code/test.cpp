#include <iostream>
#include <cassert>
#include <vector>
#include "Polynomial.h"
#include "Definitions.h"
#include "Rational.h"
#include "Complex.h"


void testIntegerAddition(){

    // Rational<int> myint(1);
    Rational<Polynomial> int1(Polynomial({1}));
    
    Rational<Polynomial> int2(Polynomial({3}));
    Rational<Polynomial> result = int1 + int2;

     result.GetNumerator().Dump() ;
     result.GetDenominator().Dump() ;
    
    
    
}

void testDoubleAddition(){
    Rational<Polynomial> double1(Polynomial({2.5}));
    Rational<Polynomial> double2(Polynomial({3.5}));
    Rational<Polynomial> result = double1 + double2;

    assert(result.GetNumerator().GetCoeff(0) == 6.0);

}

void testPolynomialAddition(){
    Rational<Polynomial> poly1(Polynomial({1, 2, 3}));
    Rational<Polynomial> poly2(Polynomial({3}));
    Rational<Polynomial> result = poly1 + poly2;

    assert(result.GetNumerator().GetCoeff(0) == 4.0);
    assert(result.GetNumerator().GetCoeff(1) == 2.0);
    assert(result.GetNumerator().GetCoeff(2) == 3.0);
}


void testIntegerSubtraction(){
    Rational<Polynomial> int1(Polynomial({1}));
    Rational<Polynomial> int2(Polynomial({3}));
    Rational<Polynomial> result = int1 - int2;

    assert(result.GetNumerator().GetCoeff(0) == -2);
}

void testDoubleSubtraction(){
    Rational<Polynomial> double1(Polynomial({2.5}));
    Rational<Polynomial> double2(Polynomial({3.5}));
    Rational<Polynomial> result = double1 - double2;

    assert(result.GetNumerator().GetCoeff(0) == -1.0);
}

void testPolynomialSubtraction(){
    Rational<Polynomial> poly1(Polynomial({1, 2, 3}));
    Rational<Polynomial> poly2(Polynomial({3}));
    Rational<Polynomial> result = poly1 - poly2;

    assert(result.GetNumerator().GetCoeff(0) == -2.0);
    assert(result.GetNumerator().GetCoeff(1) == 2.0);
    assert(result.GetNumerator().GetCoeff(2) == 3.0);
}


void testIntegerMultiplication(){
    Rational<Polynomial> int1(Polynomial({1}));
    Rational<Polynomial> int2(Polynomial({3}));
    Rational<Polynomial> result = int1 * int2;

    assert(result.GetNumerator().GetCoeff(0) == 3.0);
}


void testDoubleMultiplication(){
    Rational<Polynomial> double1(Polynomial({2.5}));
    Rational<Polynomial> double2(Polynomial({3.5}));
    Rational<Polynomial> result = double1 * double2;

    assert(result.GetNumerator().GetCoeff(0) == 8.75);
}


void testPolynomialMultiplication(){
    Rational<Polynomial> poly1(Polynomial({1, 2, 3}));
    Rational<Polynomial> poly2(Polynomial({3}));
    Rational<Polynomial> result = poly1 * poly2;

    assert(result.GetNumerator().GetCoeff(0) == 3.0);
    assert(result.GetNumerator().GetCoeff(1) == 6.0);
    assert(result.GetNumerator().GetCoeff(2) == 9.0);
}

void testIntegerDivision(){
    Rational<Polynomial> poly1(Polynomial({1}));
    Rational<Polynomial> poly2(Polynomial({3}));
    Rational<Polynomial> result = poly1 / poly2;

    assert(result.GetNumerator().GetCoeff(0) == 1);
    assert(result.GetDenominator().GetCoeff(0) == 3);
}

void testDoubleDivision(){
    Rational<Polynomial> poly1(Polynomial({2.5}));
    Rational<Polynomial> poly2(Polynomial({3.5}));
    Rational<Polynomial> result = poly1 / poly2;

    assert(result.GetNumerator().GetCoeff(0) == 2.5);
    assert(result.GetDenominator().GetCoeff(0) == 3.5);
}

void testPolynomialDivision(){
    Rational<Polynomial> poly1(Polynomial({1, 2, 3}));
    Rational<Polynomial> poly2(Polynomial({3}));
    Rational<Polynomial> result = poly1 / poly2;

    assert(result.GetNumerator().GetCoeff(0) == 1.0);
    assert(result.GetNumerator().GetCoeff(1) == 2.0);
    assert(result.GetNumerator().GetCoeff(2) == 3.0);
    assert(result.GetDenominator().GetCoeff(0) == 3.0);
}


void testIntegerRemainder(){
    ;
}

void testPolynomialRemainder(){
    ;
}


void moreTesting(){
    ;
}



void testPolynomialOperations() {
    std::cout << "Testing Polynomial Operations..." << std::endl;

    Polynomial poly1({1, 2, 3}); // Represents 1 + 2x + 3x^2
    Polynomial poly2({3, 4});    // Represents 3 + 4x

    // Addition
    Polynomial polyAdd = poly1 + poly2;
    std::cout << "poly1 + poly2 = ";
    for (double coeff : polyAdd.getListCoeffsIn()) {
        std::cout << coeff << " ";
    }
    std::cout << std::endl;

    // Subtraction
    Polynomial polySub = poly1 - poly2;
    std::cout << "poly1 - poly2 = ";
    for (double coeff : polySub.getListCoeffsIn()) {
        std::cout << coeff << " ";
    }
    std::cout << std::endl;

    // Multiplication
    Polynomial polyMul = poly1 * poly2;
    std::cout << "poly1 * poly2 = ";
    for (double coeff : polyMul.getListCoeffsIn()) {
        std::cout << coeff << " ";
    }
    std::cout << std::endl;

    // Division (assuming Polynomial division is implemented)
    Polynomial polyDiv = poly1 / poly2;
    std::cout << "poly1 / poly2 = ";
    for (double coeff : polyDiv.getListCoeffsIn()) {
        std::cout << coeff << " ";
    }
    std::cout << std::endl;

    std::cout << "Polynomial Operations Test Completed!" << std::endl;
}

void testComplexPolyInitialization() {
    std::cout << "\nTesting ComplexPoly Initialization..." << std::endl;

    ComplexPoly cp1; // Default initialization
    std::cout << "cp1: ";
    for (double coeff : cp1.getListCoeffsIn()) {
        std::cout << coeff << " ";
    }
    std::cout << std::endl;

    std::vector<double> polyPart = {1, 0, 2};       // Represents 1 + 2x^2
    std::vector<double> complexPart = {0.5, -0.5};  // Complex roots at 0.5 and -0.5
    ComplexPoly cp2(polyPart, complexPart);
    std::cout << "cp2: ";
    for (double coeff : cp2.getListCoeffsIn()) {
        std::cout << coeff << " ";
    }
    std::cout << std::endl;
    std::cout << "Complex parts: ";
    for (double coeff : cp2.getComplexPolyCoeffLocations()) {
        std::cout << coeff << " ";
    }
    std::cout << std::endl;

    std::cout << "ComplexPoly Initialization Test Completed!" << std::endl;
}

void testComplexPolyOperations() {
    std::cout << "\nTesting ComplexPoly Operations..." << std::endl;

    ComplexPoly cp1({1, 2, 3}, {0.5, -0.5});
    ComplexPoly cp2({3, 4}, {1.0});

    // Addition
    ComplexPoly cpAdd = cp1 + cp2;
    std::cout << "cp1 + cp2 = ";
    for (double coeff : cpAdd.getListCoeffsIn()) {
        std::cout << coeff << " ";
    }
    std::cout << std::endl;

    // Subtraction
    ComplexPoly cpSub = cp1 - cp2;
    std::cout << "cp1 - cp2 = ";
    for (double coeff : cpSub.getListCoeffsIn()) {
        std::cout << coeff << " ";
    }
    std::cout << std::endl;

    // Multiplication
    ComplexPoly cpMul = cp1 * cp2;
    std::cout << "cp1 * cp2 = ";
    for (double coeff : cpMul.getListCoeffsIn()) {
        std::cout << coeff << " ";
    }
    std::cout << std::endl;

    // Division (assuming ComplexPoly division is implemented)
    ComplexPoly cpDiv = cp1 / cp2;
    std::cout << "cp1 / cp2 = ";
    for (double coeff : cpDiv.getListCoeffsIn()) {
        std::cout << coeff << " ";
    }
    std::cout << std::endl;

    std::cout << "ComplexPoly Operations Test Completed!" << std::endl;
}

void testMixedOperations() {
    std::cout << "\nTesting Mixed Polynomial and ComplexPoly Operations..." << std::endl;

    Polynomial poly({1, -1}); // Represents 1 - x
    ComplexPoly cp({2, 3}, {0.2, -0.2});

    // Addition
    ComplexPoly mixedAdd = cp + poly;
    std::cout << "cp + poly = ";
    for (double coeff : mixedAdd.getListCoeffsIn()) {
        std::cout << coeff << " ";
    }
    std::cout << std::endl;

    // Subtraction
    ComplexPoly mixedSub = poly - cp;
    std::cout << "poly - cp = ";
    for (double coeff : mixedSub.getListCoeffsIn()) {
        std::cout << coeff << " ";
    }
    std::cout << std::endl;

    // Multiplication
    ComplexPoly mixedMul = cp * poly;
    std::cout << "cp * poly = ";
    for (double coeff : mixedMul.getListCoeffsIn()) {
        std::cout << coeff << " ";
    }
    std::cout << std::endl;

    // Division
    ComplexPoly mixedDiv = poly / cp;
    std::cout << "poly / cp = ";
    for (double coeff : mixedDiv.getListCoeffsIn()) {
        std::cout << coeff << " ";
    }
    std::cout << std::endl;

    std::cout << "Mixed Operations Test Completed!" << std::endl;
}




int main() {

    testIntegerAddition();
    testDoubleAddition();
    testPolynomialAddition();

    std::cout << "Addition passed" << std::endl;
    

    testIntegerSubtraction();
    testDoubleSubtraction();
    testPolynomialSubtraction();

    std::cout << "Subtraction passed" << std::endl;

    testIntegerMultiplication();
    testDoubleMultiplication();
    testPolynomialMultiplication();

    std::cout << "Multiplication passed" << std::endl;

    testIntegerDivision();
    testDoubleDivision();
    testPolynomialDivision();

    std::cout << "Division passed" << std::endl;

    testIntegerRemainder();
    testPolynomialRemainder();

    std::cout << "Remainder passed" << std::endl;

    moreTesting();



    testPolynomialOperations();
    testComplexPolyInitialization();
    testComplexPolyOperations();
    testMixedOperations();


     std::cout << "All tests passed" << std::endl;
    return 0;

}
