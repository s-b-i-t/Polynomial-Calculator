/*
 CHANGELOG (Codex robustness pass)
 - Lock in normalization expectations for Rational<int> and Rational<Polynomial>.
 - Avoid fragile exact comparisons where EPS can matter.
 */

#include <iostream>
#include <cassert>
#include <cmath>
using namespace std;

#include "Rational.h"
#include "Polynomial.h"

void Test()
{
  // test rational of ints 
  Rational<int> r1(1,2), r2(3,2);
  Rational <int> r3 = r1+r2;
  cout << "r1+r2=" << r3.GetNumerator() << ", " << r3.GetDenominator() << endl;
  assert(r3.GetNumerator() == 2);
  assert(r3.GetDenominator() == 1);
  Rational <int> r4 = r1-r2;
  cout << "r1-r2=" << r4.GetNumerator() << ", " << r4.GetDenominator() << endl;
  assert(r4.GetNumerator() == -1);
  assert(r4.GetDenominator() == 1);
  Rational <int> r5 = r1*r2;
  cout << "r1*r2=" << r5.GetNumerator() << ", " << r5.GetDenominator() << endl;
  assert(r5.GetNumerator() == 3);
  assert(r5.GetDenominator() == 4);
  Rational <int> r6 = r1/r2;
  cout << "r1/r2=" << r6.GetNumerator() << ", " << r6.GetDenominator() << endl;
  assert(r6.GetNumerator() == 1);
  assert(r6.GetDenominator() == 3);
}

void Test2()
{
  // test rational of polynomials
  Polynomial p1(2), p2(2), p3(1);
  // p1=1-x+x^2
  p1.SetCoeffAt(0, 1);
  p1.SetCoeffAt(1, -1);
  p1.SetCoeffAt(2, 1);
  // p2=1-x^2
  p2.SetCoeffAt(0, 1);
  p2.SetCoeffAt(2, -1);
  // p3=1+2x
  p3.SetCoeffAt(0, 1);
  p3.SetCoeffAt(1, 2);
  // Create rational: p1/p2 and p2/p3
  Rational<Polynomial> r1(p1, p2), r2(p2, p3);

  // Denominators are canonicalized to monic.
  assert(!r1.GetDenominator().IsZero());
  assert(std::abs(r1.GetDenominator().LeadingTerm().second - 1.0) < Polynomial::EPS);
  assert(!r2.GetDenominator().IsZero());
  assert(std::abs(r2.GetDenominator().LeadingTerm().second - 1.0) < Polynomial::EPS);

  Rational<Polynomial> r3 = r1+ r2;
  // r1+r2= ( 2+ x -3x^2+ 2x^3+ x^4)/ ( 1+ 2x -x^2 -2x^3) 
  cout << "r1+r2=";
  r3.GetNumerator().Dump();
  cout << "  ";
  r3.GetDenominator().Dump();
  cout << endl;
  Rational<Polynomial> r4 = r1 * r2;
  // r1*r2= ( 1- x + x^3 - x^4)/ ( 1+ 2x -x^2 -2x^3) 
  cout << "r1*r2=";
  r4.GetNumerator().Dump();
  cout << "  ";
  r4.GetDenominator().Dump();
  cout << endl;
}


int main()
{
  Test();
  Test2();
}
