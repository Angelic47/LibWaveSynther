#include <stdio.h>
#include "keydelta.h"
#include "synthtable.h"
#include "synther.h"
#include "adsr_functions.h"

//获取下个采样数据 (ADSR)
synthdata_t synth_get_next_data_adsr(synther_t *synther)
{
	register synth_arg_t data = synth_get_next_data(synther);
	return (data * adsr_get_volume(synther)) >> 16;
}

#ifdef HIGH_QUALITY_ENABLE
	//获取下个采样数据 (HIGH_QUALITY_ADSR)
	synth_arg_t synth_get_next_data_high_quality_adsr(synther_t *synther)
	{
		register synth_arg_t data = synth_get_next_data(synther);
		data *= high_quality_adsr_get_volume(synther);
		data = data >> 16;

		if (synther->high_quality_adsr.on_last_releasing)
		{
			adsrvolume_t result;
			if (synther->high_quality_adsr.last_time >= synther->high_quality_adsr.release_ms)
			{
				synther->high_quality_adsr.on_last_releasing = 0;
			}
			else
			{
				synther->high_quality_adsr.last_time++;
				synther->high_quality_adsr.last_volume = synther->high_quality_adsr.last_release == NULL ? 0 : synther->high_quality_adsr.last_release(synther->high_quality_adsr.last_time, synther->high_quality_adsr.last_volume, &(synther->high_quality_adsr.last_release_info.args)) + synther->high_quality_adsr.last_volume_begin;
			}
			result = synther->high_quality_adsr.last_volume;
			if (synther->last_key < 41)
			{
			}
			else if (synther->last_key < 65)
			{
				result = (result >> 2);
			}
			else
			{
				result = (result >> 3) + (result >> 4);
			}
			data += (synth_get_last_next_data(synther)  * result) >> 16;
		}
		return data;
	}
#endif // HIGH_QUALITY_ENABLE


//获取下个采样数据
synthdata_t synth_get_next_data(synther_t *synther)
{
	synther_position_t position0, position1;
	synthdata_t result;

	if (synther->keynum == SYNTH_KEY_OFF)
		return 0;
		
	position0 = synther->current_position >> 16;
	position1 = (position0 + 1) & SYNTH_TABLE_SIZE_MASK_SHORT;
	
	// calculate delta
	result = ((synther->synth_table[position1] - synther->synth_table[position0]) * (synther->current_position & 0xffff)) >> 16;
	// calculate base
	result += synther->synth_table[position0];
	
	ADD_KEY_DELTA(synther->current_position, synther->keynum);

	return result;
}

//获取上一个音符的延音的下个采样数据
synthdata_t synth_get_last_next_data(synther_t *synther)
{
	synther_position_t position0, position1;
	synthdata_t result;
	
	if (synther->last_key == SYNTH_KEY_OFF)
		return 0;

	position0 = synther->last_position >> 16;
	position1 = (position0 + 1) & SYNTH_TABLE_SIZE_MASK_SHORT;

	// calculate delta
	result = ((synther->synth_table[position1] - synther->synth_table[position0]) * (synther->last_position & 0xffff)) >> 16;
	// calculate base
	result += synther->synth_table[position0];

	ADD_KEY_DELTA(synther->last_position, synther->last_key);

	return result;
}

