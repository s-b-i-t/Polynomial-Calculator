/*
 CHANGELOG (Codex robustness pass)
 - Restore legacy dense list accessors (`getListCoeffsIn`, `getcomplexCoeffsList`) as bounded adapters.
 - Add `NearlyEquals` and centralize EPS trimming semantics.
 - Keep sparse storage as the canonical truth and avoid read-path map mutation.
 - Keep complex long division (no conjugate trick) and deterministic Dump formatting.
 */

#ifndef ComplexPoly_h
#define ComplexPoly_h

#include "Polynomial.h"

#include <complex>
#include <initializer_list>
#include <map>
#include <stdexcept>
#include <utility>
#include <vector>

class ComplexPoly
{
public:
    static constexpr double EPS = 1e-10;
    static constexpr int DENSE_EXPORT_MAX_EXPONENT = 10000;

    ComplexPoly() = default;
    ComplexPoly(const ComplexPoly &cp) = default;

    explicit ComplexPoly(const Polynomial &poly);

    // Construct from dense real/imag arrays (index = exponent).
    ComplexPoly(const std::vector<double> &Polypart, const std::vector<double> &ComplexPart);

    // Construct from dense real coefficients (imag=0).
    ComplexPoly(const std::initializer_list<double> &Polypart);

    bool IsZero() const { return terms.empty(); }
    int GetDegree() const;

    std::pair<int, std::complex<double>> LeadingTerm() const;

    ComplexPoly Scale(const std::complex<double> &factor) const;

    ComplexPoly operator+(const Polynomial &rhs) const;
    ComplexPoly operator-(const Polynomial &rhs) const;
    ComplexPoly operator*(const Polynomial &rhs) const;
    ComplexPoly operator/(const Polynomial &rhs) const;

    ComplexPoly operator+(const ComplexPoly &rhs) const;
    ComplexPoly operator-(const ComplexPoly &rhs) const;
    ComplexPoly operator*(const ComplexPoly &rhs) const;
    ComplexPoly operator/(const ComplexPoly &rhs) const;

    void Dump() const;

    // Real coefficient for x^index (0 if missing).
    double GetCoeff(int index) const;

    // Imag coefficient for x^index (0 if missing).
    double getComplexCoeff(int index) const;

    // Legacy adapters: dense coefficient lists (index = exponent).
    // These are bounded to prevent huge allocations; for high degrees, they are truncated.
    const std::vector<double> &getListCoeffsIn() const;
    const std::vector<double> &getcomplexCoeffsList() const;
    std::vector<double> getListCoeffsIn(int maxExponent) const;
    std::vector<double> getcomplexCoeffsList(int maxExponent) const;

    bool NearlyEquals(const ComplexPoly &rhs, double eps = EPS) const;

    static std::pair<ComplexPoly, ComplexPoly> DivMod(const ComplexPoly &a, const ComplexPoly &b);

    static ComplexPoly Undefined();
    bool isUndefined() const { return undefined; }

private:
    static bool NearlyZero(double x) { return std::abs(x) < EPS; }
    static bool NearlyZero(const std::complex<double> &z)
    {
        return NearlyZero(z.real()) && NearlyZero(z.imag());
    }

    void Normalize();
    std::vector<double> ToDenseRealBounded(int maxExponent) const;
    std::vector<double> ToDenseImagBounded(int maxExponent) const;

    // Sparse terms: exponent -> coefficient
    // Invariants:
    // - coefficients satisfy abs(re) >= EPS OR abs(im) >= EPS
    // - zero polynomial has empty map
    std::map<int, std::complex<double>> terms;

    // Simple sentinel for "undefined" state (kept for API compatibility).
    bool undefined = false;

    mutable std::vector<double> dense_real_cache;
    mutable std::vector<double> dense_imag_cache;
};

inline ComplexPoly operator+(const Polynomial &lhs, const ComplexPoly &rhs) { return rhs + lhs; }
inline ComplexPoly operator-(const Polynomial &lhs, const ComplexPoly &rhs) { return ComplexPoly(lhs) - rhs; }
inline ComplexPoly operator*(const Polynomial &lhs, const ComplexPoly &rhs) { return rhs * lhs; }
inline ComplexPoly operator/(const Polynomial &lhs, const ComplexPoly &rhs)
{
    ComplexPoly lhs_cp(lhs);
    return lhs_cp / rhs;
}

#endif
