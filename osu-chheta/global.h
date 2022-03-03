typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

typedef unsigned char devno_t;

#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )

#define swab32(x) ((u32)( \
				(((u32)(x) & (u32)0x000000ffUL) << 24) | \
				(((u32)(x) & (u32)0x0000ff00UL) <<  8) | \
				(((u32)(x) & (u32)0x00ff0000UL) >>  8) | \
				(((u32)(x) & (u32)0xff000000UL) >> 24)))
#define swab16(x) ((u16)( \
				(((u16)(x) & (u16)0x00ffU) << 8) | \
				(((u16)(x) & (u16)0xff00U) >> 8)))