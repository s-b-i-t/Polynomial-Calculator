//
//  ECPolynomial.cpp
//
//
//  Created by Yufeng Wu on 9/17/22.
//

#include "ECPolynomial.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iterator>
using namespace std;
// ECPolynomial ::  ECPolynomial(const std::vector<double> &listCoeffsIn){}

// ECPolynomial ::  ECPolynomial(const ECPolynomial &rhs){}

ECPolynomial::ECPolynomial(double coeff) : listCoeffsIn({coeff}) {}

// get coeff function
double ECPolynomial::GetCoeff(int index) const
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

void ECPolynomial ::SetCoeffAt(int index, double value)
{

    if (index > listCoeffsIn.size()){
        listCoeffsIn.resize(index + 1, 0);
    }

    if (index >= 0 && index >= listCoeffsIn.size())
    {
       
     this->listCoeffsIn[index] = value;      
    }
}

// degree function
int ECPolynomial ::GetDegree() const
{
    if (listCoeffsIn.size() == 0)
    {
        return 0;
    }
    
    int degree = listCoeffsIn.size() - 1;
    while ((abs(listCoeffsIn[degree]) < 1e-10 || listCoeffsIn[degree] == 0) && !listCoeffsIn.empty())
    {
        degree--;
    }

    return degree;
}

// scale function
ECPolynomial ECPolynomial ::Scale(double factor)
{
    vector<double> scaledCoeffs = listCoeffsIn;
    for (auto &x : scaledCoeffs)
    {
        x *= factor;
    }
    return ECPolynomial(scaledCoeffs);
}

// add function
ECPolynomial ECPolynomial ::operator+(const ECPolynomial &rhs) const
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

    return ECPolynomial(OutputVec);
}

// subtraction function

ECPolynomial ECPolynomial ::operator-(const ECPolynomial &rhs) const
{
    vector<double> tmpVec = rhs.listCoeffsIn;
    for (auto &x : tmpVec)
    {
        x *= -1;
    }
    ECPolynomial tmpPoly(tmpVec);

    return *this + tmpPoly;
}

// multiply function
ECPolynomial ECPolynomial ::operator*(const ECPolynomial &rhs) const
{
    vector<double> OutputVec(rhs.listCoeffsIn.size() + listCoeffsIn.size() - 1, 0);

    for (int i = 0; i < listCoeffsIn.size(); ++i)
    {
        for (int j = 0; j < rhs.listCoeffsIn.size(); ++j)
        {
            OutputVec[i + j] += listCoeffsIn[i] * rhs.listCoeffsIn[j];
        }
    }
    return ECPolynomial(OutputVec);
}
// divide function

ECPolynomial ECPolynomial::operator/(const ECPolynomial &rhs) const
{
    int dividendDegree = GetDegree();
    int divisorDegree = rhs.GetDegree();

    if (dividendDegree < divisorDegree)
    {
        return ECPolynomial(vector<double>{0});
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
    return ECPolynomial(quotient);
}

// remainder function
ECPolynomial ECPolynomial ::operator%(const ECPolynomial &rhs) const
{
    vector<double> remVec = remainder;

    int tmp = remVec.size() - 1;

    while (remVec[tmp] == 0)
    {
        remVec.pop_back();
        tmp--;
    }
    return ECPolynomial(remVec);
}

// equal function (assignment)
ECPolynomial ECPolynomial ::operator=(const ECPolynomial &rhs) const
{
    vector<double> tmpVec;
    for (auto x : rhs.listCoeffsIn)
    {
        tmpVec.push_back(x);
    }
    ECPolynomial tmpPoly(tmpVec);
    return tmpPoly;
}

// dump function
void ECPolynomial ::Dump() const
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

// void ECPolynomial::Dump() const
// {
//     cout << "Deg: " << GetDegree() << ":  ";
//     for (int i = 0; i < listCoeffsIn.size(); i++)
//     {
//         cout << listCoeffsIn[i] << " ";
//     }
//     cout << endl;
// }