// 计算ADSR包络
adsrvolume_t adsr_get_volume(synther_t *synther)
{
	adsrval_t delta;
	
	if(synther->keynum == SYNTH_KEY_OFF)
		return 0;
	
	if(synther->adsr.adsr_status == ADSR_ATTACK)
	{
		if(synther->adsr.adsr_time >= synther->adsr.attack_ms)
		{
			synther->adsr.adsr_status = ADSR_DECAY;
			synther->adsr.adsr_time = 0;
			
			synther->adsr.volume = synther->adsr.attack_volume;
			synther->adsr.volume_base = synther->adsr.attack_volume;
			synther->adsr.adding_delta = 0;
		}
		else
		{
			delta = synther->adsr.adding_delta + 2 * synther->adsr.adsr_args[0].a;
			synther->adsr.volume = synther->adsr.adsr_args[0].c + synther->adsr.volume_base + (delta > synther->adsr.adsr_args[0].b ? 1 : 0);
			synther->adsr.volume_base += synther->adsr.adsr_args[0].c + (delta > synther->adsr.adsr_args[0].b * 2 ? 1 : 0);
			synther->adsr.adding_delta = delta > synther->adsr.adsr_args[0].b * 2 ? delta - synther->adsr.adsr_args[0].b * 2 : delta;
			synther->adsr.adsr_time ++;
		}
	}
	if(synther->adsr.adsr_status == ADSR_DECAY)
	{
		if(synther->adsr.adsr_time >= synther->adsr.decay_ms)
		{
			synther->adsr.adsr_status = ADSR_SUSTAIN;
			synther->adsr.adsr_time = 0;
			
			synther->adsr.volume = synther->adsr.decay_volume;
			synther->adsr.volume_base = synther->adsr.decay_volume;
			synther->adsr.adding_delta = 0;
		}
		else
		{
			delta = synther->adsr.adding_delta + 2 * synther->adsr.adsr_args[1].a;
			synther->adsr.volume = synther->adsr.adsr_args[1].c + synther->adsr.volume_base + (delta > synther->adsr.adsr_args[1].b ? 1 : 0);
			synther->adsr.volume_base += synther->adsr.adsr_args[1].c + (delta > synther->adsr.adsr_args[1].b * 2 ? 1 : 0);
			synther->adsr.adding_delta = delta > synther->adsr.adsr_args[1].b * 2 ? delta - synther->adsr.adsr_args[1].b * 2 : delta;
			synther->adsr.adsr_time ++;
		}
	}
	if(synther->adsr.adsr_status == ADSR_SUSTAIN)
	{
		if(synther->adsr.adsr_time >= synther->adsr.sustain_ms)
		{
			synther->adsr.volume = synther->adsr.sustain_volume;
		}
		else
		{
			delta = synther->adsr.adding_delta + 2 * synther->adsr.adsr_args[2].a;
			synther->adsr.volume = synther->adsr.adsr_args[2].c + synther->adsr.volume_base + (delta > synther->adsr.adsr_args[2].b ? 1 : 0);
			synther->adsr.volume_base += synther->adsr.adsr_args[2].c + (delta > synther->adsr.adsr_args[2].b * 2 ? 1 : 0);
			synther->adsr.adding_delta = delta > synther->adsr.adsr_args[2].b * 2 ? delta - synther->adsr.adsr_args[2].b * 2 : delta;
			synther->adsr.adsr_time ++;
		}
	}
	if(synther->adsr.adsr_status == ADSR_RELEASE)
	{
		if(synther->adsr.adsr_time >= synther->adsr.release_ms)
		{
			synther->adsr.volume = 0;
			synther->keynum = SYNTH_KEY_OFF;
		}
		else
		{
			delta = synther->adsr.adding_delta + 2 * synther->adsr.adsr_args[3].a;
			synther->adsr.volume = synther->adsr.adsr_args[3].c + synther->adsr.volume_base + (delta > synther->adsr.adsr_args[3].b ? 1 : 0);
			synther->adsr.volume_base += synther->adsr.adsr_args[3].c + (delta > synther->adsr.adsr_args[3].b * 2 ? 1 : 0);
			synther->adsr.adding_delta = delta > synther->adsr.adsr_args[3].b * 2 ? delta - synther->adsr.adsr_args[3].b * 2 : delta;
			synther->adsr.adsr_time ++;
		}
	}
	
	return synther->adsr.volume;
}

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
)
{
	synther->adsr.attack_ms = attack_ms;
	synther->adsr.attack_volume = attack_volume;
	synther->adsr.decay_ms = decay_ms;
	synther->adsr.decay_volume = decay_volume;
	synther->adsr.sustain_ms = sustain_ms;
	synther->adsr.sustain_volume = sustain_volume;
	synther->adsr.release_ms = release_ms;
	
	// init attack
	if(synther->adsr.attack_ms != 0)
	{
		synther->adsr.adsr_args[0].c = attack_volume / (synth_arg_t)attack_ms;
		synther->adsr.adsr_args[0].a = attack_volume - synther->adsr.adsr_args[0].c * attack_ms;
		if (synther->adsr.adsr_args[0].a < 0)
		{
			synther->adsr.adsr_args[0].a += attack_ms;
			synther->adsr.adsr_args[0].c--;
		}
		synther->adsr.adsr_args[0].b = attack_ms;
	}
	
	// init decay
	if(synther->adsr.decay_ms != 0)
	{
		synther->adsr.adsr_args[1].c = (decay_volume - attack_volume) / (synth_arg_t)decay_ms;
		synther->adsr.adsr_args[1].a = (decay_volume - attack_volume) - synther->adsr.adsr_args[1].c * decay_ms;
		if (synther->adsr.adsr_args[1].a < 0)
		{
			synther->adsr.adsr_args[1].a += decay_ms;
			synther->adsr.adsr_args[1].c--;
		}
		synther->adsr.adsr_args[1].b = decay_ms;
	}
	
	// init sustain
	if(synther->adsr.sustain_ms != 0)
	{
		synther->adsr.adsr_args[2].c = (sustain_volume - decay_volume) / (synth_arg_t)sustain_ms;
		synther->adsr.adsr_args[2].a = (sustain_volume - decay_volume) - synther->adsr.adsr_args[2].c * sustain_ms;
		if (synther->adsr.adsr_args[2].a < 0)
		{
			synther->adsr.adsr_args[2].a += sustain_ms;
			synther->adsr.adsr_args[2].c--;
		}
		synther->adsr.adsr_args[2].b = sustain_ms;
	}
	
	// init release_ms
	if(synther->adsr.release_ms != 0)
	{
		synther->adsr.adsr_args[3].c = (-sustain_volume) / (synth_arg_t)release_ms;
		synther->adsr.adsr_args[3].a = (-sustain_volume) - synther->adsr.adsr_args[3].c * release_ms;
		if (synther->adsr.adsr_args[3].a < 0)
		{
			synther->adsr.adsr_args[3].a += release_ms;
			synther->adsr.adsr_args[3].c--;
		}
		synther->adsr.adsr_args[3].b = release_ms;
	}
}

