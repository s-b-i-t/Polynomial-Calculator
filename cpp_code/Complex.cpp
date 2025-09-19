#include "Polynomial.h"
#include "Complex.h"
#include <vector>
#include <algorithm>
using namespace std;


double ComplexPoly::GetCoeff(int index) const
{
    if (index < 0 || index >= listCoeffsIn.size())
    {
        return 0;
    }

    double coeff = listCoeffsIn[index];

    if (abs(coeff) < 1e-10)
    {
        return 0;
    }


    return coeff;
}


double ComplexPoly::getComplexCoeff(int index) const
{
    if (index < 0 || index >= complexCoeffs.size())
    {
        return 0;
    }

    double coeff = complexCoeffs[index];

    if (abs(coeff) < 1e-10)
    {
        return 0;
    }


    return coeff;
}


ComplexPoly ComplexPoly::operator+(const ComplexPoly &rhs) const{
    Polynomial tmpPoly = Polynomial(this->listCoeffsIn);
    Polynomial tmpPoly2 = Polynomial(rhs.listCoeffsIn);
    tmpPoly += tmpPoly2;

    // now to add the complex parts     
    vector<double> complexCoeffs = this->complexCoeffs;
    vector<double> complexCoeffs2 = rhs.complexCoeffs;

    // first pad smaller vector with zeros
    if (complexCoeffs.size() < complexCoeffs2.size())
    {
        complexCoeffs.resize(complexCoeffs2.size(), 0);
    }
    else if (complexCoeffs.size() > complexCoeffs2.size())
    {
        complexCoeffs2.resize(complexCoeffs.size(), 0);
    }

    // now add the vectors

    for (int i = 0; i < complexCoeffs.size(); ++i)
    {
        complexCoeffs[i] += complexCoeffs2[i];
    }

    return ComplexPoly(tmpPoly.getListCoeffsIn(), complexCoeffs);



}

ComplexPoly ComplexPoly::operator-(const ComplexPoly &rhs) const{
    // Subtract the real parts using the Polynomial class
    Polynomial tmpPoly = Polynomial(this->listCoeffsIn);
    Polynomial tmpPoly2 = Polynomial(rhs.listCoeffsIn);
    tmpPoly -= tmpPoly2;

    // Handle the complex parts (imaginary coefficients)
    std::vector<double> complexCoeffs = this->complexCoeffs;
    std::vector<double> complexCoeffs2 = rhs.complexCoeffs;

    // Pad the smaller vector with zeros
    if (complexCoeffs.size() < complexCoeffs2.size()) {
        complexCoeffs.resize(complexCoeffs2.size(), 0);
    } else if (complexCoeffs.size() > complexCoeffs2.size()) {
        complexCoeffs2.resize(complexCoeffs.size(), 0);
    }

    // Subtract the imaginary parts
    for (size_t i = 0; i < complexCoeffs.size(); ++i) {
        complexCoeffs[i] -= complexCoeffs2[i];
    }

    // Return the resulting ComplexPoly
    return ComplexPoly(tmpPoly.getListCoeffsIn(), complexCoeffs);
}



ComplexPoly ComplexPoly::operator*(const ComplexPoly &rhs) const{
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
    for (size_t i = 0; i < complexCoeffs.size(); ++i) {
        for (size_t j = 0; j < rhs.complexCoeffs.size(); ++j) {
            resultReal[i + j] -= complexCoeffs[i] * rhs.complexCoeffs[j]; // -ab
        }
    }

    // Multiply real with imaginary terms
    for (size_t i = 0; i < listCoeffsIn.size(); ++i) {
        for (size_t j = 0; j < rhs.complexCoeffs.size(); ++j) {
            resultImag[i + j] += listCoeffsIn[i] * rhs.complexCoeffs[j];
        }
    }

    for (size_t i = 0; i < complexCoeffs.size(); ++i) {
        for (size_t j = 0; j < rhs.listCoeffsIn.size(); ++j) {
            resultImag[i + j] += complexCoeffs[i] * rhs.listCoeffsIn[j];
        }
    }

    // Return the resulting ComplexPoly
    return ComplexPoly(resultReal, resultImag);
}

ComplexPoly ComplexPoly::operator/(const ComplexPoly &rhs) const{
    // Ensure the denominator is not zero
    if (rhs.listCoeffsIn.empty() || 
        (std::all_of(rhs.listCoeffsIn.begin(), rhs.listCoeffsIn.end(), [](double c) { return c == 0.0; }) &&
         std::all_of(rhs.complexCoeffs.begin(), rhs.complexCoeffs.end(), [](double c) { return c == 0.0; }))) {
        throw std::invalid_argument("Division by zero polynomial is not allowed.");
    }

    // Step 1: Calculate the conjugate of the denominator
    std::vector<double> conjugateImag = rhs.complexCoeffs;
    for (double &coeff : conjugateImag) {
        coeff = -coeff; // Negate the imaginary coefficients
    }
    ComplexPoly rhsConjugate(rhs.listCoeffsIn, conjugateImag);

    // Step 2: Multiply numerator and denominator by the conjugate
    ComplexPoly numerator = (*this) * rhsConjugate;
    ComplexPoly denominator =  rhsConjugate * rhs ;

    // Step 3: Simplify the coefficients (divide real and imaginary parts by the denominator's leading coefficient)
    std::vector<double> resultReal = numerator.listCoeffsIn;
    std::vector<double> resultImag = numerator.complexCoeffs;

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

ComplexPoly ComplexPoly::operator+ (const Polynomial &rhs) const{
    ComplexPoly new_complex = ComplexPoly(rhs);
    new_complex = *this + new_complex;
    return new_complex;

}

ComplexPoly ComplexPoly::operator- (const Polynomial &rhs) const{
    ComplexPoly new_complex = ComplexPoly(rhs);
    new_complex = *this - new_complex;
    return new_complex;

}

ComplexPoly ComplexPoly::operator* (const Polynomial &rhs) const{
    ComplexPoly new_complex = ComplexPoly(rhs);
    new_complex =  *this * new_complex;
    return new_complex;

}
ComplexPoly ComplexPoly::operator/ (const Polynomial &rhs) const{
    ComplexPoly new_complex = ComplexPoly(rhs);
    new_complex = *this / new_complex;
    return new_complex;

}



void ComplexPoly::Dump() const
{
    int maxDegree = std::max(listCoeffsIn.size(), complexCoeffs.size());
    bool firstTerm = true;

    for (int i = 0; i < maxDegree; ++i)
    {
        double realPart = (i < listCoeffsIn.size() ? listCoeffsIn[i] : 0.0);
        double imagPart = (i < complexCoeffs.size() ? complexCoeffs[i] : 0.0);

        // Skip if both real and imag are 0
        if (std::abs(realPart) < 1e-10 && std::abs(imagPart) < 1e-10)
            std::cout << "0";

        if (!firstTerm)
            std::cout << " + ";
        firstTerm = false;

        // Always wrap the coefficient in parentheses
        std::cout << "(";
        bool printedSomething = false;

        if (std::abs(realPart) > 1e-10) {
            std::cout << realPart;
            printedSomething = true;
        }

        if (std::abs(imagPart) > 1e-10) {
            if (printedSomething && imagPart >= 0) std::cout << "+";
            std::cout << imagPart << "i";
        }

        std::cout << ")";

        if (i > 0)
            std::cout << "x^" << i;
    }

    if (firstTerm) { // nothing printed (all zero)
        std::cout << "0";
    }

    std::cout << std::endl;
}
