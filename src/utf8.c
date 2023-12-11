#include "utf8.h"

unsigned int utf8_encode_len(_UTF8_IN_ const wchar_t *wstr)
{
	if (!wstr)
		return 0;

	unsigned int count = 0;
	unsigned int i = 0;
	while (wstr[i] != 0)
	{
		if (wstr[i] < 0x80)
			count++;
		else if (wstr[i] < 0x800)
			count += 2;
		else if (wstr[i] < 0x10000)
			count += 3;
		else if (wstr[i] < 0x10000)
			count += 4;
		else
			return 0;
		i++;
	}
	return count;
}

bool utf8_encode(_UTF8_IN_ const wchar_t *wstr,
								 _UTF8_INOUT_ char *buf,
								 _UTF8_INOUT_ unsigned int *buf_size)
{
	if (wstr == NULL || buf == NULL || buf_size == NULL)
	{
		*buf_size = 0;
		return false;
	}

	unsigned int wi = 0;
	unsigned int i = 0;
	while (wstr[wi] != 0 && i < *buf_size)
	{
		if (wstr[wi] < 0x80)
		{
			if (i + 1 > *buf_size)
			{
				*buf_size = 0;
				return false;
			}
			buf[i] = (char)wstr[wi] & 0b1111111;
			i++;
			wi++;
		}
		else if (wstr[wi] < 0x800)
		{
			if (i + 2 > *buf_size)
			{
				*buf_size = 0;
				return false;
			}
			buf[i] = (wstr[wi] >> 6) & 0b11111 | (0b110 << 5);
			buf[i + 1] = (wstr[wi] >> 0) & 0b111111 | (0b10 << 6);
			i += 2;
			wi++;
		}
		else if (wstr[wi] < 0x10000)
		{
			if (i + 3 > *buf_size)
			{
				*buf_size = 0;
				return false;
			}
			buf[i] = (wstr[wi] >> 12) & 0b1111 | (0b1110 << 4);
			buf[i + 1] = (wstr[wi] >> 6) & 0b111111 | (0b10 << 6);
			buf[i + 2] = (wstr[wi] >> 0) & 0b111111 | (0b10 << 6);
			i += 3;
			wi++;
		}
		else if (sizeof(wchar_t) > 2 && (unsigned int)wstr[wi] < (unsigned int)0x110000) // prevent Warning C4333
		{
			if (i + 4 > *buf_size)
			{
				*buf_size = 0;
				return false;
			}
			buf[i] = ((unsigned int)wstr[wi] >> 18) & 0b111 | (0b11110 << 3);
			buf[i + 1] = (wstr[wi] >> 12) & 0b111111 | (0b10 << 6);
			buf[i + 2] = (wstr[wi] >> 6) & 0b111111 | (0b10 << 6);
			buf[i + 3] = (wstr[wi] >> 0) & 0b111111 | (0b10 << 6);
			i += 4;
			wi++;
		}
		else
		{
			*buf_size = 0;
			return false;
		}
	}

	*buf_size = i;
	return true;
}

void print_bits(char ch)
{
	for (unsigned int i = 8; i > 0; i--)
		printf("%s", ch & (1 << (i - 1)) ? "1" : "0");
}

unsigned int utf8_decode_len(_UTF8_IN_ const char *str)
{
	if (!str)
		return 0;

	unsigned int count = 0;
	unsigned int i = 0;
	while (str[i] != 0)
	{
		if ((unsigned char)(str[i] >> 7) == 0b0)
		{
			i++;
			count++;
		}
		else if (
				str[i + 1] != 0 &&
				(unsigned char)(str[i] >> 5 & 0b111) == 0b110 &&
				(unsigned char)(str[i + 1] >> 6 & 0b11) == 0b10)
		{
			i += 2;
			count++;
		}
		else if (
				str[i + 1] != 0 &&
				str[i + 2] != 0 &&
				(unsigned char)(str[i] >> 4 & 0b1111) == 0b1110 &&
				(unsigned char)(str[i + 1] >> 6 & 0b11) == 0b10 &&
				(unsigned char)(str[i + 2] >> 6 & 0b11) == 0b10)
		{
			i += 3;
			count++;
		}
		else if (
				sizeof(wchar_t) > 2 &&
				str[i + 1] != 0 &&
				str[i + 2] != 0 &&
				str[i + 3] != 0 &&
				(unsigned char)(str[i] >> 3 & 0b11111) == 0b11110 &&
				(unsigned char)(str[i + 1] >> 6 & 0b11) == 0b10 &&
				(unsigned char)(str[i + 2] >> 6 & 0b11) == 0b10 &&
				(unsigned char)(str[i + 3] >> 6 & 0b11) == 0b10)
		{
			i += 4;
			count++;
		}
		else
			return 0;
	}
	return count;
}

bool utf8_decode(_UTF8_IN_ const char *str,
								 _UTF8_OUT_ wchar_t *buf,
								 _UTF8_INOUT_ unsigned int *buf_size)
{
	if (!str || !buf || !buf_size || !*buf_size)
		return false;

	unsigned int i = 0;
	unsigned int wi = 0;
	while (str[i] != 0)
	{
		if ((unsigned char)(str[i] >> 7) == 0b0)
		{
			buf[wi] = (str[i] & 0b1111111);
			i++;
			wi++;
		}
		else if (
				str[i + 1] != 0 &&
				(unsigned char)(str[i] >> 5 & 0b111) == 0b110 &&
				(unsigned char)(str[i + 1] >> 6 & 0b11) == 0b10)
		{
			buf[wi] = (((str[i] & 0b11111) << 6) | ((str[i + 1] & 0b111111) << 0));
			i += 2;
			wi++;
		}
		else if (
				str[i + 1] != 0 &&
				str[i + 2] != 0 &&
				(unsigned char)(str[i] >> 4 & 0b1111) == 0b1110 &&
				(unsigned char)(str[i + 1] >> 6 & 0b11) == 0b10 &&
				(unsigned char)(str[i + 2] >> 6 & 0b11) == 0b10)
		{
			buf[wi] = (((str[i] & 0b1111) << 12) | ((str[i + 1] & 0b111111) << 6) | ((str[i + 2] & 0b111111) << 0));
			i += 3;
			wi++;
		}
		else if (
				sizeof(wchar_t) > 2 &&
				str[i + 1] != 0 &&
				str[i + 2] != 0 &&
				str[i + 3] != 0 &&
				(unsigned char)(str[i] >> 3 & 0b11111) == 0b11110 &&
				(unsigned char)(str[i + 1] >> 6 & 0b11) == 0b10 &&
				(unsigned char)(str[i + 2] >> 6 & 0b11) == 0b10 &&
				(unsigned char)(str[i + 3] >> 6 & 0b11) == 0b10)
		{
			buf[wi] = (((str[i] & 0b111) << 18) | ((str[i + 1] & 0b111111) << 12) | ((str[i + 2] & 0b111111) << 6) | ((str[i + 3] & 0b111111) << 0));
			i += 4;
			wi++;
		}
		else
		{
			*buf_size = 0;
			return false;
		}
	}

	*buf_size = wi;
	return true;
}
