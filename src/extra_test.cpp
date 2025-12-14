// More deprecated test cases
// void testIntegerAddition() {
//     Rational<int> result; 
    
//     result = int1 + int2; // 1 + 2 -> 3
//     assert(result.GetValue() == 3);

//     result = int3 + int4; // 3 + 4 -> 7
//     assert(result.GetValue() == 7);
// }

// void testDoubleAddition(){
//     Rational<double> result;

//     result = double1 + double2; // 1.0 + 2.0 -> 3.0
//     assert(result.GetValue() == 3.0);

//     result = double1 + double1point5; // 1.0 + 1.5 -> 2.5
//     assert(result.GetValue() == 2.5);

//     result = double2 + double3; // 2.0 + 3.0 -> 5.0
//     assert(result.GetValue() == 5.0);
// }

// void testPolynomialAddition() {
//     Rational<Polynomial> result;

//     result = poly1 + poly2; // 0 + 1 -> {1}
//     assert(result.GetValue().GetCoeff(0) == 1);
//     result = poly3 + poly4; // (3x) + (7x) -> {0, 10}
//     assert(result.GetValue().GetCoeff(0) == 0 && result.GetValue().GetCoeff(1) == 10);

//     result = poly5 + poly6; // (2 + 4x) + (1 + 5x^2) -> {3, 4, 5}
//     assert(result.GetValue().GetCoeff(0) == 3 && result.GetValue().GetCoeff(1) == 4 && result.GetValue().GetCoeff(2) == 5);

//     result = poly7 + poly8; // (3 + 2x^2) + (1 + 2x + 3x^2) -> {4, 2x, 5x^2}
//     assert(result.GetValue().GetCoeff(0) == 4 && result.GetValue().GetCoeff(1) == 2 && result.GetValue().GetCoeff(2) == 5);

//     result = poly1 + poly10; // 0 + (12 + 32x + 4x^2) -> {12, 32x, 4x^2}
//     assert(result.GetValue().GetCoeff(0) == 12 && result.GetValue().GetCoeff(1) == 32 && result.GetValue().GetCoeff(2) == 4);


//     result =   poly6 + int2; // 2 + (1 + 5x^2) -> {3, 0x,  5x^2}
//     assert(result.GetValue().GetCoeff(0) == 3 && result.GetValue().GetCoeff(1) == 0 && result.GetValue().GetCoeff(2) == 5);
// }


// void testInterTypeAddition(){
//     Rational<Polynomial> result;

//     result = poly6 + int2; // 2 + (1 + 5x^2) -> {3, 0x,  5x^2}
//     assert(result.GetValue().GetCoeff(0) == 3 && result.GetValue().GetCoeff(1) == 0 && result.GetValue().GetCoeff(2) == 5);


//     // result = int1 + poly6; // 1 + (1 + 5x^2) -> {2, 0, 5}
    
//     // assert(result.GetValue() == 2.0);

//     // result = int1 + double1point5; // 1 + 1.5 -> 2.5
//     // assert(result.GetValue() == 2.5);

//     // result = int2 + double2; // 2 + 2.0 -> 4.0
//     // assert(result.GetValue() == 4.0);


    
// }

// void testIntegerSubtraction() {
//     Rational<int> result;

//     result = int1 - int2; // 1 - 2 -> -1
//     assert(result.GetValue() == -1);

//     result = int3 - int4; // 3 - 4 -> -1
//     assert(result.GetValue() == -1);
// }

// void testDoubleSubtraction() {
//     Rational<double> result;

//     result = double1 - double2; // 1.0 - 2.0 -> -1.0
//     assert(result.GetValue() == -1.0);

//     result = double1 - double1point5; // 1.0 - 1.5 -> -0.5
//     assert(result.GetValue() == -0.5);
// }

// void testPolynomialSubtraction() {
//     Rational<Polynomial> result;

//     result = poly1 - poly2; // 0 - 1 -> {-1}
//     assert(result.GetValue().GetCoeff(0) == -1);

//     result = poly3 - poly4; // 3x - 7x -> {0, -4}
//     assert(result.GetValue().GetCoeff(0) == 0 && result.GetValue().GetCoeff(1) == -4);

