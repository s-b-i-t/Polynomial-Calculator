    #ifndef Polynomial_h
    #define Polynomial_h

    #include <vector>
    #include <cmath>
    #include <iostream>
    #include <type_traits>
    #include <complex>
    // *****************************************************************
    // Represent a polynomial of a single variable, x, and
    // support common operations on polynomial: add, scaling, multiple, and division

    class Polynomial
    {
    public:
            
        Polynomial() : listCoeffsIn({0}) {}

        Polynomial(int coeff) : listCoeffsIn({static_cast<double>(coeff)}) {}

        Polynomial(double coeff) : listCoeffsIn({coeff}) {}

        Polynomial(const std::vector<double> &InputVec) : listCoeffsIn(InputVec)
    {
        while (listCoeffsIn.size() > 0 && std::abs(listCoeffsIn.back()) < 1e-10)
        {
            listCoeffsIn.pop_back();
        }
    }
        
        // Copy constructor
        Polynomial(const Polynomial &rhs) : listCoeffsIn(rhs.listCoeffsIn) {}
        
        
        // Scale by a constant and return resulting NEW polynomial. For example, if polynomial is 1+3x, and
        // factor = 2, the result is 2+6x
        Polynomial Scale(double factor) const;
        
        // Add a polynomial to the current polynomial (and return the result). Example: (1+2x) + (2x+3x^2) = 1+4x+3x^2
        Polynomial operator+(const Polynomial &rhs) const;

        Polynomial operator-(const Polynomial &rhs) const;
        
        // Multiply a polynomial by another polynomail and return the result. Example: (1+2x)*(1-x^2) = 1 + 2x - x^2 -2x^3
        Polynomial operator*(const Polynomial &rhs) const;

        Polynomial& operator=(const Polynomial &rhs);
        
        // Divide a polynomial by another, and return the quotient (and save the remainder to the passed in parameter called remainder)
        // Example: if we divide x^3-2x^2-4 by x-3, then quotient = x^2+x+3 and remainder is 5
        // For now, assume rhs (denominator) is zero
        Polynomial operator/(const Polynomial &rhs) const;

        // related operator: remainder
        Polynomial operator%(const Polynomial &rhs) const;

        //  Compound operators 

        Polynomial& operator+=(const Polynomial &rhs);

        Polynomial& operator-=(const Polynomial &rhs);

        Polynomial& operator*=(const Polynomial &rhs);

        Polynomial& operator/=(const Polynomial &rhs);

        Polynomial& operator%=(const Polynomial &rhs);

        bool operator==(const Polynomial &rhs) const;

        bool operator!=(const Polynomial &rhs) const;


        // Get the degree. Example: if polynomial=1+x+3x^3, degree=3
        int GetDegree() const;

        double  GetCoeff(int index) const; 
        
        void Dump() const;

        void SetCoeffAt(int index,double value);

        const std::vector<double>& getListCoeffsIn() const {return listCoeffsIn;}    


        // umm gotta figure out how to handle complex still

        // std::vector<double> getComplexCoeffLocations() const {return complexCoeffLocations;}\
        // void setComplexCoeffLocations(std::vector<double> complexCoeffLocationsIn) {complexCoeffLocations = complexCoeffLocationsIn;}

    private:

        std::vector<double> listCoeffsIn;
        // location and value of complex coefficients
        // std::vector<double> complexCoeffLocations;

        mutable std::vector<double> remainder;

        
    };


    // turn num * poly to poly * num
    Polynomial operator+(double lhs, const Polynomial& rhs);
    Polynomial operator+(int lhs, const Polynomial& rhs);        
    Polynomial operator-(double lhs, const Polynomial& rhs);
    Polynomial operator-(int lhs, const Polynomial& rhs);
    Polynomial operator*(double lhs, const Polynomial& rhs);
    Polynomial operator*(int lhs, const Polynomial& rhs);
    Polynomial operator/(double lhs, const Polynomial& rhs);
    Polynomial operator/(int lhs, const Polynomial& rhs);


    #endif 
