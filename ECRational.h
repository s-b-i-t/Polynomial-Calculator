//  ECRational.h
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
    ECRational() {}
    
    ECRational(const T &numeratorIn, const T &denominatorIn) : numerator(numeratorIn) , denominator(denominatorIn) {}
    
    // copy constructor
    ECRational(const ECRational<T> &object) : numerator(object.numerator), denominator(object.denominator) {}
 
    // assignment operator
    ECRational<T>& operator=(const ECRational &rhs); 
    
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
