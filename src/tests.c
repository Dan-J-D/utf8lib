#include "utf8.h"

#include <stdio.h>
#include <malloc.h>

#define EXAMPLE(name, input, correct_output)                                                                 \
	{                                                                                                          \
		unsigned int len = utf8_encode_len(input);                                                               \
		if (!len)                                                                                                \
		{                                                                                                        \
			printf("ERROR: invalid encoded length on EXAMPLE %s\n", name);                                         \
			return -1;                                                                                             \
		}                                                                                                        \
                                                                                                             \
		unsigned char *output = (unsigned char *)malloc(len + 1);                                                \
		if (!output)                                                                                             \
		{                                                                                                        \
			printf("ERROR: failed malloc()\n");                                                                    \
			return -1;                                                                                             \
		}                                                                                                        \
		memset(output, 0, len + 1);                                                                              \
                                                                                                             \
		if (!utf8_encode(input, output, &len))                                                                   \
		{                                                                                                        \
			printf("ERROR: encoding failed on EXAMPLE %s\n", name);                                                \
			return -1;                                                                                             \
		}                                                                                                        \
                                                                                                             \
		if (memcmp(output, correct_output, strlen(correct_output)) != 0)                                         \
		{                                                                                                        \
			printf("ERROR: incorrect encoding output on EXAMPLE %s\n", name);                                      \
			printf("Got:\n");                                                                                      \
			for (unsigned int i = 0; i < len; i++)                                                                 \
				printf("%02x ", output[i]);                                                                          \
			printf("\n");                                                                                          \
			printf("Expected:\n");                                                                                 \
			for (unsigned int i = 0; i < len; i++)                                                                 \
				printf("%02x ", (unsigned char)correct_output[i]);                                                   \
			free(output);                                                                                          \
			return -1;                                                                                             \
		}                                                                                                        \
                                                                                                             \
		len = utf8_decode_len(output);                                                                           \
		if (!len)                                                                                                \
		{                                                                                                        \
			free(output);                                                                                          \
			printf("ERROR: invalid decoded length on EXAMPLE %s\n", name);                                         \
			return -1;                                                                                             \
		}                                                                                                        \
                                                                                                             \
		wchar_t *decoded_output = (wchar_t *)malloc((len + 1) * 2);                                              \
		if (!decoded_output)                                                                                     \
		{                                                                                                        \
			printf("ERROR: failed malloc()\n");                                                                    \
			return -1;                                                                                             \
		}                                                                                                        \
		memset(decoded_output, 0, (len + 1) * 2);                                                                \
                                                                                                             \
		if (!utf8_decode(output, decoded_output, &len))                                                          \
		{                                                                                                        \
			free(output);                                                                                          \
			free(decoded_output);                                                                                  \
			printf("ERROR: decoding failed on EXAMPLE %s\n", name);                                                \
			return -1;                                                                                             \
		}                                                                                                        \
                                                                                                             \
		if (memcmp(input, decoded_output, len * 2) != 0)                                                         \
		{                                                                                                        \
			printf("ERROR: incorrect decoding output on EXAMPLE %s\n", name);                                      \
			printf("Got:\n");                                                                                      \
			for (unsigned int i = 0; i < len * 2; i += 2)                                                          \
				printf("%02x%02x ", ((unsigned char *)decoded_output)[i], ((unsigned char *)decoded_output)[i + 1]); \
			printf("Expected:\n");                                                                                 \
			for (unsigned int i = 0; i < len * 2; i += 2)                                                          \
				printf("%02x%02x ", ((unsigned char *)input)[i], ((unsigned char *)input)[i + 1]);                   \
			free(output);                                                                                          \
			free(decoded_output);                                                                                  \
			return -1;                                                                                             \
		}                                                                                                        \
                                                                                                             \
		printf("EXAMPLE %s was a success!\n", name);                                                             \
                                                                                                             \
		free(output);                                                                                            \
		free(decoded_output);                                                                                    \
	}

int main(int argc, char **argv)
{
	EXAMPLE("Test 1", L"\u0041\u2262\u0391\u002E", "\x41\xE2\x89\xA2\xCE\x91\x2E");
	EXAMPLE("Test 2", L"\uD55C\uAD6D\uC5B4", "\xED\x95\x9C\xEA\xB5\xAD\xEC\x96\xB4");
	EXAMPLE("Test 3", L"\u65E5\u672C\u8A9E", "\xE6\x97\xA5\xE6\x9C\xAC\xE8\xAA\x9E");
}