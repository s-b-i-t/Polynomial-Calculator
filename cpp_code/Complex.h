#ifndef ComplexPoly_h
#define ComplexPoly_h

#include<vector>
#include "Polynomial.h"
class ComplexPoly
{
    public:
    
    ComplexPoly() : listCoeffsIn({0}) {}
    ComplexPoly(const ComplexPoly &object) : listCoeffsIn(object.listCoeffsIn), ComplexPolyCoeffLocations(object.ComplexPolyCoeffLocations) {}


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

    ComplexPoly operator+(const Polynomial &rhs);
    ComplexPoly operator-(const Polynomial &rhs);
    ComplexPoly operator*(const Polynomial &rhs);
    ComplexPoly operator/(const Polynomial &rhs);


    ComplexPoly operator+(const ComplexPoly &rhs);
    ComplexPoly operator-(const ComplexPoly &rhs);
    ComplexPoly operator*(const ComplexPoly &rhs);
    ComplexPoly operator/(const ComplexPoly &rhs);


    const std::vector<double>& getListCoeffsIn() const {return listCoeffsIn;}
    const std::vector<double>& getComplexPolyCoeffLocations() const {return ComplexPolyCoeffLocations;}
    private:
    std::vector<double> listCoeffsIn;
    std::vector<double> ComplexPolyCoeffLocations;


    // todo: trim trailing zeros









};


    ComplexPoly operator+(const Polynomial &lhs,  ComplexPoly &rhs){
        return rhs + lhs;
    }

    ComplexPoly operator-(const Polynomial &lhs,  ComplexPoly &rhs){
        Polynomial tmpPoly = Polynomial(rhs.getListCoeffsIn());

        Polynomial res = lhs - tmpPoly;
        return ComplexPoly(res.getListCoeffsIn(), rhs.getComplexPolyCoeffLocations());


    } 

#endif