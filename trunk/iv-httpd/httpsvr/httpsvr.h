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

#pragma once

#include "main.h"
#include "functions.h"

using std::string;
using std::stringstream;

struct Context
{
	mg_context *context;
};

class CHTTP
{
private:
	Context *contexts;

public:
	CHTTP(void);
	~CHTTP(void);

	size_t urlDecode(const char *src, size_t src_len, char *dst, size_t dst_len, bool is_form_url_encoded);
};

extern CHTTP *http;