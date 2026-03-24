/*
 CHANGELOG (Codex robustness pass)
 - Add bounded dense adapters for UI/legacy compatibility.
 - Avoid `operator[]` insertion traps on read paths; use find() everywhere.
 - Add `NearlyEquals` for tolerance-aware comparisons.
 - Rewrite Dump() as a clean case-based formatter (stable ordering, sane +/- handling, no junk zeros).
 - Fix ComplexPoly::Scale() to propagate `undefined` and keep control-flow simple.
 */

#include "Complex.h"

#include <algorithm>
#include <iostream>

ComplexPoly::ComplexPoly(const Polynomial &poly)
{
    for (const auto &[exp, coeff] : poly.GetTerms())
    {
        if (!NearlyZero(coeff))
        {
            terms[exp] = std::complex<double>(coeff, 0.0);
        }
    }
    Normalize();
}

ComplexPoly::ComplexPoly(const std::vector<double> &Polypart, const std::vector<double> &ComplexPart)
{
    const int maxSize = std::max(static_cast<int>(Polypart.size()), static_cast<int>(ComplexPart.size()));
    for (int i = 0; i < maxSize; ++i)
    {
        const double re = i < static_cast<int>(Polypart.size()) ? Polypart[i] : 0.0;
        const double im = i < static_cast<int>(ComplexPart.size()) ? ComplexPart[i] : 0.0;
        const std::complex<double> z(re, im);
        if (!NearlyZero(z))
        {
            terms[i] = z;
        }
    }
    Normalize();
}

ComplexPoly::ComplexPoly(const std::initializer_list<double> &Polypart)
{
    int i = 0;
    for (double re : Polypart)
    {
        if (!NearlyZero(re))
        {
            terms[i] = std::complex<double>(re, 0.0);
        }
        ++i;
    }
    Normalize();
}

int ComplexPoly::GetDegree() const
{
    if (IsZero())
    {
        return -1;
    }
    return terms.rbegin()->first;
}

std::pair<int, std::complex<double>> ComplexPoly::LeadingTerm() const
{
    if (IsZero())
    {
        throw std::invalid_argument("LeadingTerm() of zero complex polynomial");
    }
    const auto it = terms.rbegin();
    return {it->first, it->second};
}

void ComplexPoly::Normalize()
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

std::vector<double> ComplexPoly::ToDenseRealBounded(int maxExponent) const
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
            out[static_cast<size_t>(exp)] = coeff.real();
        }
    }
    return out;
}

std::vector<double> ComplexPoly::ToDenseImagBounded(int maxExponent) const
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
            out[static_cast<size_t>(exp)] = coeff.imag();
        }
    }
    return out;
}

const std::vector<double> &ComplexPoly::getListCoeffsIn() const
{
    dense_real_cache = ToDenseRealBounded(DENSE_EXPORT_MAX_EXPONENT);
    return dense_real_cache;
}

const std::vector<double> &ComplexPoly::getcomplexCoeffsList() const
{
    dense_imag_cache = ToDenseImagBounded(DENSE_EXPORT_MAX_EXPONENT);
    return dense_imag_cache;
}

std::vector<double> ComplexPoly::getListCoeffsIn(int maxExponent) const
{
    return ToDenseRealBounded(maxExponent);
}

std::vector<double> ComplexPoly::getcomplexCoeffsList(int maxExponent) const
{
    return ToDenseImagBounded(maxExponent);
}

double ComplexPoly::GetCoeff(int index) const
{
    if (index < 0)
    {
        return 0.0;
    }
    const auto it = terms.find(index);
    if (it == terms.end())
    {
        return 0.0;
    }
    return NearlyZero(it->second.real()) ? 0.0 : it->second.real();
}

double ComplexPoly::getComplexCoeff(int index) const
{
    if (index < 0)
    {
        return 0.0;
    }
    const auto it = terms.find(index);
    if (it == terms.end())
    {
        return 0.0;
    }
    return NearlyZero(it->second.imag()) ? 0.0 : it->second.imag();
}

