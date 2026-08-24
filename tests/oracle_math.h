#ifndef POLYCALC_ORACLE_MATH_H
#define POLYCALC_ORACLE_MATH_H

#include <algorithm>
#include <cmath>
#include <complex>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace oracle {

using Coefficient = std::complex<double>;
using Polynomial = std::vector<Coefficient>;
using RealPolynomial = std::vector<double>;

constexpr double kTrimTolerance = 1e-12;

inline bool finite(const Coefficient &value) {
    return std::isfinite(value.real()) && std::isfinite(value.imag());
}

inline bool near_zero(const Coefficient &value, double tolerance = kTrimTolerance) {
    return finite(value) && std::abs(value) <= tolerance;
}

inline Polynomial normalize(Polynomial value, double tolerance = kTrimTolerance) {
    while (value.size() > 1 && near_zero(value.back(), tolerance)) {
        value.pop_back();
    }
    if (value.empty()) {
        value.push_back({0.0, 0.0});
    }
    for (Coefficient &coefficient : value) {
        if (near_zero(coefficient, tolerance)) {
            coefficient = {0.0, 0.0};
        }
    }
    return value;
}

inline RealPolynomial normalize_real(RealPolynomial value,
                                    double tolerance = kTrimTolerance) {
    while (value.size() > 1 && std::isfinite(value.back()) &&
           std::abs(value.back()) <= tolerance) {
        value.pop_back();
    }
    if (value.empty()) {
        value.push_back(0.0);
    }
    for (double &coefficient : value) {
        if (std::isfinite(coefficient) && std::abs(coefficient) <= tolerance) {
            coefficient = 0.0;
        }
    }
    return value;
}

inline Polynomial from_real(const RealPolynomial &real) {
    Polynomial result;
    result.reserve(real.size());
    for (double value : real) {
        result.push_back({value, 0.0});
    }
    return normalize(std::move(result));
}

inline bool is_zero(const Polynomial &value, double tolerance = kTrimTolerance) {
    for (const Coefficient &coefficient : value) {
        if (!near_zero(coefficient, tolerance)) {
            return false;
        }
    }
    return true;
}

inline Polynomial add(const Polynomial &left, const Polynomial &right) {
    Polynomial result(std::max(left.size(), right.size()), {0.0, 0.0});
    for (std::size_t index = 0; index < result.size(); ++index) {
        if (index < left.size()) {
            result[index] += left[index];
        }
        if (index < right.size()) {
            result[index] += right[index];
        }
    }
    return normalize(std::move(result));
}

inline Polynomial subtract(const Polynomial &left, const Polynomial &right) {
    Polynomial result(std::max(left.size(), right.size()), {0.0, 0.0});
    for (std::size_t index = 0; index < result.size(); ++index) {
        if (index < left.size()) {
            result[index] += left[index];
        }
        if (index < right.size()) {
            result[index] -= right[index];
        }
    }
    return normalize(std::move(result));
}

inline Polynomial scale(const Polynomial &value, const Coefficient &factor) {
    Polynomial result = value;
    for (Coefficient &coefficient : result) {
        coefficient *= factor;
    }
    return normalize(std::move(result));
}

inline Polynomial multiply(const Polynomial &left, const Polynomial &right) {
    if (is_zero(left) || is_zero(right)) {
        return {{0.0, 0.0}};
    }
    Polynomial result(left.size() + right.size() - 1, {0.0, 0.0});
    for (std::size_t left_index = 0; left_index < left.size(); ++left_index) {
        for (std::size_t right_index = 0; right_index < right.size(); ++right_index) {
            result[left_index + right_index] +=
                left[left_index] * right[right_index];
        }
    }
    return normalize(std::move(result));
}

