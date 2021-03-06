﻿#include <windows.h>
#include "xts_test.h"
#ifdef SMALL_CODE
	#include "xts_small.h"
#else
	#include "xts_fast.h"
	#include "crc32.h"
#endif

static const struct { /* see IEEE 1619 */
	const unsigned char key1[32];
	const unsigned char key2[32];
	const unsigned char index[8];	
	const unsigned char ciphertext[XTS_SECTOR_SIZE];

} xts_vectors[] = {
	{
		{ 0x27, 0x18, 0x28, 0x18, 0x28, 0x45, 0x90, 0x45, 0x23, 0x53, 0x60, 0x28, 0x74, 0x71, 0x35, 0x26, 
		  0x62, 0x49, 0x77, 0x57, 0x24, 0x70, 0x93, 0x69, 0x99, 0x59, 0x57, 0x49, 0x66, 0x96, 0x76, 0x27 },
		{ 0x31, 0x41, 0x59, 0x26, 0x53, 0x58, 0x97, 0x93, 0x23, 0x84, 0x62, 0x64, 0x33, 0x83, 0x27, 0x95, 
		  0x02, 0x88, 0x41, 0x97, 0x16, 0x93, 0x99, 0x37, 0x51, 0x05, 0x82, 0x09, 0x74, 0x94, 0x45, 0x92 },
	    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff },
		{ 0x1c, 0x3b, 0x3a, 0x10, 0x2f, 0x77, 0x03, 0x86, 0xe4, 0x83, 0x6c, 0x99, 0xe3, 0x70, 0xcf, 0x9b, 
		  0xea, 0x00, 0x80, 0x3f, 0x5e, 0x48, 0x23, 0x57, 0xa4, 0xae, 0x12, 0xd4, 0x14, 0xa3, 0xe6, 0x3b,
		  0x5d, 0x31, 0xe2, 0x76, 0xf8, 0xfe, 0x4a, 0x8d, 0x66, 0xb3, 0x17, 0xf9, 0xac, 0x68, 0x3f, 0x44, 
		  0x68, 0x0a, 0x86, 0xac, 0x35, 0xad, 0xfc, 0x33, 0x45, 0xbe, 0xfe, 0xcb, 0x4b, 0xb1, 0x88, 0xfd,
		  0x57, 0x76, 0x92, 0x6c, 0x49, 0xa3, 0x09, 0x5e, 0xb1, 0x08, 0xfd, 0x10, 0x98, 0xba, 0xec, 0x70, 
		  0xaa, 0xa6, 0x69, 0x99, 0xa7, 0x2a, 0x82, 0xf2, 0x7d, 0x84, 0x8b, 0x21, 0xd4, 0xa7, 0x41, 0xb0,
		  0xc5, 0xcd, 0x4d, 0x5f, 0xff, 0x9d, 0xac, 0x89, 0xae, 0xba, 0x12, 0x29, 0x61, 0xd0, 0x3a, 0x75, 
		  0x71, 0x23, 0xe9, 0x87, 0x0f, 0x8a, 0xcf, 0x10, 0x00, 0x02, 0x08, 0x87, 0x89, 0x14, 0x29, 0xca,
		  0x2a, 0x3e, 0x7a, 0x7d, 0x7d, 0xf7, 0xb1, 0x03, 0x55, 0x16, 0x5c, 0x8b, 0x9a, 0x6d, 0x0a, 0x7d, 
		  0xe8, 0xb0, 0x62, 0xc4, 0x50, 0x0d, 0xc4, 0xcd, 0x12, 0x0c, 0x0f, 0x74, 0x18, 0xda, 0xe3, 0xd0,
		  0xb5, 0x78, 0x1c, 0x34, 0x80, 0x3f, 0xa7, 0x54, 0x21, 0xc7, 0x90, 0xdf, 0xe1, 0xde, 0x18, 0x34, 
		  0xf2, 0x80, 0xd7, 0x66, 0x7b, 0x32, 0x7f, 0x6c, 0x8c, 0xd7, 0x55, 0x7e, 0x12, 0xac, 0x3a, 0x0f,
		  0x93, 0xec, 0x05, 0xc5, 0x2e, 0x04, 0x93, 0xef, 0x31, 0xa1, 0x2d, 0x3d, 0x92, 0x60, 0xf7, 0x9a, 
		  0x28, 0x9d, 0x6a, 0x37, 0x9b, 0xc7, 0x0c, 0x50, 0x84, 0x14, 0x73, 0xd1, 0xa8, 0xcc, 0x81, 0xec,
		  0x58, 0x3e, 0x96, 0x45, 0xe0, 0x7b, 0x8d, 0x96, 0x70, 0x65, 0x5b, 0xa5, 0xbb, 0xcf, 0xec, 0xc6, 
		  0xdc, 0x39, 0x66, 0x38, 0x0a, 0xd8, 0xfe, 0xcb, 0x17, 0xb6, 0xba, 0x02, 0x46, 0x9a, 0x02, 0x0a,
		  0x84, 0xe1, 0x8e, 0x8f, 0x84, 0x25, 0x20, 0x70, 0xc1, 0x3e, 0x9f, 0x1f, 0x28, 0x9b, 0xe5, 0x4f, 
		  0xbc, 0x48, 0x14, 0x57, 0x77, 0x8f, 0x61, 0x60, 0x15, 0xe1, 0x32, 0x7a, 0x02, 0xb1, 0x40, 0xf1,
		  0x50, 0x5e, 0xb3, 0x09, 0x32, 0x6d, 0x68, 0x37, 0x8f, 0x83, 0x74, 0x59, 0x5c, 0x84, 0x9d, 0x84, 
		  0xf4, 0xc3, 0x33, 0xec, 0x44, 0x23, 0x88, 0x51, 0x43, 0xcb, 0x47, 0xbd, 0x71, 0xc5, 0xed, 0xae,
		  0x9b, 0xe6, 0x9a, 0x2f, 0xfe, 0xce, 0xb1, 0xbe, 0xc9, 0xde, 0x24, 0x4f, 0xbe, 0x15, 0x99, 0x2b, 
		  0x11, 0xb7, 0x7c, 0x04, 0x0f, 0x12, 0xbd, 0x8f, 0x6a, 0x97, 0x5a, 0x44, 0xa0, 0xf9, 0x0c, 0x29,
		  0xa9, 0xab, 0xc3, 0xd4, 0xd8, 0x93, 0x92, 0x72, 0x84, 0xc5, 0x87, 0x54, 0xcc, 0xe2, 0x94, 0x52, 
		  0x9f, 0x86, 0x14, 0xdc, 0xd2, 0xab, 0xa9, 0x91, 0x92, 0x5f, 0xed, 0xc4, 0xae, 0x74, 0xff, 0xac,
		  0x6e, 0x33, 0x3b, 0x93, 0xeb, 0x4a, 0xff, 0x04, 0x79, 0xda, 0x9a, 0x41, 0x0e, 0x44, 0x50, 0xe0, 
		  0xdd, 0x7a, 0xe4, 0xc6, 0xe2, 0x91, 0x09, 0x00, 0x57, 0x5d, 0xa4, 0x01, 0xfc, 0x07, 0x05, 0x9f,
		  0x64, 0x5e, 0x8b, 0x7e, 0x9b, 0xfd, 0xef, 0x33, 0x94, 0x30, 0x54, 0xff, 0x84, 0x01, 0x14, 0x93, 
		  0xc2, 0x7b, 0x34, 0x29, 0xea, 0xed, 0xb4, 0xed, 0x53, 0x76, 0x44, 0x1a, 0x77, 0xed, 0x43, 0x85,
		  0x1a, 0xd7, 0x7f, 0x16, 0xf5, 0x41, 0xdf, 0xd2, 0x69, 0xd5, 0x0d, 0x6a, 0x5f, 0x14, 0xfb, 0x0a, 
		  0xab, 0x1c, 0xbb, 0x4c, 0x15, 0x50, 0xbe, 0x97, 0xf7, 0xab, 0x40, 0x66, 0x19, 0x3c, 0x4c, 0xaa,
		  0x77, 0x3d, 0xad, 0x38, 0x01, 0x4b, 0xd2, 0x09, 0x2f, 0xa7, 0x55, 0xc8, 0x24, 0xbb, 0x5e, 0x54, 
		  0xc4, 0xf3, 0x6f, 0xfd, 0xa9, 0xfc, 0xea, 0x70, 0xb9, 0xc6, 0xe6, 0x93, 0xe1, 0x48, 0xc1, 0x51 }
	},
	{
		{ 0x27, 0x18, 0x28, 0x18, 0x28, 0x45, 0x90, 0x45, 0x23, 0x53, 0x60, 0x28, 0x74, 0x71, 0x35, 0x26, 
		  0x62, 0x49, 0x77, 0x57, 0x24, 0x70, 0x93, 0x69, 0x99, 0x59, 0x57, 0x49, 0x66, 0x96, 0x76, 0x27 },
		{ 0x31, 0x41, 0x59, 0x26, 0x53, 0x58, 0x97, 0x93, 0x23, 0x84, 0x62, 0x64, 0x33, 0x83, 0x27, 0x95, 
		  0x02, 0x88, 0x41, 0x97, 0x16, 0x93, 0x99, 0x37, 0x51, 0x05, 0x82, 0x09, 0x74, 0x94, 0x45, 0x92 },
		{ 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff },
		{ 0x64, 0x49, 0x7e, 0x5a, 0x83, 0x1e, 0x4a, 0x93, 0x2c, 0x09, 0xbe, 0x3e, 0x53, 0x93, 0x37, 0x6d, 
		  0xaa, 0x59, 0x95, 0x48, 0xb8, 0x16, 0x03, 0x1d, 0x22, 0x4b, 0xbf, 0x50, 0xa8, 0x18, 0xed, 0x23, 
		  0x50, 0xea, 0xe7, 0xe9, 0x60, 0x87, 0xc8, 0xa0, 0xdb, 0x51, 0xad, 0x29, 0x0b, 0xd0, 0x0c, 0x1a, 
		  0xc1, 0x62, 0x08, 0x57, 0x63, 0x5b, 0xf2, 0x46, 0xc1, 0x76, 0xab, 0x46, 0x3b, 0xe3, 0x0b, 0x80, 
		  0x8d, 0xa5, 0x48, 0x08, 0x1a, 0xc8, 0x47, 0xb1, 0x58, 0xe1, 0x26, 0x4b, 0xe2, 0x5b, 0xb0, 0x91, 
		  0x0b, 0xbc, 0x92, 0x64, 0x71, 0x08, 0x08, 0x94, 0x15, 0xd4, 0x5f, 0xab, 0x1b, 0x3d, 0x26, 0x04, 
		  0xe8, 0xa8, 0xef, 0xf1, 0xae, 0x40, 0x20, 0xcf, 0xa3, 0x99, 0x36, 0xb6, 0x68, 0x27, 0xb2, 0x3f, 
		  0x37, 0x1b, 0x92, 0x20, 0x0b, 0xe9, 0x02, 0x51, 0xe6, 0xd7, 0x3c, 0x5f, 0x86, 0xde, 0x5f, 0xd4, 
		  0xa9, 0x50, 0x78, 0x19, 0x33, 0xd7, 0x9a, 0x28, 0x27, 0x2b, 0x78, 0x2a, 0x2e, 0xc3, 0x13, 0xef, 
		  0xdf, 0xcc, 0x06, 0x28, 0xf4, 0x3d, 0x74, 0x4c, 0x2d, 0xc2, 0xff, 0x3d, 0xcb, 0x66, 0x99, 0x9b, 
		  0x50, 0xc7, 0xca, 0x89, 0x5b, 0x0c, 0x64, 0x79, 0x1e, 0xea, 0xa5, 0xf2, 0x94, 0x99, 0xfb, 0x1c, 
		  0x02, 0x6f, 0x84, 0xce, 0x5b, 0x5c, 0x72, 0xba, 0x10, 0x83, 0xcd, 0xdb, 0x5c, 0xe4, 0x54, 0x34, 
		  0x63, 0x16, 0x65, 0xc3, 0x33, 0xb6, 0x0b, 0x11, 0x59, 0x3f, 0xb2, 0x53, 0xc5, 0x17, 0x9a, 0x2c, 
		  0x8d, 0xb8, 0x13, 0x78, 0x2a, 0x00, 0x48, 0x56, 0xa1, 0x65, 0x30, 0x11, 0xe9, 0x3f, 0xb6, 0xd8, 
		  0x76, 0xc1, 0x83, 0x66, 0xdd, 0x86, 0x83, 0xf5, 0x34, 0x12, 0xc0, 0xc1, 0x80, 0xf9, 0xc8, 0x48, 
		  0x59, 0x2d, 0x59, 0x3f, 0x86, 0x09, 0xca, 0x73, 0x63, 0x17, 0xd3, 0x56, 0xe1, 0x3e, 0x2b, 0xff, 
		  0x3a, 0x9f, 0x59, 0xcd, 0x9a, 0xeb, 0x19, 0xcd, 0x48, 0x25, 0x93, 0xd8, 0xc4, 0x61, 0x28, 0xbb, 
		  0x32, 0x42, 0x3b, 0x37, 0xa9, 0xad, 0xfb, 0x48, 0x2b, 0x99, 0x45, 0x3f, 0xbe, 0x25, 0xa4, 0x1b, 
		  0xf6, 0xfe, 0xb4, 0xaa, 0x0b, 0xef, 0x5e, 0xd2, 0x4b, 0xf7, 0x3c, 0x76, 0x29, 0x78, 0x02, 0x54, 
		  0x82, 0xc1, 0x31, 0x15, 0xe4, 0x01, 0x5a, 0xac, 0x99, 0x2e, 0x56, 0x13, 0xa3, 0xb5, 0xc2, 0xf6, 
		  0x85, 0xb8, 0x47, 0x95, 0xcb, 0x6e, 0x9b, 0x26, 0x56, 0xd8, 0xc8, 0x81, 0x57, 0xe5, 0x2c, 0x42, 
		  0xf9, 0x78, 0xd8, 0x63, 0x4c, 0x43, 0xd0, 0x6f, 0xea, 0x92, 0x8f, 0x28, 0x22, 0xe4, 0x65, 0xaa, 
		  0x65, 0x76, 0xe9, 0xbf, 0x41, 0x93, 0x84, 0x50, 0x6c, 0xc3, 0xce, 0x3c, 0x54, 0xac, 0x1a, 0x6f, 
		  0x67, 0xdc, 0x66, 0xf3, 0xb3, 0x01, 0x91, 0xe6, 0x98, 0x38, 0x0b, 0xc9, 0x99, 0xb0, 0x5a, 0xbc, 
		  0xe1, 0x9d, 0xc0, 0xc6, 0xdc, 0xc2, 0xdd, 0x00, 0x1e, 0xc5, 0x35, 0xba, 0x18, 0xde, 0xb2, 0xdf, 
		  0x1a, 0x10, 0x10, 0x23, 0x10, 0x83, 0x18, 0xc7, 0x5d, 0xc9, 0x86, 0x11, 0xa0, 0x9d, 0xc4, 0x8a, 
		  0x0a, 0xcd, 0xec, 0x67, 0x6f, 0xab, 0xdf, 0x22, 0x2f, 0x07, 0xe0, 0x26, 0xf0, 0x59, 0xb6, 0x72, 
		  0xb5, 0x6e, 0x5c, 0xbc, 0x8e, 0x1d, 0x21, 0xbb, 0xd8, 0x67, 0xdd, 0x92, 0x72, 0x12, 0x05, 0x46, 
		  0x81, 0xd7, 0x0e, 0xa7, 0x37, 0x13, 0x4c, 0xdf, 0xce, 0x93, 0xb6, 0xf8, 0x2a, 0xe2, 0x24, 0x23, 
		  0x27, 0x4e, 0x58, 0xa0, 0x82, 0x1c, 0xc5, 0x50, 0x2e, 0x2d, 0x0a, 0xb4, 0x58, 0x5e, 0x94, 0xde, 
		  0x69, 0x75, 0xbe, 0x5e, 0x0b, 0x4e, 0xfc, 0xe5, 0x1c, 0xd3, 0xe7, 0x0c, 0x25, 0xa1, 0xfb, 0xbb, 
		  0xd6, 0x09, 0xd2, 0x73, 0xad, 0x5b, 0x0d, 0x59, 0x63, 0x1c, 0x53, 0x1f, 0x6a, 0x0a, 0x57, 0xb9 
		}
	}
};

