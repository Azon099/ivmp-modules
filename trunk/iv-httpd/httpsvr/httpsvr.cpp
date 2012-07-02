/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is IV:MP HTTPd (C) Infernus Group.
 *
 * The Initial Developer of the Original Code is
 * Flávio Henrique Toribio Destro.
 * Portions created by the Initial Developer are Copyright (C) 2010
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Flávio Toribio Destro <flavio_toribio@hotmail.com>
 *   Jones Nathan Sperandio <jones.nathan@hotmail.com>
 *
 * ***** END LICENSE BLOCK ***** */

#include "httpsvr.h"

CHTTP::CHTTP(void)
{
	#ifdef _WIN32
		CreateDirectory(L"files/htdocs", NULL);
	#else
		mkdir("files/htdocs", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	#endif

	logprintf(" HTTPd " __DATE__ " by Flavio Toribio loaded.");
}

CHTTP::~CHTTP(void)
{

}

size_t CHTTP::urlDecode(const char *src, size_t src_len, char *dst, size_t dst_len, bool is_form_url_encoded)
{
	size_t	i, j;
	int	a, b;

	#define	HEXTOI(x)	(isdigit(x) ? x - '0' : x - 'W')

	for(i = j = 0; i < src_len && j < dst_len - 1; i++, j++)
	{
		if(src[i] == '%' &&
		    isxdigit(*(unsigned char *)(src + i + 1)) &&
		    isxdigit(*(unsigned char *)(src + i + 2)))
		{
			a = tolower(*(unsigned char *)(src + i + 1));
			b = tolower(*(unsigned char *)(src + i + 2));
			dst[j] = ((HEXTOI(a) << 4) | HEXTOI(b)) & 0xFF;
			i += 2;
		}
		else if (is_form_url_encoded && src[i] == '+')
		{
			dst[j] = ' ';
		}
		else
		{
			dst[j] = src[i];
		}
	}
	dst[j] = '\0';
	return j;
}