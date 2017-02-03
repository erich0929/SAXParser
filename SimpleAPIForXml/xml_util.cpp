#include "stdafx.h"
#include "xml_util.h"
namespace sax_parser {
	int is_combining_char(int hex_value)
	{
		if (hex_value >= 0x0300 && hex_value <= 0x0345)			return 1;
		else if (hex_value >= 0x0360 && hex_value <= 0x0361)	return 1;
		else if (hex_value >= 0x0483 && hex_value <= 0x0486)	return 1;
		else if (hex_value >= 0x0591 && hex_value <= 0x05A1)	return 1;
		else if (hex_value >= 0x05A3 && hex_value <= 0x05B9)	return 1;
		else if (hex_value >= 0x05BB && hex_value <= 0x05BD)	return 1;
		else if (hex_value == 0x05BF)							return 1;
		else if (hex_value >= 0x05C1 && hex_value <= 0x05C2)	return 1;
		else if (hex_value >= 0x05C4)							return 1;
		else if (hex_value >= 0x064B && hex_value <= 0x0652)	return 1;
		else if (hex_value >= 0x0670)							return 1;
		else if (hex_value >= 0x06D6 && hex_value <= 0x06DC) 	return 1;
		else if (hex_value >= 0x06DD && hex_value <= 0x06DF) 	return 1;
		else if (hex_value >= 0x06E0 && hex_value <= 0x06E4) 	return 1;
		else if (hex_value >= 0x06E7 && hex_value <= 0x06E8) 	return 1;
		else if (hex_value >= 0x06EA && hex_value <= 0x06ED) 	return 1;
		else if (hex_value >= 0x0901 && hex_value <= 0x0903) 	return 1;
		else if (hex_value >= 0x093C)							return 1;
		else if (hex_value >= 0x093E && hex_value <= 0x094C) 	return 1;
		else if (hex_value >= 0x094D)							return 1;
		else if (hex_value >= 0x0951 && hex_value <= 0x0954) 	return 1;
		else if (hex_value >= 0x0962 && hex_value <= 0x0963) 	return 1;
		else if (hex_value >= 0x0981 && hex_value <= 0x0983) 	return 1;
		else if (hex_value >= 0x09BC)							return 1;
		else if (hex_value >= 0x09BE)							return 1;
		else if (hex_value >= 0x09BF)							return 1;
		else if (hex_value >= 0x09C0 && hex_value <= 0x09C4) 	return 1;
		else if (hex_value >= 0x09C7 && hex_value <= 0x09C8) 	return 1;
		else if (hex_value >= 0x09CB && hex_value <= 0x09CD) 	return 1;
		else if (hex_value >= 0x09D7)							return 1;
		else if (hex_value >= 0x09E2 && hex_value <= 0x09E3) 	return 1;
		else if (hex_value >= 0x0A02)							return 1;
		else if (hex_value >= 0x0A3C)							return 1;
		else if (hex_value >= 0x0A3E)							return 1;
		else if (hex_value >= 0x0A3F)							return 1;
		else if (hex_value >= 0x0A40 && hex_value <= 0x0A42) 	return 1;
		else if (hex_value >= 0x0A47 && hex_value <= 0x0A48) 	return 1;
		else if (hex_value >= 0x0A4B && hex_value <= 0x0A4D) 	return 1;
		else if (hex_value >= 0x0A70 && hex_value <= 0x0A71) 	return 1;
		else if (hex_value >= 0x0A81 && hex_value <= 0x0A83) 	return 1;
		else if (hex_value >= 0x0ABC)							return 1;
		else if (hex_value >= 0x0ABE && hex_value <= 0x0AC5) 	return 1;
		else if (hex_value >= 0x0AC7 && hex_value <= 0x0AC9) 	return 1;
		else if (hex_value >= 0x0ACB && hex_value <= 0x0ACD) 	return 1;
		else if (hex_value >= 0x0B01 && hex_value <= 0x0B03) 	return 1;
		else if (hex_value >= 0x0B3C)							return 1;
		else if (hex_value >= 0x0B3E && hex_value <= 0x0B43) 	return 1;
		else if (hex_value >= 0x0B47 && hex_value <= 0x0B48) 	return 1;
		else if (hex_value >= 0x0B4B && hex_value <= 0x0B4D) 	return 1;
		else if (hex_value >= 0x0B56 && hex_value <= 0x0B57) 	return 1;
		else if (hex_value >= 0x0B82 && hex_value <= 0x0B83) 	return 1;
		else if (hex_value >= 0x0BBE && hex_value <= 0x0BC2) 	return 1;
		else if (hex_value >= 0x0BC6 && hex_value <= 0x0BC8) 	return 1;
		else if (hex_value >= 0x0BCA && hex_value <= 0x0BCD) 	return 1;
		else if (hex_value >= 0x0BD7)							return 1;
		else if (hex_value >= 0x0C01 && hex_value <= 0x0C03) 	return 1;
		else if (hex_value >= 0x0C3E && hex_value <= 0x0C44) 	return 1;
		else if (hex_value >= 0x0C46 && hex_value <= 0x0C48) 	return 1;
		else if (hex_value >= 0x0C4A && hex_value <= 0x0C4D) 	return 1;
		else if (hex_value >= 0x0C55 && hex_value <= 0x0C56) 	return 1;
		else if (hex_value >= 0x0C82 && hex_value <= 0x0C83) 	return 1;
		else if (hex_value >= 0x0CBE && hex_value <= 0x0CC4) 	return 1;
		else if (hex_value >= 0x0CC6 && hex_value <= 0x0CC8) 	return 1;
		else if (hex_value >= 0x0CCA && hex_value <= 0x0CCD) 	return 1;
		else if (hex_value >= 0x0CD5 && hex_value <= 0x0CD6) 	return 1;
		else if (hex_value >= 0x0D02 && hex_value <= 0x0D03) 	return 1;
		else if (hex_value >= 0x0D3E && hex_value <= 0x0D43) 	return 1;
		else if (hex_value >= 0x0D46 && hex_value <= 0x0D48) 	return 1;
		else if (hex_value >= 0x0D4A && hex_value <= 0x0D4D) 	return 1;
		else if (hex_value >= 0x0D57)							return 1;
		else if (hex_value >= 0x0E31)							return 1;
		else if (hex_value >= 0x0E34 && hex_value <= 0x0E3A) 	return 1;
		else if (hex_value >= 0x0E47 && hex_value <= 0x0E4E) 	return 1;
		else if (hex_value >= 0x0EB1)							return 1;
		else if (hex_value >= 0x0EB4 && hex_value <= 0x0EB9) 	return 1;
		else if (hex_value >= 0x0EBB && hex_value <= 0x0EBC) 	return 1;
		else if (hex_value >= 0x0EC8 && hex_value <= 0x0ECD) 	return 1;
		else if (hex_value >= 0x0F18 && hex_value <= 0x0F19) 	return 1;
		else if (hex_value >= 0x0F35)							return 1;
		else if (hex_value >= 0x0F37)							return 1;
		else if (hex_value >= 0x0F39)							return 1;
		else if (hex_value >= 0x0F3E)							return 1;
		else if (hex_value >= 0x0F3F)							return 1;
		else if (hex_value >= 0x0F71 && hex_value <= 0x0F84)	return 1;
		else if (hex_value >= 0x0F86 && hex_value <= 0x0F8B)	return 1;
		else if (hex_value >= 0x0F90 && hex_value <= 0x0F95)	return 1;
		else if (hex_value >= 0x0F97)							return 1;
		else if (hex_value >= 0x0F99 && hex_value <= 0x0FAD)	return 1;
		else if (hex_value >= 0x0FB1 && hex_value <= 0x0FB7)	return 1;
		else if (hex_value >= 0x0FB9)							return 1;
		else if (hex_value >= 0x20D0 && hex_value <= 0x20DC)	return 1;
		else if (hex_value >= 0x20E1)							return 1;
		else if (hex_value >= 0x302A && hex_value <= 0x302F)	return 1;
		else if (hex_value >= 0x3099)							return 1;
		else if (hex_value >= 0x309A)							return 1;

		return 0;
	}