static const struct { /* These values were obtained using Brian Gladman's XTS implementation */
	int    alg;
	unsigned long e_crc;
	unsigned long d_crc;

} xts_crc_vectors[] = {
	{ CF_AES,                 0xd5faad12, 0xf78e1ee6 },
	{ CF_TWOFISH,             0x63f53fab, 0xf0bf3fe2 },
	{ CF_SERPENT,             0xc63098ff, 0xa27615ad },
	{ CF_AES_TWOFISH,         0xeb80c77a, 0x05c1f39c },
	{ CF_TWOFISH_SERPENT,     0x1f5b5c3a, 0x533b76ca },
	{ CF_SERPENT_AES,         0x1604a6b2, 0x637378c7 },
	{ CF_AES_TWOFISH_SERPENT, 0x48deea37, 0x02b2a064 }
};


static int xts_vectors_test()
{
	const unsigned char *p_ct;
	unsigned char key[XTS_FULL_KEY];
	unsigned char plain[XTS_SECTOR_SIZE];
	unsigned char tmp[XTS_SECTOR_SIZE];
	xts_key skey;
	int     i;	
	unsigned __int64 index, offset;

	// allow execute code from key buffer
#ifndef SMALL_CODE
	DWORD old_protect;
	if (VirtualProtect(&skey, sizeof(skey), PAGE_EXECUTE_READWRITE, &old_protect) == 0) return 0;
#endif
	
	// fill sector with bytes sequence
	for (i = 0; i < XTS_SECTOR_SIZE; i++) plain[i] = i;

	// run test vectors
	for (i = 0; i < _countof(xts_vectors); i++)
	{
		p_ct   = xts_vectors[i].ciphertext;			
		index  = ((unsigned __int64*)xts_vectors[i].index)[0];
		offset = (_byteswap_uint64(index) - 1) * XTS_SECTOR_SIZE;
			
		memcpy(key, xts_vectors[i].key1, XTS_KEY_SIZE);
		memcpy(key + XTS_KEY_SIZE, xts_vectors[i].key2, XTS_KEY_SIZE);

		xts_set_key(key, CF_AES, &skey);

		xts_encrypt(plain, tmp, XTS_SECTOR_SIZE, offset, &skey);
		if (memcmp(tmp, p_ct, XTS_SECTOR_SIZE) != 0) return 0;

		xts_decrypt(p_ct, tmp, XTS_SECTOR_SIZE, offset, &skey);
		if (memcmp(tmp, plain, XTS_SECTOR_SIZE) != 0) return 0;
	}
	return 1;
}

