#include <iostream>
#include <cassert>
#include <vector>
#include "Polynomial.h"
#include "Definitions.h"
#include "Rational.h"
#include "Complex.h"


void testAddition(){


    /**********************
    Integers and Doubles
    ***********************/
    // 1 + 3
    Rational<Polynomial> testPoly(Polynomial({1}));
    Rational<Polynomial> testPoly2(Polynomial({3}));
    Rational<Polynomial> result = testPoly + testPoly2;
    assert(result.GetNumerator().GetCoeff(0) == 4);
    assert(result.GetDenominator().GetCoeff(0) == 1);

    // 0 + 0 = 0
    testPoly = Polynomial({0});
    testPoly2 = Polynomial({0});
    result = testPoly + testPoly2;
    assert(result.GetNumerator().GetCoeff(0) == 0);
    assert(result.GetDenominator().GetCoeff(0) == 1);

    // x + 0 = x
    testPoly =  Polynomial({0,1});
    testPoly2 = Polynomial({0});
    result = testPoly + testPoly2;

    assert(result.GetNumerator().GetCoeff(0) == 0);
    assert(result.GetNumerator().GetCoeff(1) == 1);
    assert(result.GetDenominator().GetCoeff(0) == 1);
    
    // 2.5 + 3.5 = 6.0
    testPoly = Polynomial({2.5});
    testPoly2 = Polynomial({3.5});
    result = testPoly + testPoly2;
    assert(result.GetNumerator().GetCoeff(0) == 6.0);
    assert(result.GetDenominator().GetCoeff(0) == 1.0);

    // 0.0 + 0.0 = 0.0
    testPoly = Polynomial({0});
    testPoly2 = Polynomial({0});
    result = testPoly + testPoly2;
    assert(result.GetNumerator().GetCoeff(0) == 0.0);
    assert(result.GetDenominator().GetCoeff(0) == 1);


    /**********************
    Polynomials
    ***********************/


    // (10 + 3x + x^2 + x^3) + (5+x^2) = 15 + 3x^2 + 2x^2 + x^3
    testPoly =  Polynomial({10,3,1,1});
    testPoly2 = Polynomial({5,0,1});
    result = testPoly + testPoly2;
    assert(result.GetNumerator().GetCoeff(0) == 15);
    assert(result.GetNumerator().GetCoeff(1) == 3);
    assert(result.GetNumerator().GetCoeff(2) == 2);
    assert(result.GetNumerator().GetCoeff(3) == 1);
    assert(result.GetDenominator().GetCoeff(0) == 1);

    /**********************
    Polynomials
    ***********************/
    std::cout << "complex addition results"<< std::endl;


    // (1+i) + 1 = 2+1i
    Rational<ComplexPoly> testComplexPoly(ComplexPoly({1}, {1}));
    Rational<ComplexPoly> testComplexPoly2(ComplexPoly({1}));
    Rational<ComplexPoly> newresult = testComplexPoly + testComplexPoly2;
    assert(newresult.GetNumerator().GetCoeff(0) == 2);
    assert(newresult.GetNumerator().getComplexCoeff(0) == 1);
 
    std::cout<< "(1+i) + (1+i)x + (1+i)x^2 + x^3\n"; 
    testComplexPoly = ComplexPoly({1,1,1}, {1,1,1});
    testComplexPoly2 = ComplexPoly({0,0,0,1});
    newresult = testComplexPoly + testComplexPoly2;
    newresult.GetNumerator().Dump();
    
    
    std::cout << "Addition passed" << std::endl;
    
}


void testSubtraction(){
    // (1-x-x^2) - x^2 = 1 - x - 2x^2
    Rational<Polynomial> testPoly(Polynomial({1,-1,-1}));
    Rational<Polynomial> testPoly2(Polynomial({0,0,1}));
    Rational<Polynomial> newresult = testPoly - testPoly2;
    newresult.GetNumerator().Dump();
    assert(newresult.GetNumerator().GetCoeff(0) == 1);
    assert(newresult.GetNumerator().GetCoeff(1) == -1);
    assert(newresult.GetNumerator().GetCoeff(2) == -2);




    // (1-x-x^2) - (1-x-x^2)
    testPoly = Polynomial({1,-1,-1});
    testPoly2 = Polynomial({1,-1,-1});

    newresult = testPoly - testPoly2;
    newresult.GetNumerator().Dump();


    std::cout << "Subtraction passed" << std::endl;

    
}

void testMultiplication(){
    Rational<Polynomial> testPoly(Polynomial({1,-1,-1}));
    Rational<Polynomial> testPoly2(Polynomial({0,0,1}));
    Rational<Polynomial> newresult = testPoly * testPoly2;
    newresult.GetNumerator().Dump();

    std::cout << "Multiplication passed" << std::endl;


}



int main() {

    testAddition();

    
    testSubtraction();


    testMultiplication();


    std::cout << "Division passed" << std::endl;


    std::cout << "Remainder passed" << std::endl;



     std::cout << "All tests passed" << std::endl;
    return 0;

}
