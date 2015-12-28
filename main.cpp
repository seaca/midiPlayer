#include <stdio.h>
#include <string.h>
#include "midi.h"
#include "prop.h"


void PrintUsage();
/*
int main(int argc, char*argv[]){
	FILE *fp;

	if (argc < 3){
		PrintUsage();
		return 0;
	}
	
}
*/

int main(void){
	FILE *fin, *fout;
	MidiHeader midiHeader;
	uint16_t buf16;
	uint32_t buf32;
	MidiTrack *midiTrack;

	fopen_s(&fin, "test.mid", "rb");
	fopen_s(&fout, "result.txt", "w");
	// read header
	fread_s(&buf32, sizeof(buf32), sizeof(uint32_t), 1, fin);
	buf32 = ChangeEndian32(buf32);
	if (buf32 != 0x4D546864){
		fprintf(stderr, "ERROR : this is not midi file.\n");
		return -1;
	}
	fread_s(&buf32, sizeof(buf32), sizeof(uint32_t), 1, fin);
	midiHeader.chankSize = ChangeEndian32(buf32);
	printf("[ƒwƒbƒ_’·]%dBytes\n\n",midiHeader.chankSize);
	fread_s(&buf16, sizeof(buf16), sizeof(uint16_t), 1, fin);
	midiHeader.format = ChangeEndian16(buf16);
	fread_s(&buf16, sizeof(buf16), sizeof(uint16_t), 1, fin);
	midiHeader.tracks = ChangeEndian16(buf16);
	fread_s(&buf16, sizeof(buf16), sizeof(uint16_t), 1, fin);
	midiHeader.timeBase = ChangeEndian16(buf16);
	printf("[ FORMAT ]%d\n[ TRACKS ]%d\n[ •ª‰ð”\ ]%d\n\n",
		midiHeader.format, midiHeader.tracks, midiHeader.timeBase);

	// read tracks
	midiTrack = new MidiTrack[midiHeader.tracks];
	for (int i = 0; i<midiHeader.tracks; i++){
		fread_s(&buf32, sizeof(buf32), sizeof(uint32_t), 1, fin);
		buf32 = ChangeEndian32(buf32);
		if (buf32!=0x4D54726B){
			fprintf(stderr, "ERROR : track header should be \"MTrk\" .\n");
			return -1;
		}
		fread_s(&buf32,sizeof(uint32_t), sizeof(buf32), 1, fin);
		midiTrack[i].chankSize = ChangeEndian32(buf32);
		fprintf(stderr, "--------%d th Track-------------------\n", i);
		fprintf(stderr, "[DataSize]%8dBytes\n",midiTrack[i].chankSize);
		ReadTrackData(&midiTrack[i], fin); 
	}

	// print tracks to debug
	for (int i = 1; i < midiHeader.tracks; i++){
		fprintf(fout, "------Track %d--------------\n", i);
		PrintTrackData(fout, &midiTrack[i]);
	}
	delete midiTrack;
	fclose(fin);
	return 0;
}
void PrintUsage(){
	puts("$main music.midi output");
}