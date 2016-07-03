
#include "text.h"

#define LINES_PER_DISPLAY (128)
#define BYTES_PER_SCAN (128 / 4 / 2)
#define BYTES_PER_LINE (232 / 8)


#include "images/alphabet/ascii0x20.xbm"
#include "images/alphabet/ascii0x21.xbm"
#include "images/alphabet/ascii0x22.xbm"
#include "images/alphabet/ascii0x23.xbm"
#include "images/alphabet/ascii0x24.xbm"
#include "images/alphabet/ascii0x25.xbm"
#include "images/alphabet/ascii0x26.xbm"
#include "images/alphabet/ascii0x27.xbm"
#include "images/alphabet/ascii0x28.xbm"
#include "images/alphabet/ascii0x29.xbm"
#include "images/alphabet/ascii0x2a.xbm"
#include "images/alphabet/ascii0x2b.xbm"
#include "images/alphabet/ascii0x2c.xbm"
#include "images/alphabet/ascii0x2d.xbm"
#include "images/alphabet/ascii0x2e.xbm"
#include "images/alphabet/ascii0x2f.xbm"
#include "images/alphabet/ascii0x30.xbm"
#include "images/alphabet/ascii0x31.xbm"
#include "images/alphabet/ascii0x32.xbm"
#include "images/alphabet/ascii0x33.xbm"
#include "images/alphabet/ascii0x34.xbm"
#include "images/alphabet/ascii0x35.xbm"
#include "images/alphabet/ascii0x36.xbm"
#include "images/alphabet/ascii0x37.xbm"
#include "images/alphabet/ascii0x38.xbm"
#include "images/alphabet/ascii0x39.xbm"
#include "images/alphabet/ascii0x3a.xbm"
#include "images/alphabet/ascii0x3b.xbm"
#include "images/alphabet/ascii0x3c.xbm"
#include "images/alphabet/ascii0x3d.xbm"
#include "images/alphabet/ascii0x3e.xbm"
#include "images/alphabet/ascii0x3f.xbm"
#include "images/alphabet/ascii0x40.xbm"
#include "images/alphabet/ascii0x41.xbm"
#include "images/alphabet/ascii0x42.xbm"
#include "images/alphabet/ascii0x43.xbm"
#include "images/alphabet/ascii0x44.xbm"
#include "images/alphabet/ascii0x45.xbm"
#include "images/alphabet/ascii0x46.xbm"
#include "images/alphabet/ascii0x47.xbm"
#include "images/alphabet/ascii0x48.xbm"
#include "images/alphabet/ascii0x49.xbm"
#include "images/alphabet/ascii0x4a.xbm"
#include "images/alphabet/ascii0x4b.xbm"
#include "images/alphabet/ascii0x4c.xbm"
#include "images/alphabet/ascii0x4d.xbm"
#include "images/alphabet/ascii0x4e.xbm"
#include "images/alphabet/ascii0x4f.xbm"
#include "images/alphabet/ascii0x50.xbm"
#include "images/alphabet/ascii0x51.xbm"
#include "images/alphabet/ascii0x52.xbm"
#include "images/alphabet/ascii0x53.xbm"
#include "images/alphabet/ascii0x54.xbm"
#include "images/alphabet/ascii0x55.xbm"
#include "images/alphabet/ascii0x56.xbm"
#include "images/alphabet/ascii0x57.xbm"
#include "images/alphabet/ascii0x58.xbm"
#include "images/alphabet/ascii0x59.xbm"
#include "images/alphabet/ascii0x5a.xbm"
#include "images/alphabet/ascii0x5b.xbm"
#include "images/alphabet/ascii0x5c.xbm"
#include "images/alphabet/ascii0x5d.xbm"
#include "images/alphabet/ascii0x5e.xbm"
#include "images/alphabet/ascii0x5f.xbm"
#include "images/alphabet/ascii0x60.xbm"
#include "images/alphabet/ascii0x61.xbm"
#include "images/alphabet/ascii0x62.xbm"
#include "images/alphabet/ascii0x63.xbm"
#include "images/alphabet/ascii0x64.xbm"
#include "images/alphabet/ascii0x65.xbm"
#include "images/alphabet/ascii0x66.xbm"
#include "images/alphabet/ascii0x67.xbm"
#include "images/alphabet/ascii0x68.xbm"
#include "images/alphabet/ascii0x69.xbm"
#include "images/alphabet/ascii0x6a.xbm"
#include "images/alphabet/ascii0x6b.xbm"
#include "images/alphabet/ascii0x6c.xbm"
#include "images/alphabet/ascii0x6d.xbm"
#include "images/alphabet/ascii0x6e.xbm"
#include "images/alphabet/ascii0x6f.xbm"
#include "images/alphabet/ascii0x70.xbm"
#include "images/alphabet/ascii0x71.xbm"
#include "images/alphabet/ascii0x72.xbm"
#include "images/alphabet/ascii0x73.xbm"
#include "images/alphabet/ascii0x74.xbm"
#include "images/alphabet/ascii0x75.xbm"
#include "images/alphabet/ascii0x76.xbm"
#include "images/alphabet/ascii0x77.xbm"
#include "images/alphabet/ascii0x78.xbm"
#include "images/alphabet/ascii0x79.xbm"
#include "images/alphabet/ascii0x7a.xbm"
#include "images/alphabet/ascii0x7b.xbm"
#include "images/alphabet/ascii0x7c.xbm"
#include "images/alphabet/ascii0x7d.xbm"
#include "images/alphabet/ascii0x7e.xbm"

