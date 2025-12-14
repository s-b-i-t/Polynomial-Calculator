#include "Polynomial.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <string>
#include <cstring>

using namespace std;
// Polynomial ::  Polynomial(const std::vector<double> &listCoeffsIn){}

// Polynomial ::  Polynomial(const Polynomial &rhs){}

// Polynomial::Polynomial(double coeff) : listCoeffsIn({coeff}) {}

// get coeff function
double Polynomial::GetCoeff(int index) const
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

void Polynomial::SetCoeffAt(int index, double value) {
    if (index < 0) return;
    
    if (index >= (int)listCoeffsIn.size()) 
    {
        listCoeffsIn.resize(index + 1, 0.0);
    }
    listCoeffsIn[index] = value;
}

// degree function
int Polynomial ::GetDegree() const
{
int degree = (int)listCoeffsIn.size() - 1;
while (degree >= 0 && std::abs(listCoeffsIn[degree]) < 1e-10) {
    --degree;
}
return std::max(degree, 0);

}

// scale function
Polynomial Polynomial ::Scale(double factor) const 
{
    vector<double> scaledCoeffs = listCoeffsIn;
    for (auto &x : scaledCoeffs)
    {
        x *= factor;
    }
    return Polynomial(scaledCoeffs);
}

// add function
Polynomial Polynomial ::operator+(const Polynomial &rhs) const
{
    vector<double> OutputVec;
    int tmpSize = max(listCoeffsIn.size(), rhs.listCoeffsIn.size());

    for (int i = 0; i < tmpSize; ++i)
    {
        double cur = 0;
        if (i < listCoeffsIn.size())
        {
            cur += listCoeffsIn[i];
        }
        if (i < rhs.listCoeffsIn.size())
        {
            cur += rhs.listCoeffsIn[i];
        }
        OutputVec.push_back(cur);
    }

    return Polynomial(OutputVec);
}

// subtraction function

Polynomial Polynomial ::operator-(const Polynomial &rhs) const
{
    vector<double> tmpVec = rhs.listCoeffsIn;
    for (auto &x : tmpVec)
    {
        x *= -1;
    }
    Polynomial tmpPoly(tmpVec);

    return *this + tmpPoly;
}

// multiply function
Polynomial Polynomial ::operator*(const Polynomial &rhs) const
{
    vector<double> OutputVec(rhs.listCoeffsIn.size() + listCoeffsIn.size() - 1, 0);

    for (int i = 0; i < listCoeffsIn.size(); ++i)
    {
        for (int j = 0; j < rhs.listCoeffsIn.size(); ++j)
        {
            OutputVec[i + j] += listCoeffsIn[i] * rhs.listCoeffsIn[j];
        }
    }
    return Polynomial(OutputVec);
}
// divide function

Polynomial Polynomial::operator/(const Polynomial &rhs) const
{
    int dividendDegree = GetDegree();
    int divisorDegree = rhs.GetDegree();

    if (dividendDegree < divisorDegree)
    {
        return Polynomial(vector<double>{0});
    }

    vector<double> quotient(dividendDegree - divisorDegree + 1, 0);
    vector<double> workingCoeffs = listCoeffsIn;

    for (int i = 0; i <= dividendDegree - divisorDegree; ++i)
    {
        int currentDegree = dividendDegree - i;

        double factor = workingCoeffs[currentDegree] / rhs.listCoeffsIn[divisorDegree];
        quotient[currentDegree - divisorDegree] = factor;

        for (int j = 0; j <= divisorDegree; ++j)
        {
            workingCoeffs[currentDegree - j] -= factor * rhs.listCoeffsIn[divisorDegree - j];
        }
    }
    remainder = workingCoeffs;
    return Polynomial(quotient);
}

// remainder function
// a % b = a - (b * int(a/b))

Polynomial Polynomial ::operator%(const Polynomial &rhs) const
{
    Polynomial quotient = *this / rhs;
    return Polynomial(remainder);
}