#ifdef HIGH_QUALITY_ENABLE
	// 计算HIGH_QUALITY_ADSR包络
	adsrvolume_t high_quality_adsr_get_volume(synther_t *synther)
	{
		adsrvolume_t result;

		if (synther->keynum == SYNTH_KEY_OFF)
			synther->high_quality_adsr.volume = 0;

		if (synther->high_quality_adsr.adsr_status == ADSR_ATTACK)
		{
			if (synther->high_quality_adsr.adsr_time >= synther->high_quality_adsr.attack_ms)
			{
				synther->high_quality_adsr.adsr_status = ADSR_DECAY;
				synther->high_quality_adsr.adsr_time = 0;
			}
			else
			{
				synther->high_quality_adsr.adsr_time++;
				synther->high_quality_adsr.volume = synther->high_quality_adsr.attack == NULL ? synther->high_quality_adsr.attack_volume : synther->high_quality_adsr.attack(synther->high_quality_adsr.adsr_time, synther->high_quality_adsr.volume, &(synther->high_quality_adsr.attack_info.args));
			}
		}
		else if (synther->high_quality_adsr.adsr_status == ADSR_DECAY)
		{
			if (synther->high_quality_adsr.adsr_time >= synther->high_quality_adsr.decay_ms)
			{
				synther->high_quality_adsr.adsr_status = ADSR_SUSTAIN;
				synther->high_quality_adsr.adsr_time = 0;
			}
			else
			{
				synther->high_quality_adsr.adsr_time++;
				synther->high_quality_adsr.volume = synther->high_quality_adsr.decay == NULL ? synther->high_quality_adsr.decay_volume : synther->high_quality_adsr.decay(synther->high_quality_adsr.adsr_time, synther->high_quality_adsr.volume, &(synther->high_quality_adsr.decay_info.args)) + synther->high_quality_adsr.attack_volume;
			}
		}
		else if (synther->high_quality_adsr.adsr_status == ADSR_SUSTAIN)
		{
			if (synther->high_quality_adsr.adsr_time >= synther->high_quality_adsr.sustain_ms)
			{
				synther->high_quality_adsr.volume = synther->high_quality_adsr.sustain_volume;
			}
			else
			{
				synther->high_quality_adsr.adsr_time++;
				synther->high_quality_adsr.volume = synther->high_quality_adsr.sustain == NULL ? synther->high_quality_adsr.sustain_volume : synther->high_quality_adsr.sustain(synther->high_quality_adsr.adsr_time, synther->high_quality_adsr.volume, &(synther->high_quality_adsr.sustain_info.args)) + synther->high_quality_adsr.decay_volume;
			}
		}
		else if (synther->high_quality_adsr.adsr_status == ADSR_RELEASE)
		{
			if (synther->high_quality_adsr.adsr_time >= synther->high_quality_adsr.release_ms)
			{
				synther->high_quality_adsr.volume = 0;
				synther->keynum = SYNTH_KEY_OFF;
			}
			else
			{
				synther->high_quality_adsr.adsr_time++;
				synther->high_quality_adsr.volume = synther->high_quality_adsr.release == NULL ? 0 : synther->high_quality_adsr.release(synther->high_quality_adsr.adsr_time, synther->high_quality_adsr.volume, &(synther->high_quality_adsr.release_info.args)) + synther->high_quality_adsr.sustain_volume;
			}
		}

		result = synther->high_quality_adsr.volume;

		if (synther->keynum < 41)
		{
		}
		else if (synther->keynum < 65)
		{
			result = (result >> 2);
		}
		else
		{
			result = (result >> 3) + (result >> 4);
		}

		return result;
	}

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
	)
	{
		synther->high_quality_adsr.attack_ms = attack_ms;
		synther->high_quality_adsr.attack_volume = attack_volume;
		synther->high_quality_adsr.attack_info.type = attack_type;
		synther->high_quality_adsr.decay_ms = decay_ms;
		synther->high_quality_adsr.decay_volume = decay_volume;
		synther->high_quality_adsr.decay_info.type = decay_type;
		synther->high_quality_adsr.sustain_ms = sustain_ms;
		synther->high_quality_adsr.sustain_volume = sustain_volume;
		synther->high_quality_adsr.sustain_info.type = sustain_type;
		synther->high_quality_adsr.release_ms = release_ms;
		synther->high_quality_adsr.release_info.type = release_type;
		synther->high_quality_adsr.attack = get_adsr_function(synther, synther->high_quality_adsr.attack_ms, synther->high_quality_adsr.attack_volume, ADSR_ATTACK);
		synther->high_quality_adsr.decay = get_adsr_function(synther, synther->high_quality_adsr.decay_ms, synther->high_quality_adsr.decay_volume - synther->high_quality_adsr.attack_volume, ADSR_DECAY);
		synther->high_quality_adsr.sustain = get_adsr_function(synther, synther->high_quality_adsr.sustain_ms, synther->high_quality_adsr.sustain_volume - synther->high_quality_adsr.decay_volume, ADSR_SUSTAIN);
		synther->high_quality_adsr.release = get_adsr_function(synther, synther->high_quality_adsr.release_ms, -synther->high_quality_adsr.sustain_volume, ADSR_RELEASE);
	}
	adsrvolume_t(*get_adsr_function(synther_t *synther, adsrval_t x_range, adsrval_t y_range, adsr_mod status))(adsrval_t, adsrval_t, funciton_args*)
	{
		high_quality_adsr_function type;
		switch (status)
		{
		case ADSR_ATTACK:
			type = synther->high_quality_adsr.attack_info.type;
			switch (type)
			{
			case PROPORTIONAL_FUNCTION:
				return get_proportional_function(x_range, y_range, &(synther->high_quality_adsr.attack_info.args));
				break;
			case EXP_FUNCTION_1:
				return get_exp_function(x_range, y_range, 1, &(synther->high_quality_adsr.attack_info.args));
				break;
			case EXP_FUNCTION_2:
				return get_exp_function(x_range, y_range, 2, &(synther->high_quality_adsr.attack_info.args));
				break;
			case EXP_FUNCTION_3:
				return get_exp_function(x_range, y_range, 3, &(synther->high_quality_adsr.attack_info.args));
				break;
			default:
				break;
			}
			break;
		case ADSR_DECAY:
			type = synther->high_quality_adsr.decay_info.type;
			switch (type)
			{
			case PROPORTIONAL_FUNCTION:
				return get_proportional_function(x_range, y_range, &(synther->high_quality_adsr.decay_info.args));
				break;
			case EXP_FUNCTION_1:
				return get_exp_function(x_range, y_range, 1, &(synther->high_quality_adsr.decay_info.args));
				break;
			case EXP_FUNCTION_2:
				return get_exp_function(x_range, y_range, 2, &(synther->high_quality_adsr.decay_info.args));
				break;
			case EXP_FUNCTION_3:
				return get_exp_function(x_range, y_range, 3, &(synther->high_quality_adsr.decay_info.args));
				break;
			default:
				break;
			}
			break;
		case ADSR_SUSTAIN:
			type = synther->high_quality_adsr.sustain_info.type;
			switch (type)
			{
			case PROPORTIONAL_FUNCTION:
				return get_proportional_function(x_range, y_range, &(synther->high_quality_adsr.sustain_info.args));
				break;
			case EXP_FUNCTION_1:
				return get_exp_function(x_range, y_range, 1, &(synther->high_quality_adsr.sustain_info.args));
				break;
			case EXP_FUNCTION_2:
				return get_exp_function(x_range, y_range, 2, &(synther->high_quality_adsr.sustain_info.args));
				break;
			case EXP_FUNCTION_3:
				return get_exp_function(x_range, y_range, 3, &(synther->high_quality_adsr.sustain_info.args));
				break;
			default:
				break;
			}
			break;
		case ADSR_RELEASE:
			type = synther->high_quality_adsr.release_info.type;
			switch (type)
			{
			case PROPORTIONAL_FUNCTION:
				return get_proportional_function(x_range, y_range, &(synther->high_quality_adsr.release_info.args));
				break;
			case EXP_FUNCTION_1:
				return get_exp_function(x_range, y_range, 1, &(synther->high_quality_adsr.release_info.args));
				break;
			case EXP_FUNCTION_2:
				return get_exp_function(x_range, y_range, 2, &(synther->high_quality_adsr.release_info.args));
				break;
			case EXP_FUNCTION_3:
				return get_exp_function(x_range, y_range, 3, &(synther->high_quality_adsr.release_info.args));
				break;
			default:
				break;
			}
			break;
		default:
			return NULL;
		}
	}