ComplexPoly ComplexPoly::Scale(const std::complex<double> &factor) const
{
    if (isUndefined())
    {
        return Undefined();
    }
    if (IsZero() || factor == std::complex<double>(0.0, 0.0))
    {
        return ComplexPoly();
    }

    ComplexPoly out;
    for (const auto &[exp, coeff] : terms)
    {
        const std::complex<double> scaled = coeff * factor;
        if (!NearlyZero(scaled))
        {
            out.terms.emplace(exp, scaled);
        }
    }
    out.Normalize();
    return out;
}

ComplexPoly ComplexPoly::operator+(const ComplexPoly &rhs) const
{
    if (isUndefined() || rhs.isUndefined())
    {
        return Undefined();
    }
    if (IsZero())
    {
        return rhs;
    }
    if (rhs.IsZero())
    {
        return *this;
    }

    ComplexPoly out = *this;
    for (const auto &[exp, coeff] : rhs.terms)
    {
        const auto it = out.terms.find(exp);
        const std::complex<double> cur = (it == out.terms.end()) ? std::complex<double>(0.0, 0.0) : it->second;
        const std::complex<double> sum = cur + coeff;
        if (NearlyZero(sum))
        {
            out.terms.erase(exp);
        }
        else
        {
            out.terms[exp] = sum;
        }
    }
    out.Normalize();
    return out;
}

ComplexPoly ComplexPoly::operator-(const ComplexPoly &rhs) const
{
    if (isUndefined() || rhs.isUndefined())
    {
        return Undefined();
    }
    if (rhs.IsZero())
    {
        return *this;
    }

    ComplexPoly out = *this;
    for (const auto &[exp, coeff] : rhs.terms)
    {
        const auto it = out.terms.find(exp);
        const std::complex<double> cur = (it == out.terms.end()) ? std::complex<double>(0.0, 0.0) : it->second;
        const std::complex<double> diff = cur - coeff;
        if (NearlyZero(diff))
        {
            out.terms.erase(exp);
        }
        else
        {
            out.terms[exp] = diff;
        }
    }
    out.Normalize();
    return out;
}

ComplexPoly ComplexPoly::operator*(const ComplexPoly &rhs) const
{
    if (isUndefined() || rhs.isUndefined())
    {
        return Undefined();
    }
    if (IsZero() || rhs.IsZero())
    {
        return ComplexPoly();
    }

    ComplexPoly out;
    for (const auto &[expA, coeffA] : terms)
    {
        for (const auto &[expB, coeffB] : rhs.terms)
        {
            const int exp = expA + expB;
            const std::complex<double> add = coeffA * coeffB;
            const auto it = out.terms.find(exp);
            const std::complex<double> cur = (it == out.terms.end()) ? std::complex<double>(0.0, 0.0) : it->second;
            const std::complex<double> next = cur + add;
            if (NearlyZero(next))
            {
                out.terms.erase(exp);
            }
            else
            {
                out.terms[exp] = next;
            }
        }
    }
    out.Normalize();
    return out;
}

std::pair<ComplexPoly, ComplexPoly> ComplexPoly::DivMod(const ComplexPoly &a, const ComplexPoly &b)
{
    if (a.isUndefined() || b.isUndefined())
    {
        return {Undefined(), Undefined()};
    }

    if (b.IsZero())
    {
        throw std::invalid_argument("Division by zero complex polynomial");
    }

    ComplexPoly quotient;
    ComplexPoly remainder = a;

    if (remainder.IsZero())
    {
        return {quotient, remainder};
    }

    const auto [degB, leadB] = b.LeadingTerm();

    while (!remainder.IsZero())
    {
        const auto [degR, leadR] = remainder.LeadingTerm();
        if (degR < degB)
        {
            break;
        }

        const int exp = degR - degB;
        const std::complex<double> factor = leadR / leadB;

        const auto itQ = quotient.terms.find(exp);
        const std::complex<double> curQ = (itQ == quotient.terms.end()) ? std::complex<double>(0.0, 0.0) : itQ->second;
        const std::complex<double> nextQ = curQ + factor;
        if (NearlyZero(nextQ))
        {
            quotient.terms.erase(exp);
        }
        else
        {
            quotient.terms[exp] = nextQ;
        }

        for (const auto &[e, c] : b.terms)
        {
            const int targetExp = e + exp;
            const auto itR = remainder.terms.find(targetExp);
            const std::complex<double> curR = (itR == remainder.terms.end()) ? std::complex<double>(0.0, 0.0) : itR->second;
            const std::complex<double> nextR = curR - factor * c;
            if (NearlyZero(nextR))
            {
                remainder.terms.erase(targetExp);
            }
            else
            {
                remainder.terms[targetExp] = nextR;
            }
        }
    }

    quotient.Normalize();
    remainder.Normalize();
    return {quotient, remainder};
}

