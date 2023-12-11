#ifndef UTF8_H_
#define UTF8_H_

#include <stdbool.h>
#include <wchar.h>

#define _UTF8_IN_
#define _UTF8_OUT_
#define _UTF8_INOUT_

// This project was all based off of https://datatracker.ietf.org/doc/html/rfc3629

/**
 * @brief Gets utf8 encoded string length
 *
 * @param wstr input wide string
 * @return unsigned int length of encoded string (not including null terminator)
 */
unsigned int utf8_encode_len(_UTF8_IN_ const wchar_t *wstr);

/**
 * @brief wide string to encoded utf8
 *
 * @param wstr input wide string
 * @param buf output buffer
 * @param buf_size input max size (not including null terminator), outputs output buffer size
 * @return true if succeeded
 * @return false if failed
 */
bool utf8_encode(_UTF8_IN_ const wchar_t *wstr,
								 _UTF8_OUT_ char *buf,
								 _UTF8_INOUT_ unsigned int *buf_size);

/**
 * @brief Get utf8 decoded string length
 *
 * @param str input encoded string
 * @return unsigned int length of decoded string (not including null terminator)
 */
unsigned int utf8_decode_len(_UTF8_IN_ unsigned char *str);

/**
 * @brief encoded utf8 to wide string
 *
 * @param str input encoded string
 * @param buf output buffer
 * @param buf_size input max size (not including null terminator), outputs output buffer size
 * @return true if succeeded
 * @return false if failed
 */
bool utf8_decode(_UTF8_IN_ const char *str,
								 _UTF8_OUT_ wchar_t *buf,
								 _UTF8_INOUT_ unsigned int *buf_size);

#endif // UTF8_H_