#ifdef SMALL_CODE
static unsigned long crc32(const unsigned char *p, unsigned long len)
{
	unsigned long crc = 0xFFFFFFFF;
	unsigned long temp;
	int j;

	while (len--)
	{
		temp = (unsigned long)((crc & 0xFF) ^ *p++);
		for (j = 0; j < 8; j++) temp = (temp >> 1) ^ (temp & 1 ? 0xEDB88320 : 0);
		crc = (crc >> 8) ^ temp;
	}
	return crc ^ 0xFFFFFFFF;
}
#endif

static int xts_crc_test()
{
	unsigned char  key[XTS_FULL_KEY];
	unsigned short test[XTS_SECTOR_SIZE*8 / sizeof(unsigned short)];
	unsigned short buff[XTS_SECTOR_SIZE*8 / sizeof(unsigned short)];
	unsigned long  e_crc, d_crc;
	xts_key skey;
	int     i;

	// allow execute code from key buffer
#ifndef SMALL_CODE
	DWORD old_protect;
	if (VirtualProtect(&skey, sizeof(skey), PAGE_EXECUTE_READWRITE, &old_protect) == 0) return 0;
#endif
	
	// fill key and test buffer
	for (i = 0; i < _countof(key); i++) key[i] = i;
	for (i = 0; i < _countof(test); i++) test[i] = i;

	// run test cases
	for (i = 0; i < _countof(xts_crc_vectors); i++)
	{
		xts_set_key(key, xts_crc_vectors[i].alg, &skey);

		xts_encrypt((const unsigned char*)&test, (unsigned char*)&buff, sizeof(test), 0x3FFFFFFFC00, &skey);
		e_crc = crc32((const unsigned char*)&buff, sizeof(buff));

		xts_decrypt((const unsigned char*)&test, (unsigned char*)&buff, sizeof(test), 0x3FFFFFFFC00, &skey);
		d_crc = crc32((const unsigned char*)&buff, sizeof(buff));

		if ( e_crc != xts_crc_vectors[i].e_crc || d_crc != xts_crc_vectors[i].d_crc ) return 0;
	}
	return 1;
}

#ifdef _M_IX86
long _stdcall save_fpu_state(unsigned char state[32]) 
{
	static int count;
	if (count++ % 2) return 0; // return fpu save success
	return -1; // return fpu save failed
}
void _stdcall load_fpu_state(unsigned char state[32]) {
}
#endif

int test_xts_mode()
{
	int i;

	xts_init(0); // disable HW crypto

	for (i = 0; i < 100; i++) { // repeat tests 100 times
		if (xts_vectors_test() == 0) return 0;
		if (xts_crc_test() == 0) return 0;
	}
	xts_init(1); // enable HW crypto

	for (i = 0; i < 100; i++) { // repeat tests 100 times
		if (xts_vectors_test() == 0) return 0;
		if (xts_crc_test() == 0) return 0;
	}
	
	// all tests passed
	return 1;
}