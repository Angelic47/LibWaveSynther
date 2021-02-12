#coding: utf-8

import mido

def getBPM(midifile):
	for msg in midifile:
		if msg.type == "set_tempo":
			return mido.tempo2bpm(msg.tempo)
	raise Exception("Get midi BPM failed: set_tempo not found")

def scanMaxSynthers(midifile):
	synthers = 0
	max_synthers = 0
	for msg in midifile:
		if msg.type == "note_on":
			if msg.velocity == 0:
				synthers -= 1
			else:
				synthers += 1
		elif msg.type == "note_off":
			synthers -= 1
		if synthers < 0:
			raise Exception("Bad midi message: note_off larger than note_on")
		if synthers > max_synthers:
			max_synthers = synthers
	return max_synthers

NOTE_KEY_OFF = 255

class SyntherClass():
	def __init__(self):
		self.notes = []
		self.sleepTime = 0
		self.keyOnTime = 0
		self.current_note = NOTE_KEY_OFF
	
	def addTime(self, deltaTime):
		if self.current_note == NOTE_KEY_OFF:
			self.sleepTime += deltaTime
		else:
			self.keyOnTime += deltaTime
	
	def key_off(self):
		if self.current_note == NOTE_KEY_OFF:
			raise Exception("Synther error: already key_off")
		self.notes.append((self.current_note, self.keyOnTime))
		self.current_note = NOTE_KEY_OFF
		self.keyOnTime = 0
	
	def key_on(self, key):
		if self.current_note != NOTE_KEY_OFF:
			raise Exception("Synther error: already key_on")
		if self.sleepTime > 0:
			self.notes.append((NOTE_KEY_OFF, self.sleepTime))
		self.current_note = key
		self.sleepTime = 0
	
	def clean_up(self):
		if self.current_note == NOTE_KEY_OFF and self.sleepTime > 0:
			self.notes.append((NOTE_KEY_OFF, self.sleepTime))
		elif self.keyOnTime > 0:
			self.notes.append((self.current_note, self.keyOnTime))
		self.current_note = NOTE_KEY_OFF
		self.sleepTime = 0
		self.keyOnTime = 0

def generateNoteSynthers(midifile, syntherNum):
	syntherClasses = [SyntherClass() for i in range(0, syntherNum)]
	for msg in midifile:
		# 处理时间
		for synther in syntherClasses:
			synther.addTime(msg.time)
		# 判断音符类型
		if (msg.type == "note_on" and msg.velocity == 0) or msg.type == "note_off":
			# key_off
			# 获取当前音符
			msgNote = msg.note - 21
			# 查找是哪个synther正在播放
			useSynther = None
			for synther in syntherClasses:
				if synther.current_note != msgNote:
					continue
				useSynther = synther
				break
			if useSynther == None:
				raise Exception("Bad midi message: note_off not matched")
			# 操作当前synther停止播放
			useSynther.key_off()
		elif msg.type == "note_on":
			# key_on
			# 获取当前音符
			msgNote = msg.note - 21
			# 处理这个音符由哪个synther播放
			useSynther = None
			maxSleepTime = -1
			for synther in syntherClasses:
				if synther.current_note != NOTE_KEY_OFF:
					continue
				if synther.sleepTime > maxSleepTime:
					maxSleepTime = synther.sleepTime
					useSynther = synther
			if useSynther == None:
				raise Exception("Synthers not enough: too many key_on messages same time")
			# 操作当前synther开始播放
			useSynther.key_on(msgNote)
	for synther in syntherClasses:
		synther.clean_up()
	return syntherClasses

def getSyntherNoteTable(synther, table_name, sampleRate):
	notes = []
	note_times = []
	for note in synther.notes:
		notes.append(note[0])
		note_times.append(int(round(note[1] * sampleRate / bpm)))
	return "#define SYNTH_NOTELEN_" + table_name.upper() + " " + str(len(notes)) + "\nDEFINE_SYNTH_NOTE_TABLE(g_" + table_name + ", SYNTH_NOTELEN_" + table_name.upper() + ") {" + ", ".join([str(x) for x in notes]) + "};\nDEFINE_SYNTH_NOTETIME_TABLE(g_" + table_name + "_time, SYNTH_NOTELEN_" + table_name.upper() + ") {" + ", ".join([str(x) for x in note_times]) + "};\n\n"

midi_name = "touhou"
midi = mido.MidiFile(ur'touhou/output.mid')
bpm = getBPM(midi)
bpm = int(round(bpm))
syntherNum = scanMaxSynthers(midi)
synthers = generateNoteSynthers(midi, syntherNum)

notetable = ""
notetable_i = 1
notetable_names = []
notetime_names = []
for synther in synthers:
	notetable += getSyntherNoteTable(synther, midi_name + '_synth_' + str(notetable_i), 44100)
	notetable_names.append('g_' + midi_name + '_synth_' + str(notetable_i))
	notetime_names.append('g_' + midi_name + '_synth_' + str(notetable_i) + '_time')
	notetable_i += 1

notetable = "#include \"notetable.h\"\n\n" + notetable
notetable += "synthnotenum_t g_tracknotenums_" + midi_name + "[" + str(syntherNum) + "] = {" + ", ".join([str(len(x.notes)) for x in synthers]) + "};\n"
notetable += "synthnote_p g_tracknotes_" + midi_name + "[" + str(len(notetable_names)) + "] = {" + ", ".join(notetable_names) + "};\n"
notetable += "synthnotetime_p g_tracknotetimes_" + midi_name + "[" + str(len(notetime_names)) + "] = {" + ", ".join(notetime_names) + "};\n"
notetable += "\nsynth_notes_array_t g_" + midi_name + "_notes = { .tracker_num = " + str(syntherNum) + ", .notenum_array = g_tracknotenums_" + midi_name + ", .notetables_array = g_tracknotes_" + midi_name + ", .notetimes_array = g_tracknotetimes_" + midi_name + ", .notebpm = " + str(bpm) + " };\n"

f = open('tohou/notetable_' + midi_name + '.c', 'wb')
f.write(notetable)
f.close()

f = open('tohou/notetable_' + midi_name + '.h', 'wb')
f.write("""#pragma once
#include "notetable.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

	extern synth_notes_array_t g_""" + midi_name + """_notes;

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif
""")
f.close()
