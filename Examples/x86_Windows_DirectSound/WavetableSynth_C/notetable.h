#ifndef __SYNTH_NOTE_TABLE__
#define __SYNTH_NOTE_TABLE__

//typedef uint8 synthtrackernum_t;
typedef unsigned char synthtrackernum_t;

//typedef uint16 synthnotenum_t;
typedef unsigned short synthnotenum_t;

//typedef uint8 synthnote_t;
typedef unsigned char synthnote_t;
typedef const synthnote_t* synthnote_p;

//typedef uint32 synthnotetime_t;
typedef unsigned short synthnotetime_t;
typedef const synthnotetime_t* synthnotetime_p;

typedef unsigned long synthnoteppm_t;

struct _synth_notes_array_t
{
	synthtrackernum_t tracker_num;
	synthnotenum_t* notenum_array;
	synthnote_p* notetables_array;
	synthnotetime_p* notetimes_array;
	synthnoteppm_t notebpm;
};

typedef struct _synth_notes_array_t synth_notes_array_t;

#define DEFINE_SYNTH_NOTE_TABLE(x,len) const synthnote_t x[len] = 
#define DEFINE_SYNTH_NOTETIME_TABLE(x,len) const synthnotetime_t x[len] = 

#endif
