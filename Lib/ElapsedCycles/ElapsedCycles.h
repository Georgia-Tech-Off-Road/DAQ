/*	Codefrom user mlu on https://forum.pjrc.com/threads/29463-A-simple-class-for-sub-microsecond-timing
 * 
 *
 *
 *
 *
 *
 *
 *
 */


#ifndef ElapsedCycles_h
#define ElapsedCycles_h
#ifdef __cplusplus

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/* Class for CPU clycle resolution timing */
/* Conversions between cycles and nanosecons are safe up to 100 mS */

class ElapsedCycles
{
private:
	unsigned long cyccount;
public:
	ElapsedCycles(void) { ARM_DEMCR |= ARM_DEMCR_TRCENA; ARM_DWT_CTRL |= 1; cyccount = ARM_DWT_CYCCNT; }
	ElapsedCycles(unsigned long val) { ARM_DWT_CTRL |= 1; cyccount = ARM_DWT_CYCCNT - val; }
	ElapsedCycles(const ElapsedCycles &orig) { ARM_DWT_CTRL |= 1; cyccount = orig.cyccount; }
	operator unsigned long () const { return ARM_DWT_CYCCNT - cyccount; }
	ElapsedCycles & operator = (const ElapsedCycles &rhs) { cyccount = rhs.cyccount; return *this; }
	ElapsedCycles & operator = (unsigned long val) { cyccount = ARM_DWT_CYCCNT - val; return *this; }
	ElapsedCycles & operator -= (unsigned long val)      { cyccount += val ; return *this; }
	ElapsedCycles & operator += (unsigned long val)      { cyccount -= val ; return *this; }
	ElapsedCycles operator - (int val) const           { ElapsedCycles r(*this); r.cyccount += val; return r; }
	ElapsedCycles operator - (unsigned int val) const  { ElapsedCycles r(*this); r.cyccount += val; return r; }
	ElapsedCycles operator - (long val) const          { ElapsedCycles r(*this); r.cyccount += val; return r; }
	ElapsedCycles operator - (unsigned long val) const { ElapsedCycles r(*this); r.cyccount += val; return r; }
	ElapsedCycles operator + (int val) const           { ElapsedCycles r(*this); r.cyccount -= val; return r; }
	ElapsedCycles operator + (unsigned int val) const  { ElapsedCycles r(*this); r.cyccount -= val; return r; }
	ElapsedCycles operator + (long val) const          { ElapsedCycles r(*this); r.cyccount -= val; return r; }
	ElapsedCycles operator + (unsigned long val) const { ElapsedCycles r(*this); r.cyccount -= val; return r; }
	static unsigned long ns2cycles(unsigned long ns) {return (ns*(F_CPU/8000000UL))/125UL; }
	static unsigned long cycles2ns(unsigned long cycles) {return (cycles*125UL)/(F_CPU/8000000UL); }
};

#endif // __cplusplus
#endif // ElapsedCycles_h