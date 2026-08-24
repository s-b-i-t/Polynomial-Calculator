#ifndef POLYCALC_TEST_HARNESS_H
#define POLYCALC_TEST_HARNESS_H

#include <cmath>
#include <exception>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>

namespace coretest {

constexpr double kDefaultTolerance = 1e-9;

struct TestCase {
    const char *group;
    const char *name;
    void (*function)();
};

inline std::vector<TestCase> &registry() {
    static std::vector<TestCase> tests;
    return tests;
}

struct Registrar {
    Registrar(const char *group, const char *name, void (*function)()) {
        registry().push_back({group, name, function});
    }
};

struct Stats {
    std::size_t checks_passed = 0;
    std::size_t checks_failed = 0;
    std::size_t tests_passed = 0;
    std::size_t tests_failed = 0;
    std::string current_test;
};

inline Stats &stats() {
    static Stats value;
    return value;
}

template <typename T>
std::string format_value(const T &value) {
    std::ostringstream out;
    out << std::boolalpha << std::setprecision(17) << value;
    return out.str();
}

inline void record(bool passed,
                   const char *expression,
                   const char *file,
                   int line,
                   const std::string &expected = {},
                   const std::string &actual = {}) {
    if (passed) {
        ++stats().checks_passed;
        return;
    }

    ++stats().checks_failed;
    std::cerr << "[FAIL] " << stats().current_test << "\n"
              << "  at " << file << ':' << line << "\n"
              << "  check: " << expression << '\n';
    if (!expected.empty()) {
        std::cerr << "  expected: " << expected << '\n';
    }
    if (!actual.empty()) {
        std::cerr << "  actual:   " << actual << '\n';
    }
}

template <typename Actual, typename Expected>
void check_equal(const Actual &actual,
                 const Expected &expected,
                 const char *expression,
                 const char *file,
                 int line) {
    record(actual == expected, expression, file, line,
           format_value(expected), format_value(actual));
}

inline void check_near(double actual,
                       double expected,
                       double tolerance,
                       const char *expression,
                       const char *file,
                       int line) {
    const bool finite = std::isfinite(actual) && std::isfinite(expected) &&
                        std::isfinite(tolerance) && tolerance >= 0.0;
    const bool close = finite && std::abs(actual - expected) <= tolerance;
    std::ostringstream expected_text;
    expected_text << std::setprecision(17) << expected << " +/- " << tolerance
                  << " (finite required)";
    record(close, expression, file, line, expected_text.str(), format_value(actual));
}

inline bool selected(const TestCase &test,
                     const std::string &group_filter,
                     const std::string &test_filter) {
    if (!group_filter.empty() && group_filter != test.group) {
        return false;
    }
    const std::string full_name = std::string(test.group) + "." + test.name;
    return test_filter.empty() || test_filter == full_name || test_filter == test.name;
}

inline int run(const std::string &group_filter,
               const std::string &test_filter,
               bool list_only) {
    std::size_t selected_count = 0;
    for (const TestCase &test : registry()) {
        if (!selected(test, group_filter, test_filter)) {
            continue;
        }
        ++selected_count;
        const std::string full_name = std::string(test.group) + "." + test.name;
        if (list_only) {
            std::cout << full_name << '\n';
            continue;
        }

        stats().current_test = full_name;
        const std::size_t failures_before = stats().checks_failed;
        std::cout << "[ RUN      ] " << full_name << '\n';
        try {
            test.function();
        } catch (const std::exception &error) {
            record(false, "unexpected exception", __FILE__, __LINE__,
                   "no exception",
                   std::string(typeid(error).name()) + ": " + error.what());
        } catch (...) {
            record(false, "unexpected exception", __FILE__, __LINE__,
                   "no exception", "non-std exception");
        }

        if (stats().checks_failed == failures_before) {
            ++stats().tests_passed;
            std::cout << "[       OK ] " << full_name << '\n';
        } else {
            ++stats().tests_failed;
            std::cout << "[  FAILED  ] " << full_name << '\n';
        }
    }

    if (list_only) {
        return selected_count == 0 ? 2 : 0;
    }
    if (selected_count == 0) {
        std::cerr << "No tests selected.\n";
        return 2;
    }

    std::cout << "[ SUMMARY  ] tests_passed=" << stats().tests_passed
              << " tests_failed=" << stats().tests_failed
              << " checks_passed=" << stats().checks_passed
              << " checks_failed=" << stats().checks_failed << '\n';
    return stats().checks_failed == 0 ? 0 : 1;
}

}  // namespace coretest

