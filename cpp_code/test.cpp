/*
 CHANGELOG (Codex robustness pass)
 - Add adapter coverage (`getListCoeffsIn` / complex dense adapters) without forcing huge allocations.
 - Strengthen division/remainder identities with NearlyEquals to avoid EPS fragility.
 - Add randomized sparse identity tests to catch regressions.
 - Add undefined-propagation test for ComplexPoly::Scale().
 */

#include <iostream>
#include <cassert>
#include <vector>
#include <random>
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

    // (1 - x - x^2) * x^2 = x^2 - x^3 - x^4
    assert(newresult.GetNumerator().GetCoeff(2) == 1);
    assert(newresult.GetNumerator().GetCoeff(3) == -1);
    assert(newresult.GetNumerator().GetCoeff(4) == -1);

    std::cout << "Multiplication passed" << std::endl;


}



void testDivisionAndRemainder()
{
    // Example: (x^3 - 2x^2 - 4) / (x - 3) = x^2 + x + 3, remainder 5
    Polynomial dividend;
    dividend.SetCoeffAt(3, 1);
    dividend.SetCoeffAt(2, -2);
    dividend.SetCoeffAt(0, -4);

    Polynomial divisor;
    divisor.SetCoeffAt(1, 1);
    divisor.SetCoeffAt(0, -3);

    Polynomial q = dividend / divisor;
    Polynomial r = dividend % divisor;

    assert(q.GetCoeff(2) == 1);
    assert(q.GetCoeff(1) == 1);
    assert(q.GetCoeff(0) == 3);
    assert(r.GetCoeff(0) == 5);

    // q*divisor + r == dividend
    assert((q * divisor + r).NearlyEquals(dividend));

    // deg(a) < deg(b) => quotient = 0, remainder = a
    Polynomial a({1, 2});       // 1 + 2x
    Polynomial b({0, 0, 1});    // x^2
    Polynomial q2 = a / b;
    Polynomial r2 = a % b;
    assert(q2.GetDegree() == -1);
    assert(r2 == a);

    // Division by zero polynomial throws
    bool threw = false;
    try
    {
        (void)(a / Polynomial());
    }
    catch (const std::invalid_argument &)
    {
        threw = true;
    }
    assert(threw);

    std::cout << "Division/remainder passed" << std::endl;
}

