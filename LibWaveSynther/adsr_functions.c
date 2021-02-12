#include "adsr_functions.h"

#ifdef HIGH_QUALITY_ENABLE

unsigned short base_proportional_function(unsigned long x, unsigned long y_old, funciton_args* args)
{
	return round(args->proportional_args.a * x);
}

unsigned short base_exp_function(unsigned long x, unsigned long y_old, funciton_args* args)
{
	return round(args->exp_args.a * exp(args->exp_args.b * x + args->exp_args.d) + args->exp_args.c);
}

unsigned short(*get_proportional_function(unsigned long x_range, signed long y_range, funciton_args* args))(unsigned long, unsigned long)
{
	args->proportional_args.a = (float)y_range / x_range;
	return &base_proportional_function;
}

unsigned short(*get_exp_function(unsigned long x_range, signed long y_range,unsigned long shape, funciton_args* args))(unsigned long, unsigned long)
{
	if (y_range >= 0)
	{
		args->exp_args.d = -1;
		args->exp_args.b = (float)shape / x_range;
		args->exp_args.a = (float)y_range / (exp(shape - 1) - exp(-1));
		args->exp_args.c = (-args->exp_args.a) * exp(-1);
	}
	else
	{
		args->exp_args.d = 1.0 - shape;
		args->exp_args.b = -(float)shape / x_range;
		args->exp_args.a = (float)y_range / (exp(-1.0-shape) - exp(-1));
		args->exp_args.c = -args->exp_args.a * exp(-1);
	}
	/*
	exp_function_args.d = shape > 0 ? shape : 10;
	exp_function_args.b = (float)exp_function_args.d / x_range;
	float temp = exp(exp_function_args.d - 1) - exp(-1);
	exp_function_args.a = (float)y_range / (temp);
	exp_function_args.c = -exp(-1)*exp_function_args.a;*/
	return &base_exp_function;
}

#endif
