/** @file Misc.h */
//
// Copyright 2020 Arvind Singh
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
//version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <stdint.h>
#include <math.h>



#if defined(TEENSYDUINO) || defined(ESP32)

// running on a MCU 

#include <Arduino.h>
#define TGX_ON_ARDUINO
#define TGX_INLINE __attribute__((always_inline))


#if defined(ARDUINO_TEENSY41)

// check existence of external ram (EXTMEM). 
extern "C" uint8_t external_psram_size;

// check is an address is in flash
#define TGX_IS_PROGMEM(X)  ((((uint32_t)(X)) >= 0x60000000)&&(((uint32_t)(X)) < 0x70000000))

// check if an address is in external ram
#define TGX_IS_EXTMEM(X) ((((uint32_t)(X)) >= 0x70000000)&&(((uint32_t)(X)) < 0x80000000))


#endif


#else 

// running on a CPU ? 

#define TGX_INLINE

#ifndef PROGMEM
#define PROGMEM
#endif

#endif 



namespace tgx
{


    /** 
    * Dummy type identified by an integer. 
    * Use to bypass partial template specialization
    * by using method overloading instead....
    **/
    template<int N> struct DummyType
        {
        // nothing here :-)
        };


    /**
    * Dummy type identified by two booleans
    * Use to bypass partial template specialization
    * by using method overloading instead....
    **/
    template<bool BB1, bool BB2> struct DummyTypeBB
        {
        // nothing here :-)
        };



/* Set this to 1 to use float as the default floating point type 
   and set it to 0 to use double precision instead. */
#ifndef TGX_SINGLE_PRECISION_COMPUTATIONS
#define TGX_SINGLE_PRECISION_COMPUTATIONS 1  
#endif


    /** sets the default floating point type for computations */
    template<typename T = int> struct DefaultFPType
        {
#if TGX_SINGLE_PRECISION_COMPUTATIONS
        typedef float fptype;   // use float as default floating point type
#else
        typedef double fptype;  // use double as default floating point type
#endif
        };

#if TGX_SINGLE_PRECISION_COMPUTATIONS
    /** ... but when already using double, keep using double...*/
    template<> struct DefaultFPType<double>
        {
        typedef double fptype;
        };
#endif



    /** baby let me swap you one more time... */
    template<typename T> TGX_INLINE inline void swap(T& a, T& b) { T c(a); a = b; b = c; }


    /** don't know why but much faster than fminf for floats.. */
    template<typename T> TGX_INLINE inline T min(const T & a, const T & b) { return((a < b) ? a : b); }

    
    /** don't know why but much faster than fmaxf for floats.. */
    template<typename T> TGX_INLINE inline T max(const T & a, const T & b) { return((a > b) ? a : b); }

        
    /** template clamp version */
    template<typename T> TGX_INLINE inline T clamp(const T & v, const T & vmin, const T & vmax)
        {
        return max(vmin, min(vmax, v));
        }


    /** rounding for floats */
    TGX_INLINE inline float roundfp(const float f) { return roundf(f);  }
    

    /** ...and rounding for doubles with the same name */
    TGX_INLINE inline double roundfp(const double f) { return round(f); }



    /**
     * Return a value smaller or equal to B such that the multiplication
     * by A is safe (no overflow with int32).
     **/
    TGX_INLINE inline int32_t safeMultB(int32_t A, int32_t B)
        {
        if ((A == 0) || (B == 0)) return B;
        const int32_t max32 = 2147483647;
        const int32_t nB = max32 / ((A > 0) ? A : (-A));
        return ((B <= nB) ? B : nB);
        }

}

/** end of file */