void stressTests()
{
    // Zero polynomial invariants
    Polynomial z;
    assert(z.IsZero());
    assert(z.GetDegree() == -1);
    assert(z.GetCoeff(0) == 0);

    ComplexPoly cz;
    assert(cz.IsZero());
    assert(cz.GetDegree() == -1);

    // Sparse huge exponent should not allocate dense storage.
    Polynomial p;
    p.SetCoeffAt(1'000'000, 1);
    assert(p.GetDegree() == 1'000'000);
    assert(p.GetCoeff(1'000'000) == 1);
    assert(p.GetCoeff(0) == 0);

    // Multiply sparse polynomials with far-apart exponents.
    Polynomial a;
    a.SetCoeffAt(0, 1);
    a.SetCoeffAt(1'000'000, 2);
    Polynomial b;
    b.SetCoeffAt(3, 5);
    b.SetCoeffAt(500'000, 7);
    Polynomial m = a * b;
    assert(m.GetCoeff(3) == 5);
    assert(m.GetCoeff(500'000) == 7);
    assert(m.GetCoeff(1'000'003) == 10);
    assert(m.GetCoeff(1'500'000) == 14);

    // Divide x^50 - 120224 x^73 by x^50 => 1 - 120224 x^23, remainder 0.
    Polynomial d;
    d.SetCoeffAt(50, 1);
    d.SetCoeffAt(73, -120224);
    Polynomial s;
    s.SetCoeffAt(50, 1);
    Polynomial q = d / s;
    Polynomial r = d % s;
    assert(q.GetCoeff(0) == 1);
    assert(q.GetCoeff(23) == -120224);
    assert(r.GetDegree() == -1);

    std::cout << "Stress tests passed" << std::endl;
}

void adapterTests()
{
    Polynomial p;
    p.SetCoeffAt(1'000'000, 1);
    const auto &dense = p.getListCoeffsIn();
    assert(dense.size() <= static_cast<size_t>(Polynomial::DENSE_EXPORT_MAX_EXPONENT + 1));

    // Bounded export should include low exponents and not crash.
    p.SetCoeffAt(0, 2);
    const auto &dense2 = p.getListCoeffsIn();
    assert(dense2.size() >= 1);
    assert(dense2[0] == 2);

    const auto dense3 = p.getListCoeffsIn(3);
    assert(dense3.size() == 4);
    assert(dense3[0] == 2);

    ComplexPoly cp({1}, {1});
    const auto &re = cp.getListCoeffsIn();
    const auto &im = cp.getcomplexCoeffsList();
    assert(re.size() == im.size());
    assert(re[0] == 1);
    assert(im[0] == 1);

    // Huge-degree ComplexPoly adapter should be bounded.
    Polynomial hp;
    hp.SetCoeffAt(1'000'000, 1);
    ComplexPoly hcp(hp);
    const auto &hre = hcp.getListCoeffsIn();
    const auto &him = hcp.getcomplexCoeffsList();
    assert(hre.size() <= static_cast<size_t>(ComplexPoly::DENSE_EXPORT_MAX_EXPONENT + 1));
    assert(him.size() == hre.size());

    std::cout << "Adapter tests passed" << std::endl;
}

Polynomial randomSparsePoly(std::mt19937 &rng, int terms, int maxExp, int coeffMin, int coeffMax)
{
    std::uniform_int_distribution<int> expDist(0, maxExp);
    std::uniform_int_distribution<int> coeffDist(coeffMin, coeffMax);

    Polynomial p;
    for (int i = 0; i < terms; ++i)
    {
        const int e = expDist(rng);
        const int c = coeffDist(rng);
        if (c == 0)
        {
            continue;
        }
        p.SetCoeffAt(e, p.GetCoeff(e) + static_cast<double>(c));
    }
    return p;
}

void randomizedDivisionIdentityTests()
{
    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> kDist(1, 25);
    for (int t = 0; t < 200; ++t)
    {
        Polynomial a = randomSparsePoly(rng, 8, 60, -5, 5);
        const int k = kDist(rng);
        Polynomial b;
        b.SetCoeffAt(k, 1.0); // monomial divisor x^k (numerically stable in doubles)

        Polynomial q = a / b;
        Polynomial r = a % b;
        Polynomial rebuilt = (q * b) + r;

        assert(rebuilt == a);
        if (!r.IsZero())
        {
            assert(r.GetDegree() < b.GetDegree());
        }
    }

    std::cout << "Randomized division identity passed" << std::endl;
}

void complexIdentityTests()
{
    // (1+i) + 1 = 2 + i already covered; add division identity + NearlyEquals.
    ComplexPoly a({1, 0, 0, 1}, {1, 0, 0, -2}); // (1+i) + (1-2i)x^3
    ComplexPoly b({1, 1}, {0, 1});              // (1) + (1+i)x

    ComplexPoly q = a / b;
    auto [q2, r2] = ComplexPoly::DivMod(a, b);
    (void)q2;

    // a ≈ q*b + r
    const auto [qdm, rdm] = ComplexPoly::DivMod(a, b);
    ComplexPoly rebuilt = (qdm * b) + rdm;
    assert(rebuilt.NearlyEquals(a, 1e-6));

    std::cout << "ComplexPoly identity passed" << std::endl;
}

void undefinedPropagationTests()
{
    ComplexPoly undef = ComplexPoly::Undefined();
    ComplexPoly scaled = undef.Scale(std::complex<double>(2.0, 0.0));
    assert(scaled.isUndefined());

    std::cout << "Undefined propagation passed" << std::endl;
}

int main() {

    testAddition();

    
    testSubtraction();


    testMultiplication();

    testDivisionAndRemainder();
    stressTests();
    adapterTests();
    randomizedDivisionIdentityTests();
    complexIdentityTests();
    undefinedPropagationTests();

	     std::cout << "All tests passed" << std::endl;
    return 0;

}
