const uint32_t cloud_bolt_width = 32;
const uint32_t cloud_bolt_height = 32;
const uint8_t cloud_bolt_data[(32*32)/2] = {
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDF, 0x59, 0x74, 0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xBF, 0x01, 0x00, 0x00, 0x00, 0xF6, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x05, 0x00, 0x00, 0x00, 0x00, 0x10, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0x5F, 0x00, 0x50, 0xEB, 0xCF, 0x17, 0x00, 0xD0, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0x08, 0x00, 0xFD, 0xFF, 0xFF, 0xFF, 0x05, 0x30, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xDF, 0x01, 0xD0, 0xFF, 0xFF, 0xFF, 0xFF, 0x4F, 0x00, 0xFA, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xAF, 0x02, 0x00, 0xF6, 0xFF, 0xFF, 0xFF, 0xFF, 0xDF, 0x00, 0xF5, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0x03, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x02, 0xF2, 0xFF, 0xFF, 0xFF, 
	0xFF, 0x2F, 0x00, 0x50, 0xB8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x04, 0x60, 0xFB, 0xFF, 0xFF, 
	0xFF, 0x05, 0x30, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x02, 0x00, 0x10, 0xFC, 0xFF, 
	0xEF, 0x00, 0xF2, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x06, 0x00, 0x00, 0xB0, 0xFF, 
	0x9F, 0x00, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xEE, 0x06, 0x10, 0xFA, 
	0x6F, 0x00, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6F, 0x00, 0xF7, 
	0x5F, 0x00, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x44, 0xFC, 
	0x9F, 0x00, 0xFA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xDF, 0x00, 0xF3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0xF9, 0xFF, 0xFF, 
	0xFF, 0x04, 0x50, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xBF, 0x00, 0xF4, 0xFF, 0xFF, 
	0xFF, 0x0E, 0x00, 0x91, 0xCC, 0xBB, 0xBB, 0xBB, 0xCB, 0xFF, 0xFF, 0x2F, 0x00, 0xFC, 0xFF, 0xFF, 
	0xFF, 0xDF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF6, 0xFF, 0x06, 0x60, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0x6F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF6, 0xBF, 0x00, 0x20, 0x02, 0xD2, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xAE, 0x89, 0x88, 0x88, 0x88, 0x88, 0xFE, 0x6F, 0x00, 0x00, 0x00, 0x90, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x33, 0x25, 0x00, 0xF1, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x2F, 0x00, 0xFB, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x06, 0x60, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x02, 0xF1, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3A, 0xFC, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	};
