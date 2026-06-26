#include "Polynomial.h"
#include "RationalComplex.h"
#include "RootFinding.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

constexpr double EPSILON = 1e-10;

std::string trim(const std::string &value) {
    const std::string whitespace = " \t\r\n";
    const std::size_t start = value.find_first_not_of(whitespace);
    if (start == std::string::npos) {
        return "";
    }

    const std::size_t end = value.find_last_not_of(whitespace);
    return value.substr(start, end - start + 1);
}

std::vector<double> parse_coefficients(const char *input) {
    if (input == nullptr) {
        throw std::invalid_argument("Input is missing.");
    }

    std::vector<double> coefficients;
    std::stringstream stream(input);
    std::string token;

    while (std::getline(stream, token, ',')) {
        token = trim(token);
        if (token.empty()) {
            continue;
        }

        std::size_t parsed = 0;
        double value = 0.0;
        try {
            value = std::stod(token, &parsed);
        } catch (const std::exception &) {
            throw std::invalid_argument("Invalid coefficient: " + token);
        }

        if (parsed != token.size()) {
            throw std::invalid_argument("Invalid coefficient: " + token);
        }

        coefficients.push_back(value);
    }

    if (coefficients.empty()) {
        throw std::invalid_argument("Enter at least one coefficient.");
    }

    while (coefficients.size() > 1 && std::abs(coefficients.back()) < EPSILON) {
        coefficients.pop_back();
    }

    if (coefficients.empty()) {
        coefficients.push_back(0.0);
    }

    return coefficients;
}

bool is_zero(const std::vector<double> &coefficients) {
    return std::all_of(coefficients.begin(), coefficients.end(), [](double value) {
        return std::abs(value) < EPSILON;
    });
}

Polynomial parse_polynomial(const char *input) {
    return Polynomial(parse_coefficients(input));
}

RationalComplex parse_rational(const char *input) {
    return make_rational(parse_polynomial(input));
}

std::string format_number(double value) {
    if (std::abs(value) < EPSILON) {
        return "0";
    }

    std::ostringstream out;
    out.precision(10);
    out << std::fixed << value;

    std::string formatted = out.str();
    while (formatted.size() > 1 && formatted.back() == '0') {
        formatted.pop_back();
    }
    if (!formatted.empty() && formatted.back() == '.') {
        formatted.pop_back();
    }

    return formatted == "-0" ? "0" : formatted;
}

std::string format_polynomial_coefficients(const std::vector<double> &coefficients) {
    std::vector<double> clean = coefficients;
    while (clean.size() > 1 && std::abs(clean.back()) < EPSILON) {
        clean.pop_back();
    }

    if (is_zero(clean)) {
        return "0";
    }

    std::vector<std::string> terms;
    for (int degree = static_cast<int>(clean.size()) - 1; degree >= 0; --degree) {
        const double coefficient = clean[degree];
        if (std::abs(coefficient) < EPSILON) {
            continue;
        }

        const double absolute = std::abs(coefficient);
        const bool is_first = terms.empty();
        const std::string sign = coefficient < 0 ? "-" : "+";

        std::string term;
        if (degree == 0) {
            term = format_number(absolute);
        } else {
            if (std::abs(absolute - 1.0) >= EPSILON) {
                term += format_number(absolute);
            }
            term += degree == 1 ? "x" : "x^" + std::to_string(degree);
        }

        terms.push_back(is_first ? (coefficient < 0 ? "-" + term : term) : sign + " " + term);
    }

    std::ostringstream out;
    for (std::size_t i = 0; i < terms.size(); ++i) {
        if (i > 0) {
            out << " ";
        }
        out << terms[i];
    }
    return out.str();
}

std::string format_complex_poly(const ComplexPoly &poly) {
    const std::vector<double> &real = poly.getListCoeffsIn();
    const std::vector<double> &imag = poly.getcomplexCoeffsList();

    const bool has_imag = std::any_of(imag.begin(), imag.end(), [](double value) {
        return std::abs(value) >= EPSILON;
    });

    if (!has_imag) {
        return format_polynomial_coefficients(real.empty() ? std::vector<double>{0.0} : real);
    }

    return "Error: complex polynomial output is not supported by the web formatter.";
}

bool is_one_denominator(const ComplexPoly &poly) {
    const std::vector<double> &real = poly.getListCoeffsIn();
    const std::vector<double> &imag = poly.getcomplexCoeffsList();

    const bool imag_zero = std::all_of(imag.begin(), imag.end(), [](double value) {
        return std::abs(value) < EPSILON;
    });

    if (!imag_zero || real.empty() || std::abs(real[0] - 1.0) >= EPSILON) {
        return false;
    }

    for (std::size_t i = 1; i < real.size(); ++i) {
        if (std::abs(real[i]) >= EPSILON) {
            return false;
        }
    }

    return true;
}

std::string format_rational(const RationalComplex &value) {
    const std::string numerator = format_complex_poly(value.GetNumerator());
    if (numerator.rfind("Error:", 0) == 0) {
        return numerator;
    }

    if (is_one_denominator(value.GetDenominator())) {
        return numerator;
    }

    const std::string denominator = format_complex_poly(value.GetDenominator());
    if (denominator.rfind("Error:", 0) == 0) {
        return denominator;
    }

    return "(" + numerator + ") / (" + denominator + ")";
}

std::string run_binary(const char *p, const char *q, char operation) {
    RationalComplex left = parse_rational(p);
    RationalComplex right = parse_rational(q);

    if (operation == '+') {
        return format_rational(left + right);
    }
    if (operation == '-') {
        return format_rational(left - right);
    }
    if (operation == '*') {
        return format_rational(left * right);
    }
    if (operation == '/') {
        return format_rational(left / right);
    }

    return "Error: unknown operation.";
}

char *copy_result(const std::string &result) {
    char *buffer = static_cast<char *>(std::malloc(result.size() + 1));
    if (buffer == nullptr) {
        return nullptr;
    }

    std::memcpy(buffer, result.c_str(), result.size() + 1);
    return buffer;
}

char *catch_errors(const std::function<std::string()> &operation) {
    try {
        return copy_result(operation());
    } catch (const std::exception &error) {
        return copy_result("Error: " + std::string(error.what()));
    } catch (...) {
        return copy_result("Error: unknown C++ exception.");
    }
}

} // namespace

extern "C" {

char *poly_add(const char *p, const char *q) {
    return catch_errors([&]() { return run_binary(p, q, '+'); });
}

char *poly_subtract(const char *p, const char *q) {
    return catch_errors([&]() { return run_binary(p, q, '-'); });
}

char *poly_multiply(const char *p, const char *q) {
    return catch_errors([&]() { return run_binary(p, q, '*'); });
}

char *poly_divide(const char *p, const char *q) {
    return catch_errors([&]() { return run_binary(p, q, '/'); });
}

char *poly_roots(const char *p) {
    return catch_errors([&]() {
        Polynomial poly = parse_polynomial(p);
        return find_real_roots(poly);
    });
}

void poly_free(char *ptr) {
    std::free(ptr);
}

}