#endif // HIGH_QUALITY_ENABLE

// 按下某个按键
void synth_key_on(synther_t *synther, keynum_t keynum)
{
	if (synther->keynum == SYNTH_KEY_OFF)
		synther->current_position = 0;
	synther->keynum = keynum;

	synther->adsr.adsr_status = ADSR_ATTACK;
	synther->adsr.adsr_time = 0;

	synther->adsr.volume = 0;
	synther->adsr.volume_base = 0;
	synther->adsr.adding_delta = 0;
}

// 抬起某个按键
void synth_key_off(synther_t *synther)
{
	synther->adsr.adsr_status = ADSR_RELEASE;
	synther->adsr.adsr_time = 0;
	
	synther->adsr.volume = synther->adsr.sustain_volume;
	synther->adsr.volume_base = synther->adsr.sustain_volume;
	synther->adsr.adding_delta = 0;
}

#ifdef HIGH_QUALITY_ENABLE
	// 按下某个按键
	void synth_key_on_HQ(synther_t *synther, keynum_t keynum)
	{
		synther->high_quality_adsr.adsr_status = ADSR_ATTACK;

		if (synther->high_quality_adsr.volume != 0)
		{
			synther->high_quality_adsr.on_last_releasing = 1;
			synther->high_quality_adsr.last_time = 0;
			synther->high_quality_adsr.last_volume = synther->high_quality_adsr.volume;
			synther->high_quality_adsr.last_volume_begin = synther->high_quality_adsr.volume;
			synther->last_key = synther->keynum;
			synther->last_position = synther->current_position;
			
			switch (synther->high_quality_adsr.release_info.type)
			{
			case PROPORTIONAL_FUNCTION:
				synther->high_quality_adsr.last_release = get_proportional_function(synther->high_quality_adsr.release_ms, -synther->high_quality_adsr.last_volume, &(synther->high_quality_adsr.last_release_info.args));
				break;
			case EXP_FUNCTION_1:
				synther->high_quality_adsr.last_release = get_exp_function(synther->high_quality_adsr.release_ms, -synther->high_quality_adsr.last_volume, 1, &(synther->high_quality_adsr.last_release_info.args));
				break;
			case EXP_FUNCTION_2:
				synther->high_quality_adsr.last_release = get_exp_function(synther->high_quality_adsr.release_ms, -synther->high_quality_adsr.last_volume, 2, &(synther->high_quality_adsr.last_release_info.args));
				break;
			case EXP_FUNCTION_3:
				synther->high_quality_adsr.last_release = get_exp_function(synther->high_quality_adsr.release_ms, -synther->high_quality_adsr.last_volume, 3, &(synther->high_quality_adsr.last_release_info.args));
				break;
			default:
				break;
			}
		}

		if(synther->keynum == SYNTH_KEY_OFF)
			synther->current_position = 0;
		synther->keynum = keynum;

		synther->high_quality_adsr.adsr_time = 0;
		synther->high_quality_adsr.volume = 0;
	}
	// 抬起某个按键
	void synth_key_off_HQ(synther_t *synther)
	{
		synther->high_quality_adsr.adsr_status = ADSR_RELEASE;
		synther->high_quality_adsr.adsr_time = 0;

		synther->high_quality_adsr.on_last_releasing = 0;

		//synther->high_quality_adsr.volume = synther->high_quality_adsr.sustain_volume;
	}
