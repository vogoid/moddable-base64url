#include "xsmc.h"
#include "xsHost.h"

void xs_base64url_encode(xsMachine *the)
{
	static const char b64[] ICACHE_XS6RO2_ATTR = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	xsType		srcType;
	uint8_t		*src, *dst;
	uint32_t	srcSize, dstSize;
	uint8_t		a, b, c;

	srcType = xsmcTypeOf(xsArg(0));
	if (xsStringType == srcType) {
		src = (uint8_t *)xsmcToString(xsArg(0));
		srcSize = c_strlen((char *)src);
	}
	else
		xsmcGetBufferReadable(xsArg(0), (void **)&src, &srcSize);
	dstSize = (((srcSize + 2) / 3) * 4) + 1;

	xsResult = xsStringBuffer(NULL, dstSize);
	if (xsStringType == srcType)
		src = (uint8_t *)xsmcToString(xsArg(0));		// refresh pointer
	else
		xsmcGetBufferReadable(xsArg(0), (void **)&src, &srcSize);
	dst = (uint8_t *)xsmcToString(xsResult);

	while (srcSize > 2) {
		a = c_read8(src++);
		b = c_read8(src++);
		c = c_read8(src++);
		*dst++ = c_read8(b64 + ((a & 0xfc) >> 2));
		*dst++ = c_read8(b64 + (((a & 0x3) << 4) | ((b & 0xf0) >> 4)));
		*dst++ = c_read8(b64 + (((b & 0xf) << 2) | ((c & 0xc0) >> 6)));
		*dst++ = c_read8(b64 + (c & 0x3f));
		srcSize -= 3;
	}

	if (srcSize == 2) {
		a = c_read8(src++);
		b = c_read8(src++);
		*dst++ = c_read8(b64 + ((a & 0xfc) >> 2));
		*dst++ = c_read8(b64 + (((a & 0x3) << 4) | ((b & 0xf0) >> 4)));
		*dst++ = c_read8(b64 + ((b & 0xf) << 2));
		*dst++ = '=';
	}
	else if (srcSize == 1) {
		a = c_read8(src++);
		*dst++ = c_read8(b64 + ((a & 0xfc) >> 2));
		*dst++ = c_read8(b64 + ((a & 0x3) << 4));
		*dst++ = '=';
		*dst++ = '=';
	}

	*dst++ = 0;
}