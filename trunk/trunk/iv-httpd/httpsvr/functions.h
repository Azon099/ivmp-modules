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

#define _DECL_HTTPD_FUNC(name,nparams,pmask) {#name, _httpd_##name, nparams, pmask}

struct Callbacks
{
	HSQUIRRELVM vm;
	char *name;
};

int _httpd_constructor(HSQUIRRELVM pVM);
int _httpd_setOption(HSQUIRRELVM pVM);
int _httpd_getOption(HSQUIRRELVM pVM);
int _httpd_setURICallback(HSQUIRRELVM pVM);
int _httpd_setErrorCallback(HSQUIRRELVM pVM);
int _httpd_setAuthCallback(HSQUIRRELVM pVM);
int _httpd_modifyPasswordsFile(HSQUIRRELVM pVM);
int _httpd_stop(HSQUIRRELVM pVM);
int _httpd_version(HSQUIRRELVM pVM);
int _httpd__typeof(HSQUIRRELVM pVM);
int _httpd_getVar(HSQUIRRELVM pVM);
int _httpd_getHeader(HSQUIRRELVM pVM);
int _httpd_write(HSQUIRRELVM pVM);
int _httpd_authorize(HSQUIRRELVM pVM);
int _httpd_urlDecode(HSQUIRRELVM pVM);

int RegisterHTTPFunctions(HSQUIRRELVM pVM);