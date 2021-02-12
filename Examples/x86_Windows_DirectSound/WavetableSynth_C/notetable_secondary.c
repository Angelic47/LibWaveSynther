#include "notetable.h"

#define SYNTH_NOTELEN_SECONDARY_SYNTH_1 22
DEFINE_SYNTH_NOTE_TABLE(g_secondary_synth_1, SYNTH_NOTELEN_SECONDARY_SYNTH_1) {255, 51, 43, 50, 41, 48, 255, 51, 43, 50, 46, 255, 41, 44, 48, 53, 54, 55, 50, 48, 47, 255};
DEFINE_SYNTH_NOTETIME_TABLE(g_secondary_synth_1_time, SYNTH_NOTELEN_SECONDARY_SYNTH_1) {200000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 16000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 24000, 480167};

synthnotenum_t g_tracknotenums_secondary[1] = {22};
synthnote_p g_tracknotes_secondary[1] = {g_secondary_synth_1};
synthnotetime_p g_tracknotetimes_secondary[1] = {g_secondary_synth_1_time};

synth_notes_array_t g_secondary_notes = { .tracker_num = 1, .notenum_array = g_tracknotenums_secondary, .notetables_array = g_tracknotes_secondary, .notetimes_array = g_tracknotetimes_secondary };
