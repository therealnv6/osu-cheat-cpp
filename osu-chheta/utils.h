#ifndef __UTILS_H__
#define __UTILS_H__



#include "global.h"



class Utils
{
	public:
		template <typename T>
		static void Pack(bool pack, T* value);
		static void Hexdump(void* buffer, u32 length);
		static char ascii(char c);


};

#endif