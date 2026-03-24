/*
 CHANGELOG (Codex robustness pass)
 - Make `Rational<T>` header-only to avoid template/linker footguns.
 - Add defensive checks: forbid zero denominators and normalize after every op.
 - Canonicalize Polynomial denominators to monic; ComplexPoly denominators to real-positive monic
   (stabilize phase so the leading coefficient becomes real > 0, then monic).
 */

//  Rational.h
// Works for Polynomial, ComplexPoly, int, double.

#ifndef Rational_h
#define Rational_h
#include "Polynomial.h"
#include "Complex.h"
#include <stdexcept>
#include <type_traits>
#include <numeric>
#include <complex>


template <class T>
class Rational
{
public:
    Rational() : numerator(T{0}), denominator(T{1})
    {
        Normalize();
    }

    Rational(const T &numeratorIn, const T &denominatorIn = T{1}) : numerator(numeratorIn), denominator(denominatorIn)
    {
        Normalize();
    }

    
    // copy constructor
    Rational(const Rational<T> &object) : numerator(object.numerator), denominator(object.denominator) {}
 
    // assignment operator
    Rational<T>& operator=(const Rational &rhs); 
    
    Rational<T> operator+(const Rational &rhs) const; 
    
    Rational<T> operator-(const Rational &rhs) const; 

    Rational<T> operator*(const Rational &rhs) const; 

    Rational<T> operator/(const Rational &rhs) const;     

    



    // access numerator and denominator
    const T &GetNumerator() const {return numerator;}
    const T &GetDenominator() const {return denominator;}
    
private:
    void Normalize();

    T numerator;
    T denominator;
};

// =========================
// Template implementation
// =========================

template <class T>
void Rational<T>::Normalize()
{
    if constexpr (std::is_integral_v<T>)
    {
        if (denominator == 0)
        {
            throw std::invalid_argument("Rational: denominator must be non-zero");
        }

        if (numerator == 0)
        {
            denominator = 1;
            return;
        }

        const T g = std::gcd(numerator, denominator);
        numerator /= g;
        denominator /= g;

        if (denominator < 0)
        {
            numerator = -numerator;
            denominator = -denominator;
        }
    }
    else if constexpr (std::is_floating_point_v<T>)
    {
        if (denominator == static_cast<T>(0))
        {
            throw std::invalid_argument("Rational: denominator must be non-zero");
        }
        if (numerator == static_cast<T>(0))
        {
            denominator = static_cast<T>(1);
            return;
        }
        if (denominator < static_cast<T>(0))
        {
            numerator = -numerator;
            denominator = -denominator;
        }
    }
    else if constexpr (std::is_same_v<T, Polynomial>)
    {
        if (denominator.IsZero())
        {
            throw std::invalid_argument("Rational<Polynomial>: denominator must be non-zero polynomial");
        }

        if (numerator.IsZero())
        {
            denominator = Polynomial(1);
            return;
        }

        const auto [deg, lead] = denominator.LeadingTerm();
        (void)deg;

        if (std::abs(lead) < Polynomial::EPS)
        {
            throw std::invalid_argument("Rational<Polynomial>: invalid leading coefficient");
        }

        // Monic canonical denominator.
        if (std::abs(lead - 1.0) >= Polynomial::EPS)
        {
            const double factor = 1.0 / lead;
            numerator = numerator.Scale(factor);
            denominator = denominator.Scale(factor);
        }

        if (denominator.IsZero())
        {
            throw std::invalid_argument("Rational<Polynomial>: denominator became zero during normalization");
        }
    }
    else if constexpr (std::is_same_v<T, ComplexPoly>)
    {
        if (denominator.IsZero())
        {
            throw std::invalid_argument("Rational<ComplexPoly>: denominator must be non-zero polynomial");
        }

        if (numerator.IsZero())
        {
            denominator = ComplexPoly({1});
            return;
        }

        const auto [deg, lead] = denominator.LeadingTerm();
        (void)deg;

        const double mag = std::abs(lead);
        if (mag < ComplexPoly::EPS)
        {
            throw std::invalid_argument("Rational<ComplexPoly>: invalid leading coefficient");
        }

        // Stabilize phase so leading coefficient becomes real-positive, then make monic.
        // Multiply by conj(lead)/|lead| to make lead -> |lead| (real >= 0).
        const std::complex<double> phase = std::conj(lead) / mag;
        numerator = numerator.Scale(phase);
        denominator = denominator.Scale(phase);

        const auto [deg2, lead2] = denominator.LeadingTerm();
        (void)deg2;
        const double leadReal = lead2.real();
        if (std::abs(lead2.imag()) > ComplexPoly::EPS)
        {
            // Best effort; should be near-real after phase stabilization.
            throw std::invalid_argument("Rational<ComplexPoly>: phase stabilization failed");
        }
        if (leadReal < ComplexPoly::EPS)
        {
            throw std::invalid_argument("Rational<ComplexPoly>: invalid stabilized leading coefficient");
        }

        // Make monic (leading coeff = 1).
        numerator = numerator.Scale(1.0 / leadReal);
        denominator = denominator.Scale(1.0 / leadReal);

        if (denominator.IsZero())
        {
            throw std::invalid_argument("Rational<ComplexPoly>: denominator became zero during normalization");
        }
    }
}

template <class T>
Rational<T> &Rational<T>::operator=(const Rational &rhs)
{
    numerator = rhs.numerator;
    denominator = rhs.denominator;
    Normalize();
    return *this;
}

template <class T>
Rational<T> Rational<T>::operator+(const Rational &rhs) const
{
    const T newNumerator = (numerator * rhs.denominator) + (rhs.numerator * denominator);
    const T newDenominator = denominator * rhs.denominator;
    return Rational<T>(newNumerator, newDenominator);
}

template <class T>
Rational<T> Rational<T>::operator-(const Rational &rhs) const
{
    const T newNumerator = (numerator * rhs.denominator) - (rhs.numerator * denominator);
    const T newDenominator = denominator * rhs.denominator;
    return Rational<T>(newNumerator, newDenominator);
}

template <class T>
Rational<T> Rational<T>::operator*(const Rational &rhs) const
{
    const T newNumerator = numerator * rhs.numerator;
    const T newDenominator = denominator * rhs.denominator;
    return Rational<T>(newNumerator, newDenominator);
}

template <class T>
Rational<T> Rational<T>::operator/(const Rational &rhs) const
{
    const T newNumerator = numerator * rhs.denominator;
    const T newDenominator = denominator * rhs.numerator;
    return Rational<T>(newNumerator, newDenominator);
}


#endif /* Rational_h */