#define CORETEST_JOIN_IMPL(a, b) a##b
#define CORETEST_JOIN(a, b) CORETEST_JOIN_IMPL(a, b)

#define TEST_CASE(group, name)                                                   \
    static void CORETEST_JOIN(test_, CORETEST_JOIN(group, CORETEST_JOIN(_, name)))(); \
    static ::coretest::Registrar                                                 \
        CORETEST_JOIN(registrar_, CORETEST_JOIN(group, CORETEST_JOIN(_, name)))( \
            #group, #name,                                                       \
            &CORETEST_JOIN(test_, CORETEST_JOIN(group, CORETEST_JOIN(_, name)))); \
    static void CORETEST_JOIN(test_, CORETEST_JOIN(group, CORETEST_JOIN(_, name)))()

#define CHECK(expression)                                                        \
    do {                                                                         \
        ::coretest::record(static_cast<bool>(expression), #expression, __FILE__, \
                           __LINE__, "true",                                     \
                           ::coretest::format_value(static_cast<bool>(expression))); \
    } while (false)

#define CHECK_EQ(actual, expected)                                               \
    do {                                                                         \
        const auto &coretest_actual = (actual);                                  \
        const auto &coretest_expected = (expected);                              \
        ::coretest::check_equal(coretest_actual, coretest_expected,              \
                                #actual " == " #expected, __FILE__, __LINE__);   \
    } while (false)

#define CHECK_NEAR(actual, expected, tolerance)                                  \
    do {                                                                         \
        const double coretest_actual = static_cast<double>(actual);              \
        const double coretest_expected = static_cast<double>(expected);          \
        const double coretest_tolerance = static_cast<double>(tolerance);        \
        ::coretest::check_near(coretest_actual, coretest_expected,               \
                               coretest_tolerance,                               \
                               #actual " ~= " #expected, __FILE__, __LINE__);    \
    } while (false)

#define CHECK_THROWS(expression, exception_type)                                 \
    do {                                                                         \
        bool coretest_caught_expected = false;                                   \
        bool coretest_caught_other = false;                                      \
        std::string coretest_other;                                              \
        try {                                                                    \
            (void)(expression);                                                  \
        } catch (const exception_type &) {                                       \
            coretest_caught_expected = true;                                     \
        } catch (const std::exception &coretest_error) {                         \
            coretest_caught_other = true;                                        \
            coretest_other = std::string(typeid(coretest_error).name()) +        \
                             ": " + coretest_error.what();                       \
        } catch (...) {                                                          \
            coretest_caught_other = true;                                        \
            coretest_other = "non-std exception";                               \
        }                                                                        \
        ::coretest::record(coretest_caught_expected && !coretest_caught_other,   \
                           #expression " throws " #exception_type, __FILE__,     \
                           __LINE__, #exception_type,                            \
                           coretest_caught_expected ? #exception_type            \
                                                    : (coretest_caught_other     \
                                                           ? coretest_other      \
                                                           : "no exception"));   \
    } while (false)

#define CHECK_NO_THROW(expression)                                               \
    do {                                                                         \
        bool coretest_threw = false;                                             \
        std::string coretest_exception;                                          \
        try {                                                                    \
            (void)(expression);                                                  \
        } catch (const std::exception &coretest_error) {                         \
            coretest_threw = true;                                               \
            coretest_exception = std::string(typeid(coretest_error).name()) +    \
                                 ": " + coretest_error.what();                   \
        } catch (...) {                                                          \
            coretest_threw = true;                                               \
            coretest_exception = "non-std exception";                           \
        }                                                                        \
        ::coretest::record(!coretest_threw, #expression " does not throw",      \
                           __FILE__, __LINE__, "no exception",                  \
                           coretest_threw ? coretest_exception : "no exception"); \
    } while (false)

#endif
