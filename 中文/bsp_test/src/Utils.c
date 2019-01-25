
#include "includes.h"
static const u8 crc8_tbl[256] = {
	0x00,0x91,0xe3,0x72,0x07,0x96,0xe4,0x75,
	0x0e,0x9f,0xed,0x7c,0x09,0x98,0xea,0x7b,
	0x1c,0x8d,0xff,0x6e,0x1b,0x8a,0xf8,0x69,
	0x12,0x83,0xf1,0x60,0x15,0x84,0xf6,0x67,
	0x38,0xa9,0xdb,0x4a,0x3f,0xae,0xdc,0x4d,
	0x36,0xa7,0xd5,0x44,0x31,0xa0,0xd2,0x43,
	0x24,0xb5,0xc7,0x56,0x23,0xb2,0xc0,0x51,
	0x2a,0xbb,0xc9,0x58,0x2d,0xbc,0xce,0x5f,
	0x70,0xe1,0x93,0x02,0x77,0xe6,0x94,0x05,
	0x7e,0xef,0x9d,0x0c,0x79,0xe8,0x9a,0x0b,
	0x6c,0xfd,0x8f,0x1e,0x6b,0xfa,0x88,0x19,
	0x62,0xf3,0x81,0x10,0x65,0xf4,0x86,0x17,
	0x48,0xd9,0xab,0x3a,0x4f,0xde,0xac,0x3d,
	0x46,0xd7,0xa5,0x34,0x41,0xd0,0xa2,0x33,
	0x54,0xc5,0xb7,0x26,0x53,0xc2,0xb0,0x21,
	0x5a,0xcb,0xb9,0x28,0x5d,0xcc,0xbe,0x2f,
	0xe0,0x71,0x03,0x92,0xe7,0x76,0x04,0x95,
	0xee,0x7f,0x0d,0x9c,0xe9,0x78,0x0a,0x9b,
	0xfc,0x6d,0x1f,0x8e,0xfb,0x6a,0x18,0x89,
	0xf2,0x63,0x11,0x80,0xf5,0x64,0x16,0x87,
	0xd8,0x49,0x3b,0xaa,0xdf,0x4e,0x3c,0xad,
	0xd6,0x47,0x35,0xa4,0xd1,0x40,0x32,0xa3,
	0xc4,0x55,0x27,0xb6,0xc3,0x52,0x20,0xb1,
	0xca,0x5b,0x29,0xb8,0xcd,0x5c,0x2e,0xbf,
	0x90,0x01,0x73,0xe2,0x97,0x06,0x74,0xe5,
	0x9e,0x0f,0x7d,0xec,0x99,0x08,0x7a,0xeb,
	0x8c,0x1d,0x6f,0xfe,0x8b,0x1a,0x68,0xf9,
	0x82,0x13,0x61,0xf0,0x85,0x14,0x66,0xf7,
	0xa8,0x39,0x4b,0xda,0xaf,0x3e,0x4c,0xdd,
	0xa6,0x37,0x45,0xd4,0xa1,0x30,0x42,0xd3,
	0xb4,0x25,0x57,0xc6,0xb3,0x22,0x50,0xc1,
	0xba,0x2b,0x59,0xc8,0xbd,0x2c,0x5e,0xcf
};

