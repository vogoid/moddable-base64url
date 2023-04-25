#include "mbedtls/base64.h"
#include "xsmc.h"
#include "xsHost.h"

void xs_base64url_decode(xsMachine *the)
{
    uint8_t		*src;
	uint32_t	srcLen;

	src = (uint8_t *)xsmcToString(xsArg(0));
	srcLen = c_strlen((char *)src);

    size_t i;
    unsigned char *urlsafe_base64 = (unsigned char *) malloc(srcLen + 4); // Allocate extra space for padding

    if (!urlsafe_base64) {
        return MBEDTLS_ERR_BASE64_BUFFER_TOO_SMALL;
    }

    memcpy(urlsafe_base64, src, srcLen);

    // Replace URL-safe characters '-' -> '+' and '_' -> '/'
    for (i = 0; i < srcLen; i++) {
        if (urlsafe_base64[i] == '-') {
            urlsafe_base64[i] = '+';
        } else if (urlsafe_base64[i] == '_') {
            urlsafe_base64[i] = '/';
        }
    }

    // // // Add required padding '='
    size_t padding = (4 - (srcLen % 4)) % 4;
    for (i = 0; i < padding; i++) {
        urlsafe_base64[srcLen + i] = '=';
    }

    uint8_t dst[srcLen];
    size_t olen = 0;

    int ret = mbedtls_base64_decode(dst, srcLen, &olen, urlsafe_base64, srcLen + padding);
    free(urlsafe_base64);

    xsmcSetArrayBuffer(xsResult, dst, olen);
}