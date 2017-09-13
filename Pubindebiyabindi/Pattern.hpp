#pragma once

bool Compare(const unsigned char* pData, const unsigned char* bMask, const char* szMask);
unsigned long FindPattern(unsigned long dwAddress, unsigned long dwLen, unsigned char *bMask, char * szMask);