static const uint16_t crc16NBS_tbl[256] =
{
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
	0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
	0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
	0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
	0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
	0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
	0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
	0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
	0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
	0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
	0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
	0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
	0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
	0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
	0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
	0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
	0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
	0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
	0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
	0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
	0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
	0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
	0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

#ifndef isdigit
#define in_range(c, lo, up)  ((u8_t)c >= lo && (u8_t)c <= up)
#define isdigit(c)           in_range(c, '0', '9')
#endif
int chk_crc8(u8 *ptr, u32 len)
{
	u8 crc8;
	u8 data;

	crc8=0;
	while (len--!=0) {
		data = *ptr++;
		crc8 = crc8_tbl[crc8^data];
	}
	
	if(crc8==0x00) {return 0;}
	else {return -1;}
}

u8 get_crc8(u8 *ptr, u32 len)
{
	u8 crc8;
	u8 data;

	crc8=0;
	while (len--!=0) {
		data = *ptr++;
		crc8 = crc8_tbl[crc8^data];
	}
	
	return crc8;
}

u8 calculate_crc8(u8 crc8, u8 *ptr, u32 len)
{
	u8 data;

	while (len--!=0) {
		data = *ptr++;
		crc8 = crc8_tbl[crc8^data];
	}
	
	return crc8;
}


/** Get CRC 16 NBS **/
uint16_t get_crc16NBS(uint8_t* ptr, uint32_t len)
{
        uint16_t crc = 0;
        uint8_t  temp;
	while(len--)
	{
		temp  = (unsigned char)(crc >> 8);		// get high byte
		crc <<= 8;						        // get low byte
		crc  ^= crc16NBS_tbl[*ptr ^ temp];
		ptr++;
	}

	return crc;
}

static u32 _cal_crc32(u32 crc_result, u8 data_8)
{
	u8 crc_out[32];
	u8 crc_buf[32];
	u8 in_data_buf[8];
	u32 i;
	u32 flag;

	flag = 0x01;

	for (i = 0; i < 32; i++) {
		crc_out[i] = 0;
	}
	

	for (i = 0; i < 8; i++) {
		in_data_buf[i] = (data_8 >> i) & flag;
	}

	for (i = 0; i < 32; i++) {
		crc_buf[i] = (unsigned char)(crc_result >> i) & flag;
	}

	crc_out[0]  = in_data_buf[1]^in_data_buf[7]^crc_buf[30]^crc_buf[24];
	crc_out[1]  = in_data_buf[0]^in_data_buf[1]^in_data_buf[6]^in_data_buf[7]^crc_buf[31]^crc_buf[30]^crc_buf[25]^crc_buf[24];
	crc_out[2]  = in_data_buf[0]^in_data_buf[1]^in_data_buf[5]^in_data_buf[6]^in_data_buf[7]^crc_buf[31]^crc_buf[30]^crc_buf[26]^crc_buf[25]^crc_buf[24];
	crc_out[3]  = in_data_buf[0]^in_data_buf[4]^in_data_buf[5]^in_data_buf[6]^crc_buf[31]^crc_buf[27]^crc_buf[26]^crc_buf[25];
	crc_out[4]  = in_data_buf[1]^in_data_buf[3]^in_data_buf[4]^in_data_buf[5]^in_data_buf[7]^crc_buf[30]^crc_buf[28]^crc_buf[27]^crc_buf[26]^crc_buf[24];
	crc_out[5]  = in_data_buf[0]^in_data_buf[1]^in_data_buf[2]^in_data_buf[3]^in_data_buf[4]^in_data_buf[6]^in_data_buf[7]^
                 crc_buf[31]^crc_buf[30]^crc_buf[29]^crc_buf[28]^crc_buf[27]^crc_buf[25]^crc_buf[24];
	crc_out[6]  = in_data_buf[0]^in_data_buf[1]^in_data_buf[2]^in_data_buf[3]^in_data_buf[5]^in_data_buf[6]^
                 crc_buf[31]^crc_buf[30]^crc_buf[29]^crc_buf[28]^crc_buf[26]^crc_buf[25];
	crc_out[7]  = in_data_buf[0]^in_data_buf[2]^in_data_buf[4]^in_data_buf[5]^in_data_buf[7]^crc_buf[31]^crc_buf[29]^crc_buf[27]^crc_buf[26]^crc_buf[24];
	crc_out[8]  = in_data_buf[3]^in_data_buf[4]^in_data_buf[6]^in_data_buf[7]^crc_buf[28]^crc_buf[27]^crc_buf[25]^crc_buf[24]^crc_buf[0];
	crc_out[9]  = in_data_buf[2]^in_data_buf[3]^in_data_buf[5]^in_data_buf[6]^crc_buf[29]^crc_buf[28]^crc_buf[26]^crc_buf[25]^crc_buf[1];
	crc_out[10] = in_data_buf[2]^in_data_buf[4]^in_data_buf[5]^in_data_buf[7]^crc_buf[29]^crc_buf[27]^crc_buf[26]^crc_buf[24]^crc_buf[2];
	crc_out[11] = in_data_buf[3]^in_data_buf[4]^in_data_buf[6]^in_data_buf[7]^crc_buf[28]^crc_buf[27]^crc_buf[25]^crc_buf[24]^crc_buf[3];
  
	crc_out[12] = in_data_buf[1]^in_data_buf[2]^in_data_buf[3]^in_data_buf[5]^in_data_buf[6]^in_data_buf[7]^
                 crc_buf[30]^crc_buf[29]^crc_buf[28]^crc_buf[26]^crc_buf[25]^crc_buf[24]^crc_buf[4];
	crc_out[13] = in_data_buf[0]^in_data_buf[1]^in_data_buf[2]^in_data_buf[4]^in_data_buf[5]^in_data_buf[6]^
                 crc_buf[31]^crc_buf[30]^crc_buf[29]^crc_buf[27]^crc_buf[26]^crc_buf[25]^crc_buf[5];
	crc_out[14] = in_data_buf[0]^in_data_buf[1]^in_data_buf[3]^in_data_buf[4]^in_data_buf[5]^crc_buf[31]^crc_buf[30]^crc_buf[28]^crc_buf[27]^crc_buf[26]^crc_buf[6];
	crc_out[15] = in_data_buf[0]^in_data_buf[2]^in_data_buf[3]^in_data_buf[4]^crc_buf[31]^crc_buf[29]^crc_buf[28]^crc_buf[27]^crc_buf[7];
	crc_out[16] = in_data_buf[2]^in_data_buf[3]^in_data_buf[7]^crc_buf[29]^crc_buf[28]^crc_buf[24]^crc_buf[8];
	crc_out[17] = in_data_buf[1]^in_data_buf[2]^in_data_buf[6]^crc_buf[30]^crc_buf[29]^crc_buf[25]^crc_buf[9];
	crc_out[18] = in_data_buf[0]^in_data_buf[1]^in_data_buf[5]^crc_buf[31]^crc_buf[30]^crc_buf[26]^crc_buf[10];
	crc_out[19] = in_data_buf[0]^in_data_buf[4]^crc_buf[31]^crc_buf[27]^crc_buf[11];
	crc_out[20] = in_data_buf[3]^crc_buf[28]^crc_buf[12];
	crc_out[21] = in_data_buf[2]^crc_buf[29]^crc_buf[13];
	crc_out[22] = in_data_buf[7]^crc_buf[24]^crc_buf[14];
	crc_out[23] = in_data_buf[1]^in_data_buf[6]^in_data_buf[7]^crc_buf[30]^crc_buf[25]^crc_buf[24]^crc_buf[15];
	crc_out[24] = in_data_buf[0]^in_data_buf[5]^in_data_buf[6]^crc_buf[31]^crc_buf[26]^crc_buf[25]^crc_buf[16];
	crc_out[25] = in_data_buf[4]^in_data_buf[5]^crc_buf[27]^crc_buf[26]^crc_buf[17];
	crc_out[26] = in_data_buf[1]^in_data_buf[3]^in_data_buf[4]^in_data_buf[7]^crc_buf[30]^crc_buf[28]^crc_buf[27]^crc_buf[24]^crc_buf[18];
	crc_out[27] = in_data_buf[0]^in_data_buf[2]^in_data_buf[3]^in_data_buf[6]^crc_buf[31]^crc_buf[29]^crc_buf[28]^crc_buf[25]^crc_buf[19];
	crc_out[28] = in_data_buf[1]^in_data_buf[2]^in_data_buf[5]^crc_buf[30]^crc_buf[29]^crc_buf[26]^crc_buf[20];
	crc_out[29] = in_data_buf[0]^in_data_buf[1]^in_data_buf[4]^crc_buf[31]^crc_buf[30]^crc_buf[27]^crc_buf[21];
	crc_out[30] = in_data_buf[0]^in_data_buf[3]^crc_buf[31]^crc_buf[28]^crc_buf[22];
	crc_out[31] = in_data_buf[2]^crc_buf[23]^crc_buf[29];
 
	crc_result = 0;
	for (i = 0; i < 32; i++) {
		if (crc_out[i]) {crc_result |= (1<<i);}
	}
	
	return crc_result;
}

u32 get_crc32(u8 *data, u32 data_size)
{
	u32 i;
	u32 val;
	int crc_result = 0xffffffff;
	
	for (i = 0; i < data_size; i++) {	
		crc_result = _cal_crc32(crc_result, data[i]);		
	}

	val = 0;
	for (i = 0; i < 32; i++) {
		if ((crc_result>>i) & 0x1) {val |= (1<<(31-i));}
	}

//	TLS_DBGPRT_INFO("calculate crc -0x%x .\n", ~val);
	return ~val;
}

u32 checksum(u32 *data, u32 length, u32 init)
{
	static long long sum = 0;
	u32 checksum;
	u32 i;

	/*
	    Calculate the checksum.
	*/
	if (!init) {sum = 0;}

	for (i = 0; i < length; i++) {sum+=*(data + i);}
	//checksum = ~((u32)(sum>>32)+(u32)sum);
    checksum = (u32)sum;

	return checksum;
}
u32 checksum_char(u8 *data, u32 length, u32 init)
{
    long long sum = 0;
	u32 checksum;
	u32 i;

	/*
	    Calculate the checksum.
	*/
	if (!init) {sum = 0;}

	for (i = 0; i < length; i++) {sum+=*(data + i);}
	//checksum = ~((u32)(sum>>32)+(u32)sum);
    checksum = (u32)sum;

	return checksum;
}
int atodec(char ch)
{
	int dec = -1;
	
	if ((ch >= '0') && (ch <= '9')) {dec = ch - '0';}

	return dec;
}
#if 1
int strtodec(char *str,int *dec)
{
#if 1
	int i;
	int j;
	int dd;

	i = -1;
	dd = 0;


	while (*str) {
		i = atodec(*str++);
		j++;
		if (i < 0) {break;}
		dd = dd*10 + i;
	}
    *dec = dd;
	return ((i < 0) ? -1 : 0);

#else
	int i;
	int j;
	int dd;
	int sign;

	i = -1;
	dd = 0;
	sign = 1;

	if (*str == '-') {
		str++;
		sign = -1;
	}

	while (*str) {
		i = atodec(*str++);
		j++;
		if (i < 0) {break;}
		dd = dd*10 + i;
	}
	*dec = dd*sign;
	return j;//((i < 0) ? -1 : 0);
#endif
}
#else
int strtodec(int *dec, char *str)
{
	int i;
	int j;
	int dd;
	int sign;

	i = -1;
	dd = 0;
	sign = 1;

	if (*str == '-') {
		str++;
		sign = -1;
	}

	while (*str) {
		i = atodec(*str++);
		j++;
		if (i < 0) {break}
		dd = dd*10 + i;
	}

	*dec = dd*sign;

	return j;//((i < 0) ? -1 : 0);
}
#endif
#if 0

int atohex(char ch)
{
	int hex;

	hex = -1;
	
	if ((ch >= '0') && (ch <= '9')) {hex = ch - '0';}
	else if ((ch >= 'a') && (ch <= 'f')) {hex = ch - 'a' + 0xa;}
	else if ((ch >= 'A') && (ch <= 'F')) {hex = ch - 'A' + 0xa;}

	return hex;
}

int strtohex(u32 *hex, char *str)
{
	int n;
	int i;
	u32 dd;

	n = -1;
	i = 0;
	dd = 0;

	while(*str){
		n = atohex(*str++);
		if (n < 0) {return -1;}
		dd = (dd<<4) + n;
		if (++i > 8){return -1;}
	}

	*hex = dd;

	return (n<0?-1:0);
}

int strtohexarray(u8 array[], int cnt, char *str)
{
	int hex;
	u8 tmp;
	u8 *des;

	des = array;
	
	while (cnt-- > 0) {
		hex = atohex(*str++);
		if (hex < 0) {return -1;}
		else {tmp = (hex << 4) & 0xf0;}

		hex = atohex(*str++);
		if (hex < 0) {return -1;}
		else {tmp = tmp | (hex & 0x0f);}
		
		*des++ = (INT8U) tmp;
	}
	
	return ((*str==0) ? 0 : -1);
}

int strtoip(u32 *ipadr, char * str)
{
	int n;
	u32 i;
	u32 ip;
	char *head;
	char *tail;

	ip = 0;
	head = str;
	tail = str;
	
	for (i = 0; i < 3; ) {
		if (*tail == '.') {
			i++;
			*tail = 0;
			ip <<= 8;
			if (strtodec(&n, head) < 0) {return -1;}
			if ((n < 0) || (n > 255)) {return -1;}
			ip += n;
			*tail = '.';
			head = tail + 1;
		}		
		tail++;
	}

	if (i < 3) {return -1;}

	ip <<= 8;
	if (strtodec(&n, head) < 0) {return -1;}
	if ((n < 0) || (n > 255)) {return -1;}
	ip += n;

	*ipadr = ip;
	
	return ((ip == 0) ? -1 : 0);
}

void iptostr(u32 ip, char *str)
{
	sprintf(str, "%d.%d.%d.%d",
		((ip >> 24) & 0xff),((ip >> 16) & 0xff),\
		((ip >>  8) & 0xff), ((ip >>  0) & 0xff));
}


void mactostr(u8 mac[], char *str)
{
	sprintf(str, "%02x%02x%02x%02x%02x%02x",
		mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
}


int hex_to_digit(int c)
{
	if( '0' <= c && c <= '9' )
		return c - '0';
	if( 'A' <= c && c <= 'F' )
		return c - ('A' - 10);
	if( 'a' <= c && c <= 'f' )
		return c - ('a' - 10);
	return -1;
}

int digit_to_hex(int c)
{
	if( 0 <= c && c <= 9 )
		return c + '0';
	if( 0xA <= c && c <= 0xF )
		return c - 0xA + 'A' ;
	return -1; 
}

int hexstr_to_uinit(char *buf, u32 *d)
{
    int i;
    int len = strlen(buf);
    int c;
    *d = 0;

    if (len > 8)
        return -1;
    for (i=0; i<len; i++) {
        c = hex_to_digit(buf[i]);
        if (c < 0)
            return -1;
        *d = (u8)c | (*d << 4);
    }
    return 0;
}

int string_to_uint(char *buf, u32 *d)
{
    int i;
    int len = strlen(buf);

    if (len > 11 || len == 0)
        return -1;
    for(i=0; i<len; i++) {
        if (!isdigit(buf[i]))
            return -1;
    }
    *d = atoi(buf);
    return 0;
}

int string_to_ipaddr(const char *buf, u8 *addr)
{
	int count = 0, rc = 0;
	int in[4];
	char c;

	rc = sscanf(buf, "%u.%u.%u.%u%c",
		    &in[0], &in[1], &in[2], &in[3], &c);
	if (rc != 4 && (rc != 5 || c != '\n'))
		return -1;
	for (count = 0; count < 4; count++) {
		if (in[count] > 255)
			return -1;
		addr[count] = in[count];
	}
	return 0;
}


char * strdup(const char *s)
{
	char * ret;
	int len;
	if(s == NULL)
		return NULL;
	len = strlen(s) + 1;
	ret = tls_mem_alloc(len);
	if(ret == NULL)
		return NULL;
	memset(ret, 0, len);
	memcpy(ret, s, len-1);
	return ret;
}

char * strndup(const char *s, size_t len)
{
	char * ret;
	if(s == NULL)
		return NULL;
	ret = tls_mem_alloc(len + 1);
	if(ret == NULL)
		return NULL;
	memset(ret, 0, len + 1);
	memcpy(ret, s, len);
	return ret;
}
#endif



