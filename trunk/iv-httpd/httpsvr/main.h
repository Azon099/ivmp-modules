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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>

#ifdef _WIN32
#	include <windows.h>
#else
#	include <errno.h>
#	include <sys/stat.h>
#	include <ctype.h>
#endif

#include "mongoose.h"
#include "SDK.h"

#ifdef _WIN32
#	pragma comment(lib, "wsock32.lib")
#else
#	define _stricmp strcasecmp
#endif

#define logprintf LogPrintf

#define HTDOCS "files/htdocs/"

// Safe mode disables the server from accessing folders beyond the HTDOCS directory.
// It also disables the "aliases" option, which would let the user grant access
// to any folder on the computer as an URL.
// -
// This is useful for hosting services.

#define SAFE_MODE