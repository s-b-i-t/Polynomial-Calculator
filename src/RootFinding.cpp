#include "RootFinding.h"

#include <algorithm>
#include <cmath>
#include <sstream>
#include <string>
#include <vector>

namespace {
constexpr double EPSILON = 1e-10;

bool is_zero_polynomial(const Polynomial &p) {
    const std::vector<double> &coefficients = p.getListCoeffsIn();
    return std::all_of(coefficients.begin(), coefficients.end(), [](double value) {
        return std::abs(value) < EPSILON;
    });
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
}

std::string find_real_roots(const Polynomial &p) {
    if (is_zero_polynomial(p)) {
        return "Infinite roots";
    }

    const int degree = p.GetDegree();
    if (degree == 0) {
        return "No roots";
    }

    if (degree == 1) {
        const double b = p.GetCoeff(1);
        if (std::abs(b) < EPSILON) {
            return "No roots";
        }
        return format_number(-p.GetCoeff(0) / b);
    }

    if (degree == 2) {
        const double a = p.GetCoeff(2);
        const double b = p.GetCoeff(1);
        const double c = p.GetCoeff(0);
        const double discriminant = b * b - 4.0 * a * c;

        if (discriminant < -EPSILON) {
            return "No real roots";
        }

        if (std::abs(discriminant) <= EPSILON) {
            return format_number(-b / (2.0 * a));
        }

        const double sqrt_discriminant = std::sqrt(discriminant);
        double first = (-b - sqrt_discriminant) / (2.0 * a);
        double second = (-b + sqrt_discriminant) / (2.0 * a);
        if (first > second) {
            std::swap(first, second);
        }
        return format_number(first) + ", " + format_number(second);
    }

    return "Root finding supports only degree 1 and 2 polynomials.";
}