//     result = poly5 - poly6; // 2 + 4x - 1 - 5x^2 -> {1, 4, -5}
//     assert(result.GetValue().GetCoeff(0) == 1 && result.GetValue().GetCoeff(1) == 4 && result.GetValue().GetCoeff(2) == -5);
// }

// void testInterTypeSubtraction(){
//     ;
// }





// void testIntegerMultiplication() {
//     Rational<int> result;

//     result = int1 * int2; // 1 * 2 -> 2
//     assert(result.GetValue() == 2);

//     result = int3 * int4; // 3 * 4 -> 12
//     assert(result.GetValue() == 12);
// }

// void testDoubleMultiplication() {
//     Rational<double> result;

//     result = double1 * double2; // 1.0 * 2.0 -> 2.0
//     assert(result.GetValue() == 2.0);

//     result = double1 * double1point5; // 1.0 * 1.5 -> 1.5
//     assert(result.GetValue() == 1.5);
// }

// void testPolynomialMultiplication(){

//     Rational<Polynomial> result;

//     result = poly6 * poly7; // (1 + 5x^2) * (3 + 2x^2) -> {3, 0, 17, 0, 10}
//     assert(result.GetValue().GetCoeff(0) == 3 && result.GetValue().GetCoeff(1) == 0 && result.GetValue().GetCoeff(2) == 17 && result.GetValue().GetCoeff(3) == 0 && result.GetValue().GetCoeff(4) == 10);

//     result = poly8 * poly9; // (1 + 2x + 3x^2) * (4 + 5x + 6x^2) -> {4, 13, 28, 27, 18}
//     assert(result.GetValue().GetCoeff(0) == 4 && result.GetValue().GetCoeff(1) == 13 && result.GetValue().GetCoeff(2) == 28 && result.GetValue().GetCoeff(3) == 27 && result.GetValue().GetCoeff(4) == 18);


// }

// void testInterTypeMultiplication(){
//     // Rational<Polynomial> result;

//     // result = int1 * poly6; // 1 * (1 + 5x^2) -> {1, 0, 5}
// }

// void testIntegerDivision() {
//     Rational<int> result;

//     result = int1 / int2; // 1 / 2 -> 0
//     assert(result.GetValue() == 0);

//     result = int3 / int4; // 3 / 4 -> 0
//     assert(result.GetValue() == 0);
// }

// void testDoubleDivision() {
//     Rational<double> result;

//     result = double1 / double2; // 1.0 / 2.0 -> 0.5
//     assert(result.GetValue() == 0.5);

//     result = double1 / double1point5; // 1.0 / 1.5 -> 0.6666666666666666
//     assert(result.GetValue() == 0.6666666666666666);
// }

// void testPolynomialDivision() {
//     ;
// }

// void testInterTypeDivision(){
//     ;
// }




// void testIntegerRemainder() {
//     Rational<int> remainder;

//     remainder = int1 % int2; // 1 % 2 -> 1
//     assert(remainder.GetValue() == 1);

//     remainder = int3 % int4; // 3 % 4 -> 3
//     assert(remainder.GetValue() == 3);
// }

// void testPolynomialRemainder() {
//     ;
// }


// void testInterTypeRemainder(){
//     ;
// }




// void moreTesting(){
//     int int1 = 100;
//     double double1 = 1.5;
//     Polynomial poly1({1, 2, 3}); // 1 + 2x + 3x^2


//     // int & poly
//     Rational<Polynomial> result;
//     // result = int1 + poly1; // 100 + (1 + 2x + 3x^2) -> 101 + 2x + 3x^2
//     // assert(result.GetValue().GetCoeff(0) == 101 && result.GetValue().GetCoeff(1) == 2 && result.GetValue().GetCoeff(2) == 3);

//     // result = poly1 + int1; // (1 + 2x + 3x^2) + 100 -> 101 + 2x + 3x^2
//     // assert(result.GetValue().GetCoeff(0) == 101 && result.GetValue().GetCoeff(1) == 2 && result.GetValue().GetCoeff(2) == 3);
    
    
    
//     // result = int1 - poly1; // 100 - (1 + 2x + 3x^2) -> 99 - 2x - 3x^2
//     // assert(result.GetValue().GetCoeff(0) == 99 && result.GetValue().GetCoeff(1) == -2 && result.GetValue().GetCoeff(2) == -3);

