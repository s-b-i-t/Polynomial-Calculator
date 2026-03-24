/*
 CHANGELOG (Codex robustness pass)
 - Implement bounded dense adapters (`getListCoeffsIn`) without changing sparse truth.
 - Add `NearlyEquals` for tolerance-aware comparisons; keep `operator==` strict on normalized terms.
 - Avoid accidental map mutation on reads (no operator[] for read paths).
 - Keep DivMod side-effect-free and deterministic.
 - Rewrite Scale() as a single clear block (no drift/dangling guards).
 */

#include "Polynomial.h"

#include <algorithm>

Polynomial::Polynomial(int coeff)
{
    SetCoeffAt(0, static_cast<double>(coeff));
}

Polynomial::Polynomial(double coeff)
{
    SetCoeffAt(0, coeff);
}

Polynomial::Polynomial(const std::vector<double> &InputVec)
{
    for (int i = 0; i < static_cast<int>(InputVec.size()); ++i)
    {
        SetCoeffAt(i, InputVec[i]);
    }
    Normalize();
}

double Polynomial::GetCoeff(int exponent) const
{
    if (exponent < 0)
    {
        return 0.0;
    }
    auto it = terms.find(exponent);
    if (it == terms.end())
    {
        return 0.0;
    }
    return it->second;
}

void Polynomial::SetCoeffAt(int exponent, double value)
{
    if (exponent < 0)
    {
        return;
    }

    if (NearlyZero(value))
    {
        terms.erase(exponent);
        return;
    }

    terms[exponent] = value;
}

int Polynomial::GetDegree() const
{
    if (IsZero())
    {
        return -1;
    }
    return terms.rbegin()->first;
}

std::pair<int, double> Polynomial::LeadingTerm() const
{
    if (IsZero())
    {
        throw std::invalid_argument("LeadingTerm() of zero polynomial");
    }
    const auto it = terms.rbegin();
    return {it->first, it->second};
}