#if (ascii0x41_width > EPAPER_WIDTH) || (ascii0x41_width == 0)
#error "Invalid character width"
#endif
#if (ascii0x41_height > EPAPER_HEIGHT) || (ascii0x41_height == 0)
#error "Invalid character height"
#endif

#if ascii0x21_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x21 is thus invalid"
#endif
#if ascii0x22_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x22 is thus invalid"
#endif
#if ascii0x23_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x23 is thus invalid"
#endif
#if ascii0x24_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x24 is thus invalid"
#endif
#if ascii0x25_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x25 is thus invalid"
#endif
#if ascii0x26_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x26 is thus invalid"
#endif
#if ascii0x27_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x27 is thus invalid"
#endif
#if ascii0x28_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x28 is thus invalid"
#endif
#if ascii0x29_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x29 is thus invalid"
#endif
#if ascii0x2a_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x2a is thus invalid"
#endif
#if ascii0x2b_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x2b is thus invalid"
#endif
#if ascii0x2c_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x2c is thus invalid"
#endif
#if ascii0x2d_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x2d is thus invalid"
#endif
#if ascii0x2e_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x2e is thus invalid"
#endif
#if ascii0x2f_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x2f is thus invalid"
#endif
#if ascii0x30_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x30 is thus invalid"
#endif
#if ascii0x31_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x31 is thus invalid"
#endif
#if ascii0x32_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x32 is thus invalid"
#endif
#if ascii0x33_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x33 is thus invalid"
#endif
#if ascii0x34_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x34 is thus invalid"
#endif
#if ascii0x35_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x35 is thus invalid"
#endif
#if ascii0x36_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x36 is thus invalid"
#endif
#if ascii0x37_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x37 is thus invalid"
#endif
#if ascii0x38_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x38 is thus invalid"
#endif
#if ascii0x39_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x39 is thus invalid"
#endif
#if ascii0x3a_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x3a is thus invalid"
#endif
#if ascii0x3b_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x3b is thus invalid"
#endif
#if ascii0x3c_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x3c is thus invalid"
#endif
#if ascii0x3d_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x3d is thus invalid"
#endif
#if ascii0x3e_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x3e is thus invalid"
#endif
#if ascii0x3f_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x3f is thus invalid"
#endif
#if ascii0x40_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x40 is thus invalid"
#endif
#if ascii0x41_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x41 is thus invalid"
#endif
#if ascii0x42_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x42 is thus invalid"
#endif
#if ascii0x43_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x43 is thus invalid"
#endif
#if ascii0x44_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x44 is thus invalid"
#endif
#if ascii0x45_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x45 is thus invalid"
#endif
#if ascii0x46_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x46 is thus invalid"
#endif
#if ascii0x47_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x47 is thus invalid"
#endif
#if ascii0x48_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x48 is thus invalid"
#endif
#if ascii0x49_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x49 is thus invalid"
#endif
#if ascii0x4a_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x4a is thus invalid"
#endif
#if ascii0x4b_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x4b is thus invalid"
#endif
#if ascii0x4c_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x4c is thus invalid"
#endif
#if ascii0x4d_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x4d is thus invalid"
#endif
#if ascii0x4e_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x4e is thus invalid"
#endif
#if ascii0x4f_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x4f is thus invalid"
#endif
#if ascii0x50_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x50 is thus invalid"
#endif
#if ascii0x51_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x51 is thus invalid"
#endif
#if ascii0x52_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x52 is thus invalid"
#endif
#if ascii0x53_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x53 is thus invalid"
#endif
#if ascii0x54_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x54 is thus invalid"
#endif
#if ascii0x55_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x55 is thus invalid"
#endif
#if ascii0x56_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x56 is thus invalid"
#endif
#if ascii0x57_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x57 is thus invalid"
#endif
#if ascii0x58_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x58 is thus invalid"
#endif
#if ascii0x59_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x59 is thus invalid"
#endif
#if ascii0x5a_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x5a is thus invalid"
#endif
#if ascii0x5b_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x5b is thus invalid"
#endif
#if ascii0x5c_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x5c is thus invalid"
#endif
#if ascii0x5d_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x5d is thus invalid"
#endif
#if ascii0x5e_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x5e is thus invalid"
#endif
#if ascii0x5f_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x5f is thus invalid"
#endif
#if ascii0x60_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x60 is thus invalid"
#endif
#if ascii0x61_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x61 is thus invalid"
#endif
#if ascii0x62_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x62 is thus invalid"
#endif
#if ascii0x63_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x63 is thus invalid"
#endif
#if ascii0x64_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x64 is thus invalid"
#endif
#if ascii0x65_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x65 is thus invalid"
#endif
#if ascii0x66_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x66 is thus invalid"
#endif
#if ascii0x67_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x67 is thus invalid"
#endif
#if ascii0x68_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x68 is thus invalid"
#endif
#if ascii0x69_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x69 is thus invalid"
#endif
#if ascii0x6a_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x6a is thus invalid"
#endif
#if ascii0x6b_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x6b is thus invalid"
#endif
#if ascii0x6c_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x6c is thus invalid"
#endif
#if ascii0x6d_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x6d is thus invalid"
#endif
#if ascii0x6e_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x6e is thus invalid"
#endif
#if ascii0x6f_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x6f is thus invalid"
#endif
#if ascii0x70_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x70 is thus invalid"
#endif
#if ascii0x71_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x71 is thus invalid"
#endif
#if ascii0x72_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x72 is thus invalid"
#endif
#if ascii0x73_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x73 is thus invalid"
#endif
#if ascii0x74_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x74 is thus invalid"
#endif
#if ascii0x75_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x75 is thus invalid"
#endif
#if ascii0x76_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x76 is thus invalid"
#endif
#if ascii0x77_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x77 is thus invalid"
#endif
#if ascii0x78_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x78 is thus invalid"
#endif
#if ascii0x79_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x79 is thus invalid"
#endif
#if ascii0x7a_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x7a is thus invalid"
#endif
#if ascii0x7b_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x7b is thus invalid"
#endif
#if ascii0x7c_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x7c is thus invalid"
#endif
#if ascii0x7d_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x7d is thus invalid"
#endif
#if ascii0x7e_height != ascii0x41_height
#error "Only fixed-height fonts are currently supported, ascii0x7e is thus invalid"
#endif