//     // result = poly1 - int1; // (1 + 2x + 3x^2) - 100 -> -99 + 2x + 3x^2
//     // assert(result.GetValue().GetCoeff(0) == -99 && result.GetValue().GetCoeff(1) == 2 && result.GetValue().GetCoeff(2) == 3);


//     // result = int1 * poly1; // 100 * (1 + 2x + 3x^2) -> 100 + 200x + 300x^2
//     // assert(result.GetValue().GetCoeff(0) == 100 && result.GetValue().GetCoeff(1) == 200 && result.GetValue().GetCoeff(2) == 300);


//     // result = poly1 * int1; // (1 + 2x + 3x^2) * 100 -> 100 + 200x + 300x^2
//     // assert(result.GetValue().GetCoeff(0) == 100 && result.GetValue().GetCoeff(1) == 200 && result.GetValue().GetCoeff(2) == 300);

//     // result = int1 / poly1; // 100 / (1 + 2x + 3x^2) 
//     // std::cout << result.GetValue().GetCoeff(0) << result.GetValue().GetCoeff(1) << result.GetValue().GetCoeff(2) << std::endl;


//     // result = poly1 / int1; // (1 + 2x + 3x^2) / 100 -> (1/100) + (2/100)x + (3/100)x^2
//     // assert(result.GetValue().GetCoeff(0) == 0.01 && result.GetValue().GetCoeff(1) == 0.02 && result.GetValue().GetCoeff(2) == 0.03);

//     // // double & poly

//     // result = double1 + poly1; // 1.5 + (1 + 2x + 3x^2) -> 2.5 + 2x + 3x^2
//     // assert(result.GetValue().GetCoeff(0) == 2.5 && result.GetValue().GetCoeff(1) == 2 && result.GetValue().GetCoeff(2) == 3);

//     // result = poly1 + double1; // (1 + 2x + 3x^2) + 1.5 -> 2.5 + 2x + 3x^2
//     // assert(result.GetValue().GetCoeff(0) == 2.5 && result.GetValue().GetCoeff(1) == 2 && result.GetValue().GetCoeff(2) == 3);


//     // result = double1 - poly1; // 1.5 - (1 + 2x + 3x^2) -> 0.5 - 2x - 3x^2
//     // assert(result.GetValue().GetCoeff(0) == 0.5 && result.GetValue().GetCoeff(1) == -2 && result.GetValue().GetCoeff(2) == -3);

//     // result = poly1 - double1; // (1 + 2x + 3x^2) - 1.5 -> -0.5 + 2x + 3x^2
//     // assert(result.GetValue().GetCoeff(0) == -0.5 && result.GetValue().GetCoeff(1) == 2 && result.GetValue().GetCoeff(2) == 3);


//     // result = double1 * poly1; // 1.5 * (1 + 2x + 3x^2) -> 1.5 + 3x + 4.5x^2
//     // assert(result.GetValue().GetCoeff(0) == 1.5 && result.GetValue().GetCoeff(1) == 3 && result.GetValue().GetCoeff(2) == 4.5);

//     // result = poly1 * double1; // (1 + 2x + 3x^2) * 1.5 -> 1.5 + 3x + 4.5x^2
//     // assert(result.GetValue().GetCoeff(0) == 1.5 && result.GetValue().GetCoeff(1) == 3 && result.GetValue().GetCoeff(2) == 4.5);

    
//     // poly & double


//     // int & double


//     // double & int 





// }







// // void testPolynomialSubtraction() {
// //     Polynomial result;

// //     result = poly1 - poly2; // 0 - 1 -> {-1}
// //     assert(result.GetCoeff(0) == -1);

// //     result = poly3 - poly4; // 3x - 7x -> {0, -4}
// //     assert(result.GetCoeff(0) == 0 && result.GetCoeff(1) == -4);

// //     result = poly5 - poly6; // 2 + 4x - 1 - 5x^2 -> {1, 4, -5}

// //     assert(result.GetCoeff(0) == 1 && result.GetCoeff(1) == 4 && result.GetCoeff(2) == -5);

// //     std::cout << "Passed Subtraction" << std::endl;
// // }


