#include <vector>
#include "RationalComplex.h"
#include "Complex.h"
#include "Polynomial.h"



extern "C"{
    double wasm_add_double(double a, double b){
        return a + b;
    }
}