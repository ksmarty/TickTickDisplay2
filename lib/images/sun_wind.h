const uint32_t sun_wind_width = 32;
const uint32_t sun_wind_height = 32;
const uint8_t sun_wind_data[(32*32)/2] = {
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xB7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9F, 0x20, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x5F, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6F, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0x6F, 0xE4, 0xFF, 0xFF, 0xDF, 0x95, 0xFF, 0xFF, 0xFF, 0x47, 0xFD, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0x0E, 0x00, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6F, 0x00, 0xF5, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0x4F, 0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x00, 0xFC, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x35, 0xFE, 0xFF, 0x8C, 0x65, 0xFA, 0xFF, 0x8F, 0xC3, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E, 0x00, 0x00, 0x00, 0xF9, 0xFF, 0xFF, 0xFF, 0xCC, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xDF, 0x00, 0x00, 0x00, 0x00, 0x60, 0xFF, 0xFF, 0x8F, 0x00, 0xD2, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0x00, 0xB4, 0xDE, 0x08, 0x00, 0xFA, 0xFF, 0x7F, 0x00, 0x00, 0xFE, 
	0xFF, 0xFF, 0xFF, 0xFF, 0x08, 0x50, 0xFF, 0xFF, 0xCF, 0x00, 0xF2, 0xFF, 0xFF, 0x4A, 0x00, 0xF7, 
	0xFF, 0xAB, 0xFC, 0xFF, 0x03, 0xF0, 0xFF, 0xFF, 0xFF, 0x59, 0xFE, 0xFF, 0xFF, 0xFF, 0x01, 0xF4, 
	0x8F, 0x00, 0xB0, 0xFF, 0x01, 0xF4, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xBF, 0x00, 0xF5, 
	0x6F, 0x00, 0xA0, 0xFF, 0x01, 0xF4, 0xFF, 0xFF, 0xFF, 0x4E, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFB, 
	0xFF, 0x99, 0xFA, 0xFF, 0x02, 0xF0, 0xFF, 0xFF, 0xFF, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x60, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x60, 0xFF, 0xFF, 0xFF, 0x7F, 0x44, 0x44, 0x44, 0x54, 0xFB, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x00, 0xF5, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xCF, 0x00, 0x90, 0x3F, 0x11, 0x11, 0x11, 0x11, 0x51, 0xFE, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1C, 0xE1, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x57, 0xFF, 0xFF, 0xFF, 0x7F, 0x55, 0x55, 0x55, 0x55, 0x02, 0x40, 0xFF, 0xFF, 
	0xFF, 0xFF, 0x6F, 0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x4F, 0x00, 0xFE, 0xFF, 
	0xFF, 0xFF, 0x0E, 0x00, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x5F, 0x00, 0xFE, 0xFF, 
	0xFF, 0xFF, 0x4F, 0xC2, 0xFF, 0xFF, 0xEF, 0xA7, 0xFF, 0xFF, 0xFF, 0x7E, 0x04, 0x30, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x00, 0xFF, 0xFF, 0xFF, 0x04, 0x00, 0xD0, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x5F, 0x00, 0xFF, 0xFF, 0xFF, 0x09, 0x30, 0xFB, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x8F, 0x10, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xA6, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	};