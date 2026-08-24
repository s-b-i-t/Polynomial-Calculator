#include "Complex.h"
#include "Polynomial.h"
#include "RationalComplex.h"

#include <type_traits>
#include <utility>
#include <vector>

template <typename Left, typename Right, typename = void>
struct has_addition : std::false_type {};
template <typename Left, typename Right>
struct has_addition<Left, Right,
                    std::void_t<decltype(std::declval<Left>() +
                                         std::declval<Right>())>> : std::true_type {};

template <typename Left, typename Right, typename = void>
struct has_subtraction : std::false_type {};
template <typename Left, typename Right>
struct has_subtraction<Left, Right,
                       std::void_t<decltype(std::declval<Left>() -
                                            std::declval<Right>())>> : std::true_type {};

template <typename Left, typename Right, typename = void>
struct has_multiplication : std::false_type {};
template <typename Left, typename Right>
struct has_multiplication<Left, Right,
                          std::void_t<decltype(std::declval<Left>() *
                                               std::declval<Right>())>> : std::true_type {};

template <typename Left, typename Right, typename = void>
struct has_division : std::false_type {};
template <typename Left, typename Right>
struct has_division<Left, Right,
                    std::void_t<decltype(std::declval<Left>() /
                                         std::declval<Right>())>> : std::true_type {};

#define ASSERT_ALL_OPERATIONS(left, right)             \
    static_assert(has_addition<left, right>::value);    \
    static_assert(has_subtraction<left, right>::value); \
    static_assert(has_multiplication<left, right>::value); \
    static_assert(has_division<left, right>::value)

#define ASSERT_NO_OPERATIONS(left, right)                \
    static_assert(!has_addition<left, right>::value);     \
    static_assert(!has_subtraction<left, right>::value);  \
    static_assert(!has_multiplication<left, right>::value); \
    static_assert(!has_division<left, right>::value)

ASSERT_ALL_OPERATIONS(Polynomial, Polynomial);
ASSERT_ALL_OPERATIONS(Polynomial, int);
ASSERT_ALL_OPERATIONS(Polynomial, double);
ASSERT_ALL_OPERATIONS(int, Polynomial);
ASSERT_ALL_OPERATIONS(double, Polynomial);
ASSERT_ALL_OPERATIONS(Polynomial, ComplexPoly);
ASSERT_ALL_OPERATIONS(ComplexPoly, Polynomial);
ASSERT_ALL_OPERATIONS(ComplexPoly, ComplexPoly);
ASSERT_ALL_OPERATIONS(RationalComplex, RationalComplex);

ASSERT_NO_OPERATIONS(RationalComplex, Polynomial);
ASSERT_NO_OPERATIONS(Polynomial, RationalComplex);
ASSERT_NO_OPERATIONS(RationalComplex, ComplexPoly);
ASSERT_NO_OPERATIONS(ComplexPoly, RationalComplex);

static_assert(std::is_constructible_v<ComplexPoly, Polynomial>);
static_assert(!std::is_convertible_v<Polynomial, ComplexPoly>);
static_assert(std::is_constructible_v<ComplexPoly, std::vector<double>>);
static_assert(!std::is_convertible_v<std::vector<double>, ComplexPoly>);
static_assert(std::is_constructible_v<RationalComplex, ComplexPoly>);
static_assert(!std::is_convertible_v<ComplexPoly, RationalComplex>);
static_assert(!std::is_constructible_v<RationalComplex, Polynomial>);
static_assert(!std::is_convertible_v<Polynomial, RationalComplex>);
static_assert(std::is_same_v<decltype(make_rational(std::declval<const Polynomial &>())),
                             RationalComplex>);
static_assert(std::is_same_v<decltype(make_rational(std::declval<const ComplexPoly &>())),
                             RationalComplex>);

int main() {
    return 0;
}
