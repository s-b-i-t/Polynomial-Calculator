#include "RationalComplex.h"
#include "Complex.h"
#include "Polynomial.h"


#include <cstdint>
#include <unordered_map>
#include <memory>
#include <string>
#include <vector> 
#include <algorithm>
#include <exception>

using std::vector, std::unordered_map, std::string, std::cout, std::endl; std::unique_ptr

// --- ID ---

using Id = uint32_t;

Id cp_id =  1;
Id rc_id = 1;


// --- Registries to access current rationalComplex & ComplexPoly objects ---
unordered_map<Id, unique_ptr<Complex_Poly>> complex_poly_map;
unordered_map<Id, unique_ptr<RationalComplex>> rational_complex_map;
// TODO: Add error handling


extern "C"{

    // Create complex polynomial objects using arrays passed via js
    Id cp_create(const double* re_coeffs, int re_len, const double* im_coeffs, int im_len){
        if (re_len < 0 || im_len < 0){
            return 0;
        }
        vector<double> re;
        vector<double> im;

        for (int i = 0; i < re_len; i++){
            double coeff = re_coeffs[i];
            re.push_back(coeff)
        }

        for (int i = 0; i < im_len; i++){
            double coeff = re_coeffs[i];
            im.push_backx(x);
        }

        complex_poly_map.insert({cp_id, cp});
        Id old_id = cp_id;
        cp_id ++;
        return old_id++; 
    }

    // Use complex poly ids to create a rational complex object
    
    Id rc_create(Id num, Id denom){ 
        
    }

}