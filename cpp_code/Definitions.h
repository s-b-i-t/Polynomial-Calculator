#include "Polynomial.h"
#include "Rational.h"

#ifndef Definitions_h
#define Definitions_h


Rational<int> int1(1);
Rational<int> int2(2);
Rational<int> int3(3);
Rational<int> int4(4);
Rational<int> int5(5);
Rational<int> int6(6);
Rational<int> int7(7);
Rational<int> int8(8);
Rational<int> int9(9);
Rational<int> int10(10);


Rational<double> double1(1.0);
Rational<double> double1point5(1.5);
Rational<double> double2(2.0);
Rational<double> double2point5(2.5);
Rational<double> double3(3.0);




// 0
Polynomial poly1; // Constant zero polynomial
Rational<Polynomial> Poly1;
// 1
Polynomial poly2({1}); // Constant polynomial
Rational<Polynomial> Poly2(poly2);


// 3x
Polynomial  poly3({0, 3}); // Degree 1
Rational<Polynomial> Poly3(poly3);

// 7x
Polynomial poly4({0, 7}); // Degree 1
Rational<Polynomial> Poly4(poly4);
// 2 + 4x
Polynomial poly5({2, 4}); // Degree 1
Rational<Polynomial> Poly5(poly5);
// 1 + 5x^2
Polynomial poly6({1, 0, 5}); // Degree 2
Rational<Polynomial> Poly6(poly6);
// 3 + 0x + 2x^2
Polynomial poly7({3, 0, 2}); // Degree 2
Rational<Polynomial> Poly7(poly7);
// 1 + 2x + 3x^2
Polynomial poly8({1, 2, 3}); // Degree 2
Rational<Polynomial> Poly8(poly8);

// 4 + 5x + 6x^2
Polynomial poly9({4, 5, 6}); // Degree 2
Rational<Polynomial> Poly9(poly9);
// 12 + 32x + 4x^2
Polynomial poly10({12, 32, 4}); // Degree 2
Rational<Polynomial> Poly10(poly10);
// 4x^2 + 7x^3
Polynomial poly11({0, 0, 4, 7}); // Degree 3
Rational<Polynomial> Poly11(poly11);
// 5 + 6x + 0x^2 + 3x^3
Polynomial poly12({5, 6, 0, 3}); // Degree 3
Rational<Polynomial> Poly12(poly12);
// 1 + 2x + 3x^2 + 0x^3 + 5x^4
Polynomial poly13({1, 2, 3, 0, 5}); // Degree 4
Rational<Polynomial> Poly13(poly13);
// 1 + 2x + 3x^2 + 4x^3 + 5x^4
Polynomial poly14({1, 2, 3, 4, 5}); // Degree 4
Rational<Polynomial> Poly14(poly14);


// 12 + 0x + 7x^2 + 0x^3 + 3x^4 + 5x^5
Polynomial poly15({12, 0, 7, 0, 3, 5}); // Degree 5
Rational<Polynomial> Poly15(poly15);
// 0 + 0x + 0x^2 + 4x^3 + 2x^4 + 6x^5
Polynomial poly16({0, 0, 0, 4, 2, 6}); // Degree 5
Rational<Polynomial> Poly16(poly16);
// 1x + 0x^2 + 2x^3 + 3x^4 + 4x^5
Polynomial poly17({0, 1, 0, 2, 3, 4}); // Degree 5
Rational<Polynomial> Poly17(poly17);
// 1 + 1x + 1x^2 + 1x^3 + 1x^4 + 1x^5 + 1x^6
Polynomial poly18({1, 1, 1, 1, 1, 1, 1}); // Degree 6
Rational<Polynomial> Poly18(poly18);
// 5x + 6x^2 + 7x^3 + 8x^4 + 9x^5 + 10x^6 + 11x^7
Polynomial poly19({0, 5, 6, 7, 8, 9, 10, 11}); // Degree 7
Rational<Polynomial> Poly19(poly19);
// 2 + 3x + 5x^2 + 7x^3 + 11x^4 + 13x^5 + 17x^6 + 19x^7
Polynomial poly20({2, 3, 5, 7, 11, 13, 17, 19}); // Degree 7
Rational<Polynomial> Poly20(poly20);



#endif