#define CHARACTER_HEIGHT ascii0x41_height
#define HEIGHT_SPACING 0
#define WIDTH_SPACING 4

/* Create a space, based on an 'i' */
//#define ascii0x20_width ascii0x69_width
//static const uint8_t ascii0x20_bits[ascii0x20_width] = { 0 };

static const uint8_t *alphabet_bits[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	ascii0x20_bits,
	ascii0x21_bits,
	ascii0x22_bits,
	ascii0x23_bits,
	ascii0x24_bits,
	ascii0x25_bits,
	ascii0x26_bits,
	ascii0x27_bits,
	ascii0x28_bits,
	ascii0x29_bits,
	ascii0x2a_bits,
	ascii0x2b_bits,
	ascii0x2c_bits,
	ascii0x2d_bits,
	ascii0x2e_bits,
	ascii0x2f_bits,
	ascii0x30_bits,
	ascii0x31_bits,
	ascii0x32_bits,
	ascii0x33_bits,
	ascii0x34_bits,
	ascii0x35_bits,
	ascii0x36_bits,
	ascii0x37_bits,
	ascii0x38_bits,
	ascii0x39_bits,
	ascii0x3a_bits,
	ascii0x3b_bits,
	ascii0x3c_bits,
	ascii0x3d_bits,
	ascii0x3e_bits,
	ascii0x3f_bits,
	ascii0x40_bits,
	ascii0x41_bits,
	ascii0x42_bits,
	ascii0x43_bits,
	ascii0x44_bits,
	ascii0x45_bits,
	ascii0x46_bits,
	ascii0x47_bits,
	ascii0x48_bits,
	ascii0x49_bits,
	ascii0x4a_bits,
	ascii0x4b_bits,
	ascii0x4c_bits,
	ascii0x4d_bits,
	ascii0x4e_bits,
	ascii0x4f_bits,
	ascii0x50_bits,
	ascii0x51_bits,
	ascii0x52_bits,
	ascii0x53_bits,
	ascii0x54_bits,
	ascii0x55_bits,
	ascii0x56_bits,
	ascii0x57_bits,
	ascii0x58_bits,
	ascii0x59_bits,
	ascii0x5a_bits,
	ascii0x5b_bits,
	ascii0x5c_bits,
	ascii0x5d_bits,
	ascii0x5e_bits,
	ascii0x5f_bits,
	ascii0x60_bits,
	ascii0x61_bits,
	ascii0x62_bits,
	ascii0x63_bits,
	ascii0x64_bits,
	ascii0x65_bits,
	ascii0x66_bits,
	ascii0x67_bits,
	ascii0x68_bits,
	ascii0x69_bits,
	ascii0x6a_bits,
	ascii0x6b_bits,
	ascii0x6c_bits,
	ascii0x6d_bits,
	ascii0x6e_bits,
	ascii0x6f_bits,
	ascii0x70_bits,
	ascii0x71_bits,
	ascii0x72_bits,
	ascii0x73_bits,
	ascii0x74_bits,
	ascii0x75_bits,
	ascii0x76_bits,
	ascii0x77_bits,
	ascii0x78_bits,
	ascii0x79_bits,
	ascii0x7a_bits,
	ascii0x7b_bits,
	ascii0x7c_bits,
	ascii0x7d_bits,
	ascii0x7e_bits,
	NULL,
};

