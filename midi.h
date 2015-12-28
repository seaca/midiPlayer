#ifndef MIDIH
#define MIDIH

#include "windows.h"
#include <stdio.h>
#include <stdint.h>


#define MAX_EVENTS 65535
// イベントタイプ
#define MET_NOTEOFF	8
#define MET_NOTEON	9
#define MET_POLYKEYPRES	10
#define MET_CTRLCHANGE	11
#define MET_PROGCHANGE	12
#define MET_CHANNELPRES	13
#define MET_PITCHBEND	14
//--------------------------------------
//ヘッダ用構造体
typedef struct{
	uint32_t chankSize;
	uint16_t format;           //フォーマット(0 or 1）
	uint16_t tracks;           //トラック数
	uint16_t timeBase;         //分解能
}MidiHeader;


typedef union{
	struct {
		char channel;
		char noteNum;
		char velocity;
	}ME_NoteOff;
	struct {
		char channel;
		char noteNum;
		char velocity;
	}ME_NoteOn;
	struct {
		char channel;
		char noteNum;
		char pressure;
	}ME_PolyKeyPres;
	struct {
		char channel;
		char ctrlNum;
		char data;
	}ME_CtrlChange;
	struct {
		char channel;
		char progNum;
	}ME_ProgChange;
	struct {
		char channel;
		char pressure;
	}ME_ChannelPres;
	struct {
		char channel;
		char pBendMSB;
		char pBendLSB;
	}ME_PitchBend;
}MidiEvent;

typedef struct{
	uint32_t chankSize;
	int events;
	int deltaTime[MAX_EVENTS];
	int eventType[MAX_EVENTS];
	MidiEvent midiEvent[MAX_EVENTS];
}MidiTrack;


//--------------------------------------
int GetCompressed(FILE *fin);


//--------------------------------------
void ReadTrackData(MidiTrack *trackOut, FILE *fin);
void PrintTrackData(FILE *fout, MidiTrack*trackIn);

#endif