	int is_extender_char(int hex_value)
	{
		if (hex_value == 0x00B7)								return 1;
		else if (hex_value == 0x02D0)							return 1;
		else if (hex_value == 0x02D1)							return 1;
		else if (hex_value == 0x0387)							return 1;
		else if (hex_value == 0x0640)							return 1;
		else if (hex_value == 0x0E46)							return 1;
		else if (hex_value == 0x0EC6)							return 1;
		else if (hex_value == 0x3005)							return 1;
		else if (hex_value >= 0x3031 && hex_value <= 0x3035)	return 1;
		else if (hex_value >= 0x309D && hex_value <= 0x309E)	return 1;
		else if (hex_value >= 0x30FC && hex_value <= 0x30FE)	return 1;

		return 0;
	}
	void uc2utf8(int uc, char str_u8[], int *length) {
		if (uc < 0x80)
		{
			str_u8[0] = uc;
			*length = 1;
			return;	
		}
		else
		{
			int count;

			if (uc < 0x800)
				count = 2;
			else if (uc < 0x10000)
			{
				if (uc < 0xd800 || uc >= 0xe000)
					count = 3;
				else {
					*length = 0;
					return;
				}
			}

			else if (uc < 0x110000)
				count = 4;

			else {
				*length = 0;
				return;
			}
			
			switch (count) /* note: code falls through cases! */
			{
			case 4: str_u8[3] = 0x80 | (uc & 0x3f); uc = uc >> 6; uc |= 0x10000;
			case 3: str_u8[2] = 0x80 | (uc & 0x3f); uc = uc >> 6; uc |= 0x800;
			case 2: str_u8[1] = 0x80 | (uc & 0x3f); uc = uc >> 6; uc |= 0xc0;
			/*case 1:*/ str_u8[0] = uc;
			}
			*length = count;
			return;
			
		}
		*length = 0;
		return;
	}
	int get_entity_char(int key, char *ch)
	{
		const int xml_entity_key[] =
		{
			'q' << 24 | 'u' << 16 | 'o' << 8 | 't' ,
			'a' << 16 | 'm' << 8 | 'p',
			'a' << 24 | 'p' << 16 | 'o' << 8 | 's',
			'l' << 8 | 't',
			'g' << 8 | 't'
		};
		const char xml_entity_ref[] = { '"' , '&', '\'', '<', '>' };

		for (int i = 0; i < sizeof(xml_entity_key) / sizeof(int); i++) {
			if (xml_entity_key[i] == key) {
				*ch = xml_entity_ref[i];
				return 1;
			}
		}

		return 0;
	}
}

