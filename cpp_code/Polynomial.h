    /*
     CHANGELOG (Codex robustness pass)
     - Keep sparse `std::map<int,double>` as the canonical representation.
     - Restore legacy dense accessors (`getListCoeffsIn`) as bounded adapters (never canonical).
     - Centralize EPS trimming and add `NearlyEquals` for tolerance-aware comparisons.
     - Keep division/remainder side-effect-free via `DivMod` and expose consistent invariants.
     */

    #ifndef Polynomial_h
    #define Polynomial_h

    #include <cmath>
    #include <iostream>
    #include <map>
    #include <stdexcept>
    #include <utility>
    #include <vector>

    // *****************************************************************
    // Represent a polynomial of a single variable, x, and
    // support common operations on polynomial: add, scaling, multiple, and division

    class Polynomial
    {
    public:
        static constexpr double EPS = 1e-10;
        // Legacy dense adapters are bounded to prevent huge allocations on sparse high-degree polynomials.
        // If degree exceeds this cap, exported vectors are truncated to [0..DENSE_EXPORT_MAX_EXPONENT].
        static constexpr int DENSE_EXPORT_MAX_EXPONENT = 10000;

        Polynomial() = default;
        Polynomial(int coeff);
        Polynomial(double coeff);
        Polynomial(const std::vector<double> &InputVec);

        // Copy/assign
        Polynomial(const Polynomial &rhs) = default;
        Polynomial &operator=(const Polynomial &rhs) = default;

        bool IsZero() const { return terms.empty(); }

        // Scale by a constant and return resulting NEW polynomial. For example, if polynomial is 1+3x, and
        // factor = 2, the result is 2+6x
        Polynomial Scale(double factor) const;

        // Add a polynomial to the current polynomial (and return the result). Example: (1+2x) + (2x+3x^2) = 1+4x+3x^2
        Polynomial operator+(const Polynomial &rhs) const;
        Polynomial operator-(const Polynomial &rhs) const;

        // Multiply a polynomial by another polynomial and return the result. Example: (1+2x)*(1-x^2) = 1 + 2x - x^2 -2x^3
        Polynomial operator*(const Polynomial &rhs) const;

        // Divide a polynomial by another and return the quotient (remainder via operator%).
        Polynomial operator/(const Polynomial &rhs) const;
        Polynomial operator%(const Polynomial &rhs) const;

        // Compound operators
        Polynomial &operator+=(const Polynomial &rhs);
        Polynomial &operator-=(const Polynomial &rhs);
        Polynomial &operator*=(const Polynomial &rhs);
        Polynomial &operator/=(const Polynomial &rhs);
        Polynomial &operator%=(const Polynomial &rhs);

        bool operator==(const Polynomial &rhs) const;
        bool operator!=(const Polynomial &rhs) const { return !(*this == rhs); }
        bool NearlyEquals(const Polynomial &rhs, double eps = EPS) const;

        // Get the degree. Example: if polynomial=1+x+3x^3, degree=3
        // Degree of the zero polynomial is -1.
        int GetDegree() const;

        // Return coefficient for x^exponent; 0 if missing or exponent < 0.
        double GetCoeff(int exponent) const;

        // Set coefficient for x^exponent. Removes term if abs(value) < EPS.
        void SetCoeffAt(int exponent, double value);

        // Deterministic debug print.
        void Dump() const;

        // Legacy adapter: dense coefficient list (index = exponent).
        // NOTE: This is an adapter for compatibility only; it never changes sparse storage.
        // For huge degrees, it returns a truncated vector up to DENSE_EXPORT_MAX_EXPONENT.
        const std::vector<double> &getListCoeffsIn() const;
        std::vector<double> getListCoeffsIn(int maxExponent) const;

        // Long division. Throws std::invalid_argument on division by 0 polynomial.
        static std::pair<Polynomial, Polynomial> DivMod(const Polynomial &a, const Polynomial &b);

        // Expose terms for interop (read-only).
        const std::map<int, double> &GetTerms() const { return terms; }

        // (deg, coeff) for leading term. Undefined for zero polynomial (throws).
        std::pair<int, double> LeadingTerm() const;

    private:
        static bool NearlyZero(double x) { return std::abs(x) < EPS; }
        void Normalize();
        std::vector<double> ToDenseBounded(int maxExponent) const;

        // Sparse terms: exponent -> coefficient.
        // Invariants:
        // - externally-visible objects are normalized: abs(coeff) >= EPS for any stored term
        // - zero polynomial has empty map
        std::map<int, double> terms;

        // Cached dense adapter view.
        mutable std::vector<double> dense_cache;
    };

    // turn num * poly to poly * num
    Polynomial operator+(double lhs, const Polynomial &rhs);
    Polynomial operator+(int lhs, const Polynomial &rhs);
    Polynomial operator-(double lhs, const Polynomial &rhs);
    Polynomial operator-(int lhs, const Polynomial &rhs);
    Polynomial operator*(double lhs, const Polynomial &rhs);
    Polynomial operator*(int lhs, const Polynomial &rhs);
    Polynomial operator/(double lhs, const Polynomial &rhs);
    Polynomial operator/(int lhs, const Polynomial &rhs);

    #endif
