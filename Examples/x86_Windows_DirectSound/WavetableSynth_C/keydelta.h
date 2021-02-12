#ifndef __SYNTH_KEY_DELTA__
#define __SYNTH_KEY_DELTA__

#include "synthtable.h"

#if defined(__cplusplus) || defined(c_plusplus) //跨平台定义方法
extern "C" {
#endif

	//typedef uint32 keydelta_t;
	typedef unsigned long keydelta_t;

	//typedef uint8 keynum_t;
	typedef unsigned char keynum_t;

	#define SYNTH_KEY_OFF 0xff

	#define ADD_KEY_DELTA(a,key) \
	{ \
		a += key_delta[key]; \
		a &= SYNTH_TABLE_SIZE_MASK; \
	}

	extern keydelta_t key_delta[88];


#if defined(__cplusplus) || defined(c_plusplus)
}
#endif 

#endif