inline std::pair<Polynomial, Polynomial> divide_with_remainder(
    const Polynomial &dividend_input,
    const Polynomial &divisor_input) {
    Polynomial dividend = normalize(dividend_input);
    const Polynomial divisor = normalize(divisor_input);
    if (is_zero(divisor)) {
        throw std::invalid_argument("oracle division by zero polynomial");
    }
    if (dividend.size() < divisor.size()) {
        return {{{0.0, 0.0}}, dividend};
    }

    Polynomial quotient(dividend.size() - divisor.size() + 1, {0.0, 0.0});
    while (!is_zero(dividend) && dividend.size() >= divisor.size()) {
        const std::size_t offset = dividend.size() - divisor.size();
        const Coefficient factor = dividend.back() / divisor.back();
        quotient[offset] += factor;
        for (std::size_t index = 0; index < divisor.size(); ++index) {
            dividend[offset + index] -= factor * divisor[index];
        }
        dividend = normalize(std::move(dividend));
    }
    return {normalize(std::move(quotient)), normalize(std::move(dividend))};
}

inline std::pair<RealPolynomial, RealPolynomial> real_divide_with_remainder(
    const RealPolynomial &dividend,
    const RealPolynomial &divisor) {
    const auto complex_result = divide_with_remainder(from_real(dividend),
                                                       from_real(divisor));
    RealPolynomial quotient;
    RealPolynomial remainder;
    for (const Coefficient &value : complex_result.first) {
        quotient.push_back(value.real());
    }
    for (const Coefficient &value : complex_result.second) {
        remainder.push_back(value.real());
    }
    return {normalize_real(std::move(quotient)),
            normalize_real(std::move(remainder))};
}

inline Coefficient evaluate(const Polynomial &polynomial, double x) {
    Coefficient result{0.0, 0.0};
    for (auto iterator = polynomial.rbegin(); iterator != polynomial.rend(); ++iterator) {
        result = result * x + *iterator;
    }
    return result;
}

struct Rational {
    Polynomial numerator;
    Polynomial denominator;
};

inline Rational normalize_rational(Rational value) {
    value.numerator = normalize(std::move(value.numerator));
    value.denominator = normalize(std::move(value.denominator));
    if (is_zero(value.denominator)) {
        throw std::invalid_argument("oracle rational zero denominator");
    }
    if (is_zero(value.numerator)) {
        return {{{0.0, 0.0}}, {{1.0, 0.0}}};
    }
    return value;
}

inline Rational rational_add(const Rational &left, const Rational &right) {
    return normalize_rational({
        add(multiply(left.numerator, right.denominator),
            multiply(right.numerator, left.denominator)),
        multiply(left.denominator, right.denominator)});
}

inline Rational rational_subtract(const Rational &left, const Rational &right) {
    return normalize_rational({
        subtract(multiply(left.numerator, right.denominator),
                 multiply(right.numerator, left.denominator)),
        multiply(left.denominator, right.denominator)});
}

inline Rational rational_multiply(const Rational &left, const Rational &right) {
    return normalize_rational({multiply(left.numerator, right.numerator),
                               multiply(left.denominator, right.denominator)});
}

inline Rational rational_divide(const Rational &left, const Rational &right) {
    if (is_zero(right.numerator)) {
        throw std::invalid_argument("oracle division by zero rational");
    }
    return normalize_rational({multiply(left.numerator, right.denominator),
                               multiply(left.denominator, right.numerator)});
}

inline Coefficient evaluate(const Rational &rational, double x) {
    return evaluate(rational.numerator, x) / evaluate(rational.denominator, x);
}

inline bool near(const Coefficient &actual,
                 const Coefficient &expected,
                 double tolerance) {
    return finite(actual) && finite(expected) && std::isfinite(tolerance) &&
           std::abs(actual - expected) <= tolerance;
}

inline std::string format(const Coefficient &value) {
    std::ostringstream out;
    out << std::setprecision(17) << '(' << value.real();
    if (value.imag() >= 0.0) {
        out << '+';
    }
    out << value.imag() << "i)";
    return out.str();
}

inline std::string format(const Polynomial &polynomial) {
    std::ostringstream out;
    out << '[';
    for (std::size_t index = 0; index < polynomial.size(); ++index) {
        if (index != 0) {
            out << ", ";
        }
        out << format(polynomial[index]);
    }
    out << ']';
    return out.str();
}

inline std::string format(const Rational &rational) {
    return "num=" + format(rational.numerator) + " den=" + format(rational.denominator);
}

}  // namespace oracle

#endif