#define ALIGN_WIDTH(x) ((x+7)/8)
#define GET_BLOCK2(x) (x/8)
#define GET_BLOCK(x) ((x)>>3)
static const uint8_t alphabet_bit_width[] = {
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	ascii0x20_width,
	ascii0x21_width,
	ascii0x22_width,
	ascii0x23_width,
	ascii0x24_width,
	ascii0x25_width,
	ascii0x26_width,
	ascii0x27_width,
	ascii0x28_width,
	ascii0x29_width,
	ascii0x2a_width,
	ascii0x2b_width,
	ascii0x2c_width,
	ascii0x2d_width,
	ascii0x2e_width,
	ascii0x2f_width,
	ascii0x30_width,
	ascii0x31_width,
	ascii0x32_width,
	ascii0x33_width,
	ascii0x34_width,
	ascii0x35_width,
	ascii0x36_width,
	ascii0x37_width,
	ascii0x38_width,
	ascii0x39_width,
	ascii0x3a_width,
	ascii0x3b_width,
	ascii0x3c_width,
	ascii0x3d_width,
	ascii0x3e_width,
	ascii0x3f_width,
	ascii0x40_width,
	ascii0x41_width,
	ascii0x42_width,
	ascii0x43_width,
	ascii0x44_width,
	ascii0x45_width,
	ascii0x46_width,
	ascii0x47_width,
	ascii0x48_width,
	ascii0x49_width,
	ascii0x4a_width,
	ascii0x4b_width,
	ascii0x4c_width,
	ascii0x4d_width,
	ascii0x4e_width,
	ascii0x4f_width,
	ascii0x50_width,
	ascii0x51_width,
	ascii0x52_width,
	ascii0x53_width,
	ascii0x54_width,
	ascii0x55_width,
	ascii0x56_width,
	ascii0x57_width,
	ascii0x58_width,
	ascii0x59_width,
	ascii0x5a_width,
	ascii0x5b_width,
	ascii0x5c_width,
	ascii0x5d_width,
	ascii0x5e_width,
	ascii0x5f_width,
	ascii0x60_width,
	ascii0x61_width,
	ascii0x62_width,
	ascii0x63_width,
	ascii0x64_width,
	ascii0x65_width,
	ascii0x66_width,
	ascii0x67_width,
	ascii0x68_width,
	ascii0x69_width,
	ascii0x6a_width,
	ascii0x6b_width,
	ascii0x6c_width,
	ascii0x6d_width,
	ascii0x6e_width,
	ascii0x6f_width,
	ascii0x70_width,
	ascii0x71_width,
	ascii0x72_width,
	ascii0x73_width,
	ascii0x74_width,
	ascii0x75_width,
	ascii0x76_width,
	ascii0x77_width,
	ascii0x78_width,
	ascii0x79_width,
	ascii0x7a_width,
	ascii0x7b_width,
	ascii0x7c_width,
	ascii0x7d_width,
	ascii0x7e_width,
	0,
};

