#include "midi.h"
#include "prop.h"


int GetCompressed(FILE *fin){
	unsigned char buf8;
	int rslt = 0;
	
	do{
		fread_s(&buf8, 1, 1, 1, fin);
		rslt = (rslt << 7) | (buf8 & 0x7f);
	} while (buf8 & 0x80);

	return rslt;
}

void ReadTrackData(MidiTrack *trackOut, FILE *fin){
	int deltaTm; // ウェイトカウント数記憶用
	int channel;
	int evCount = 0;
	int bufInt;
	unsigned char statByte;
	unsigned char ctrlCode;
	unsigned char buf8;

	while (1){
		deltaTm = GetCompressed(fin);
		trackOut->deltaTime[evCount] = deltaTm;
		fprintf(stderr, "\ndeltaTime = %d\n", deltaTm);
		fread_s(&buf8, 1, 1, 1, fin);
		if (GET_MSB(buf8)){ // MSBが立っていればstatバイト更新
			statByte = buf8;
		}
		else{ // そうでなければ今のバイトは後でもう一回読み直す
			fseek(fin, -1, SEEK_CUR);
		}
		channel = statByte & 0x0F;
		switch ((statByte & 0xF0) >> 4){ // 上位4bit取り出し
		case 0xf: // SysEx
			if (statByte == 0xff){ // meta event
				fread_s(&ctrlCode, 1, 1, 1, fin);
				if (ctrlCode == 0x2f){ // トラック終了コード
					fprintf(stderr, "end of track\n");
					fread_s(&buf8, 1, 1, 1, fin); // 00を読み飛ばす
					trackOut->events = evCount;
					return;
				}
				printf("[%d]%02x-%02x\n", deltaTm, statByte, ctrlCode);
			}
			else {
				printf("[%d]%02x\n", deltaTm, statByte);
			}
			bufInt = GetCompressed(fin); // データサイズ
			for (int i = 0; i < bufInt; i++){ // データサイズ分だけ出力
					fread_s(&buf8, 1, 1, 1, fin);
					fprintf(stderr, "-%02x", buf8);
					if (buf8 >= 32 & buf8 < 127) fprintf(stderr, "(%c)", buf8);
			}
			fprintf(stderr, "\n");
			break;
		case 0x8:
			trackOut->eventType[evCount] = MET_NOTEOFF;
			fprintf(stderr, "[NOTEOFF]\n");
			trackOut->midiEvent[evCount].ME_NoteOff.channel= channel;
			fprintf(stderr, "\tchannel = %d\n", channel);
			fread_s(&buf8, 1, 1, 1, fin);
			trackOut->midiEvent[evCount].ME_NoteOff.noteNum = buf8;
			fprintf(stderr, "\tnoteNum = %d\n", buf8);
			fread_s(&buf8, 1, 1, 1, fin);
			trackOut->midiEvent[evCount].ME_NoteOff.velocity = buf8;
			fprintf(stderr, "\tvelocity = %d\n", buf8);
			evCount++;
			break;
		case 0x9:
			trackOut->eventType[evCount] = MET_NOTEON;
			fprintf(stderr, "[NOTEON]\n");
			trackOut->midiEvent[evCount].ME_NoteOn.channel = channel;
			fprintf(stderr, "\tchannel = %d\n", channel);
			fread_s(&buf8, 1, 1, 1, fin);
			trackOut->midiEvent[evCount].ME_NoteOn.noteNum = buf8;
			fprintf(stderr, "\tnoteNum = %d\n", buf8);
			fread_s(&buf8, 1, 1, 1, fin);
			trackOut->midiEvent[evCount].ME_NoteOn.velocity = buf8;
			fprintf(stderr, "\tvelocity = %d\n", buf8);
			evCount++;
			break;
		case 0xA:
			trackOut->eventType[evCount] = MET_POLYKEYPRES;
			fprintf(stderr, "[POLY KEY PRESSURE]\n");
			trackOut->midiEvent[evCount].ME_PolyKeyPres.channel = channel;
			fprintf(stderr, "\tchannel = %d\n", channel);
			fread_s(&buf8, 1, 1, 1, fin);
			trackOut->midiEvent[evCount].ME_PolyKeyPres.noteNum = buf8;
			fprintf(stderr, "\tnoteNum = %d\n", buf8);
			fread_s(&buf8, 1, 1, 1, fin);
			trackOut->midiEvent[evCount].ME_PolyKeyPres.pressure = buf8;
			fprintf(stderr, "\tpressure = %d\n", buf8);
			evCount++;
			break;
		case 0xB:
			trackOut->eventType[evCount] = MET_CTRLCHANGE;
			fprintf(stderr, "[CONTROL CHANGE]\n");
			trackOut->midiEvent[evCount].ME_CtrlChange.channel = channel;
			fprintf(stderr, "\tchannel = %d\n", channel);
			fread_s(&buf8, 1, 1, 1, fin);
			trackOut->midiEvent[evCount].ME_CtrlChange.ctrlNum = buf8;
			fprintf(stderr, "\tctrlNum = %d\n", buf8);
			fread_s(&buf8, 1, 1, 1, fin);
			trackOut->midiEvent[evCount].ME_CtrlChange.data = buf8;
			fprintf(stderr, "\tdata = %d\n", buf8);
			evCount++;
			break;
		case 0xC:
			trackOut->eventType[evCount] = MET_PROGCHANGE;
			fprintf(stderr, "[PROGRAM CHANGE]\n");
			trackOut->midiEvent[evCount].ME_ProgChange.channel = channel;
			fprintf(stderr, "\tchannel = %d\n", channel);
			fread_s(&buf8, 1, 1, 1, fin);
			trackOut->midiEvent[evCount].ME_ProgChange.progNum = buf8;
			fprintf(stderr, "\tprogNum = %d\n", buf8);
			evCount++;
			break;
		case 0xD:
			trackOut->eventType[evCount] = MET_CHANNELPRES;
			fprintf(stderr, "[CHANNEL PRESSURE]\n");
			trackOut->midiEvent[evCount].ME_ChannelPres.channel = channel;
			fprintf(stderr, "\tchannel = %d\n", channel);
			fread_s(&buf8, 1, 1, 1, fin);
			trackOut->midiEvent[evCount].ME_ChannelPres.pressure= buf8;
			fprintf(stderr, "\tpressure = %d\n", buf8);
			evCount++;
			break;
		case 0xE:
			trackOut->eventType[evCount] = MET_PITCHBEND;
			fprintf(stderr, "[PITCH BEND]\n");
			trackOut->midiEvent[evCount].ME_PitchBend.channel = channel;
			fprintf(stderr, "\tchannel = %d\n", channel);
			fread_s(&buf8, 1, 1, 1, fin);
			trackOut->midiEvent[evCount].ME_PitchBend.pBendMSB = buf8;
			fprintf(stderr, "\tbend(MSB) = %d\n", buf8);
			fread_s(&buf8, 1, 1, 1, fin);
			trackOut->midiEvent[evCount].ME_PitchBend.pBendLSB = buf8;
			fprintf(stderr, "\tbend(LSB) = %d\n", buf8);
			evCount++;
			break;
		}
	}
}

void PrintTrackData(FILE *fout, MidiTrack*trackIn){
	for (int i = 0; i < trackIn->events; i++){
		fprintf(fout, "[%d] ", trackIn->deltaTime[i]);
		switch (trackIn->eventType[i]){
		case MET_NOTEON:
			fprintf(fout, "NOTE ON\n");
			fprintf(fout, "\tchannel : %d\n", trackIn->midiEvent[i].ME_NoteOn.channel);
			fprintf(fout, "\tNoteNum : %d\n", trackIn->midiEvent[i].ME_NoteOn.noteNum);
			fprintf(fout, "\tVelocity : %d\n", trackIn->midiEvent[i].ME_NoteOn.velocity);
			fprintf(fout, "\n");
			break;
		case MET_NOTEOFF:
			fprintf(fout, "NOTE OFF\n");
			fprintf(fout, "\n");
			break;
		default:
			fprintf(fout, "undefined event(%02x)\n", trackIn->eventType[i]);
			fprintf(fout, "\n");
			break;
		}
	}
}
