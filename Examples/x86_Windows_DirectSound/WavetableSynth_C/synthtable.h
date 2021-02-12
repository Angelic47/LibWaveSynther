#ifndef __SYNTH_TABLE__
#define __SYNTH_TABLE__

#define SYNTH_TABLE_SIZE 512
#define SYNTH_TABLE_SIZE_MASK 0x01FFFFFF
#define SYNTH_TABLE_SIZE_MASK_SHORT (SYNTH_TABLE_SIZE_MASK >> 16)

//typedef int16 synthdata_t;
typedef signed short synthdata_t;

#define DEFINE_SYNTH_TABLE(x) synthdata_t x[SYNTH_TABLE_SIZE] = 

#endif
