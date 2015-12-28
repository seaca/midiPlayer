#ifndef PROPH
#define PROPH

#include <Windows.h>
#include <stdint.h>

#define GET_MSB(X) (((X)&0x80)>>7)

// エンディアン変換
uint16_t ChangeEndian16(uint16_t dt);
uint32_t ChangeEndian32(uint32_t dt);


#endif