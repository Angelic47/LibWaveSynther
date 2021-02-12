#ifndef __SYNTH__
#define __SYNTH__

#include "keydelta.h"
#include "adsr_functions.h"
#include "synthtable.h"


//#define HIGH_QUALITY_ENABLE //编译HIGH_QUALITY模式

#if defined(__cplusplus) || defined(c_plusplus) //跨平台定义方法
extern "C" {
#endif
	
	#define SYNTH_SAMPLE_RATE 44100
	
	//typedef uint32 adsrval_t;
	typedef unsigned long adsrval_t;
	
	//typedef uint16 adsrvolume_t;
	typedef unsigned short adsrvolume_t;
	
	enum _adsr_mod
	{
		ADSR_ATTACK = 0,
		ADSR_DECAY = 1,
		ADSR_SUSTAIN = 2,
		ADSR_RELEASE = 3
	};
	
	typedef enum _adsr_mod adsr_mod;
	
	//typedef int32 synth_arg_t;
	typedef signed long synth_arg_t;
	
	struct _synth_adsr_args
	{
		synth_arg_t a;
		adsrval_t b;
		synth_arg_t c;
	};
	
	typedef struct _synth_adsr_args synth_adsr_args;
	
	struct _synther_adsr
	{
		adsrval_t attack_ms;
		adsrvolume_t attack_volume;
		adsrval_t decay_ms;
		adsrvolume_t decay_volume;
		adsrval_t sustain_ms;
		adsrvolume_t sustain_volume;
		adsrval_t release_ms;
		
		synth_adsr_args adsr_args[4];
		
		adsrvolume_t volume;
		adsrvolume_t volume_base;
		adsrval_t adding_delta;
		
		adsrval_t adsr_time;
		adsr_mod adsr_status;
	};

	typedef struct _synther_adsr synther_adsr;

	#ifdef HIGH_QUALITY_ENABLE
		enum _high_quality_adsr_function
		{
			PROPORTIONAL_FUNCTION = 0,
			EXP_FUNCTION_1 = 1,
			EXP_FUNCTION_2 = 2,
			EXP_FUNCTION_3 = 3
		};

		typedef enum _high_quality_adsr_function high_quality_adsr_function;

		struct _high_quality_adsr_function_info
		{
			high_quality_adsr_function type;
			funciton_args args;
		};

		typedef struct _high_quality_adsr_function_info high_quality_adsr_function_info;

		struct _synther_high_quality_adsr
		{
			adsrval_t attack_ms;
			adsrvolume_t attack_volume;
			adsrval_t decay_ms;
			adsrvolume_t decay_volume;
			adsrval_t sustain_ms;
			adsrvolume_t sustain_volume;
			adsrval_t release_ms;

			unsigned char on_last_releasing;
			adsrvolume_t last_volume;
			adsrvolume_t last_volume_begin;
			adsrval_t last_time;
			high_quality_adsr_function_info last_release_info;
			adsrvolume_t(*last_release)(adsrval_t, adsrval_t, funciton_args*);

			high_quality_adsr_function_info attack_info;
			high_quality_adsr_function_info decay_info;
			high_quality_adsr_function_info sustain_info;
			high_quality_adsr_function_info release_info;

			adsrvolume_t(*attack)(adsrval_t, adsrval_t, funciton_args*);
			adsrvolume_t(*decay)(adsrval_t, adsrval_t, funciton_args*);
			adsrvolume_t(*sustain)(adsrval_t, adsrval_t, funciton_args*);
			adsrvolume_t(*release)(adsrval_t, adsrval_t, funciton_args*);

			adsrvolume_t volume;

			adsrval_t adsr_time;
			adsr_mod adsr_status;
		};

		typedef struct _synther_high_quality_adsr synther_high_quality_adsr;
	#endif // HIGH_QUALITY_ENABLE
	
	struct _synther_t
	{
		keydelta_t current_position;
		const synthdata_t *synth_table;
		keynum_t keynum;
		synther_adsr adsr;

		keynum_t last_key;
		keydelta_t last_position;
		
	#ifdef HIGH_QUALITY_ENABLE
			synther_high_quality_adsr high_quality_adsr;
	#endif // HIGH_QUALITY_ENABLE

	};

	typedef struct _synther_t synther_t;

	// typedef uint16 synther_position_t;
	typedef unsigned short synther_position_t;

	//获取下个采样数据 (ADSR)
	synthdata_t synth_get_next_data_adsr(synther_t *synther);

	#ifdef HIGH_QUALITY_ENABLE
		//获取下个采样数据 (HIGH_QUALITY_ADSR)
		synth_arg_t synth_get_next_data_high_quality_adsr(synther_t *synther);
	#endif // HIGH_QUALITY_ENABLE

	// 获取下一个采样数据
	synthdata_t synth_get_next_data(synther_t *synther);
	//获取上一个音符的延音的下个采样数据
	synthdata_t synth_get_last_next_data(synther_t *synther);
	// 按下某个按键
	void synth_key_on(synther_t *synther, keynum_t keynum);
	// 抬起某个按键
	void synth_key_off(synther_t *synther);

	#ifdef  HIGH_QUALITY_ENABLE
		// 按下某个按键
		void synth_key_on_HQ(synther_t *synther, keynum_t keynum);
		// 抬起某个按键
		void synth_key_off_HQ(synther_t *synther);
	#endif //  HIGH_QUALITY_ENABLE


	// 初始化混音器数据结构
	void synth_initalize(synther_t *synther, const synthdata_t *synth_table);
	// 计算ADSR包络
	adsrvolume_t adsr_get_volume(synther_t *synther);
	// 设置ADSR包络信息
	void synth_adsr_set(
		synther_t *synther,
		adsrval_t attack_ms,
		adsrvolume_t attack_volume,
		adsrval_t decay_ms,
		adsrvolume_t decay_volume,
		adsrval_t sustain_ms,
		adsrvolume_t sustain_volume,
		adsrval_t release_ms
	);

	#ifdef HIGH_QUALITY_ENABLE
		// 计算HIGH_QUALITY_ADSR包络
		adsrvolume_t high_quality_adsr_get_volume(synther_t *synther);
		// 设置HIGH_QUALITY_ADSR包络信息
		void high_quality_synth_adsr_set(
			synther_t *synther,
			adsrval_t attack_ms,
			adsrvolume_t attack_volume,
			adsrval_t decay_ms,
			adsrvolume_t decay_volume,
			adsrval_t sustain_ms,
			adsrvolume_t sustain_volume,
			adsrval_t release_ms,
			high_quality_adsr_function attack_type,
			high_quality_adsr_function decay_type,
			high_quality_adsr_function sustain_type,
			high_quality_adsr_function release_type
		);
		adsrvolume_t(*get_adsr_function(synther_t*, adsrval_t, adsrval_t, adsr_mod))(adsrval_t, adsrval_t, funciton_args*);
	#endif // HIGH_QUALITY_ENABLE

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif 

#endif
