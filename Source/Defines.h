#pragma once

// epsilon
#define EPSILON (0.0001f)
#define ODD  1
#define EVEN 0

// bits
#define BIT_ON(var, bit) (var |= (1 << bit))
#define BITS_ON(var, bits) (var |= bits)
#define BIT_TOGGLE(var, bit) (var ^= (1 << bit))
#define BITS_TOGGLE(var, bits) (var ^= bits)
#define BIT_OFF(var, bit) (var &= ~(1 << bit))
#define BITS_OFF(var, bits) (var &= ~bits)
#define BIT_TEST_ON(var, bit) ((var & (1 << bit)))
#define BIT_TEST_OFF(var, bit) (!(var & (1 << bit)))
#define BITS_TEST_ON(var, bits) ((var & bits) == bits)
#define BITS_TEST_OFF(var, bits) ((var & bits) != bits)
#define BITS_FLIP_ALL(var) (var = ~var)
#define BITS_SET(var, val, place) (var = (var | (val << place)) & (~(val << place)))
#define BITS_SET_ALL_ON(var) (var = -1)
#define BITS_SET_ALL_OFF(var) (var = 0)
#define BITS_RESET(var) (BITS_SET_ALL_OFF(var))

// rands
#define RAND_FLOAT(min, max) (((rand()/(float)RAND_MAX)*((max)-(min)))+(min))
#define RAND_DOUBLE(min, max) (((rand()/(double)RAND_MAX)*((max)-(min)))+(min))
#define RAND_INT(min, max) (rand() % (max - min + 1) + min)
//#define RAND_D3DXVECTOR2(min, max) (D3DXVECTOR2(RAND_FLOAT(min.x, max.x), RAND_FLOAT(min.y, max.y)))
//#define RAND_D3DXVECTOR3(min, max) (D3DXVECTOR3(RAND_FLOAT(min.x, max.x), RAND_FLOAT(min.y, max.y), RAND_FLOAT(min.z, max.z)))
#define RAND_U32COLOR() (rand() % 0x01000000 + 0xFF000000)

// colors (DWORDs)
#define DARKRED			0xffff3333
#define DARKGREEN		0xff33ff33
#define DARKBLUE		0xff3333ff
#define WHITE			0xffffffff
#define GREY			0xff555555
#define BLUE			0xff0000ff
#define RED				0xffff0000
#define LIGHT_RED		0xffff5555
#define YELLOW			0xffffff00
#define YELLOW_WHITE	0xfff2e5aa

// depths -- closest to farthest (into screen)
#define DEPTH_MOUSE			0.0f
#define DEPTH_QBOBJDRAG		0.009f
#define DEPTH_WNDOPTIONS	0.0095f
#define DEPTH_WNDBG			0.01f
#define DEPTH_QBICONHIGHLT  0.028f
#define DEPTH_QBICONS		0.029f
#define DEPTH_QUICKBAR		0.03f
#define DEPTH_HUDBORDER 	0.04f
#define DEPTH_RESOURCES 	0.05f
#define DEPTH_PLAYERINFO	0.05f
#define DEPTH_BUTTONTXT		0.05f
#define DEPTH_PLAYERINFOBG	0.06f
#define DEPTH_BUTTON		0.06f
#define DEPTH_HUDFRAME		0.07f
#define DEPTH_UNIT			0.86f
#define DEPTH_CITY			0.87f
#define DEPTH_BUILDING		0.87f
#define DEPTH_OBJECT		0.9f
#define DEPTH_TILES			1.0f

#define DEPTH_OS_FACTION	-0.01f
#define DEPTH_OS_WNDFRAME	-0.001f
