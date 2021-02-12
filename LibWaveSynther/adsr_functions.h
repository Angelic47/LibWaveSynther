#ifndef ADSR_FUNCTIONS
#define  ADSR_FUNCTIONS

#ifdef HIGH_QUALITY_ENABLE

#include <stdio.h>
#include <math.h>

struct _proportional_function_args //y=ax
{
	float a;
};

struct _exp_function_args //y=aExp(bx+d)+c
{
	float a, b, c, d;
};

union _function_args
{
	struct _proportional_function_args proportional_args;
	struct _exp_function_args exp_args;
};

typedef union _function_args funciton_args;

unsigned short base_proportional_function(unsigned long, unsigned long, funciton_args*);
unsigned short base_exp_function(unsigned long, unsigned long, funciton_args*);

unsigned short(*get_proportional_function(unsigned long, signed long, funciton_args*))(unsigned long, unsigned long);
unsigned short(*get_exp_function(unsigned long, signed long, unsigned long, funciton_args*))(unsigned long, unsigned long);

#endif

#endif
