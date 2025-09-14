#ifndef ComplexPoly_h
#define ComplexPoly_h

#include<vector>
#include "Polynomial.h"
class ComplexPoly
{
    public:
    
    ComplexPoly() : listCoeffsIn({0}) {}
    ComplexPoly(const ComplexPoly &cp) : listCoeffsIn(cp.listCoeffsIn), ComplexPolyCoeffLocations(cp.ComplexPolyCoeffLocations) {}

    explicit ComplexPoly(const Polynomial &poly) : listCoeffsIn(poly.getListCoeffsIn()),  ComplexPolyCoeffLocations({}) {}


    ComplexPoly(const std::vector<double>& Polypart, const std::vector<double>& ComplexPart) : listCoeffsIn(Polypart), ComplexPolyCoeffLocations(ComplexPart) 
    {
        while (listCoeffsIn.size() > 0 && std::abs(listCoeffsIn.back()) < 1e-10)
        {
            listCoeffsIn.pop_back();
        }

        while (ComplexPolyCoeffLocations.size() > 0 && std::abs(ComplexPolyCoeffLocations.back()) < 1e-10)
        {
            ComplexPolyCoeffLocations.pop_back();
        }

    }

    ComplexPoly operator+(const Polynomial &rhs) const;
    ComplexPoly operator-(const Polynomial &rhs) const;
    ComplexPoly operator*(const Polynomial &rhs) const;
    ComplexPoly operator/(const Polynomial &rhs) const;


    ComplexPoly operator+(const ComplexPoly &rhs) const;
    ComplexPoly operator-(const ComplexPoly &rhs) const;
    ComplexPoly operator*(const ComplexPoly &rhs) const;
    ComplexPoly operator/(const ComplexPoly &rhs) const;


    const std::vector<double>& getListCoeffsIn() const {return listCoeffsIn;}
    const std::vector<double>& getComplexPolyCoeffLocations() const {return ComplexPolyCoeffLocations;}
    private:
    std::vector<double> listCoeffsIn;
    std::vector<double> ComplexPolyCoeffLocations;

    // todo: trim trailing zeros


};

inline ComplexPoly operator+(const Polynomial &lhs, const ComplexPoly &rhs) { return rhs + lhs; }
inline ComplexPoly operator-(const Polynomial &lhs, const ComplexPoly &rhs) { return ComplexPoly(lhs) - rhs; }
inline ComplexPoly operator*(const Polynomial &lhs, const ComplexPoly &rhs) { return rhs * lhs; }
inline ComplexPoly operator/(const Polynomial &lhs, const ComplexPoly &rhs) { 
    ComplexPoly lhs_cp(lhs);
    return lhs_cp / rhs;
}



#endif