// // void testPolynomialMultiplication() {
// //     Polynomial result;

// //     result = poly2 * poly3; // 1 * 3x -> {0, 3}
// //     assert(result.GetCoeff(0) == 0 && result.GetCoeff(1) == 3);

// //     // testing to just scale via multiplication (works good)
// //     Polynomial poly1000({5});
// //     result = poly1000 * poly5 ; // 5 * (2 + 4x) -> {10, 20}
// //     assert(result.GetCoeff(0) == 10 && result.GetCoeff(1) == 20);
// //     Polynomial poly1001({7});
// //     result = poly1000 * poly1001; // 5 * 7 -> {35}
// //     assert(result.GetCoeff(0) == 35);





// //     result = poly4 * poly5; // 7x * (2 + 4x) -> {0, 14, 28}
// //     assert(result.GetCoeff(0) == 0 && result.GetCoeff(1) == 14 && result.GetCoeff(2) == 28);


// //     std::cout << "Passed Multiplication" << std::endl;
// // }

// // void testPolynomialDivision() {
// //     Polynomial result;

// //     result = poly10 / poly2; // (12 + 32x + 4x^2) / 1 -> {12, 32, 4}
// //     assert(result.GetCoeff(0) == 12 && result.GetCoeff(1) == 32 && result.GetCoeff(2) == 4);
    
// //     result = poly12 / poly3; // (5 + 6x + 3x^3) / 3x -> {2, 0, 1}
// //     assert(result.GetCoeff(0) == 2 && result.GetCoeff(1) == 0  && result.GetCoeff(2) == 1);

// //     std::cout << "Passed Division" << std::endl;
// // }


// // void testPolynomialRemainder() {
// //     Polynomial remainder;

// //     remainder = poly10 % poly2; // (12 + 32x + 4x^2) % 1 -> {0}
// //     assert(remainder.GetDegree() == 0 && remainder.GetCoeff(0) == 0);

// //     remainder = poly12 % poly3; // (5 + 6x + 3x^3) % 3x -> {5}
// //     assert(remainder.GetDegree() == 0 && remainder.GetCoeff(0) == 5);

// //     remainder = poly13 % poly7; // (1 + 2x + 3x^2 + 5x^4) % (3 + 2x^2) -> {7.75, 2}
// //     assert(remainder.GetDegree() == 1 && remainder.GetCoeff(0) == 7.75 && remainder.GetCoeff(1) == 2);

// //     std::cout << "Passed Remainder" << std::endl;
// // }

// // void testRational() {
// //     Rational<int> r1(5);    
// //     Rational<int> r2(2);    
// //     Rational<int> r3;       

// //     r3 = r1 + r2; // 5 + 2 -> 7
// //     assert(r3.GetValue() == 7);

// //     Rational<Polynomial> r4(poly13);
// //     Rational<Polynomial> r5(poly7);
// //     Rational<Polynomial> r6;
// //     r6 = r4 % r5; // (1 + 5x^2) + (3 + 2x^2) -> 4 + 7x^2
// //     assert(r6.GetValue().GetCoeff(0) == 7.75 && r6.GetValue().GetCoeff(1) == 2);

// //     Rational<double> r7(5.0);
// //     Rational<double> r8;
// //     r8 = r7 / r2;
// //     std::cout << r8.GetValue() << std::endl;

// //     Rational<Polynomial> r9(poly6);
// //     Rational<int> r10(2);

// //     Rational<Polynomial> r11;

// //     // r11 = r9 * r10; // 3x + 2 -> 2 + 3x

// //     // std::cout << r11.GetValue().GetCoeff(0) << " " << r11.GetValue().GetCoeff(1) << r11.GetValue().GetCoeff(2) << std::endl;


// //     Rational<Polynomial> r12(poly12); // 5 + 6x + 0x^2 + 3x^3
// //     Rational<int> r13(3); // 3
// //     Rational<Polynomial> result;
// //     result = r12 * r13; // 15 + 18x + 0x^2 + 9x^3

// //     std::cout << result.GetValue().GetCoeff(0) << " " << result.GetValue().GetCoeff(1) << " " << result.GetValue().GetCoeff(2) << " " << result.GetValue().GetCoeff(3) << std::endl;


// // }
