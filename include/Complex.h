#ifndef ComplexPoly_h
#define ComplexPoly_h

#include<vector>
#include "Polynomial.h"
class ComplexPoly
{
    public:
    
    ComplexPoly() 
    : listCoeffsIn({0}) {}

    

    ComplexPoly(const ComplexPoly &cp) 
    : listCoeffsIn(cp.listCoeffsIn), complexCoeffs(cp.complexCoeffs) {}

    explicit ComplexPoly(const Polynomial &poly)
    : listCoeffsIn(poly.getListCoeffsIn()),  complexCoeffs({}) {}


    ComplexPoly(const std::vector<double>& Polypart, const std::vector<double>& ComplexPart) 
    : listCoeffsIn(Polypart), complexCoeffs(ComplexPart) 
    {
        while (listCoeffsIn.size() > 0 && std::abs(listCoeffsIn.back()) < 1e-10)
        {
            listCoeffsIn.pop_back();
        }

        while (complexCoeffs.size() > 0 && std::abs(complexCoeffs.back()) < 1e-10)
        {
            complexCoeffs.pop_back();
        }

        if(listCoeffsIn.empty()){
            listCoeffsIn = {0};
        }

    }
    ComplexPoly(const std::initializer_list<double>& Polypart) 
    : listCoeffsIn(Polypart), complexCoeffs({}) 
    {
        while (listCoeffsIn.size() > 0 && std::abs(listCoeffsIn.back()) < 1e-10)
        {
            listCoeffsIn.pop_back();
        }

        while (complexCoeffs.size() > 0 && std::abs(complexCoeffs.back()) < 1e-10)
        {
            complexCoeffs.pop_back();
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
    const std::vector<double>& getcomplexCoeffsList() const {return complexCoeffs;}
    void Dump() const;
    double GetCoeff(int index) const;
    double getComplexCoeff(int index) const;

    static ComplexPoly Undefined();
    bool isUndefined() const;



    private:
    std::vector<double> listCoeffsIn;
    std::vector<double> complexCoeffs;



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