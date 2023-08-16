<<<<<<< HEAD
//
//  ECRational.h
//  
//
//  Created by Yufeng Wu on 1/7/23.
//

#ifndef ECRational_h
#define ECRational_h


=======
//  ECRational.h
#ifndef ECRational_h
#define ECRational_h

>>>>>>> 5d456af5ad28e948d4ea00556da10a34cb874093
// *****************************************************************
// Generic rational of two quantities (e.g. integers, floating point, complex number, or polynomila)
// Assume the quantity class T supports (i) default constructor and copy constructor, (ii) assignment operator, and (iii) arithmatic operators: +, -, * and /
// Note: you don't need to simplify the rational. That is, it is OK to have common factors in numerator and denominoator. For example, 4/4 = 1/1
template <class T>
class ECRational
{
public:
<<<<<<< HEAD
    // YW: change the following code if needed...
=======
>>>>>>> 5d456af5ad28e948d4ea00556da10a34cb874093
    ECRational() {}
    
    ECRational(const T &numeratorIn, const T &denominatorIn) : numerator(numeratorIn) , denominator(denominatorIn) {}
    
    // copy constructor
    ECRational(const ECRational<T> &object) : numerator(object.numerator), denominator(object.denominator) {}
 
    // assignment operator
    ECRational<T>& operator=(const ECRational &rhs); 
    
<<<<<<< HEAD
    // operators: define +, -, *, / operators yourself
=======
>>>>>>> 5d456af5ad28e948d4ea00556da10a34cb874093
    ECRational<T> operator+(const ECRational &rhs); 
    
    ECRational<T> operator-(const ECRational &rhs); 

    ECRational<T> operator*(const ECRational &rhs); 

    ECRational<T> operator/(const ECRational &rhs); 

    // access numerator and denominator
    const T &GetNumerator() const {return numerator;}
    const T &GetDenominator() const {return denominator;}
    
private:
    // your code
    T numerator;
    T denominator;
};

<<<<<<< HEAD


=======
>>>>>>> 5d456af5ad28e948d4ea00556da10a34cb874093
#endif /* ECRational_h */
