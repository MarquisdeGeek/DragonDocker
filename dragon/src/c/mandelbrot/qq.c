/* Project: Q-Number (Q16.16, signed) library
 * Author:  Richard James Howe
 * License: The Unlicense
 * Email:   howe.r.j.89@gmail.com
 * Repo:    <https://github.com/q> 
 *
 *
 * A Q32.32 version would be useful. 
 *
 * The following should be changed/done for this library:
 *
 * - Moving towards a header-only model.
 * - Removal of dependencies such as 'isalpha', 'tolower'
 *   as they are locale dependent.
 * - Make components optional (filters, expression parser, ...)
 * - Make hyperbolic arc sin/cos/tan functions.
 * - Fix bugs / inaccuracies in CORDIC code.
 * - Improve accuracy of all the functions and quantify error and
 *   their limits. 
 *
 * BUG: Enter: 2.71791, get 2.0625, 2.7179 works fine. (Need to
 * limit decimal places).
 */

#include "qq.h"

#define CHAR_BIT 8
//

q_t qqintfract(const int toq, const int fractional, const int fractionDivisor) {
	q_t value = qqint(toq);
	q_t fract = qqint(fractional);
	q_t divsr = qqint(fractionDivisor);

	qqdiv(&fract, &divsr);

	if (toq < 0) { // if we have -1 and 1/10 we expect -1.1 and not -1 + 0.1 = -0.9
		qqsub(&value, &fract);
	} else {
		qqadd(&value, &fract);
	}

	return value;
}


#if USE_MACROS == 0


q_t qqint(const int toq)                 { return ((u_t) ((d_t)toq) ) << QBITS; }

q_t qqispositive(const q_t a)            { return !(a & QQHIGH); }


void qqadd(q_t* pa, const q_t* pb) { *pa += *pb; }
void qqsub(q_t* pa, const q_t* pb) { *pa -= *pb; }


void qqmul(q_t* pa, const q_t* pb) {	
// FIXP)( ((FIXP_BIG)(__a) * (__b)) >> FIX_FRACBITS )

	// + qhigh is, I think, to add 0.5 and so round up
	const ld_t dd = ((ld_t)*pa * (ld_t)*pb) + (lu_t)QHIGH;

	// TODO: Reinstate the original code
	*pa = (q_t)(dd >> QBITS);
	return;
	/* N.B. portable version of "dd >> QBITS", for double width signed values */
	if (dd < 0) {
		// Originally, qbits was 16 - making the shift 32
		// *pa = (-1ull << (2 * QBITS)) | ((lu_t)dd >> QBITS);
		*pa = (-1ull << (sizeof(q_t)*8)) | ((lu_t)dd >> QBITS);
		
	} else {
		*pa = dd >> QBITS;
	}
}

#endif

// NOTE: Only used once by mandelbrot code
d_t qqdivn(const d_t v, const unsigned p) {
	/* return v / (1l << p); */
	const u_t shifted = ((u_t)v) >> p;
	if (qqispositive(v))
		return shifted;
    // << ((4*8) - 1 - 1)
    // << 32 - 1 - 1 = 30, i.e. fill the two MSB with 1 - although I think only 1 of these is needed, as
    // -128 gets shifted to 64, so 64 is already set
	const u_t leading = ((u_t)(-1l)) << ((sizeof(v)*CHAR_BIT) - p - 1);
	return leading | shifted;
}

q_t qqdiv(q_t* a, const q_t* b) {
	const ld_t dd = ((ld_t)*a) << QBITS;
	ld_t bd2 = *b/2;//qqdivn(*b, 1);
	if (!((dd >= 0 && *b > 0) || (dd < 0 && *b < 0)))
		bd2 = -bd2;
	/* Overflow not checked! */
	/*return (dd/b) + (bd2/b);*/
	*a = (q_t)((dd + bd2) / *b);
	return *a;
}

