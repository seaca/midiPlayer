#include "prop.h"


 uint16_t ChangeEndian16(uint16_t dt){
	WORD rslt;

	((char*)&rslt)[0] = ((char*)&dt)[1];
	((char*)&rslt)[1] = ((char*)&dt)[0];
	return rslt;
}

uint32_t ChangeEndian32(uint32_t dt){
	DWORD rslt;

	((char*)&rslt)[0] = ((char*)&dt)[3];
	((char*)&rslt)[1] = ((char*)&dt)[2];
	((char*)&rslt)[2] = ((char*)&dt)[1];
	((char*)&rslt)[3] = ((char*)&dt)[0];
	return rslt;
}
