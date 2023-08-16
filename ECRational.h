//
//  ECRational.h
//  
//
//  Created by Yufeng Wu on 1/7/23.
//

#ifndef ECRational_h
#define ECRational_h


// *****************************************************************
// Generic rational of two quantities (e.g. integers, floating point, complex number, or polynomila)
// Assume the quantity class T supports (i) default constructor and copy constructor, (ii) assignment operator, and (iii) arithmatic operators: +, -, * and /
// Note: you don't need to simplify the rational. That is, it is OK to have common factors in numerator and denominoator. For example, 4/4 = 1/1
template <class T>
class ECRational
{
public:
    // YW: change the following code if needed...
    ECRational() {}
    
    ECRational(const T &numeratorIn, const T &denominatorIn) : numerator(numeratorIn) , denominator(denominatorIn) {}
    
    // copy constructor
    ECRational(const ECRational<T> &object) : numerator(object.numerator), denominator(object.denominator) {}
 
    // assignment operator
    ECRational<T>& operator=(const ECRational &rhs); 
    
    // operators: define +, -, *, / operators yourself
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



#endif /* ECRational_h */
