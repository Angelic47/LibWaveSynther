#coding: utf-8
import math

def TriangleWave():
	wave = []
	for x in range(0, 128):
		y = 32767 * x / 128
		wave.append(int(round(y)))
	for x in range(0, 256):
		y = 32767 - 65535 * x / 256
		wave.append(int(round(y)))
	for x in range(0, 128):
		y = -32768 + 256 * x
		wave.append(int(round(y)))
	return wave
	
def PianoWave():
	wave = []
	for x in range(0, 128):
		w = 1.0 / 128
		#y = (- 1.0 / 4 * math.sin(3 * math.pi * w * x) + 1.0 / 4 * math.sin(math.pi * w * x) + math.sqrt(3) / 2 * math.cos(math.pi * w * x)) * 32768
		y = 0.6 * math.sin(2 * math.pi * w * x) * math.exp(-0.0015 * 2 * math.pi * w * x)
		y += 0.4 * math.sin(4 * math.pi * w * x) * math.exp(-0.0015 * 2 * math.pi * w * x)
		y += y * y * y
		y *= 20480
		wave.append(int(round(y)))
	return wave
	
def PianoWave2():
	wave = []
	maxy = 0
	for x in range(0, 512):
		w = 1.0 / 512
		#y = (- 1.0 / 4 * math.sin(3 * math.pi * w * x) + 1.0 / 4 * math.sin(math.pi * w * x) + math.sqrt(3) / 2 * math.cos(math.pi * w * x)) * 32768
		
		y = 1.0 * math.sin(2 * math.pi * w * x)
		y += 0.2 * math.sin(4 * math.pi * w * x)
		y += 0.1 * math.sin(8 * math.pi * w * x)
		
		#y += 0.1 * math.sin(16 * math.pi * w * x)
		#y += 0.05 * math.sin(32 * math.pi * w * x)
		#y += 0.025 * math.sin(64 * math.pi * w * x)
		y += y * y * y
		wave.append(y)
		if maxy < abs(y):
			maxy = abs(y)
	return [int(round(32767.0 / maxy * y)) for y in wave]
	
def Rect25_75Wave():
	wave = []
	for x in range(0, 32):
		wave.append(32767)
	for x in range(0, 96):
		wave.append(-32768)
	return wave

def Rect75_25Wave():
	wave = []
	for x in range(0, 384):
		wave.append(32767)
	for x in range(0, 128):
		wave.append(-32768)
	return wave
	
def Rect50_50Wave():
	wave = []
	for x in range(0, 256):
		wave.append(32767)
	for x in range(0, 256):
		wave.append(-32768)
	return wave

def From_WaveFile(filename, ratio = 1):
    import wave
    import struct
    w = []
    wavefile = wave.open(filename, 'rb')
    _min = 32767
    _max = -32768
    if wavefile.getnframes() != 512:
        raise Exception('Input file %s not a 512 sized wave format' % filename)
    for i in range(0, wavefile.getnframes()):
        buffer = wavefile.readframes(1)
        buffer = int(round(struct.unpack('<h', buffer)[0] * ratio))
        w.append(buffer)
        _min = min(_min, buffer)
        _max = max(_max, buffer)
    if _min < -32768:
        raise Exception('Input file sampled reached min limit, min: %d, max: %d' % (_min, _max))
    if _max > 32767:
        raise Exception('Input file sampled reached max limit, min: %d, max: %d' % (_min, _max))
    print 'Input file sampled with min: %d, max: %d' % (_min, _max)
    return w

wave = From_WaveFile('piano_wave.wav', 1.63906)
f = open('wavetable.c', 'wb')
f.write("#include \"synthtable.h\"\n\nDEFINE_SYNTH_TABLE(g_table_piano_wavefile) {" + ", ".join([str(x) for x in wave]) + "};\n")
f.close()