ComplexPoly ComplexPoly::operator/(const ComplexPoly &rhs) const
{
    return DivMod(*this, rhs).first;
}

ComplexPoly ComplexPoly::operator+(const Polynomial &rhs) const
{
    return *this + ComplexPoly(rhs);
}

ComplexPoly ComplexPoly::operator-(const Polynomial &rhs) const
{
    return *this - ComplexPoly(rhs);
}

ComplexPoly ComplexPoly::operator*(const Polynomial &rhs) const
{
    return *this * ComplexPoly(rhs);
}

ComplexPoly ComplexPoly::operator/(const Polynomial &rhs) const
{
    return *this / ComplexPoly(rhs);
}

ComplexPoly ComplexPoly::Undefined()
{
    ComplexPoly out;
    out.undefined = true;
    return out;
}

bool ComplexPoly::NearlyEquals(const ComplexPoly &rhs, double eps) const
{
    if (isUndefined() || rhs.isUndefined())
    {
        return false;
    }

    auto itA = terms.begin();
    auto itB = rhs.terms.begin();

    while (itA != terms.end() || itB != rhs.terms.end())
    {
        std::complex<double> a(0.0, 0.0);
        std::complex<double> b(0.0, 0.0);

        if (itB == rhs.terms.end() || (itA != terms.end() && itA->first < itB->first))
        {
            a = itA->second;
            ++itA;
        }
        else if (itA == terms.end() || (itB != rhs.terms.end() && itB->first < itA->first))
        {
            b = itB->second;
            ++itB;
        }
        else
        {
            a = itA->second;
            b = itB->second;
            ++itA;
            ++itB;
        }

        if (std::abs(a.real() - b.real()) > eps || std::abs(a.imag() - b.imag()) > eps)
        {
            return false;
        }
    }

    return true;
}

void ComplexPoly::Dump() const
{
    if (isUndefined())
    {
        std::cout << "undefined" << std::endl;
        return;
    }
    if (IsZero())
    {
        std::cout << "0" << std::endl;
        return;
    }

    bool first = true;
    for (auto it = terms.rbegin(); it != terms.rend(); ++it)
    {
        const int exp = it->first;
        const std::complex<double> coeff = it->second;
        if (NearlyZero(coeff))
        {
            continue;
        }

        const double re = coeff.real();
        const double im = coeff.imag();

        const bool negative = !NearlyZero(re) ? (re < 0.0) : (im < 0.0);
        const std::complex<double> magCoeff = negative ? -coeff : coeff;

        if (first)
        {
            if (negative)
            {
                std::cout << "-";
            }
        }
        else
        {
            std::cout << (negative ? " - " : " + ");
        }

        const double magRe = magCoeff.real();
        const double magIm = magCoeff.imag();
        const bool hasRe = !NearlyZero(magRe);
        const bool hasIm = !NearlyZero(magIm);

        const bool omitCoeff = (exp != 0) && hasRe && !hasIm && NearlyZero(magRe - 1.0);
        if (!omitCoeff)
        {
            if (hasRe && !hasIm)
            {
                std::cout << magRe;
            }
            else if (!hasRe && hasIm)
            {
                const double absIm = std::abs(magIm);
                if (NearlyZero(absIm - 1.0))
                {
                    std::cout << "i";
                }
                else
                {
                    std::cout << absIm << "i";
                }
            }
            else
            {
                std::cout << "(" << magRe;
                if (magIm >= 0.0)
                {
                    std::cout << "+";
                }
                else
                {
                    std::cout << "-";
                }

                const double absIm = std::abs(magIm);
                if (NearlyZero(absIm - 1.0))
                {
                    std::cout << "i";
                }
                else
                {
                    std::cout << absIm << "i";
                }

                std::cout << ")";
            }
        }

        if (exp != 0)
        {
            std::cout << "x";
            if (exp != 1)
            {
                std::cout << "^" << exp;
            }
        }

        first = false;
    }

    std::cout << std::endl;
}
