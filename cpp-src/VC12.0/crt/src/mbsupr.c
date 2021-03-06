/***
*mbsupr.c - Convert string upper case (MBCS)
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Convert string upper case (MBCS)
*
*******************************************************************************/

#ifdef _MBCS

#include <awint.h>
#include <mtdll.h>
#include <cruntime.h>
#include <ctype.h>
#include <mbdata.h>
#include <mbstring.h>
#include <mbctype.h>
#include <internal.h>
#include <internal_securecrt.h>
#include <locale.h>
#include <setlocal.h>
#include <string.h>


/***
* _mbsupr - Convert string upper case (MBCS)
*
*Purpose:
*       Converts all the lower case characters in a string
*       to upper case in place.   Handles MBCS chars correctly.
*
*Entry:
*       unsigned char *string = pointer to string
*
*Exit:
*       Returns a pointer to the input string.
*       Returns NULL on error.
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

extern "C" errno_t __cdecl _mbsupr_s_l(
        unsigned char *string,
        size_t sizeInBytes,
        _locale_t plocinfo
        )
{
        size_t stringlen;

        /* validation section */
        _VALIDATE_RETURN_ERRCODE((string != NULL && sizeInBytes > 0) || (string == NULL && sizeInBytes == 0), EINVAL);

        if (string == NULL)
        {
            /* nothing to do */
            return 0;
        }

        stringlen = strnlen((char *)string, sizeInBytes);
        if (stringlen >= sizeInBytes)
        {
            _RESET_STRING(string, sizeInBytes);
            _RETURN_DEST_NOT_NULL_TERMINATED(string, sizeInBytes);
        }
        _FILL_STRING(string, sizeInBytes, stringlen + 1);

        unsigned char *cp, *dst;
        _LocaleUpdate _loc_update(plocinfo);

        for (cp = string, dst = string; *cp; ++cp)
        {
            if ( _ismbblead_l(*cp, _loc_update.GetLocaleT()) )
            {


                int retval;
                unsigned char ret[4];

                if ( (retval = __crtLCMapStringA(
                                _loc_update.GetLocaleT(),
                                _loc_update.GetLocaleT()->mbcinfo->mblocalename,
                                LCMAP_UPPERCASE,
                                (const char *)cp,
                                2,
                                (char *)ret,
                                2,
                                _loc_update.GetLocaleT()->mbcinfo->mbcodepage,
                                TRUE )) == 0 )
                {
                    errno = EILSEQ;
                    _RESET_STRING(string, sizeInBytes);
                    return errno;
                }

                *(dst++) = ret[0];
                ++cp;
                if (retval > 1)
                {
                    *(dst++) = ret[1];
                }


            }
            else
                /* single byte, macro version */
                *(dst++) = (unsigned char) _mbbtoupper_l(*cp, _loc_update.GetLocaleT());
        }
        /* null terminate the string */
        *dst = '\0';

        return 0;
}

extern "C" errno_t (__cdecl _mbsupr_s)(
        unsigned char *string,
        size_t sizeInBytes
        )
{
    return _mbsupr_s_l(string, sizeInBytes, NULL);
}

extern "C" unsigned char * (__cdecl _mbsupr_l)(
        unsigned char *string,
        _locale_t plocinfo
        )
{
    return (_mbsupr_s_l(string, (string == NULL ? 0 : (size_t)-1), plocinfo) == 0 ? string : NULL);
}

extern "C" unsigned char * (__cdecl _mbsupr)(
        unsigned char *string
        )
{
    return (_mbsupr_s_l(string, (string == NULL ? 0 : (size_t)-1), NULL) == 0 ? string : NULL);
}

#endif  /* _MBCS */
