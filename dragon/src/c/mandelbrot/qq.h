/* Project: Q-Number (Q16.16, signed) library
 * Author:  Richard James Howe
 * License: The Unlicense
 * Email:   howe.r.j.89@gmail.com
 * Repo:    <https://github.com/q> 
 * 
 * 
 * This version: A complete hachete job on the original. Sorry!
 * I took the original q functions, and renamed them to qq, to try
 * and get some speed on the 8-bit machines by using pass-by-pointer
 * not value, and using the 1st arg as the destimation/result.
 * */

#ifndef MANDEL_QQ_H
#define MANDEL_QQ_H

#ifdef __cplusplus
extern "C" {
#endif


typedef short int16_t;
typedef long int32_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;


// Hi-precision mode uses some fixed-point shortcuts. Unless you're au fair with FP the assumptions made
// made appear opaque. (And certainly won't match the maths in wikipedia!)
#if HIPRECISION
// Magic numbers!
// qbits: 256, 192
// 12   : 16,  21
// 11   :  8   11
// 10   :  4    5  <-- more bits than this and weird fringing occurs. Might be bug? Might be a limit?
//  9   :  2    3
//  8   :  1    1
#define QBITS       (10)

#if QBITS == 12
#define QBITS_RECIPROCAL_256       (16)
#define QBITS_RECIPROCAL_192       (21)

#elif QBITS == 11
#define QBITS_RECIPROCAL_256       (8)
#define QBITS_RECIPROCAL_192       (11)

#elif QBITS == 10
#define QBITS_RECIPROCAL_256       (4)
#define QBITS_RECIPROCAL_192       (5)

#elif QBITS == 9
#define QBITS_RECIPROCAL_256       (2)
#define QBITS_RECIPROCAL_192       (3)

#elif QBITS == 8
#define QBITS_RECIPROCAL_256       (1)
#define QBITS_RECIPROCAL_192       (1)

#endif

#else
// For 16 bit calculations, a screen width of 256 requires this to be no larger than 6
#define QBITS       (6)
#endif


#if 0
#include <stdint.h>
#include <stddef.h>

typedef int32_t  q_t; /* Q Fixed Point Number, (Q16.16, Signed) */
typedef int64_t ld_t; /* Double width of Q, signed, for internal calculations, not in Q format */
typedef int32_t  d_t; /* same width as Q,  signed, but not in Q format */
typedef uint32_t u_t; /* same width as Q, unsigned, but not in Q format */

// typedef  int16_t hd_t; /* half Q width,      signed */
typedef uint64_t lu_t; /* double Q width,  unsigned */

#else

typedef int16_t  q_t; /* Q Fixed Point Number, (Q16.16, Signed) */
typedef int32_t ld_t; /* Double width of Q, signed, for internal calculations, not in Q format */
typedef int16_t  d_t; /* same width as Q,  signed, but not in Q format */
typedef uint16_t u_t; /* same width as Q, unsigned, but not in Q format */

// typedef  int16_t hd_t; /* half Q width,      signed */
typedef uint32_t lu_t; /* double Q width,  unsigned */

#endif


#define QHIGH       (1ULL << (QBITS  - 1ULL))
// MSB, i.e. sign bit
#define QQHIGH      (1ULL << (sizeof(q_t)*8  - 1ULL))
#define QQMASK_INTEGER      (~((1<<QBITS)-1))       // isolates the integer, i.e. int(q) is q &= QQMASK_INTEGER
#define QQMASK_FRACTION     (((1<<QBITS)-1))        // isolates the fraction, i.e. fract(q) is q &= QQMASK_FRACTION

// The Code:

#if USE_MACROS

#define qqint(toq)         (((u_t)((d_t)toq)) << QBITS )
#define qqispositive(a)    (!(a & QQHIGH))
q_t qqintfract(const int toq, const int fractional, const int fractionDivisor);

#define qqadd(pa, pb) (*pa += *pb)
#define qqsub(pa, pb) (*pa -= *pb)
#define qqmul(pa, pb) (*pa=((q_t)( ((ld_t)(((ld_t)*pa * (ld_t)*pb) + (lu_t)QHIGH)) >> QBITS)))


q_t qqdiv(q_t* a, const q_t* b);

#else


q_t qqint(const int toq);
q_t qqintfract(const int toq, const int fractional, const int fractionDivisor);
q_t qqispositive(const q_t a);

void qqadd(q_t* pa, const q_t* pb);
void qqsub(q_t* pa, const q_t* pb);
void qqmul(q_t* pa, const q_t* pb);
q_t qqdiv(q_t* a, const q_t* b);


#endif



#ifdef __cplusplus
}
#endif
#endif
