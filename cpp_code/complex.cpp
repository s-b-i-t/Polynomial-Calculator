#include "Polynomial.h"
#include "Complex.h"
#include <vector>
#include <algorithm>
using namespace std;

// ComplexPoly ComplexPoly::operator+(const Polynomial &rhs){
//     vector<double> listCoeffsIn = rhs.getListCoeffsIn();
//     vector<double> ComplexPolyCoeffLocations = rhs.getComplexPolyCoeffLocations();
//     int degree = rhs.GetDegree();

//     Polynomial Res;

//     Res = rhs + this->real;

//     if (this->imag != 0)
//     {
//         if (ComplexPolyCoeffLocations.empty())
//         {
//             ComplexPolyCoeffLocations.push_back(this->imag);
//         }
//         else
//         {
//             ComplexPolyCoeffLocations[0] += this->imag;
//         }
//     }

//     Res.setComplexPolyCoeffLocations(ComplexPolyCoeffLocations);
//     return Res;
//     }


// ComplexPoly ComplexPoly::operator-(const Polynomial &rhs){
    
//     ComplexPoly new_ComplexPoly = ComplexPoly(-this->real, -this->imag);

//     Polynomial Res;

//     Res = new_ComplexPoly + rhs;

//     return Res;

// }



ComplexPoly ComplexPoly::operator+(const Polynomial &rhs){
    Polynomial tmpPoly = Polynomial(this->listCoeffsIn);
    tmpPoly += rhs;
    return ComplexPoly(tmpPoly.getListCoeffsIn(), ComplexPolyCoeffLocations);

}

ComplexPoly ComplexPoly::operator-(const Polynomial &rhs){
    Polynomial tmpPoly = Polynomial(this->listCoeffsIn);
    tmpPoly -= rhs;
    return ComplexPoly(tmpPoly.getListCoeffsIn(), ComplexPolyCoeffLocations);
}


ComplexPoly ComplexPoly::operator*(const Polynomial &rhs){
    ;
}



ComplexPoly ComplexPoly::operator/(const Polynomial &rhs)
{
;
}





ComplexPoly ComplexPoly::operator+(const ComplexPoly &rhs){
    Polynomial tmpPoly = Polynomial(this->listCoeffsIn);
    Polynomial tmpPoly2 = Polynomial(rhs.listCoeffsIn);
    tmpPoly += tmpPoly2;

    // now to add the complex parts     
    vector<double> ComplexPolyCoeffLocations = this->ComplexPolyCoeffLocations;
    vector<double> ComplexPolyCoeffLocations2 = rhs.ComplexPolyCoeffLocations;

    // first pad smaller vector with zeros
    if (ComplexPolyCoeffLocations.size() < ComplexPolyCoeffLocations2.size())
    {
        ComplexPolyCoeffLocations.resize(ComplexPolyCoeffLocations2.size(), 0);
    }
    else if (ComplexPolyCoeffLocations.size() > ComplexPolyCoeffLocations2.size())
    {
        ComplexPolyCoeffLocations2.resize(ComplexPolyCoeffLocations.size(), 0);
    }

    // now add the vectors

    for (int i = 0; i < ComplexPolyCoeffLocations.size(); ++i)
    {
        ComplexPolyCoeffLocations[i] += ComplexPolyCoeffLocations2[i];
    }



}

ComplexPoly ComplexPoly::operator-(const ComplexPoly &rhs) {
    // Subtract the real parts using the Polynomial class
    Polynomial tmpPoly = Polynomial(this->listCoeffsIn);
    Polynomial tmpPoly2 = Polynomial(rhs.listCoeffsIn);
    tmpPoly -= tmpPoly2;

    // Handle the complex parts (imaginary coefficients)
    std::vector<double> ComplexPolyCoeffLocations = this->ComplexPolyCoeffLocations;
    std::vector<double> ComplexPolyCoeffLocations2 = rhs.ComplexPolyCoeffLocations;

    // Pad the smaller vector with zeros
    if (ComplexPolyCoeffLocations.size() < ComplexPolyCoeffLocations2.size()) {
        ComplexPolyCoeffLocations.resize(ComplexPolyCoeffLocations2.size(), 0);
    } else if (ComplexPolyCoeffLocations.size() > ComplexPolyCoeffLocations2.size()) {
        ComplexPolyCoeffLocations2.resize(ComplexPolyCoeffLocations.size(), 0);
    }

    // Subtract the imaginary parts
    for (size_t i = 0; i < ComplexPolyCoeffLocations.size(); ++i) {
        ComplexPolyCoeffLocations[i] -= ComplexPolyCoeffLocations2[i];
    }

    // Return the resulting ComplexPoly
    return ComplexPoly(tmpPoly.getListCoeffsIn(), ComplexPolyCoeffLocations);
}



