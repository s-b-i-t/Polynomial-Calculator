#include "Complex.h"
#include "Polynomial.h"
#include "RationalComplex.h"

#include "oracle_math.h"
#include "test_harness.h"

#include <algorithm>
#include <cmath>
#include <complex>
#include <cstdint>
#include <exception>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

namespace {

constexpr double kTolerance = coretest::kDefaultTolerance;
constexpr std::uint32_t kSeed = 0x5EED1234U;
constexpr int kGeneratedCases = 100;

oracle::Polynomial opoly(std::initializer_list<oracle::Coefficient> coefficients) {
    return oracle::normalize(oracle::Polynomial(coefficients));
}

oracle::Rational orational(oracle::Polynomial numerator,
                           oracle::Polynomial denominator) {
    return {std::move(numerator), std::move(denominator)};
}

oracle::Polynomial oracle_from(const Polynomial &polynomial) {
    return oracle::from_real(polynomial.getListCoeffsIn());
}

oracle::Polynomial oracle_from(const ComplexPoly &polynomial) {
    const auto &real = polynomial.getListCoeffsIn();
    const auto &imaginary = polynomial.getcomplexCoeffsList();
    oracle::Polynomial result(std::max(real.size(), imaginary.size()), {0.0, 0.0});
    if (result.empty()) {
        result.push_back({0.0, 0.0});
    }
    for (std::size_t index = 0; index < result.size(); ++index) {
        result[index] = {
            index < real.size() ? real[index] : 0.0,
            index < imaginary.size() ? imaginary[index] : 0.0};
    }
    return oracle::normalize(std::move(result));
}

oracle::Rational oracle_from(const RationalComplex &rational) {
    return {oracle_from(rational.GetNumerator()),
            oracle_from(rational.GetDenominator())};
}

std::vector<double> real_part(const oracle::Polynomial &polynomial) {
    std::vector<double> result;
    result.reserve(polynomial.size());
    for (const auto &coefficient : polynomial) {
        result.push_back(coefficient.real());
    }
    return result;
}

std::vector<double> imaginary_part(const oracle::Polynomial &polynomial) {
    std::vector<double> result;
    result.reserve(polynomial.size());
    for (const auto &coefficient : polynomial) {
        result.push_back(coefficient.imag());
    }
    while (!result.empty() && std::abs(result.back()) <= oracle::kTrimTolerance) {
        result.pop_back();
    }
    return result;
}

Polynomial production_real(const oracle::Polynomial &polynomial) {
    return Polynomial(real_part(oracle::normalize(polynomial)));
}

ComplexPoly production_complex(const oracle::Polynomial &polynomial) {
    const auto normalized = oracle::normalize(polynomial);
    return ComplexPoly(real_part(normalized), imaginary_part(normalized));
}

RationalComplex production_rational(const oracle::Rational &rational) {
    return RationalComplex(production_complex(rational.numerator),
                           production_complex(rational.denominator));
}

void check_oracle_polynomial(const oracle::Polynomial &actual_input,
                             const oracle::Polynomial &expected_input,
                             const std::string &context,
                             const char *file,
                             int line) {
    const auto actual = oracle::normalize(actual_input);
    const auto expected = oracle::normalize(expected_input);
    bool passed = actual.size() == expected.size();
    if (passed) {
        for (std::size_t index = 0; index < actual.size(); ++index) {
            if (!oracle::near(actual[index], expected[index], kTolerance)) {
                passed = false;
                break;
            }
        }
    }
    coretest::record(passed, context.c_str(), file, line,
                     oracle::format(expected) + " (all finite, tol=1e-9)",
                     oracle::format(actual));
}

void check_polynomial(const Polynomial &actual,
                      const oracle::Polynomial &expected,
                      const std::string &context,
                      const char *file,
                      int line) {
    check_oracle_polynomial(oracle_from(actual), expected, context, file, line);
}

void check_complex(const ComplexPoly &actual,
                   const oracle::Polynomial &expected,
                   const std::string &context,
                   const char *file,
                   int line) {
    check_oracle_polynomial(oracle_from(actual), expected, context, file, line);
}

void check_rational_structural(const RationalComplex &actual,
                               const oracle::Rational &expected,
                               const std::string &context,
                               const char *file,
                               int line) {
    const auto actual_oracle = oracle_from(actual);
    const auto normalized_expected = oracle::normalize_rational(expected);
    bool passed = true;
    const auto actual_num = oracle::normalize(actual_oracle.numerator);
    const auto expected_num = oracle::normalize(normalized_expected.numerator);
    const auto actual_den = oracle::normalize(actual_oracle.denominator);
    const auto expected_den = oracle::normalize(normalized_expected.denominator);
    if (actual_num.size() != expected_num.size() ||
        actual_den.size() != expected_den.size()) {
        passed = false;
    }
    if (passed) {
        for (std::size_t index = 0; index < actual_num.size(); ++index) {
            passed = passed && oracle::near(actual_num[index], expected_num[index],
                                            kTolerance);
        }
        for (std::size_t index = 0; index < actual_den.size(); ++index) {
            passed = passed && oracle::near(actual_den[index], expected_den[index],
                                            kTolerance);
        }
    }
    coretest::record(passed, context.c_str(), file, line,
                     oracle::format(normalized_expected) + " (structural, tol=1e-9)",
                     oracle::format(actual_oracle));
}

void check_complex_value(const oracle::Coefficient &actual,
                         const oracle::Coefficient &expected,
                         const std::string &context,
                         const char *file,
                         int line) {
    coretest::record(oracle::near(actual, expected, kTolerance), context.c_str(),
                     file, line,
                     oracle::format(expected) + " (finite, tol=1e-9)",
                     oracle::format(actual));
}

#define CHECK_POLYNOMIAL(actual, expected, context) \
    check_polynomial((actual), (expected), (context), __FILE__, __LINE__)
#define CHECK_COMPLEX(actual, expected, context) \
    check_complex((actual), (expected), (context), __FILE__, __LINE__)
#define CHECK_RATIONAL(actual, expected, context) \
    check_rational_structural((actual), (expected), (context), __FILE__, __LINE__)
#define CHECK_COMPLEX_VALUE(actual, expected, context) \
    check_complex_value((actual), (expected), (context), __FILE__, __LINE__)

std::string generated_context(const char *property,
                              int iteration,
                              const oracle::Polynomial &left,
                              const oracle::Polynomial &right = {}) {
    std::string value = std::string(property) + " seed=" + std::to_string(kSeed) +
                        " iteration=" + std::to_string(iteration) +
                        " left=" + oracle::format(left);
    if (!right.empty()) {
        value += " right=" + oracle::format(right);
    }
    return value;
}

oracle::Polynomial random_real_oracle(std::mt19937 &generator,
                                      bool require_nonzero = false) {
    std::uniform_int_distribution<int> degree_distribution(0, 5);
    std::uniform_int_distribution<int> coefficient_distribution(-3, 3);
    const int degree = degree_distribution(generator);
    oracle::Polynomial result(static_cast<std::size_t>(degree + 1), {0.0, 0.0});
    for (auto &coefficient : result) {
        coefficient = {static_cast<double>(coefficient_distribution(generator)), 0.0};
    }
    if (require_nonzero && oracle::is_zero(result)) {
        result[0] = {1.0, 0.0};
    }
    return oracle::normalize(std::move(result));
}

oracle::Polynomial random_safe_complex_oracle(std::mt19937 &generator,
                                              bool require_nonzero = false) {
    std::uniform_int_distribution<int> degree_distribution(0, 5);
    std::uniform_int_distribution<int> coefficient_distribution(-3, 3);
    const int degree = degree_distribution(generator);
    oracle::Polynomial result(static_cast<std::size_t>(degree + 1));
    for (auto &coefficient : result) {
        coefficient = {static_cast<double>(coefficient_distribution(generator)),
                       static_cast<double>(coefficient_distribution(generator))};
    }
    // Keep the real representation at least as long as the imaginary one. This
    // lets the broad property tests run; separate sanitizer regressions exercise
    // the implementation's high-imaginary-degree memory defect directly.
    if (result.back().real() == 0.0) {
        result.back().real(1.0);
    }
    if (require_nonzero && oracle::is_zero(result)) {
        result[0] = {1.0, 0.0};
    }
    return oracle::normalize(std::move(result));
}

void check_exception_message_contains(const std::function<void()> &function,
                                      const std::string &needle,
                                      const std::string &context,
                                      const char *file,
                                      int line) {
    bool correct_type = false;
    std::string actual = "no exception";
    try {
        function();
    } catch (const std::invalid_argument &error) {
        correct_type = true;
        actual = error.what();
    } catch (const std::exception &error) {
        actual = std::string(typeid(error).name()) + ": " + error.what();
    } catch (...) {
        actual = "non-std exception";
    }
    coretest::record(correct_type && actual.find(needle) != std::string::npos,
                     context.c_str(), file, line,
                     "std::invalid_argument message containing '" + needle + "'",
                     actual);
}

#define CHECK_INVALID_ARGUMENT_MESSAGE(function, needle, context) \
    check_exception_message_contains((function), (needle), (context), __FILE__, __LINE__)

void check_runtime_error_message_contains(const std::function<void()> &function,
                                          const std::string &needle,
                                          const std::string &context,
                                          const char *file,
                                          int line) {
    bool correct_type = false;
    std::string actual = "no exception";
    try {
        function();
    } catch (const std::runtime_error &error) {
        correct_type = true;
        actual = error.what();
    } catch (const std::exception &error) {
        actual = std::string(typeid(error).name()) + ": " + error.what();
    } catch (...) {
        actual = "non-std exception";
    }
    coretest::record(correct_type && actual.find(needle) != std::string::npos,
                     context.c_str(), file, line,
                     "std::runtime_error message containing '" + needle + "'",
                     actual);
}

#define CHECK_RUNTIME_ERROR_MESSAGE(function, needle, context) \
    check_runtime_error_message_contains((function), (needle), (context), __FILE__, __LINE__)

bool oracle_polynomials_near(const oracle::Polynomial &actual_input,
                             const oracle::Polynomial &expected_input) {
    const auto actual = oracle::normalize(actual_input);
    const auto expected = oracle::normalize(expected_input);
    if (actual.size() != expected.size()) {
        return false;
    }
    for (std::size_t index = 0; index < actual.size(); ++index) {
        if (!oracle::near(actual[index], expected[index], kTolerance)) {
            return false;
        }
    }
    return true;
}

void check_isolated_complex_product(const ComplexPoly &left,
                                    const ComplexPoly &right,
                                    const oracle::Polynomial &expected,
                                    const std::string &context,
                                    const char *file,
                                    int line) {
    std::cout.flush();
    std::cerr.flush();
    const pid_t child = fork();
    if (child < 0) {
        coretest::record(false, context.c_str(), file, line,
                         "isolated child exits 0", "fork failed");
        return;
    }
    if (child == 0) {
        try {
            const ComplexPoly result = left * right;
            const auto actual = oracle_from(result);
            if (!oracle_polynomials_near(actual, expected)) {
                std::cerr << "[ISOLATED CHILD MISMATCH] " << context << "\n"
                          << "  expected: " << oracle::format(expected) << "\n"
                          << "  actual:   " << oracle::format(actual) << '\n';
                _exit(3);
            }
            _exit(0);
        } catch (const std::exception &error) {
            std::cerr << "[ISOLATED CHILD EXCEPTION] " << context << ": "
                      << error.what() << '\n';
            _exit(4);
        } catch (...) {
            std::cerr << "[ISOLATED CHILD EXCEPTION] " << context
                      << ": non-std exception\n";
            _exit(5);
        }
    }

    int status = 0;
    if (waitpid(child, &status, 0) != child) {
        coretest::record(false, context.c_str(), file, line,
                         "isolated child exits 0", "waitpid failed");
        return;
    }
    std::string actual_status;
    bool passed = false;
    if (WIFEXITED(status)) {
        const int exit_code = WEXITSTATUS(status);
        passed = exit_code == 0;
        actual_status = "child exit " + std::to_string(exit_code);
    } else if (WIFSIGNALED(status)) {
        actual_status = "child signal " + std::to_string(WTERMSIG(status));
    } else {
        actual_status = "child ended abnormally";
    }
    coretest::record(passed, context.c_str(), file, line,
                     "isolated child exits 0 with exact finite product",
                     actual_status);
}

#define CHECK_ISOLATED_COMPLEX_PRODUCT(left, right, expected, context) \
    check_isolated_complex_product((left), (right), (expected), (context), __FILE__, __LINE__)

TEST_CASE(polynomial, representation_and_api) {
    const Polynomial default_value;
    CHECK_POLYNOMIAL(default_value, oracle::from_real({0.0}), "default is zero");
    CHECK_EQ(default_value.GetDegree(), 0);

    const Polynomial from_int(-3);
    const Polynomial from_double(2.5);
    CHECK_POLYNOMIAL(from_int, oracle::from_real({-3.0}), "int construction");
    CHECK_POLYNOMIAL(from_double, oracle::from_real({2.5}), "double construction");

    const Polynomial ordered(std::vector<double>{1.0, -2.0, 3.5});
    CHECK_NEAR(ordered.GetCoeff(0), 1.0, kTolerance);
    CHECK_NEAR(ordered.GetCoeff(1), -2.0, kTolerance);
    CHECK_NEAR(ordered.GetCoeff(2), 3.5, kTolerance);
    CHECK_EQ(ordered.GetDegree(), 2);
    CHECK_NEAR(oracle::evaluate(oracle_from(ordered), 2.0).real(), 11.0, kTolerance);

    const Polynomial empty(std::vector<double>{});
    const Polynomial all_zero(std::vector<double>{0.0, 0.0, 0.0});
    const Polynomial trailing(std::vector<double>{1.0, -2.0, 0.0, 0.0});
    CHECK_POLYNOMIAL(empty, oracle::from_real({0.0}), "empty vector becomes zero");
    CHECK_POLYNOMIAL(all_zero, oracle::from_real({0.0}), "all-zero vector normalized");
    CHECK_EQ(all_zero.getListCoeffsIn().size(), std::size_t{1});
    CHECK_POLYNOMIAL(trailing, oracle::from_real({1.0, -2.0}), "trailing zeros trimmed");
    CHECK_EQ(trailing.getListCoeffsIn().size(), std::size_t{2});

    const Polynomial sparse(std::vector<double>{-1.25, 0.0, 0.5, 0.0, -3.0});
    CHECK_POLYNOMIAL(sparse, oracle::from_real({-1.25, 0.0, 0.5, 0.0, -3.0}),
                     "negative fractional sparse coefficients");
    CHECK_NEAR(sparse.GetCoeff(-1), 0.0, kTolerance);
    CHECK_NEAR(sparse.GetCoeff(99), 0.0, kTolerance);

    Polynomial copied(sparse);
    Polynomial assigned;
    assigned = sparse;
    CHECK_POLYNOMIAL(copied, oracle_from(sparse), "copy construction preserves value");
    CHECK_POLYNOMIAL(assigned, oracle_from(sparse), "copy assignment preserves value");
    assigned = assigned;
    CHECK_POLYNOMIAL(assigned, oracle_from(sparse), "self assignment preserves value");

    Polynomial changed(std::vector<double>{1.0, 2.0});
    changed.SetCoeffAt(4, 7.0);
    CHECK_POLYNOMIAL(changed, oracle::from_real({1.0, 2.0, 0.0, 0.0, 7.0}),
                     "SetCoeffAt expands and sets high coefficient");
    CHECK_EQ(changed.GetDegree(), 4);
    changed.SetCoeffAt(4, 0.0);
    CHECK_POLYNOMIAL(changed, oracle::from_real({1.0, 2.0}),
                     "setting high coefficient back to zero preserves normalized value");
    CHECK_EQ(changed.GetDegree(), 1);
    changed.SetCoeffAt(-1, 99.0);
    CHECK_POLYNOMIAL(changed, oracle::from_real({1.0, 2.0}),
                     "negative SetCoeffAt index is ignored");

    const auto scaled = sparse.Scale(-0.5);
    CHECK_POLYNOMIAL(scaled, oracle::scale(oracle_from(sparse), {-0.5, 0.0}),
                     "Scale uses every coefficient");
    CHECK_POLYNOMIAL(sparse, oracle::from_real({-1.25, 0.0, 0.5, 0.0, -3.0}),
                     "Scale does not mutate operand");
}

TEST_CASE(polynomial, equality_and_inequality) {
    const Polynomial left(std::vector<double>{1.0, -2.0, 3.0});
    const Polynomial same(std::vector<double>{1.0, -2.0, 3.0});
    const Polynomial different(std::vector<double>{1.0, -2.0, 4.0});
    CHECK(left == same);
    CHECK(!(left != same));
    CHECK(!(left == different));
    CHECK(left != different);

    Polynomial expanded_zero(std::vector<double>{1.0, -2.0, 3.0});
    expanded_zero.SetCoeffAt(7, 0.0);
    CHECK_POLYNOMIAL(expanded_zero, oracle_from(left),
                     "expanded trailing zero is mathematically identical");
    CHECK(left == expanded_zero);
    CHECK(!(left != expanded_zero));
}

TEST_CASE(polynomial, arithmetic_and_identities) {
    const Polynomial p(std::vector<double>{-1.5, 2.0, 0.0, -3.0});
    const Polynomial q(std::vector<double>{4.0, -0.5, 2.0});
    const Polynomial r(std::vector<double>{0.0, -2.0, 1.0, 0.0, 0.5});
    const Polynomial zero;
    const Polynomial one(1);
    const auto op = oracle_from(p);
    const auto oq = oracle_from(q);
    const auto or_value = oracle_from(r);

    CHECK_POLYNOMIAL(p + q, oracle::add(op, oq), "different-degree addition");
    CHECK_POLYNOMIAL(p - q, oracle::subtract(op, oq), "different-degree subtraction");
    CHECK_POLYNOMIAL(p * q, oracle::multiply(op, oq), "fractional sparse convolution");
    CHECK_POLYNOMIAL(p + zero, op, "p + 0 = p");
    CHECK_POLYNOMIAL(p - zero, op, "p - 0 = p");
    CHECK_POLYNOMIAL(p - p, oracle::from_real({0.0}), "p - p = 0");
    CHECK_POLYNOMIAL(p * zero, oracle::from_real({0.0}), "p * 0 = 0");
    CHECK_POLYNOMIAL(p * one, op, "p * 1 = p");
    CHECK_POLYNOMIAL(p + q, oracle_from(q + p), "addition commutativity");
    CHECK_POLYNOMIAL(p * q, oracle_from(q * p), "multiplication commutativity");
    CHECK_POLYNOMIAL(p + (q + r), oracle::add(oracle::add(op, oq), or_value),
                     "addition associativity");
    CHECK_POLYNOMIAL((p + q) + r, oracle::add(op, oracle::add(oq, or_value)),
                     "addition associativity alternate grouping");
    CHECK_POLYNOMIAL(p * (q + r),
                     oracle::multiply(op, oracle::add(oq, or_value)),
                     "left distributivity");
    CHECK_POLYNOMIAL(p * q + p * r,
                     oracle::add(oracle::multiply(op, oq),
                                 oracle::multiply(op, or_value)),
                     "expanded distributive result");
    CHECK_POLYNOMIAL(p, op, "non-mutating arithmetic preserves p");
    CHECK_POLYNOMIAL(q, oq, "non-mutating arithmetic preserves q");

    Polynomial compound = p;
    compound += q;
    CHECK_POLYNOMIAL(compound, oracle::add(op, oq), "+= result");
    compound = p;
    compound -= q;
    CHECK_POLYNOMIAL(compound, oracle::subtract(op, oq), "-= result");
    compound = p;
    compound *= q;
    CHECK_POLYNOMIAL(compound, oracle::multiply(op, oq), "*= result");
    compound = p;
    compound += compound;
    CHECK_POLYNOMIAL(compound, oracle::scale(op, {2.0, 0.0}), "self +=");
    compound = p;
    compound -= compound;
    CHECK_POLYNOMIAL(compound, oracle::from_real({0.0}), "self -=");
    compound = p;
    compound *= compound;
    CHECK_POLYNOMIAL(compound, oracle::multiply(op, op), "self *=");
}

TEST_CASE(polynomial, division_and_remainder) {
    struct DivisionCase {
        std::vector<double> dividend;
        std::vector<double> divisor;
        const char *name;
    };
    const std::vector<DivisionCase> cases{
        {{-1.0, 0.0, 1.0}, {-1.0, 1.0}, "x^2-1 over x-1"},
        {{5.0, 6.0, 0.0, 3.0}, {0.0, 3.0}, "nonzero remainder"},
        {{1.0, -2.0, 3.0}, {2.0}, "constant divisor"},
        {{2.0, -1.0}, {1.0, 0.0, 1.0}, "lower-degree dividend"},
        {{0.0}, {1.0, 1.0}, "zero dividend"},
    };

    for (const auto &item : cases) {
        const auto expected = oracle::real_divide_with_remainder(item.dividend,
                                                                  item.divisor);
        const Polynomial dividend(item.dividend);
        const Polynomial divisor(item.divisor);
        const Polynomial quotient = dividend / divisor;
        const Polynomial remainder = dividend % divisor;
        CHECK_POLYNOMIAL(quotient, oracle::from_real(expected.first),
                         std::string(item.name) + " quotient");
        CHECK_POLYNOMIAL(remainder, oracle::from_real(expected.second),
                         std::string(item.name) + " remainder");
        CHECK_POLYNOMIAL(divisor * quotient + remainder,
                         oracle::from_real(item.dividend),
                         std::string(item.name) + " reconstruction");
        if (!oracle::is_zero(oracle::from_real(expected.second))) {
            CHECK(remainder.GetDegree() < divisor.GetDegree());
        }
        CHECK_POLYNOMIAL(dividend, oracle::from_real(item.dividend),
                         std::string(item.name) + " dividend immutable");
        CHECK_POLYNOMIAL(divisor, oracle::from_real(item.divisor),
                         std::string(item.name) + " divisor immutable");
    }

    Polynomial compound(std::vector<double>{5.0, 6.0, 0.0, 3.0});
    const Polynomial divisor(std::vector<double>{0.0, 3.0});
    const auto expected = oracle::real_divide_with_remainder(
        {5.0, 6.0, 0.0, 3.0}, {0.0, 3.0});
    Polynomial quotient_compound = compound;
    quotient_compound /= divisor;
    CHECK_POLYNOMIAL(quotient_compound, oracle::from_real(expected.first), "/= quotient");
    Polynomial remainder_compound = compound;
    remainder_compound %= divisor;
    CHECK_POLYNOMIAL(remainder_compound, oracle::from_real(expected.second), "%= remainder");

    const Polynomial zero(std::vector<double>{0.0, 0.0, 0.0});
    CHECK_THROWS((compound / zero), std::invalid_argument);
    CHECK_THROWS((compound % zero), std::invalid_argument);
    CHECK_INVALID_ARGUMENT_MESSAGE([&] { (void)(compound / zero); }, "zero polynomial",
                                   "division by zero message");
    CHECK_INVALID_ARGUMENT_MESSAGE([&] { (void)(compound % zero); }, "zero polynomial",
                                   "remainder by zero message");
}

TEST_CASE(polynomial, remainder_state_regressions) {
    const Polynomial high(std::vector<double>{1.0, 0.0, 1.0});
    const Polynomial linear(std::vector<double>{1.0, 1.0});
    const Polynomial x(std::vector<double>{0.0, 1.0});
    const Polynomial constant_two(2.0);

    Polynomial fresh(std::vector<double>{2.0, -1.0});
    CHECK_POLYNOMIAL(fresh / high, oracle::from_real({0.0}),
                     "1 lower-degree quotient");
    CHECK_POLYNOMIAL(fresh % high, oracle::from_real({2.0, -1.0}),
                     "2 corresponding lower-degree remainder");
    CHECK_POLYNOMIAL(fresh % high, oracle::from_real({2.0, -1.0}),
                     "3 repeated lower-degree remainder");

    Polynomial order_a(std::vector<double>{2.0, -1.0});
    (void)(order_a / x);
    CHECK_POLYNOMIAL(order_a % high, oracle::from_real({2.0, -1.0}),
                     "4 division followed by lower-degree remainder");

    Polynomial order_b(std::vector<double>{2.0, -1.0});
    (void)(order_b % x);
    CHECK_POLYNOMIAL(order_b / high, oracle::from_real({0.0}),
                     "5 remainder followed by lower-degree division quotient");
    CHECK_POLYNOMIAL(order_b % high, oracle::from_real({2.0, -1.0}),
                     "5 remainder followed by lower-degree division state");

    Polynomial sequence(std::vector<double>{1.0, 1.0});
    CHECK_POLYNOMIAL(sequence % linear, oracle::from_real({0.0}),
                     "6 exact division remainder");
    CHECK_POLYNOMIAL(sequence % x, oracle::from_real({1.0}),
                     "6 non-exact division after exact");
    CHECK_POLYNOMIAL(sequence % high, oracle::from_real({1.0, 1.0}),
                     "7 lower-degree remainder after non-exact");

    Polynomial multiple(std::vector<double>{1.0, 1.0, 1.0});
    CHECK_POLYNOMIAL(multiple % x, oracle::from_real({1.0}),
                     "8 divisor x");
    CHECK_POLYNOMIAL(multiple % linear, oracle::from_real({1.0}),
                     "8 divisor 1+x");
    CHECK_POLYNOMIAL(multiple % constant_two, oracle::from_real({0.0}),
                     "8 constant divisor");
    CHECK_POLYNOMIAL(multiple % x, oracle::from_real({1.0}),
                     "9 repeat divisors in different order");

    Polynomial first(std::vector<double>{2.0, -1.0});
    Polynomial second(std::vector<double>{3.0, 4.0});
    (void)(first % x);
    CHECK_POLYNOMIAL(second % high, oracle::from_real({3.0, 4.0}),
                     "10 remainder state must not leak between objects");

    Polynomial exception_state(std::vector<double>{1.0, 1.0, 1.0});
    (void)(exception_state % x);
    CHECK_THROWS((exception_state % Polynomial(0)), std::invalid_argument);
    CHECK_POLYNOMIAL(exception_state % linear, oracle::from_real({1.0}),
                     "zero-divisor exception cannot reuse stale result");
}

TEST_CASE(polynomial, deterministic_generated_properties) {
    std::mt19937 generator(kSeed);
    std::cout << "[ PROPERTY ] seed=" << kSeed << " cases=" << kGeneratedCases
              << " degrees=0..5 coefficients=-3..3\n";
    const oracle::Polynomial zero{{0.0, 0.0}};
    const oracle::Polynomial one{{1.0, 0.0}};
    for (int iteration = 0; iteration < kGeneratedCases; ++iteration) {
        const auto a = random_real_oracle(generator);
        const auto b = random_real_oracle(generator);
        const auto c = random_real_oracle(generator);
        const Polynomial pa = production_real(a);
        const Polynomial pb = production_real(b);
        const Polynomial pc = production_real(c);
        CHECK_POLYNOMIAL(pa + pb, oracle::add(a, b),
                         generated_context("addition", iteration, a, b));
        CHECK_POLYNOMIAL(pa + pb, oracle_from(pb + pa),
                         generated_context("addition commutativity", iteration, a, b));
        CHECK_POLYNOMIAL(pa * pb, oracle::multiply(a, b),
                         generated_context("multiplication", iteration, a, b));
        CHECK_POLYNOMIAL(pa * pb, oracle_from(pb * pa),
                         generated_context("multiplication commutativity", iteration, a, b));
        CHECK_POLYNOMIAL(pa + (pb + pc), oracle::add(a, oracle::add(b, c)),
                         generated_context("addition associativity", iteration, a, b));
        CHECK_POLYNOMIAL(pa * (pb + pc),
                         oracle::multiply(a, oracle::add(b, c)),
                         generated_context("distributivity lhs", iteration, a, b));
        CHECK_POLYNOMIAL(pa * pb + pa * pc,
                         oracle::add(oracle::multiply(a, b),
                                     oracle::multiply(a, c)),
                         generated_context("distributivity rhs", iteration, a, b));
        CHECK_POLYNOMIAL(pa + Polynomial(0), a,
                         generated_context("additive identity", iteration, a));
        CHECK_POLYNOMIAL(pa * Polynomial(1), a,
                         generated_context("multiplicative identity", iteration, a));
        CHECK_POLYNOMIAL(pa * Polynomial(0), zero,
                         generated_context("zero product", iteration, a));

        const auto divisor = random_real_oracle(generator, true);
        const auto quotient = random_real_oracle(generator);
        oracle::Polynomial remainder{{0.0, 0.0}};
        if (divisor.size() > 1) {
            remainder = random_real_oracle(generator);
            if (remainder.size() >= divisor.size()) {
                remainder.resize(divisor.size() - 1);
                remainder = oracle::normalize(std::move(remainder));
            }
        }
        const auto dividend = oracle::add(oracle::multiply(divisor, quotient), remainder);
        const Polynomial pdividend = production_real(dividend);
        const Polynomial pdivisor = production_real(divisor);
        const Polynomial actual_q = pdividend / pdivisor;
        const Polynomial actual_r = pdividend % pdivisor;
        CHECK_POLYNOMIAL(actual_q, quotient,
                         generated_context("division quotient", iteration, dividend, divisor));
        CHECK_POLYNOMIAL(actual_r, remainder,
                         generated_context("division remainder", iteration, dividend, divisor));
        CHECK_POLYNOMIAL(pdivisor * actual_q + actual_r, dividend,
                         generated_context("division reconstruction", iteration, dividend, divisor));
        CHECK_POLYNOMIAL(production_real(one), one,
                         generated_context("oracle identity fixture", iteration, one));
    }
}

TEST_CASE(complex, representation_and_api) {
    const ComplexPoly default_value;
    CHECK_COMPLEX(default_value, opoly({{0.0, 0.0}}), "default construction is zero");

    const ComplexPoly real_initializer({1.0, -2.0, 3.5});
    CHECK_COMPLEX(real_initializer, oracle::from_real({1.0, -2.0, 3.5}),
                  "real initializer-list construction");

    const std::vector<double> real_vector{1.0, 2.0, 0.0};
    const std::vector<double> empty_imaginary;
    const ComplexPoly real_vector_pair(real_vector, empty_imaginary);
    CHECK_COMPLEX(real_vector_pair, oracle::from_real({1.0, 2.0}),
                  "real vector plus empty imaginary vector");
    const ComplexPoly direct_real_vector(real_vector);
    CHECK_COMPLEX(direct_real_vector, oracle::from_real({1.0, 2.0}),
                  "direct real coefficient vector construction via Polynomial parameter");

    const ComplexPoly from_polynomial(Polynomial(std::vector<double>{-1.0, 0.5, 4.0}));
    CHECK_COMPLEX(from_polynomial, oracle::from_real({-1.0, 0.5, 4.0}),
                  "explicit construction from Polynomial");

    const ComplexPoly real_only(std::vector<double>{2.0, -3.0},
                                std::vector<double>{});
    const ComplexPoly imaginary_only(std::vector<double>{},
                                     std::vector<double>{2.0, -3.0});
    const ComplexPoly mixed(std::vector<double>{1.0, 2.0},
                            std::vector<double>{1.0, -3.0});
    CHECK_COMPLEX(real_only, opoly({{2.0, 0.0}, {-3.0, 0.0}}), "real-only value");
    CHECK_COMPLEX(imaginary_only, opoly({{0.0, 2.0}, {0.0, -3.0}}),
                  "imaginary-only value with empty real input");
    CHECK_COMPLEX(mixed, opoly({{1.0, 1.0}, {2.0, -3.0}}), "mixed value");

    const ComplexPoly unequal(std::vector<double>{2.0, -1.0},
                              std::vector<double>{1.0, 4.0, 0.0, 0.0, 0.0, 3.0});
    CHECK_COMPLEX(unequal,
                  opoly({{2.0, 1.0}, {-1.0, 4.0}, {0.0, 0.0}, {0.0, 0.0},
                         {0.0, 0.0}, {0.0, 3.0}}),
                  "imaginary degree greater than real degree");
    CHECK_NEAR(unequal.GetCoeff(5), 0.0, kTolerance);
    CHECK_NEAR(unequal.getComplexCoeff(5), 3.0, kTolerance);
    CHECK_NEAR(unequal.GetCoeff(-1), 0.0, kTolerance);
    CHECK_NEAR(unequal.getComplexCoeff(-1), 0.0, kTolerance);
    CHECK_NEAR(unequal.GetCoeff(99), 0.0, kTolerance);
    CHECK_NEAR(unequal.getComplexCoeff(99), 0.0, kTolerance);

    const ComplexPoly trailing(std::vector<double>{1.0, 0.0, 0.0},
                               std::vector<double>{2.0, 0.0, 0.0});
    CHECK_EQ(trailing.getListCoeffsIn().size(), std::size_t{1});
    CHECK_EQ(trailing.getcomplexCoeffsList().size(), std::size_t{1});
    CHECK_COMPLEX(trailing, opoly({{1.0, 2.0}}),
                  "real and imaginary trailing zeros trimmed");

    const ComplexPoly empty_both(std::vector<double>{}, std::vector<double>{});
    CHECK_COMPLEX(empty_both, opoly({{0.0, 0.0}}), "empty real and imaginary vectors");

    ComplexPoly copied(unequal);
    ComplexPoly assigned;
    assigned = mixed;
    CHECK_COMPLEX(copied, oracle_from(unequal), "copy construction");
    CHECK_COMPLEX(assigned, oracle_from(mixed), "copy assignment");
    assigned = assigned;
    CHECK_COMPLEX(assigned, oracle_from(mixed), "self assignment");
}

TEST_CASE(complex, addition_subtraction_and_mixed_operands) {
    const ComplexPoly a(std::vector<double>{1.0, 2.0},
                        std::vector<double>{1.0, -3.0});
    const ComplexPoly b(std::vector<double>{-4.0, 0.0, 1.5, 0.0, 2.0},
                        std::vector<double>{2.0, 0.5, -1.0});
    const ComplexPoly imaginary(std::vector<double>{},
                                std::vector<double>{0.0, 1.0, 0.0, -2.0});
    const ComplexPoly zero;
    const Polynomial real(std::vector<double>{3.0, -1.0, 0.0, 0.5});
    const auto oa = oracle_from(a);
    const auto ob = oracle_from(b);
    const auto oi = oracle_from(imaginary);
    const auto ore = oracle_from(real);

    CHECK_COMPLEX(a + b, oracle::add(oa, ob), "ComplexPoly + ComplexPoly");
    CHECK_COMPLEX(a - b, oracle::subtract(oa, ob), "ComplexPoly - ComplexPoly");
    CHECK_COMPLEX(a + b, oracle_from(b + a), "complex addition commutativity");
    CHECK_COMPLEX(a + zero, oa, "a + 0 = a");
    CHECK_COMPLEX(a - a, opoly({{0.0, 0.0}}), "a - a = 0");
    CHECK_COMPLEX(a - b, oracle::scale(oracle::subtract(ob, oa), {-1.0, 0.0}),
                  "a-b = -(b-a)");
    CHECK_COMPLEX(a + imaginary, oracle::add(oa, oi),
                  "mixed real/imaginary length addition");
    CHECK_COMPLEX(a - imaginary, oracle::subtract(oa, oi),
                  "high imaginary degree subtraction");

    CHECK_COMPLEX(a + real, oracle::add(oa, ore), "ComplexPoly + Polynomial");
    CHECK_COMPLEX(a - real, oracle::subtract(oa, ore), "ComplexPoly - Polynomial");
    CHECK_COMPLEX(real + a, oracle::add(ore, oa), "Polynomial + ComplexPoly");
    CHECK_COMPLEX(real - a, oracle::subtract(ore, oa), "Polynomial - ComplexPoly");
    CHECK_COMPLEX(a, oa, "addition/subtraction preserve complex lhs");
    CHECK_COMPLEX(b, ob, "addition/subtraction preserve complex rhs");
    CHECK_POLYNOMIAL(real, ore, "mixed operations preserve Polynomial operand");
}

TEST_CASE(complex, multiplication_safe_domain) {
    const ComplexPoly one_plus_i(std::vector<double>{1.0}, std::vector<double>{1.0});
    const ComplexPoly one_minus_i(std::vector<double>{1.0}, std::vector<double>{-1.0});
    CHECK_COMPLEX(one_plus_i * one_plus_i, opoly({{0.0, 2.0}}),
                  "(1+i)(1+i)=2i");
    CHECK_COMPLEX(one_plus_i * one_minus_i, opoly({{2.0, 0.0}}),
                  "(1+i)(1-i)=2");

    const ComplexPoly polynomial(std::vector<double>{2.0, -1.0, 3.0},
                                 std::vector<double>{1.0, 4.0, -2.0});
    const ComplexPoly constant(std::vector<double>{-2.0}, std::vector<double>{0.5});
    const Polynomial real(std::vector<double>{1.0, -3.0, 2.0});
    const auto op = oracle_from(polynomial);
    const auto oc = oracle_from(constant);
    const auto ore = oracle_from(real);
    CHECK_COMPLEX(constant * polynomial, oracle::multiply(oc, op),
                  "complex constant times complex polynomial");
    CHECK_COMPLEX(polynomial * constant, oracle::multiply(op, oc),
                  "complex multiplication commutativity");
    CHECK_COMPLEX(real * polynomial, oracle::multiply(ore, op),
                  "real polynomial times complex polynomial");
    CHECK_COMPLEX(polynomial * real, oracle::multiply(op, ore),
                  "complex polynomial times real polynomial");
    CHECK_COMPLEX(polynomial * ComplexPoly(), opoly({{0.0, 0.0}}),
                  "complex times zero");
    CHECK_COMPLEX(polynomial * ComplexPoly({1.0}), op, "complex times identity");

    const ComplexPoly q(std::vector<double>{-1.0, 2.0},
                        std::vector<double>{3.0});
    const ComplexPoly r(std::vector<double>{4.0, -2.0, 1.0},
                        std::vector<double>{-1.0, 0.5});
    CHECK_COMPLEX(polynomial * (q + r),
                  oracle::multiply(op, oracle::add(oracle_from(q), oracle_from(r))),
                  "complex distributivity lhs");
    CHECK_COMPLEX(polynomial * q + polynomial * r,
                  oracle::add(oracle::multiply(op, oracle_from(q)),
                              oracle::multiply(op, oracle_from(r))),
                  "complex distributivity rhs");

    const ComplexPoly zero_real_constant(std::vector<double>{},
                                         std::vector<double>{2.0});
    const ComplexPoly another_imaginary(std::vector<double>{0.0},
                                        std::vector<double>{-3.0});
    CHECK_COMPLEX(zero_real_constant * another_imaginary, opoly({{6.0, 0.0}}),
                  "zero/empty real vectors with nonzero constant imaginary parts");
    CHECK_COMPLEX(polynomial, op, "multiplication preserves lhs");
    CHECK_COMPLEX(constant, oc, "multiplication preserves rhs");
}

TEST_CASE(complex, constant_and_nonconstant_division) {
    struct ConstantCase {
        oracle::Polynomial numerator;
        oracle::Polynomial denominator;
        oracle::Polynomial expected;
        const char *name;
    };
    const std::vector<ConstantCase> constant_cases{
        {{{1.0, 1.0}}, {{2.0, 0.0}}, {{0.5, 0.5}}, "(1+i)/2"},
        {{{1.0, 1.0}}, {{-2.0, 0.0}}, {{-0.5, -0.5}}, "(1+i)/-2"},
        {{{1.0, 1.0}}, {{1.0, -1.0}}, {{0.0, 1.0}}, "(1+i)/(1-i)"},
        {{{2.0, 4.0}}, {{1.0, 1.0}}, {{3.0, 1.0}}, "(2+4i)/(1+i)"},
        {{{0.0, 1.0}}, {{0.0, 1.0}}, {{1.0, 0.0}}, "i/i"},
    };
    for (const auto &item : constant_cases) {
        const ComplexPoly numerator = production_complex(item.numerator);
        const ComplexPoly denominator = production_complex(item.denominator);
        CHECK_COMPLEX(numerator / denominator, item.expected, item.name);
        CHECK_COMPLEX(numerator, item.numerator, std::string(item.name) + " numerator immutable");
        CHECK_COMPLEX(denominator, item.denominator,
                      std::string(item.name) + " denominator immutable");
    }

    const ComplexPoly nonconstant_numerator(std::vector<double>{-1.0, 0.0, 1.0},
                                            std::vector<double>{});
    const ComplexPoly nonconstant_denominator(std::vector<double>{-1.0, 1.0},
                                              std::vector<double>{});
    CHECK_COMPLEX(nonconstant_numerator / nonconstant_denominator,
                  oracle::from_real({1.0, 1.0}),
                  "exact nonconstant (x^2-1)/(x-1)=x+1");

    const oracle::Polynomial complex_denominator{{1.0, 1.0}, {2.0, -1.0}};
    const oracle::Polynomial complex_quotient{{2.0, 1.0}};
    const auto complex_dividend = oracle::multiply(complex_denominator, complex_quotient);
    CHECK_COMPLEX(production_complex(complex_dividend) /
                      production_complex(complex_denominator),
                  complex_quotient,
                  "exact complex-coefficient nonconstant division");

    const ComplexPoly x(std::vector<double>{0.0, 1.0}, std::vector<double>{});
    CHECK_COMPLEX(x / x, oracle::from_real({1.0}),
                  "x/x must be finite polynomial one");

    const ComplexPoly mixed_dividend(std::vector<double>{1.0, 2.0, 1.0},
                                     std::vector<double>{1.0, -1.0, 2.0});
    const ComplexPoly mixed_divisor(std::vector<double>{1.0, 1.0},
                                    std::vector<double>{1.0, -0.5});
    const ComplexPoly returned = mixed_dividend / mixed_divisor;
    CHECK_COMPLEX(mixed_divisor * returned, oracle_from(mixed_dividend),
                  "general returned quotient must reconstruct dividend when represented as polynomial");

    const ComplexPoly zero(std::vector<double>{0.0, 0.0},
                           std::vector<double>{0.0, 0.0});
    CHECK_THROWS((mixed_dividend / zero), std::invalid_argument);
    CHECK_INVALID_ARGUMENT_MESSAGE([&] { (void)(mixed_dividend / zero); },
                                   "zero polynomial", "complex zero-divisor message");
    std::cout << "[ CONTRACT ] ComplexPoly division classification evidence: "
                 "constant cases tested; exact/general nonconstant reconstruction required\n";
}

TEST_CASE(complex, deterministic_generated_safe_properties) {
    std::mt19937 generator(kSeed);
    std::cout << "[ PROPERTY ] complex seed=" << kSeed << " cases="
              << kGeneratedCases << " degrees=0..5 coefficients=-3..3\n";
    const oracle::Polynomial zero{{0.0, 0.0}};
    const oracle::Polynomial one{{1.0, 0.0}};
    for (int iteration = 0; iteration < kGeneratedCases; ++iteration) {
        const auto a = random_safe_complex_oracle(generator);
        const auto b = random_safe_complex_oracle(generator);
        const auto c = random_safe_complex_oracle(generator);
        const ComplexPoly pa = production_complex(a);
        const ComplexPoly pb = production_complex(b);
        const ComplexPoly pc = production_complex(c);
        CHECK_COMPLEX(pa + pb, oracle::add(a, b),
                      generated_context("complex addition", iteration, a, b));
        CHECK_COMPLEX(pa + pb, oracle_from(pb + pa),
                      generated_context("complex addition commutativity", iteration, a, b));
        CHECK_COMPLEX(pa * pb, oracle::multiply(a, b),
                      generated_context("complex multiplication", iteration, a, b));
        CHECK_COMPLEX(pa * pb, oracle_from(pb * pa),
                      generated_context("complex multiplication commutativity", iteration, a, b));
        CHECK_COMPLEX(pa + (pb + pc), oracle::add(a, oracle::add(b, c)),
                      generated_context("complex addition associativity", iteration, a, b));
        CHECK_COMPLEX(pa * (pb + pc),
                      oracle::multiply(a, oracle::add(b, c)),
                      generated_context("complex distributivity lhs", iteration, a, b));
        CHECK_COMPLEX(pa * pb + pa * pc,
                      oracle::add(oracle::multiply(a, b),
                                  oracle::multiply(a, c)),
                      generated_context("complex distributivity rhs", iteration, a, b));
        CHECK_COMPLEX(pa + ComplexPoly(), a,
                      generated_context("complex additive identity", iteration, a));
        CHECK_COMPLEX(pa * ComplexPoly({1.0}), a,
                      generated_context("complex multiplicative identity", iteration, a));
        CHECK_COMPLEX(pa * ComplexPoly(), zero,
                      generated_context("complex zero product", iteration, a));
        CHECK_COMPLEX(production_complex(one), one,
                      generated_context("complex oracle identity fixture", iteration, one));
    }
}

TEST_CASE(complex, high_imaginary_degree_memory_regressions) {
    const ComplexPoly ix(std::vector<double>{0.0}, std::vector<double>{0.0, 1.0});
    CHECK_ISOLATED_COMPLEX_PRODUCT(ix, ix, oracle::from_real({0.0, 0.0, -1.0}),
                                   "minimal (i*x)(i*x)=-x^2");

    const ComplexPoly ix3(std::vector<double>{0.0},
                          std::vector<double>{0.0, 0.0, 0.0, 1.0});
    const ComplexPoly ix2(std::vector<double>{0.0},
                          std::vector<double>{0.0, 0.0, 1.0});
    CHECK_ISOLATED_COMPLEX_PRODUCT(ix3, ix2,
                                   oracle::from_real({0.0, 0.0, 0.0, 0.0, 0.0, -1.0}),
                                   "(i*x^3)(i*x^2)=-x^5");

    const ComplexPoly high_one(std::vector<double>{2.0, -1.0},
                               std::vector<double>{1.0, 4.0, 0.0, 0.0, 3.0});
    const ComplexPoly high_two(std::vector<double>{-1.0},
                               std::vector<double>{2.0, 0.0, -2.0});
    CHECK_ISOLATED_COMPLEX_PRODUCT(high_one, high_two,
                                   oracle::multiply(oracle_from(high_one),
                                                    oracle_from(high_two)),
                                   "both operands have imaginary degree greater than real degree");
}

void verify_rational_semantics(const RationalComplex &actual,
                               const oracle::Rational &expected_input,
                               const std::string &label,
                               const char *file,
                               int line) {
    const oracle::Rational actual_oracle = oracle_from(actual);
    const oracle::Rational expected = oracle::normalize_rational(expected_input);
    const std::vector<double> points{-3.0, -2.0, -0.5, 0.0, 0.75, 1.5, 3.0};
    int tested = 0;
    int skipped = 0;
    for (double point : points) {
        const auto expected_numerator = oracle::evaluate(expected.numerator, point);
        const auto expected_denominator = oracle::evaluate(expected.denominator, point);
        const auto actual_numerator = oracle::evaluate(actual_oracle.numerator, point);
        const auto actual_denominator = oracle::evaluate(actual_oracle.denominator, point);
        const bool expected_pole = oracle::finite(expected_denominator) &&
                                   std::abs(expected_denominator) <= kTolerance;
        const bool actual_pole = oracle::finite(actual_denominator) &&
                                 std::abs(actual_denominator) <= kTolerance;
        if (expected_pole || actual_pole) {
            ++skipped;
            std::cout << "[ SEMANTIC ] " << label << " x=" << point
                      << " SKIP reason="
                      << (expected_pole ? "expected denominator near zero" : "")
                      << (expected_pole && actual_pole ? "; " : "")
                      << (actual_pole ? "actual denominator near zero" : "")
                      << " expected_den=" << oracle::format(expected_denominator)
                      << " actual_den=" << oracle::format(actual_denominator) << '\n';
            continue;
        }
        ++tested;
        const auto expected_value = expected_numerator / expected_denominator;
        const auto actual_value = actual_numerator / actual_denominator;
        std::cout << "[ SEMANTIC ] " << label << " x=" << point
                  << " expected_num=" << oracle::format(expected_numerator)
                  << " expected_den=" << oracle::format(expected_denominator)
                  << " actual_num=" << oracle::format(actual_numerator)
                  << " actual_den=" << oracle::format(actual_denominator)
                  << " expected_value=" << oracle::format(expected_value)
                  << " actual_value=" << oracle::format(actual_value) << '\n';
        check_complex_value(actual_value, expected_value,
                            label + " semantic value at x=" + std::to_string(point),
                            file, line);
    }
    coretest::record(tested >= 3, (label + " has at least three safe points").c_str(),
                     file, line, "at least 3 tested points",
                     "tested=" + std::to_string(tested) +
                         " skipped=" + std::to_string(skipped));
}

#define CHECK_RATIONAL_SEMANTICS(actual, expected, label) \
    verify_rational_semantics((actual), (expected), (label), __FILE__, __LINE__)

TEST_CASE(rational, construction_normalization_and_exceptions) {
    const RationalComplex default_value;
    CHECK_RATIONAL(default_value,
                   orational(opoly({{0.0, 0.0}}), opoly({{1.0, 0.0}})),
                   "default is 0/1");

    const ComplexPoly numerator(std::vector<double>{1.0, -2.0, 3.0},
                                std::vector<double>{0.5, 1.0, -1.0});
    const RationalComplex from_numerator(numerator);
    CHECK_RATIONAL(from_numerator,
                   orational(oracle_from(numerator), opoly({{1.0, 0.0}})),
                   "numerator-only construction uses denominator one");

    const ComplexPoly denominator(std::vector<double>{2.0, 1.0},
                                  std::vector<double>{-1.0, 0.5});
    const RationalComplex pair(numerator, denominator);
    CHECK_RATIONAL(pair, orational(oracle_from(numerator), oracle_from(denominator)),
                   "complex numerator/denominator construction");

    const Polynomial real_num(std::vector<double>{1.0, 0.0, -1.0});
    const Polynomial real_den(std::vector<double>{2.0, 3.0});
    const RationalComplex real_pair{ComplexPoly(real_num), ComplexPoly(real_den)};
    CHECK_RATIONAL(real_pair,
                   orational(oracle_from(real_num), oracle_from(real_den)),
                   "real polynomial numerator and denominator via explicit ComplexPoly");

    const ComplexPoly purely_imaginary_den(std::vector<double>{0.0},
                                           std::vector<double>{2.0, 1.0});
    CHECK_NO_THROW(RationalComplex(numerator, purely_imaginary_den));
    const ComplexPoly real_den_zero_imag(std::vector<double>{2.0, 1.0},
                                         std::vector<double>{0.0, 0.0, 0.0});
    CHECK_NO_THROW(RationalComplex(numerator, real_den_zero_imag));

    const ComplexPoly many_zero_den(std::vector<double>{0.0, 0.0, 0.0},
                                    std::vector<double>{0.0, 0.0, 0.0});
    CHECK_THROWS(RationalComplex(numerator, many_zero_den), std::runtime_error);
    CHECK_RUNTIME_ERROR_MESSAGE([&] { (void)RationalComplex(numerator, many_zero_den); },
                                "zero denominator", "zero denominator message");

    const ComplexPoly zero_num(std::vector<double>{0.0, 0.0},
                               std::vector<double>{0.0, 0.0});
    const RationalComplex normalized_zero(zero_num, denominator);
    CHECK_RATIONAL(normalized_zero,
                   orational(opoly({{0.0, 0.0}}), opoly({{1.0, 0.0}})),
                   "zero numerator normalized to 0/1");

    const RationalComplex unreduced(ComplexPoly({2.0}), ComplexPoly({4.0}));
    CHECK_RATIONAL(unreduced,
                   orational(opoly({{2.0, 0.0}}), opoly({{4.0, 0.0}})),
                   "2/4 need not be structurally reduced");
    CHECK_RATIONAL_SEMANTICS(unreduced,
                             orational(opoly({{1.0, 0.0}}), opoly({{2.0, 0.0}})),
                             "unreduced 2/4 value");

    RationalComplex copied(pair);
    RationalComplex assigned;
    assigned = pair;
    CHECK_RATIONAL(copied, oracle_from(pair), "rational copy construction");
    CHECK_RATIONAL(assigned, oracle_from(pair), "rational copy assignment");
    assigned = assigned;
    CHECK_RATIONAL(assigned, oracle_from(pair), "rational self assignment");
    CHECK_COMPLEX(numerator, oracle_from(numerator), "construction preserves numerator operand");
    CHECK_COMPLEX(denominator, oracle_from(denominator),
                  "construction preserves denominator operand");
}

TEST_CASE(rational, scale_sensitive_zero_normalization) {
    const ComplexPoly tiny_numerator(std::vector<double>{5e-10},
                                     std::vector<double>{});
    const ComplexPoly small_denominator(std::vector<double>{1e-8},
                                        std::vector<double>{});
    const oracle::Rational expected{{{5e-10, 0.0}}, {{1e-8, 0.0}}};
    const RationalComplex value(tiny_numerator, small_denominator);
    CHECK_RATIONAL(value, expected,
                   "nonzero 5e-10/1e-8 must not normalize to zero");
    CHECK_RATIONAL_SEMANTICS(value, expected,
                             "scale-sensitive nonzero rational 0.05");

    const ComplexPoly tiny_nonzero_denominator(std::vector<double>{5e-10},
                                               std::vector<double>{});
    CHECK_NO_THROW(RationalComplex(ComplexPoly({1.0}), tiny_nonzero_denominator));
}

TEST_CASE(rational, arithmetic_structural_and_compound) {
    const oracle::Rational oa{
        {{1.0, 1.0}, {2.0, -0.5}},
        {{2.0, -0.5}, {1.0, 0.25}}};
    const oracle::Rational ob{
        {{-1.0, 0.5}, {1.5, 0.25}, {2.0, -1.0}},
        {{1.0, -1.0}, {2.0, 0.5}}};
    const RationalComplex a = production_rational(oa);
    const RationalComplex b = production_rational(ob);
    const auto a_before = oracle_from(a);
    const auto b_before = oracle_from(b);

    CHECK_RATIONAL(a + b, oracle::rational_add(oa, ob),
                   "(a/b)+(c/d)=(ad+cb)/(bd)");
    CHECK_RATIONAL(a - b, oracle::rational_subtract(oa, ob),
                   "(a/b)-(c/d)=(ad-cb)/(bd)");
    CHECK_RATIONAL(a * b, oracle::rational_multiply(oa, ob),
                   "(a/b)*(c/d)=ac/bd");
    CHECK_RATIONAL(a / b, oracle::rational_divide(oa, ob),
                   "(a/b)/(c/d)=ad/bc");
    CHECK_RATIONAL(a, a_before, "non-mutating rational operations preserve lhs");
    CHECK_RATIONAL(b, b_before, "non-mutating rational operations preserve rhs");

    const oracle::Rational real_a{oracle::from_real({1.0, -2.0, 1.0}),
                                  oracle::from_real({2.0, 1.0})};
    const oracle::Rational real_b{oracle::from_real({-3.0, 1.0}),
                                  oracle::from_real({1.0, 0.0, 1.0})};
    RationalComplex compound = production_rational(real_a);
    compound += production_rational(real_b);
    CHECK_RATIONAL(compound, oracle::rational_add(real_a, real_b), "+= structural formula");
    compound = production_rational(real_a);
    compound -= production_rational(real_b);
    CHECK_RATIONAL(compound, oracle::rational_subtract(real_a, real_b), "-= structural formula");
    compound = production_rational(real_a);
    compound *= production_rational(real_b);
    CHECK_RATIONAL(compound, oracle::rational_multiply(real_a, real_b), "*= structural formula");
    compound = production_rational(real_a);
    compound /= production_rational(real_b);
    CHECK_RATIONAL(compound, oracle::rational_divide(real_a, real_b), "/= structural formula");

    RationalComplex self = production_rational(real_a);
    self += self;
    CHECK_RATIONAL(self, oracle::rational_add(real_a, real_a), "self +=");
    self = production_rational(real_a);
    self -= self;
    CHECK_RATIONAL(self, orational(opoly({{0.0, 0.0}}), opoly({{1.0, 0.0}})),
                   "self -=");
    self = production_rational(real_a);
    self *= self;
    CHECK_RATIONAL(self, oracle::rational_multiply(real_a, real_a), "self *=");
    self = production_rational(real_a);
    self /= self;
    CHECK_RATIONAL_SEMANTICS(self,
                             orational(opoly({{1.0, 0.0}}), opoly({{1.0, 0.0}})),
                             "self /= equals one semantically");

    const RationalComplex zero;
    const RationalComplex one(ComplexPoly({1.0}));
    CHECK_RATIONAL(a + zero, oa, "rational additive identity");
    CHECK_RATIONAL(a * one, oa, "rational multiplicative identity");
    CHECK_RATIONAL(a * zero,
                   orational(opoly({{0.0, 0.0}}), opoly({{1.0, 0.0}})),
                   "rational zero product normalized");
    CHECK_RATIONAL(a - a,
                   orational(opoly({{0.0, 0.0}}), opoly({{1.0, 0.0}})),
                   "rational cancellation normalized");
    CHECK_THROWS((a / zero), std::runtime_error);
    CHECK_THROWS((compound /= zero), std::runtime_error);
    CHECK_RUNTIME_ERROR_MESSAGE([&] { (void)(a / zero); },
                                "division by zero rational",
                                "rational zero-divisor message");
}

TEST_CASE(rational, semantic_verification) {
    const oracle::Rational a{
        {{1.0, 1.0}, {-2.0, 0.5}, {1.0, -0.25}},
        {{2.0, 0.5}, {1.0, -0.25}, {1.0, 0.0}}};
    const oracle::Rational b{
        {{-1.0, 0.25}, {2.0, -1.0}},
        {{3.0, -0.5}, {-1.0, 0.25}, {1.0, 0.0}}};
    const RationalComplex pa = production_rational(a);
    const RationalComplex pb = production_rational(b);
    CHECK_RATIONAL_SEMANTICS(pa + pb, oracle::rational_add(a, b), "rational addition");
    CHECK_RATIONAL_SEMANTICS(pa - pb, oracle::rational_subtract(a, b), "rational subtraction");
    CHECK_RATIONAL_SEMANTICS(pa * pb, oracle::rational_multiply(a, b), "rational multiplication");
    CHECK_RATIONAL_SEMANTICS(pa / pb, oracle::rational_divide(a, b), "rational division");

    const oracle::Rational pole_expected{oracle::from_real({1.0}),
                                         oracle::from_real({0.0, 1.0})};
    const RationalComplex pole_value = production_rational(pole_expected);
    CHECK_RATIONAL_SEMANTICS(pole_value, pole_expected,
                             "explicit safe-point skip for 1/x");
}

TEST_CASE(rational, deterministic_generated_properties) {
    std::mt19937 generator(kSeed);
    std::cout << "[ PROPERTY ] rational seed=" << kSeed << " cases="
              << kGeneratedCases << " degrees=0..5 coefficients=-3..3\n";
    for (int iteration = 0; iteration < kGeneratedCases; ++iteration) {
        const oracle::Rational a{random_safe_complex_oracle(generator),
                                 random_safe_complex_oracle(generator, true)};
        const oracle::Rational b{random_safe_complex_oracle(generator, true),
                                 random_safe_complex_oracle(generator, true)};
        const RationalComplex pa = production_rational(a);
        const RationalComplex pb = production_rational(b);
        const std::string prefix = "seed=" + std::to_string(kSeed) +
                                   " iteration=" + std::to_string(iteration) +
                                   " a=" + oracle::format(a) +
                                   " b=" + oracle::format(b);
        CHECK_RATIONAL(pa + pb, oracle::rational_add(a, b),
                       "generated rational addition " + prefix);
        CHECK_RATIONAL(pa - pb, oracle::rational_subtract(a, b),
                       "generated rational subtraction " + prefix);
        CHECK_RATIONAL(pa * pb, oracle::rational_multiply(a, b),
                       "generated rational multiplication " + prefix);
        CHECK_RATIONAL(pa / pb, oracle::rational_divide(a, b),
                       "generated rational division " + prefix);
    }
}

template <typename Left, typename Right>
using addition_expression = decltype(std::declval<Left>() + std::declval<Right>());
template <typename Left, typename Right>
using subtraction_expression = decltype(std::declval<Left>() - std::declval<Right>());
template <typename Left, typename Right>
using multiplication_expression = decltype(std::declval<Left>() * std::declval<Right>());
template <typename Left, typename Right>
using division_expression = decltype(std::declval<Left>() / std::declval<Right>());

template <template <typename, typename> class Expression,
          typename Left,
          typename Right,
          typename = void>
struct expression_info {
    static constexpr bool compiles = false;
};

template <template <typename, typename> class Expression,
          typename Left,
          typename Right>
struct expression_info<Expression, Left, Right,
                       std::void_t<Expression<Left, Right>>> {
    static constexpr bool compiles = true;
    using result_type = Expression<Left, Right>;
};

template <typename Type>
const char *type_name() {
    if constexpr (std::is_same_v<Type, Polynomial>) {
        return "Polynomial";
    } else if constexpr (std::is_same_v<Type, ComplexPoly>) {
        return "ComplexPoly";
    } else if constexpr (std::is_same_v<Type, RationalComplex>) {
        return "RationalComplex";
    } else if constexpr (std::is_same_v<Type, int>) {
        return "int";
    } else if constexpr (std::is_same_v<Type, double>) {
        return "double";
    }
    return "other";
}

template <template <typename, typename> class Expression,
          typename Left,
          typename Right>
void print_expression_row(const char *operation,
                          const char *classification,
                          const char *notes,
                          bool runtime_verified) {
    using Info = expression_info<Expression, Left, Right>;
    std::cout << "[ MATRIX ] " << type_name<Left>() << " | " << operation << " | "
              << type_name<Right>() << " | " << (Info::compiles ? "YES" : "NO")
              << " | ";
    if constexpr (Info::compiles) {
        std::cout << type_name<typename Info::result_type>();
    } else {
        std::cout << "-";
    }
    std::cout << " | " << (runtime_verified ? "YES" : "NO") << " | "
              << classification << " | " << notes << '\n';
}

template <typename Left, typename Right>
void print_four_expression_rows(const char *classification,
                                const char *notes,
                                bool runtime_verified) {
    print_expression_row<addition_expression, Left, Right>(
        "+", classification, notes, runtime_verified);
    print_expression_row<subtraction_expression, Left, Right>(
        "-", classification, notes, runtime_verified);
    print_expression_row<multiplication_expression, Left, Right>(
        "*", classification, notes, runtime_verified);
    print_expression_row<division_expression, Left, Right>(
        "/", classification, notes, runtime_verified);
}

TEST_CASE(api, compile_time_expression_and_construction_matrix) {
    CHECK((std::is_constructible_v<Polynomial, int>));
    CHECK((std::is_convertible_v<int, Polynomial>));
    CHECK((std::is_constructible_v<Polynomial, double>));
    CHECK((std::is_convertible_v<double, Polynomial>));
    CHECK((std::is_constructible_v<ComplexPoly, Polynomial>));
    CHECK(!(std::is_convertible_v<Polynomial, ComplexPoly>));
    CHECK((std::is_constructible_v<ComplexPoly, std::vector<double>>));
    CHECK(!(std::is_convertible_v<std::vector<double>, ComplexPoly>));
    CHECK((std::is_constructible_v<RationalComplex, ComplexPoly>));
    CHECK(!(std::is_convertible_v<ComplexPoly, RationalComplex>));
    CHECK(!(std::is_constructible_v<RationalComplex, Polynomial>));
    CHECK(!(std::is_convertible_v<Polynomial, RationalComplex>));
    CHECK((std::is_same_v<decltype(make_rational(std::declval<const Polynomial &>())),
                          RationalComplex>));
    CHECK((std::is_same_v<decltype(make_rational(std::declval<const ComplexPoly &>())),
                          RationalComplex>));

    CHECK((expression_info<addition_expression, Polynomial, Polynomial>::compiles));
    CHECK((expression_info<division_expression, Polynomial, int>::compiles));
    CHECK((expression_info<multiplication_expression, double, Polynomial>::compiles));
    CHECK((expression_info<addition_expression, Polynomial, ComplexPoly>::compiles));
    CHECK((expression_info<division_expression, ComplexPoly, Polynomial>::compiles));
    CHECK((expression_info<multiplication_expression, ComplexPoly, ComplexPoly>::compiles));
    CHECK((expression_info<addition_expression, RationalComplex, RationalComplex>::compiles));
    CHECK(!(expression_info<addition_expression, RationalComplex, Polynomial>::compiles));
    CHECK(!(expression_info<subtraction_expression, Polynomial, RationalComplex>::compiles));
    CHECK(!(expression_info<multiplication_expression, RationalComplex, ComplexPoly>::compiles));
    CHECK(!(expression_info<division_expression, ComplexPoly, RationalComplex>::compiles));

    std::cout << "[ MATRIX HEADER ] Left Type | Operation | Right Type | Compiles | "
                 "Result Type | Runtime Verified | Classification | Notes\n";
    print_four_expression_rows<Polynomial, Polynomial>(
        "IMPLICITLY_SUPPORTED", "same-type public operators", true);
    print_four_expression_rows<Polynomial, int>(
        "IMPLICITLY_SUPPORTED", "RHS converts to Polynomial", true);
    print_four_expression_rows<Polynomial, double>(
        "IMPLICITLY_SUPPORTED", "RHS converts to Polynomial", true);
    print_four_expression_rows<int, Polynomial>(
        "DIRECT_MIXED_OPERATOR_SUPPORTED", "nonmember scalar-left overload", true);
    print_four_expression_rows<double, Polynomial>(
        "DIRECT_MIXED_OPERATOR_SUPPORTED", "nonmember scalar-left overload", true);
    print_four_expression_rows<Polynomial, ComplexPoly>(
        "DIRECT_MIXED_OPERATOR_SUPPORTED", "inline Polynomial-left overload", true);
    print_four_expression_rows<ComplexPoly, Polynomial>(
        "DIRECT_MIXED_OPERATOR_SUPPORTED", "ComplexPoly member overload", true);
    print_four_expression_rows<ComplexPoly, ComplexPoly>(
        "IMPLICITLY_SUPPORTED", "same-type public operators", true);
    print_four_expression_rows<RationalComplex, Polynomial>(
        "NOT_SUPPORTED", "use make_rational(Polynomial)", false);
    print_four_expression_rows<Polynomial, RationalComplex>(
        "NOT_SUPPORTED", "use make_rational(Polynomial)", false);
    print_four_expression_rows<RationalComplex, ComplexPoly>(
        "NOT_SUPPORTED", "use make_rational(ComplexPoly)", false);
    print_four_expression_rows<ComplexPoly, RationalComplex>(
        "NOT_SUPPORTED", "use make_rational(ComplexPoly)", false);
    print_four_expression_rows<RationalComplex, RationalComplex>(
        "IMPLICITLY_SUPPORTED", "same-type public operators", true);

    std::cout << "[ CONSTRUCTION ] Polynomial -> ComplexPoly | constructible=YES | "
                 "convertible=NO | EXPLICIT_CONSTRUCTION_SUPPORTED\n"
              << "[ CONSTRUCTION ] vector<double> -> ComplexPoly | constructible=YES | "
                 "convertible=NO | EXPLICIT_CONSTRUCTION_SUPPORTED (via Polynomial parameter)\n"
              << "[ CONSTRUCTION ] ComplexPoly -> RationalComplex | constructible=YES | "
                 "convertible=NO | EXPLICIT_CONSTRUCTION_SUPPORTED\n"
              << "[ CONSTRUCTION ] Polynomial -> RationalComplex | constructible=NO | "
                 "convertible=NO | NOT_SUPPORTED directly\n"
              << "[ CONSTRUCTION ] make_rational(Polynomial) | result=RationalComplex | "
                 "SUPPORTED_VIA_MAKE_RATIONAL\n"
              << "[ CONSTRUCTION ] make_rational(ComplexPoly) | result=RationalComplex | "
                 "SUPPORTED_VIA_MAKE_RATIONAL\n";
}

TEST_CASE(api, runtime_supported_expression_matrix) {
    const Polynomial p(std::vector<double>{4.0});
    const Polynomial q(std::vector<double>{2.0});
    CHECK_POLYNOMIAL(p + q, oracle::from_real({6.0}), "Polynomial + Polynomial");
    CHECK_POLYNOMIAL(p - q, oracle::from_real({2.0}), "Polynomial - Polynomial");
    CHECK_POLYNOMIAL(p * q, oracle::from_real({8.0}), "Polynomial * Polynomial");
    CHECK_POLYNOMIAL(p / q, oracle::from_real({2.0}), "Polynomial / Polynomial");

    CHECK_POLYNOMIAL(p + 2, oracle::from_real({6.0}), "Polynomial + int");
    CHECK_POLYNOMIAL(p - 2, oracle::from_real({2.0}), "Polynomial - int");
    CHECK_POLYNOMIAL(p * 2, oracle::from_real({8.0}), "Polynomial * int");
    CHECK_POLYNOMIAL(p / 2, oracle::from_real({2.0}), "Polynomial / int");
    CHECK_POLYNOMIAL(p + 2.5, oracle::from_real({6.5}), "Polynomial + double");
    CHECK_POLYNOMIAL(p - 2.5, oracle::from_real({1.5}), "Polynomial - double");
    CHECK_POLYNOMIAL(p * 2.5, oracle::from_real({10.0}), "Polynomial * double");
    CHECK_POLYNOMIAL(p / 2.5, oracle::from_real({1.6}), "Polynomial / double");
    CHECK_POLYNOMIAL(2 + p, oracle::from_real({6.0}), "int + Polynomial");
    CHECK_POLYNOMIAL(2 - p, oracle::from_real({-2.0}), "int - Polynomial");
    CHECK_POLYNOMIAL(2 * p, oracle::from_real({8.0}), "int * Polynomial");
    CHECK_POLYNOMIAL(2 / p, oracle::from_real({0.5}), "int / Polynomial");
    CHECK_POLYNOMIAL(2.5 + p, oracle::from_real({6.5}), "double + Polynomial");
    CHECK_POLYNOMIAL(2.5 - p, oracle::from_real({-1.5}), "double - Polynomial");
    CHECK_POLYNOMIAL(2.5 * p, oracle::from_real({10.0}), "double * Polynomial");
    CHECK_POLYNOMIAL(2.5 / p, oracle::from_real({0.625}), "double / Polynomial");

    const ComplexPoly cp(std::vector<double>{1.0}, std::vector<double>{1.0});
    const ComplexPoly cm(std::vector<double>{1.0}, std::vector<double>{-1.0});
    CHECK_COMPLEX(p + cp, opoly({{5.0, 1.0}}), "Polynomial + ComplexPoly");
    CHECK_COMPLEX(p - cp, opoly({{3.0, -1.0}}), "Polynomial - ComplexPoly");
    CHECK_COMPLEX(p * cp, opoly({{4.0, 4.0}}), "Polynomial * ComplexPoly");
    CHECK_COMPLEX(p / cp, opoly({{2.0, -2.0}}), "Polynomial / ComplexPoly");
    CHECK_COMPLEX(cp + p, opoly({{5.0, 1.0}}), "ComplexPoly + Polynomial");
    CHECK_COMPLEX(cp - p, opoly({{-3.0, 1.0}}), "ComplexPoly - Polynomial");
    CHECK_COMPLEX(cp * p, opoly({{4.0, 4.0}}), "ComplexPoly * Polynomial");
    CHECK_COMPLEX(cp / p, opoly({{0.25, 0.25}}), "ComplexPoly / Polynomial");
    CHECK_COMPLEX(cp + cm, opoly({{2.0, 0.0}}), "ComplexPoly + ComplexPoly");
    CHECK_COMPLEX(cp - cm, opoly({{0.0, 2.0}}), "ComplexPoly - ComplexPoly");
    CHECK_COMPLEX(cp * cm, opoly({{2.0, 0.0}}), "ComplexPoly * ComplexPoly");
    CHECK_COMPLEX(cp / cm, opoly({{0.0, 1.0}}), "ComplexPoly / ComplexPoly");

    const oracle::Rational r_oracle{oracle::from_real({2.0}), oracle::from_real({3.0})};
    const oracle::Rational s_oracle{oracle::from_real({4.0}), oracle::from_real({5.0})};
    const RationalComplex r = production_rational(r_oracle);
    const RationalComplex s = production_rational(s_oracle);
    CHECK_RATIONAL(r + s, oracle::rational_add(r_oracle, s_oracle),
                   "RationalComplex + RationalComplex");
    CHECK_RATIONAL(r - s, oracle::rational_subtract(r_oracle, s_oracle),
                   "RationalComplex - RationalComplex");
    CHECK_RATIONAL(r * s, oracle::rational_multiply(r_oracle, s_oracle),
                   "RationalComplex * RationalComplex");
    CHECK_RATIONAL(r / s, oracle::rational_divide(r_oracle, s_oracle),
                   "RationalComplex / RationalComplex");

    const RationalComplex rp = make_rational(p);
    const oracle::Rational op{oracle_from(p), {{1.0, 0.0}}};
    CHECK_RATIONAL(r + make_rational(p), oracle::rational_add(r_oracle, op),
                   "RationalComplex + make_rational(Polynomial)");
    CHECK_RATIONAL(r - make_rational(p), oracle::rational_subtract(r_oracle, op),
                   "RationalComplex - make_rational(Polynomial)");
    CHECK_RATIONAL(r * make_rational(p), oracle::rational_multiply(r_oracle, op),
                   "RationalComplex * make_rational(Polynomial)");
    CHECK_RATIONAL(r / make_rational(p), oracle::rational_divide(r_oracle, op),
                   "RationalComplex / make_rational(Polynomial)");
    CHECK_RATIONAL(rp + r, oracle::rational_add(op, r_oracle),
                   "make_rational(Polynomial) + RationalComplex");
    CHECK_RATIONAL(rp - r, oracle::rational_subtract(op, r_oracle),
                   "make_rational(Polynomial) - RationalComplex");
    CHECK_RATIONAL(rp * r, oracle::rational_multiply(op, r_oracle),
                   "make_rational(Polynomial) * RationalComplex");
    CHECK_RATIONAL(rp / r, oracle::rational_divide(op, r_oracle),
                   "make_rational(Polynomial) / RationalComplex");

    const RationalComplex rcp = make_rational(cp);
    const oracle::Rational ocp{oracle_from(cp), {{1.0, 0.0}}};
    CHECK_RATIONAL(r + make_rational(cp), oracle::rational_add(r_oracle, ocp),
                   "RationalComplex + make_rational(ComplexPoly)");
    CHECK_RATIONAL(r - make_rational(cp), oracle::rational_subtract(r_oracle, ocp),
                   "RationalComplex - make_rational(ComplexPoly)");
    CHECK_RATIONAL(r * make_rational(cp), oracle::rational_multiply(r_oracle, ocp),
                   "RationalComplex * make_rational(ComplexPoly)");
    CHECK_RATIONAL(r / make_rational(cp), oracle::rational_divide(r_oracle, ocp),
                   "RationalComplex / make_rational(ComplexPoly)");
    CHECK_RATIONAL(rcp + r, oracle::rational_add(ocp, r_oracle),
                   "make_rational(ComplexPoly) + RationalComplex");
    CHECK_RATIONAL(rcp - r, oracle::rational_subtract(ocp, r_oracle),
                   "make_rational(ComplexPoly) - RationalComplex");
    CHECK_RATIONAL(rcp * r, oracle::rational_multiply(ocp, r_oracle),
                   "make_rational(ComplexPoly) * RationalComplex");
    CHECK_RATIONAL(rcp / r, oracle::rational_divide(ocp, r_oracle),
                   "make_rational(ComplexPoly) / RationalComplex");
}

TEST_CASE(api, public_non_io_declaration_inventory) {
    CHECK((std::is_same_v<decltype(std::declval<const Polynomial &>().Scale(1.0)),
                          Polynomial>));
    CHECK((std::is_same_v<decltype(std::declval<const Polynomial &>().GetDegree()), int>));
    CHECK((std::is_same_v<decltype(std::declval<const Polynomial &>().GetCoeff(0)), double>));
    CHECK((std::is_same_v<decltype(std::declval<Polynomial &>().SetCoeffAt(0, 0.0)), void>));
    CHECK((std::is_same_v<decltype(std::declval<const ComplexPoly &>().GetCoeff(0)), double>));
    CHECK((std::is_same_v<decltype(std::declval<const ComplexPoly &>().getComplexCoeff(0)),
                          double>));
    CHECK((std::is_same_v<decltype(ComplexPoly::Undefined()), ComplexPoly>));
    CHECK((std::is_same_v<decltype(std::declval<const ComplexPoly &>().isUndefined()), bool>));
    CHECK((std::is_same_v<decltype(std::declval<const RationalComplex &>().GetNumerator()),
                          const ComplexPoly &>));
    CHECK((std::is_same_v<decltype(std::declval<const RationalComplex &>().GetDenominator()),
                          const ComplexPoly &>));
    std::cout << "[ API STATUS ] Polynomial public non-I/O declarations: DECLARED_AND_LINKED\n"
              << "[ API STATUS ] ComplexPoly arithmetic/getters: DECLARED_AND_LINKED\n"
              << "[ API STATUS ] ComplexPoly::Undefined(): DECLARED_BUT_UNRESOLVED "
                 "(isolated link probe)\n"
              << "[ API STATUS ] ComplexPoly::isUndefined() const: DECLARED_BUT_UNRESOLVED "
                 "(isolated link probe)\n"
              << "[ API STATUS ] RationalComplex public non-I/O declarations: "
                 "DECLARED_AND_LINKED\n";
}

TEST_CASE(promotion, value_preservation_and_chains) {
    const Polynomial polynomial(std::vector<double>{-1.25, 0.0, 2.5, 0.0, 0.0, -3.75});
    const auto expected = oracle_from(polynomial);
    const ComplexPoly complex(polynomial);
    const RationalComplex via_complex(complex);
    const RationalComplex via_polynomial_helper = make_rational(polynomial);
    const RationalComplex via_complex_helper = make_rational(complex);
    CHECK_COMPLEX(complex, expected, "Polynomial -> ComplexPoly preserves all coefficients");
    CHECK_RATIONAL(via_complex, orational(expected, opoly({{1.0, 0.0}})),
                   "ComplexPoly -> RationalComplex preserves value and denominator one");
    CHECK_RATIONAL(via_polynomial_helper, orational(expected, opoly({{1.0, 0.0}})),
                   "make_rational(Polynomial) preserves value");
    CHECK_RATIONAL(via_complex_helper, orational(expected, opoly({{1.0, 0.0}})),
                   "make_rational(ComplexPoly) preserves value");
    CHECK_RATIONAL_SEMANTICS(via_complex, oracle_from(via_polynomial_helper),
                             "equivalent Polynomial->ComplexPoly->RationalComplex chain");

    const Polynomial zero;
    CHECK_RATIONAL(make_rational(zero),
                   orational(opoly({{0.0, 0.0}}), opoly({{1.0, 0.0}})),
                   "zero promotion remains zero");
    const Polynomial fractional(std::vector<double>{0.125, -0.5, 0.0, 3.25});
    CHECK_RATIONAL(make_rational(fractional),
                   orational(oracle_from(fractional), opoly({{1.0, 0.0}})),
                   "fractional sparse promotion");

    const Polynomial tiny(5e-10);
    const RationalComplex promoted_tiny = make_rational(tiny);
    CHECK_RATIONAL(promoted_tiny,
                   orational(opoly({{5e-10, 0.0}}), opoly({{1.0, 0.0}})),
                   "nonzero tiny Polynomial promotion preserves value");
    const RationalComplex scale = make_rational(Polynomial(1e-8));
    CHECK_RATIONAL_SEMANTICS(
        promoted_tiny / scale,
        orational(opoly({{5e-10, 0.0}}), opoly({{1e-8, 0.0}})),
        "tiny promoted Polynomial divided by 1e-8 remains 0.05");

    const RationalComplex promoted = make_rational(polynomial);
    const RationalComplex offset = make_rational(Polynomial(std::vector<double>{2.0, 1.0}));
    CHECK_RATIONAL(promoted + offset,
                   oracle::rational_add(oracle_from(promoted), oracle_from(offset)),
                   "promoted value works in later arithmetic");
}

TEST_CASE(promotion, deterministic_generated_preservation) {
    std::mt19937 generator(kSeed);
    std::cout << "[ PROPERTY ] promotion seed=" << kSeed << " cases="
              << kGeneratedCases << " degrees=0..5 coefficients=-3..3\n";
    for (int iteration = 0; iteration < kGeneratedCases; ++iteration) {
        const auto real = random_real_oracle(generator);
        const Polynomial p = production_real(real);
        const ComplexPoly cp(p);
        const RationalComplex via_p = make_rational(p);
        const RationalComplex via_cp(cp);
        CHECK_COMPLEX(cp, real,
                      generated_context("Polynomial->ComplexPoly", iteration, real));
        CHECK_RATIONAL(via_p, orational(real, opoly({{1.0, 0.0}})),
                       generated_context("make_rational(Polynomial)", iteration, real));
        CHECK_RATIONAL(via_cp, orational(real, opoly({{1.0, 0.0}})),
                       generated_context("Polynomial->Complex->Rational", iteration, real));

        const auto complex = random_safe_complex_oracle(generator);
        const ComplexPoly random_cp = production_complex(complex);
        CHECK_RATIONAL(make_rational(random_cp),
                       orational(complex, opoly({{1.0, 0.0}})),
                       generated_context("make_rational(ComplexPoly)", iteration, complex));
    }
}

}  // namespace

int main(int argc, char **argv) {
    std::string group_filter;
    std::string test_filter;
    bool list_only = false;
    for (int index = 1; index < argc; ++index) {
        const std::string argument(argv[index]);
        if (argument == "--group" && index + 1 < argc) {
            group_filter = argv[++index];
        } else if (argument == "--test" && index + 1 < argc) {
            test_filter = argv[++index];
        } else if (argument == "--list") {
            list_only = true;
        } else {
            std::cerr << "Usage: " << argv[0]
                      << " [--group polynomial|complex|rational|promotion|api]"
                         " [--test group.name] [--list]\n";
            return 2;
        }
    }
    std::cout << "PolyEngineer independent native core verification\n"
              << "tolerance=" << std::setprecision(17) << kTolerance
              << " seed=" << kSeed << '\n';
    return coretest::run(group_filter, test_filter, list_only);
}
