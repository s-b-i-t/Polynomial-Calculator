#include <iostream>
using namespace std;
#include "ECPolynomial.h"

int main()
{
  // 
  vector<double> vec1;
  vec1.push_back(-3);
  vec1.push_back(1);
  ECPolynomial poly1(vec1);
  vector<double> vec2 ;
  vec2.push_back(-4);
  vec2.push_back(0);
  vec2.push_back(-2);
  vec2.push_back(1);
  ECPolynomial poly2(vec2);
  cout << "poly1 ";
  poly1.Dump();
  cout  << "poly2: ";
  poly2.Dump();

  ECPolynomial poly3 = poly1+poly2;
  cout << "Sum = ";
  poly3.Dump();

  ECPolynomial poly4 = poly1* poly2;
  cout << "Product = ";
  poly4.Dump();

  ECPolynomial poly5 = poly2 / poly1;
  ECPolynomial poly6 = poly2 % poly1;
  cout << "Divide poly2 by poly1 = ";
  poly5.Dump();
  cout << "remainder: ";
  poly6.Dump();
  
  std::vector<double> first = {1, 0 , 1}; 
  std::vector<double> second = {-1,1};

  ECPolynomial One(first);
  ECPolynomial Two(second);
  ECPolynomial Res = Two * One;
  Res.Dump();

}