ComplexPoly ComplexPoly::operator*(const ComplexPoly &rhs) {
    // Determine the size of the result polynomial
    size_t maxDegree = listCoeffsIn.size() + rhs.listCoeffsIn.size() - 1;

    // Initialize result vectors
    std::vector<double> resultReal(maxDegree, 0.0);
    std::vector<double> resultImag(maxDegree, 0.0);

    // Multiply real parts
    for (size_t i = 0; i < listCoeffsIn.size(); ++i) {
        for (size_t j = 0; j < rhs.listCoeffsIn.size(); ++j) {
            resultReal[i + j] += listCoeffsIn[i] * rhs.listCoeffsIn[j];
        }
    }

    // Multiply imaginary parts (Imaginary * Imaginary = Real with - sign)
    for (size_t i = 0; i < ComplexPolyCoeffLocations.size(); ++i) {
        for (size_t j = 0; j < rhs.ComplexPolyCoeffLocations.size(); ++j) {
            resultReal[i + j] -= ComplexPolyCoeffLocations[i] * rhs.ComplexPolyCoeffLocations[j]; // -ab
        }
    }

    // Multiply real with imaginary terms
    for (size_t i = 0; i < listCoeffsIn.size(); ++i) {
        for (size_t j = 0; j < rhs.ComplexPolyCoeffLocations.size(); ++j) {
            resultImag[i + j] += listCoeffsIn[i] * rhs.ComplexPolyCoeffLocations[j];
        }
    }

    for (size_t i = 0; i < ComplexPolyCoeffLocations.size(); ++i) {
        for (size_t j = 0; j < rhs.listCoeffsIn.size(); ++j) {
            resultImag[i + j] += ComplexPolyCoeffLocations[i] * rhs.listCoeffsIn[j];
        }
    }

    // Return the resulting ComplexPoly
    return ComplexPoly(resultReal, resultImag);
}

ComplexPoly ComplexPoly::operator/(const ComplexPoly &rhs) {
    // Ensure the denominator is not zero
    if (rhs.listCoeffsIn.empty() || 
        (std::all_of(rhs.listCoeffsIn.begin(), rhs.listCoeffsIn.end(), [](double c) { return c == 0.0; }) &&
         std::all_of(rhs.ComplexPolyCoeffLocations.begin(), rhs.ComplexPolyCoeffLocations.end(), [](double c) { return c == 0.0; }))) {
        throw std::invalid_argument("Division by zero polynomial is not allowed.");
    }

    // Step 1: Calculate the conjugate of the denominator
    std::vector<double> conjugateImag = rhs.ComplexPolyCoeffLocations;
    for (double &coeff : conjugateImag) {
        coeff = -coeff; // Negate the imaginary coefficients
    }
    ComplexPoly rhsConjugate(rhs.listCoeffsIn, conjugateImag);

    // Step 2: Multiply numerator and denominator by the conjugate
    ComplexPoly numerator = (*this) * rhsConjugate;
    ComplexPoly denominator = rhs * rhsConjugate;

    // Step 3: Simplify the coefficients (divide real and imaginary parts by the denominator's leading coefficient)
    std::vector<double> resultReal = numerator.listCoeffsIn;
    std::vector<double> resultImag = numerator.ComplexPolyCoeffLocations;

    double denomLeadingCoeff = denominator.listCoeffsIn[0]; // Assuming leading coefficient is at index 0
    for (double &coeff : resultReal) {
        coeff /= denomLeadingCoeff;
    }
    for (double &coeff : resultImag) {
        coeff /= denomLeadingCoeff;
    }

    // Step 4: Return the resulting ComplexPoly
    return ComplexPoly(resultReal, resultImag);
}