#endif // HIGH_QUALITY_ENABLE


// 初始化混音器数据结构
void synth_initalize(synther_t *synther, const synthdata_t *synth_table)
{
	synther->synth_table = synth_table;
	synther->keynum = SYNTH_KEY_OFF;
	synther->current_position = 0;
	
	synther->adsr.attack_ms = 0;
	synther->adsr.attack_volume = 65535;
	synther->adsr.decay_ms = 0;
	synther->adsr.decay_volume = 65535;
	synther->adsr.sustain_ms = 0;
	synther->adsr.sustain_volume = 65535;
	synther->adsr.release_ms = 0;
	
	synther->adsr.adsr_time = 0;
	synther->adsr.adsr_status = ADSR_ATTACK;
	
	synther->adsr.volume = 0;
	synther->adsr.volume_base = 0;
	synther->adsr.adding_delta = 0;

	synther->last_key = SYNTH_KEY_OFF;
	synther->last_position = 0;

	#ifdef HIGH_QUALITY_ENABLE
		synther->high_quality_adsr.attack_ms = 0;
		synther->high_quality_adsr.attack_volume = 65535;
		synther->high_quality_adsr.attack = NULL;
		synther->high_quality_adsr.decay_ms = 0;
		synther->high_quality_adsr.decay_volume = 65535;
		synther->high_quality_adsr.decay = NULL;
		synther->high_quality_adsr.sustain_ms = 0;
		synther->high_quality_adsr.sustain_volume = 65535;
		synther->high_quality_adsr.sustain = NULL;
		synther->high_quality_adsr.release_ms = 0;
		synther->high_quality_adsr.release = NULL;
		synther->high_quality_adsr.volume = 0;
		synther->high_quality_adsr.adsr_time = 0;
		synther->high_quality_adsr.adsr_status = ADSR_ATTACK;

		synther->high_quality_adsr.on_last_releasing = 0;
		synther->high_quality_adsr.last_volume = 0;
		synther->high_quality_adsr.last_volume_begin = 0;
		synther->high_quality_adsr.last_time = 0;
		synther->high_quality_adsr.last_release = NULL;
	#endif // HIGH_QUALITY_ENABLE
}
