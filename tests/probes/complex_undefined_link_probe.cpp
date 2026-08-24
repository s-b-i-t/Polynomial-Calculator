#include "Complex.h"

int main() {
    const ComplexPoly value = ComplexPoly::Undefined();
    return value.GetCoeff(0) == 0.0 ? 0 : 1;
}