// equal function (assignment)
Polynomial& Polynomial::operator=(const Polynomial &rhs)
{
 
    listCoeffsIn = rhs.listCoeffsIn; 

    return *this; // Return the current object
}



Polynomial& Polynomial::operator+=(const Polynomial &rhs) 
{
    *this = *this + rhs;
    return *this;
}

Polynomial& Polynomial::operator-=(const Polynomial &rhs) 
{
    *this = *this - rhs;
    return *this;
}

Polynomial& Polynomial::operator*=(const Polynomial &rhs) 
{
    *this = *this * rhs;
    return *this;
}

Polynomial& Polynomial::operator/=(const Polynomial &rhs) 
{
    *this = *this / rhs;
    return *this;
}

Polynomial& Polynomial::operator%=(const Polynomial &rhs) 
{
    *this = *this % rhs;
    return *this;
}

bool Polynomial::operator==(const Polynomial &rhs) const
{
    if (listCoeffsIn.size() != rhs.listCoeffsIn.size())
    {
        return false;
    }

    if (listCoeffsIn == rhs.listCoeffsIn)
    {
        return true;
    }
    return false;
}




bool Polynomial::operator!=(const Polynomial &rhs) const
{
    return !(listCoeffsIn == rhs.listCoeffsIn);
}



void Polynomial ::Dump() const
{

    for (int i = 0; i < listCoeffsIn.size(); ++i)
    {
        if (i == 0)
        {
            cout << listCoeffsIn[i];
        }
        else
        {
            cout << listCoeffsIn[i] << "x" << "^" << i;
        }
        if (i != listCoeffsIn.size() - 1)
            cout << " + ";
    }
    cout << endl;
}

    Polynomial operator+(double lhs, const Polynomial& rhs) {
    Polynomial lhsPoly(lhs);
    return lhsPoly + rhs;
    }

    Polynomial operator+(int lhs, const Polynomial& rhs) {
    Polynomial lhsPoly(lhs);
    return lhsPoly + rhs;
    }

    Polynomial operator-(double lhs, const Polynomial& rhs) {
    
    Polynomial lhsPoly(lhs); 
    return lhsPoly - rhs;    
    
    }

    Polynomial operator-(int lhs, const Polynomial& rhs) {

    Polynomial lhsPoly(lhs); 
    return lhsPoly - rhs;    
    }

    Polynomial operator*(double lhs, const Polynomial& rhs) {
    Polynomial lhsPoly(lhs);
    return lhsPoly * rhs;
    }

    Polynomial operator*(int lhs, const Polynomial& rhs) {
    Polynomial lhsPoly(lhs);
    return lhsPoly * rhs;
    }

    Polynomial operator/(double lhs, const Polynomial& rhs) {
    Polynomial lhsPoly(lhs);
    return lhsPoly / rhs;
    }

    Polynomial operator/(int lhs, const Polynomial& rhs) {
    Polynomial lhsPoly(lhs);
    return lhsPoly / rhs;
    }









// dump function
// char* Polynomial::Dump() const
// {
//     std::string result;

//     for (int i = 0; i < listCoeffsIn.size(); ++i)
//     {
//         if (i == 0)
//         {
//             result += std::to_string(listCoeffsIn[i]);
//         }
//         else
//         {
//             result += " + " + std::to_string(listCoeffsIn[i]) + "x^" + std::to_string(i);
//         }
//     }

//     char* cstr = new char[result.length() + 1];
//     std::strcpy(cstr, result.c_str());

//     return cstr;
// }


// No longer necesary lol
// char* Polynomial::Windows_Output() const {
//     std::string result = "Polynomial: ";
//     for (size_t i = 0; i < listCoeffsIn.size(); ++i) {
//         result += std::to_string(listCoeffsIn[i]) + "x^" + std::to_string(i) + " ";
//     }
//     char* cstr = new char[result.length() + 1];
//     std::strcpy(cstr, result.c_str());
//     return cstr;
// }

