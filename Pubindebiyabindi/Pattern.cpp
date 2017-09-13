#include "Pattern.hpp"

bool Compare(const unsigned char* pData, const unsigned char* bMask, const char* szMask)
{
	for (;*szMask;++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return false;

	return (*szMask) == 0;
}
unsigned long FindPattern(unsigned long dwAddress, unsigned long dwLen, unsigned char *bMask, char * szMask)
{
	for (unsigned long i = 0; i < dwLen; i++)
		if (Compare((unsigned char*)(dwAddress + i), bMask, szMask))
			return (unsigned long)(dwAddress + i);

	return 0;
}
