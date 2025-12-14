#include <iostream>
#include <cassert>
#include <vector>
#include "Polynomial.h"
#include "Definitions.h"
#include "RationalComplex.h"
#include "Complex.h"
void testAddition() {
    /**********************
     Pure real polynomials (via make_rational)
    ***********************/
    // 1 + 3 = 4
    RationalComplex r1 = make_rational(Polynomial({1}));
    RationalComplex r2 = make_rational(Polynomial({3}));
    RationalComplex result = r1 + r2;

    // real part 4, imag 0, denom 1
    assert(result.GetNumerator().GetCoeff(0) == 4.0);
    assert(result.GetNumerator().getComplexCoeff(0) == 0.0);
    assert(result.GetDenominator().GetCoeff(0) == 1.0);

    // x + 0 = x
    r1 = make_rational(Polynomial({0, 1}));  // x
    r2 = make_rational(Polynomial({0}));     // 0
    result = r1 + r2;

    assert(result.GetNumerator().GetCoeff(0) == 0.0);
    assert(result.GetNumerator().GetCoeff(1) == 1.0);
    assert(result.GetNumerator().getComplexCoeff(0) == 0.0);
    assert(result.GetDenominator().GetCoeff(0) == 1.0);

    // (10 + 3x + x^2 + x^3) + (5 + x^2)
    r1 = make_rational(Polynomial({10, 3, 1, 1}));
    r2 = make_rational(Polynomial({5, 0, 1}));
    result = r1 + r2;

    auto &num = result.GetNumerator();
    assert(num.GetCoeff(0) == 15.0);
    assert(num.GetCoeff(1) == 3.0);
    assert(num.GetCoeff(2) == 2.0);
    assert(num.GetCoeff(3) == 1.0);
    assert(result.GetDenominator().GetCoeff(0) == 1.0);

    /**********************
     ComplexPolys
    ***********************/
    std::cout << "complex addition results\n";

    // (1+i) + 1 = 2 + i
    RationalComplex c1(ComplexPoly(std::vector<double>{1}, std::vector<double>{1}));
    RationalComplex c2(ComplexPoly({1}));       // real 1
    RationalComplex cRes = c1 + c2;

    assert(cRes.GetNumerator().GetCoeff(0) == 2.0);
    assert(cRes.GetNumerator().getComplexCoeff(0) == 1.0);
    assert(cRes.GetDenominator().GetCoeff(0) == 1.0);

    // (1+i) + (1+i)x + (1+i)x^2 + x^3
    // std::cout << "(1+i) + (1+i)x + (1+i)x^2 + x^3\n";
    c1 = RationalComplex(
        ComplexPoly(
            std::vector<double>{1, 1, 1},    // real coeffs
            std::vector<double>{1, 1, 1}     // imag coeffs
        )
    ); // (1+i) + (1+i)x + (1+i)x^2
    c2 = RationalComplex(ComplexPoly({0, 0, 0, 1})); // x^3 (pure real)

    cRes = c1 + c2;
    // cRes.GetNumerator().Dump();

    std::cout << "Addition passed" << std::endl;
}

void testSubtraction() {
    // (1 - x - x^2) - x^2 = 1 - x - 2x^2
    RationalComplex r1 = make_rational(Polynomial({1, -1, -1}));
    RationalComplex r2 = make_rational(Polynomial({0, 0, 1}));
    RationalComplex res = r1 - r2;

    auto &num = res.GetNumerator();
    // num.Dump();

    assert(num.GetCoeff(0) == 1.0);
    assert(num.GetCoeff(1) == -1.0);
    assert(num.GetCoeff(2) == -2.0);
    assert(res.GetDenominator().GetCoeff(0) == 1.0);

    // (1 - x - x^2) - (1 - x - x^2) = 0
    r1 = make_rational(Polynomial({1, -1, -1}));
    r2 = make_rational(Polynomial({1, -1, -1}));
    res = r1 - r2;
    // res.GetNumerator().Dump();

    // All real coeffs should be 0 and imag coeffs 0
    assert(res.GetNumerator().GetCoeff(0) == 0.0);
    assert(res.GetNumerator().getComplexCoeff(0) == 0.0);
    assert(res.GetDenominator().GetCoeff(0) == 1.0);

    std::cout << "Subtraction passed" << std::endl;
}
void testMultiplication() {
    // (1 - x - x^2) * x^2  (pure real case via make_rational)
    RationalComplex r1 = make_rational(Polynomial({1, -1, -1}));
    RationalComplex r2 = make_rational(Polynomial({0, 0, 1}));
    RationalComplex res = r1 * r2;

    // res.GetNumerator().Dump();
    // Spot-check a couple of coeffs if you care
    // (1 - x - x^2) * x^2 = x^2 - x^3 - x^4
    assert(res.GetNumerator().GetCoeff(0) == 0.0);
    assert(res.GetNumerator().GetCoeff(2) == 1.0);
    assert(res.GetNumerator().GetCoeff(3) == -1.0);
    assert(res.GetNumerator().GetCoeff(4) == -1.0);
    assert(res.GetDenominator().GetCoeff(0) == 1.0);

    // Complex * complex: (1+i) * (1+i) = (0 + 2i)
    RationalComplex c1(ComplexPoly({1}, {1}));
    RationalComplex c2(ComplexPoly({1}, {1}));
    RationalComplex cRes = c1 * c2;

    auto &cNum = cRes.GetNumerator();
    // (1+i)^2 = 1 + 2i - 1 = 2i
    assert(std::abs(cNum.GetCoeff(0)) < 1e-10);          // real ~0
    assert(cNum.getComplexCoeff(0) == 2.0);              // imag 2
    assert(cRes.GetDenominator().GetCoeff(0) == 1.0);

    std::cout << "Multiplication passed" << std::endl;
}