void Polynomial::Normalize()
{
    for (auto it = terms.begin(); it != terms.end();)
    {
        if (NearlyZero(it->second))
        {
            it = terms.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

std::vector<double> Polynomial::ToDenseBounded(int maxExponent) const
{
    if (maxExponent < 0)
    {
        return std::vector<double>{0.0};
    }

    const int cap = std::min(maxExponent, DENSE_EXPORT_MAX_EXPONENT);
    const int deg = GetDegree();
    const int size = std::max(1, std::min(deg, cap) + 1);

    std::vector<double> out(static_cast<size_t>(size), 0.0);
    for (const auto &[exp, coeff] : terms)
    {
        if (exp < 0 || exp > cap)
        {
            continue;
        }
        if (exp < size)
        {
            out[static_cast<size_t>(exp)] = coeff;
        }
    }
    return out;
}

const std::vector<double> &Polynomial::getListCoeffsIn() const
{
    dense_cache = ToDenseBounded(DENSE_EXPORT_MAX_EXPONENT);
    return dense_cache;
}

std::vector<double> Polynomial::getListCoeffsIn(int maxExponent) const
{
    return ToDenseBounded(maxExponent);
}

Polynomial Polynomial::Scale(double factor) const
{
    if (IsZero() || factor == 0.0)
    {
        return Polynomial();
    }

    Polynomial out;
    for (const auto &[exp, coeff] : terms)
    {
        out.SetCoeffAt(exp, coeff * factor);
    }
    out.Normalize();
    return out;
}

Polynomial Polynomial::operator+(const Polynomial &rhs) const
{
    if (IsZero())
    {
        return rhs;
    }
    if (rhs.IsZero())
    {
        return *this;
    }

    Polynomial out = *this;
    for (const auto &[exp, coeff] : rhs.terms)
    {
        out.SetCoeffAt(exp, out.GetCoeff(exp) + coeff);
    }
    out.Normalize();
    return out;
}

Polynomial Polynomial::operator-(const Polynomial &rhs) const
{
    if (rhs.IsZero())
    {
        return *this;
    }

    Polynomial out = *this;
    for (const auto &[exp, coeff] : rhs.terms)
    {
        out.SetCoeffAt(exp, out.GetCoeff(exp) - coeff);
    }
    out.Normalize();
    return out;
}

Polynomial Polynomial::operator*(const Polynomial &rhs) const
{
    if (IsZero() || rhs.IsZero())
    {
        return Polynomial();
    }

    // Accumulate in higher precision to reduce catastrophic cancellation when
    // coefficients get large during division-derived computations.
    std::map<int, long double> acc;
    for (const auto &[expA, coeffA] : terms)
    {
        for (const auto &[expB, coeffB] : rhs.terms)
        {
            const int exp = expA + expB;
            acc[exp] += static_cast<long double>(coeffA) * static_cast<long double>(coeffB);
        }
    }

    Polynomial out;
    for (const auto &[exp, coeff] : acc)
    {
        const double v = static_cast<double>(coeff);
        if (!NearlyZero(v))
        {
            out.SetCoeffAt(exp, v);
        }
    }
    out.Normalize();
    return out;
}

std::pair<Polynomial, Polynomial> Polynomial::DivMod(const Polynomial &a, const Polynomial &b)
{
    if (b.IsZero())
    {
        throw std::invalid_argument("Division by zero polynomial");
    }

    // Compute division in long double to reduce rounding amplification.
    std::map<int, long double> rem;
    for (const auto &[e, c] : a.terms)
    {
        rem[e] = static_cast<long double>(c);
    }

    std::map<int, long double> div;
    for (const auto &[e, c] : b.terms)
    {
        div[e] = static_cast<long double>(c);
    }

    const int degB = b.GetDegree();
    const long double leadB = div.rbegin()->second;

    std::map<int, long double> quot;

    const long double internalTrim = 1e-18L;

    while (!rem.empty())
    {
        const int degR = rem.rbegin()->first;
        const long double leadR = rem.rbegin()->second;
        if (degR < degB)
        {
            break;
        }

        const int exp = degR - degB;
        const long double factor = leadR / leadB;

        quot[exp] += factor;

        for (const auto &[e, c] : div)
        {
            const int targetExp = e + exp;
            rem[targetExp] -= factor * c;
            if (std::abs(rem[targetExp]) < internalTrim)
            {
                rem.erase(targetExp);
            }
        }
    }

    Polynomial quotient;
    for (const auto &[e, c] : quot)
    {
        const double v = static_cast<double>(c);
        if (!NearlyZero(v))
        {
            quotient.SetCoeffAt(e, v);
        }
    }
    quotient.Normalize();

    Polynomial remainder;
    for (const auto &[e, c] : rem)
    {
        const double v = static_cast<double>(c);
        if (!NearlyZero(v))
        {
            remainder.SetCoeffAt(e, v);
        }
    }
    remainder.Normalize();

    return {quotient, remainder};
}

Polynomial Polynomial::operator/(const Polynomial &rhs) const
{
    return DivMod(*this, rhs).first;
}

Polynomial Polynomial::operator%(const Polynomial &rhs) const
{
    return DivMod(*this, rhs).second;
}

Polynomial &Polynomial::operator+=(const Polynomial &rhs)
{
    *this = *this + rhs;
    return *this;
}

Polynomial &Polynomial::operator-=(const Polynomial &rhs)
{
    *this = *this - rhs;
    return *this;
}

Polynomial &Polynomial::operator*=(const Polynomial &rhs)
{
    *this = *this * rhs;
    return *this;
}

Polynomial &Polynomial::operator/=(const Polynomial &rhs)
{
    *this = DivMod(*this, rhs).first;
    return *this;
}

Polynomial &Polynomial::operator%=(const Polynomial &rhs)
{
    *this = DivMod(*this, rhs).second;
    return *this;
}

bool Polynomial::operator==(const Polynomial &rhs) const
{
    return terms == rhs.terms;
}

bool Polynomial::NearlyEquals(const Polynomial &rhs, double eps) const
{
    // Compare the union of keys without relying on exact double equality.
    auto itA = terms.begin();
    auto itB = rhs.terms.begin();

    while (itA != terms.end() || itB != rhs.terms.end())
    {
        int exp = 0;
        double a = 0.0;
        double b = 0.0;

        if (itB == rhs.terms.end() || (itA != terms.end() && itA->first < itB->first))
        {
            exp = itA->first;
            a = itA->second;
            b = 0.0;
            ++itA;
        }
        else if (itA == terms.end() || (itB != rhs.terms.end() && itB->first < itA->first))
        {
            exp = itB->first;
            a = 0.0;
            b = itB->second;
            ++itB;
        }
        else
        {
            exp = itA->first;
            a = itA->second;
            b = itB->second;
            ++itA;
            ++itB;
        }

        (void)exp;
        if (std::abs(a - b) > eps)
        {
            return false;
        }
    }
    return true;
}

void Polynomial::Dump() const
{
    if (IsZero())
    {
        std::cout << "0" << std::endl;
        return;
    }

    bool first = true;

    for (auto it = terms.rbegin(); it != terms.rend(); ++it)
    {
        const int exp = it->first;
        const double coeff = it->second;

        if (NearlyZero(coeff))
        {
            continue;
        }

        if (first)
        {
            if (coeff < 0)
            {
                std::cout << "-";
            }
        }
        else
        {
            std::cout << (coeff < 0 ? " - " : " + ");
        }

        const double absCoeff = std::abs(coeff);
        const bool omitCoeff = (exp != 0) && NearlyZero(absCoeff - 1.0);

        if (!omitCoeff)
        {
            std::cout << absCoeff;
        }

        if (exp != 0)
        {
            if (exp == 1)
            {
                std::cout << "x";
            }
            else
            {
                std::cout << "x^" << exp;
            }
        }

        first = false;
    }

    std::cout << std::endl;
}

Polynomial operator+(double lhs, const Polynomial &rhs)
{
    return Polynomial(lhs) + rhs;
}

Polynomial operator+(int lhs, const Polynomial &rhs)
{
    return Polynomial(lhs) + rhs;
}

Polynomial operator-(double lhs, const Polynomial &rhs)
{
    return Polynomial(lhs) - rhs;
}

Polynomial operator-(int lhs, const Polynomial &rhs)
{
    return Polynomial(lhs) - rhs;
}

Polynomial operator*(double lhs, const Polynomial &rhs)
{
    return Polynomial(lhs) * rhs;
}

Polynomial operator*(int lhs, const Polynomial &rhs)
{
    return Polynomial(lhs) * rhs;
}

Polynomial operator/(double lhs, const Polynomial &rhs)
{
    return Polynomial(lhs) / rhs;
}

Polynomial operator/(int lhs, const Polynomial &rhs)
{
    return Polynomial(lhs) / rhs;
}
