#include <iostream>
using namespace std;

#include "Rational.h"
#include "Polynomial.h"

void Test()
{
  // test rational of ints 
  Rational<int> r1(1,2), r2(3,2);
  Rational <int> r3 = r1+r2;
  cout << "r1+r2=" << r3.GetNumerator() << ", " << r3.GetDenominator() << endl;
  Rational <int> r4 = r1-r2;
  cout << "r1-r2=" << r4.GetNumerator() << ", " << r4.GetDenominator() << endl;
  Rational <int> r5 = r1*r2;
  cout << "r1*r2=" << r5.GetNumerator() << ", " << r5.GetDenominator() << endl;
  Rational <int> r6 = r1/r2;
  cout << "r1/r2=" << r6.GetNumerator() << ", " << r6.GetDenominator() << endl;
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