#define alphabet_byte_width(x) ALIGN_WIDTH(alphabet_bit_width[x])

/* a truthy result indicates that *pimage is safe to use */
/* otherwise, pimage must be reinitialized */
uint8_t text_to_image(const char* ptext, uint8_t length, uint8_t *pimage, uint8_t offset_x, uint8_t offset_y, uint8_t upper_x, uint8_t upper_y) {
    /* Check input properly when you're thinking clearly */
    if (upper_x > EPAPER_WIDTH) {
        printf("1\n");
        return 0;
    }
    if (upper_y > EPAPER_HEIGHT) {
        printf("2\n");
        return 0;
    }

    if ((uint16_t)offset_x + ascii0x41_width > upper_x) {
        printf("3\n");
        return 0;
    }
    if ((uint16_t)offset_y + CHARACTER_HEIGHT + HEIGHT_SPACING > upper_y) {
        printf("4\n");
        return 0;
    }

    if (length == 0) {
        return 0;
    }

    // Easier to let the compiler handle offsets
    uint8_t (*ppimage)[EPAPER_HEIGHT][ALIGN_WIDTH(EPAPER_WIDTH)] = (void*)pimage;

    /* Now replace the image */
    {
        printf("performing a write\n");
        uint32_t x = offset_x;
        uint32_t y = offset_y;
        uint8_t last_line = 0;
        uint8_t temp = 0;
        // Block == byte that contains the 8 values
        // Cell == bit within a block
        uint8_t image_cell, char_cell;
        uint32_t image_block;
        uint32_t char_block;
        for (uint32_t i = 0; i < length; i++) {
            uint8_t c = ptext[i];

            if (c == 0) {
                length = i;
                break;
            }
            if ((uint8_t)c > 0x80) {
                printf("Char violation %02x\n", c);
                return 0;
            }


            const uint8_t (*pchar)[CHARACTER_HEIGHT][alphabet_byte_width(c)] = (void*)alphabet_bits[c];

            // Wrap to next line
            if (x + alphabet_bit_width[c] >= upper_x) {
                // Last line but we have more data, can't fulfill
                if (last_line) {
                    printf("More writing requested but already hit end\n");
                    return 0;
                }

                x = offset_x;
                y += CHARACTER_HEIGHT + HEIGHT_SPACING;
            }

            if (y >= upper_y) {
                printf("Bounds violation y WITHIN REPLACER\n");
                return 0;
            }

            if (y + CHARACTER_HEIGHT >= upper_y) {
                printf("Bounds violation y WITHIN REPLACER\n");
                return 0;
            }

            if (y + CHARACTER_HEIGHT + HEIGHT_SPACING >= upper_y) {
                last_line = 1;
            }

            printf("writing %c (x=%04d) starting at x=%04d, y=%04d\n", c, alphabet_bit_width[c], x, y);
            for (uint8_t yi = 0; yi < CHARACTER_HEIGHT; yi++) {
                if (x + alphabet_bit_width[c] >= upper_x) {
                    printf("Bounds violation x+abw WITHIN REPLACER\n");
                    return 0;
                }
                for (uint8_t xi = 0; xi < alphabet_bit_width[c]; xi++) {
                    image_block = GET_BLOCK(x + xi);
                    image_cell = ((x + xi) % 8);
                    char_block = GET_BLOCK(xi);
                    char_cell = (xi % 8);
                    printf("    x=%02d, xi=%02d    ib=%04d, ic=%04d    cb=%04d, cc=%04d\n", x, xi, image_block, image_cell, char_block, char_cell);

                    temp = (*ppimage)[y+yi][image_block];
                    temp &= ((uint8_t)-1) ^ (1<<image_cell);
                    temp |= (((*pchar)[yi][char_block] >> char_cell) & 1) << image_cell;

                    (*ppimage)[y+yi][image_block] = temp;
                }
            }

            x += alphabet_bit_width[c];
            x += WIDTH_SPACING;
        }
    }

    return 1;
}