void testDivision() {
    /**********************
     1) Divide by 1: (1 + 2x + 3x^2) / 1 = itself
    ***********************/
    RationalComplex r = make_rational(Polynomial({1, 2, 3}));
    RationalComplex one = make_rational(Polynomial({1}));
    RationalComplex q = r / one;

    const ComplexPoly &num1 = q.GetNumerator();
    const ComplexPoly &den1 = q.GetDenominator();

    // numerator unchanged
    assert(num1.GetCoeff(0) == 1.0);
    assert(num1.GetCoeff(1) == 2.0);
    assert(num1.GetCoeff(2) == 3.0);
    // real-only, so imag part is 0
    assert(num1.getComplexCoeff(0) == 0.0);
    assert(num1.getComplexCoeff(1) == 0.0);
    assert(num1.getComplexCoeff(2) == 0.0);

    // denominator still 1
    assert(den1.GetCoeff(0) == 1.0);
    assert(den1.getComplexCoeff(0) == 0.0);

    /**********************
     2) Divide by real scalar: (1 + 2x) / 2
        -> newNum = (1+2x), newDen = 2
    ***********************/
    r = make_rational(Polynomial({1, 2}));
    RationalComplex two = make_rational(Polynomial({2}));
    q = r / two;

    const ComplexPoly &num2 = q.GetNumerator();
    const ComplexPoly &den2 = q.GetDenominator();

    // numerator unchanged
    assert(num2.GetCoeff(0) == 1.0);
    assert(num2.GetCoeff(1) == 2.0);
    assert(num2.getComplexCoeff(0) == 0.0);
    assert(num2.getComplexCoeff(1) == 0.0);

    // denominator is now the scalar 2
    assert(den2.GetCoeff(0) == 2.0);
    assert(den2.getComplexCoeff(0) == 0.0);

    /**********************
     3) Complex / real scalar: (1 + i) / 2
        -> newNum = (1+i), newDen = 2
    ***********************/
    std::vector<double> real_one{1.0};
    std::vector<double> imag_one{1.0};
    RationalComplex c1(ComplexPoly(real_one, imag_one));
    q = c1 / two;

    const ComplexPoly &num3 = q.GetNumerator();
    const ComplexPoly &den3 = q.GetDenominator();

    // numerator is still 1 + i
    assert(num3.GetCoeff(0) == 1.0);
    assert(num3.getComplexCoeff(0) == 1.0);

    // denominator is 2 (real-only)
    assert(den3.GetCoeff(0) == 2.0);
    assert(den3.getComplexCoeff(0) == 0.0);

    /**********************
     4) Structural rule check:
        (a/b) / (c/d) = (a*d) / (b*c)
        Take:
          a = 1 + x
          b = 1 + x^2
          c = 2
          d = 3 + 4i
    ***********************/
    RationalComplex a_rc = make_rational(Polynomial({1, 1}));       // 1 + x
    RationalComplex b_rc = make_rational(Polynomial({1, 0, 1}));    // 1 + x^2
    RationalComplex c_rc = make_rational(Polynomial({2}));          // 2

    std::vector<double> d_real{3.0};
    std::vector<double> d_imag{4.0};
    RationalComplex d_rc(ComplexPoly(d_real, d_imag));              // 3 + 4i

    // r1 = a/b, r2 = c/d
    RationalComplex r1(a_rc.GetNumerator(), b_rc.GetNumerator());
    RationalComplex r2(c_rc.GetNumerator(), d_rc.GetNumerator());

    RationalComplex div = r1 / r2;

    const ComplexPoly &divNum = div.GetNumerator();
    const ComplexPoly &divDen = div.GetDenominator();

    // Expected numerator: (1 + x) * (3 + 4i)
    // => each coefficient of (1 + x) scaled by (3 + 4i)
    //    so real: [3, 3], imag: [4, 4]
    assert(divNum.GetCoeff(0) == 3.0);
    assert(divNum.getComplexCoeff(0) == 4.0);
    assert(divNum.GetCoeff(1) == 3.0);
    assert(divNum.getComplexCoeff(1) == 4.0);

    // Expected denominator: (1 + x^2) * 2 = 2 + 2x^2 (pure real)
    assert(divDen.GetCoeff(0) == 2.0);
    assert(divDen.GetCoeff(1) == 0.0);
    assert(divDen.GetCoeff(2) == 2.0);
    assert(divDen.getComplexCoeff(0) == 0.0);
    assert(divDen.getComplexCoeff(1) == 0.0);
    assert(divDen.getComplexCoeff(2) == 0.0);

    /**********************
     5) Division by zero rational should throw
    ***********************/
    RationalComplex zero; // 0 / 1
    bool threw = false;
    try {
        (void)(r / zero);
    } catch (const std::runtime_error &) {
        threw = true;
    }
    assert(threw && "Division by zero rational did not throw");

    std::cout << "Division passed" << std::endl;
}



int main() {

    testAddition();

    std::cout << "Addition passed" << std::endl;
    
    testSubtraction();

    std::cout << "Subtraction passed" << std::endl;

    testMultiplication();

    std::cout << "Multiplication passed" << std::endl;

    testDivision();

    std::cout << "Division passed" << std::endl;


    std::cout << "All tests passed!" << std::endl;

    return 0;

}
