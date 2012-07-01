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

static SQRegFunction HTTPdobj_funcs[] = 
{
	_DECL_HTTPD_FUNC(constructor,			1, "."),
	_DECL_HTTPD_FUNC(stop,					1, "x"),
	_DECL_HTTPD_FUNC(setOption,				3, "xss|b|i"),
	_DECL_HTTPD_FUNC(getOption,				3, "xs"),
	_DECL_HTTPD_FUNC(setURICallback,		3, "xss"),
	_DECL_HTTPD_FUNC(setErrorCallback,		3, "xis"),
	_DECL_HTTPD_FUNC(setAuthCallback,		3, "xss"),
	_DECL_HTTPD_FUNC(modifyPasswordsFile,	4, "xsss|o"),
	_DECL_HTTPD_FUNC(version,				1, "x"),
	_DECL_HTTPD_FUNC(_typeof,				1, "x"),
	{0, 0, 0, 0}
};

static SQRegFunction Connobj_funcs[] = 
{
	_DECL_HTTPD_FUNC(getVar,				2, "ts"),
	_DECL_HTTPD_FUNC(getHeader,				2, "ts"),
	_DECL_HTTPD_FUNC(write,					2, "ts"),
	_DECL_HTTPD_FUNC(authorize,				1, "t"),
	_DECL_HTTPD_FUNC(urlDecode,				3, "tsb"),
	{0, 0, 0, 0}
};

void _set_integer_slot(HSQUIRRELVM vm, char *name, int val)
{
	sq_pushstring(vm, name, -1);
	sq_pushinteger(vm, val);
	sq_rawset(vm, -3);
}

void _set_string_slot(HSQUIRRELVM vm, char *name, const char *val)
{
	sq_pushstring(vm, name, -1);
	sq_pushstring(vm, val, -1);
	sq_rawset(vm, -3);
}

void _push_request_info(HSQUIRRELVM pVM, const struct mg_request_info *request_info)
{
	sq_newtable(pVM);
	_set_string_slot(pVM, "request_method", request_info->request_method);
	_set_string_slot(pVM, "uri", request_info->uri);
	_set_string_slot(pVM, "query_string", request_info->query_string);
	_set_string_slot(pVM, "post_data", request_info->post_data);
	_set_string_slot(pVM, "remote_user", request_info->remote_user);
	_set_integer_slot(pVM, "remote_ip", request_info->remote_ip);
	_set_integer_slot(pVM, "remote_port", request_info->remote_port);
	_set_integer_slot(pVM, "post_data_len", request_info->post_data_len);
	_set_integer_slot(pVM, "http_version_major", request_info->http_version_major);
	_set_integer_slot(pVM, "http_version_minor", request_info->http_version_minor);
	_set_integer_slot(pVM, "status_code", request_info->status_code);
	_set_integer_slot(pVM, "num_headers", request_info->num_headers);
	
	// The "http_headers" subtable-array
	sq_pushstring(pVM, "http_headers", -1);
	sq_newarray(pVM, 0);

	for(int i = 0; i < request_info->num_headers; ++i)
	{
		if(request_info->http_headers[i].name != NULL && *request_info->http_headers[i].name)
		{
			sq_newtable(pVM);
			_set_string_slot(pVM, "name", request_info->http_headers[i].name);
			_set_string_slot(pVM, "value", request_info->http_headers[i].value);
			sq_arrayappend(pVM, -2);
		}
	}
	sq_rawset(pVM, -3);
}

void Callback(struct mg_connection *conn, const struct mg_request_info *request_info, void *user_data)
{
	Callbacks *callback = (Callbacks *)user_data;
	if(callback->vm)
	{
		int top = sq_gettop(callback->vm);
		sq_pushroottable(callback->vm);
		sq_pushstring(callback->vm, callback->name, -1);

		if(SQ_SUCCEEDED(sq_get(callback->vm, -2)))
		{
			sq_pushroottable(callback->vm);

			// The "connection" table
			sq_newtable(callback->vm);

			for(int i = 0; Connobj_funcs[i].name; ++i)
			{
				SQRegFunction &f = Connobj_funcs[i];
				sq_pushstring(callback->vm, f.name, -1);
				sq_pushuserpointer(callback->vm, conn);
				sq_newclosure(callback->vm, f.f, 1);
				sq_setparamscheck(callback->vm, f.nparamscheck, f.typemask);
				sq_setnativeclosurename(callback->vm, -1, f.name);
				sq_rawset(callback->vm, -3);
			}

			_push_request_info(callback->vm, request_info);
			sq_call(callback->vm, 3, SQFalse, SQTrue);
		}
	}
}

void Log(struct mg_connection *conn, const struct mg_request_info *request_info, void *user_data)
{
	string formated((char *)user_data);
	formated.erase(formated.end() - 1);
	logprintf("[HTTPd] %s", formated.c_str());
}

int _httpd_releasehook(SQUserPointer context, int size)
{
	mg_stop((mg_context *)context);
	return 1;
}

int _httpd_constructor(HSQUIRRELVM pVM)
{
	mg_context *context;
	context = mg_start();

	mg_set_option(context, "root", HTDOCS);
	mg_set_log_callback(context, Log);

	sq_setinstanceup(pVM, 1, context);
	sq_setreleasehook(pVM, 1, _httpd_releasehook);
	return 0;
}

int _httpd_setOption(HSQUIRRELVM pVM)
{
	const char *option;
	string value;

	mg_context *context = NULL;
	sq_getinstanceup(pVM, 1, (SQUserPointer *)&context, 0);

	if(context == NULL)
	{
		return 1;
	}

	sq_getstring(pVM, 2, &option);

	if(sq_gettype(pVM, 3) == OT_BOOL)
	{
		SQBool boolean;
		sq_getbool(pVM, 3, &boolean);
		stringstream stream;
		stream << boolean;
		value = stream.str();
	}
	else if(sq_gettype(pVM, 3) == OT_INTEGER)
	{
		int integer;
		sq_getinteger(pVM, 3, &integer);
		stringstream stream;
		stream << integer;
		value = stream.str();
	}
	else
	{
		const char *param;
		sq_getstring(pVM, 3, &param);
		value.assign(param);
	}

	#ifdef SAFE_MODE
		if(_stricmp(option, "root") == 0)
		{
			value.insert(0, HTDOCS);
		}
		else if(_stricmp(option, "aliases") == 0)
		{
			sq_pushinteger(pVM, 0); // 0 on error
			return 1;
		}
	#endif

	sq_pushinteger(pVM, mg_set_option(context, option, value.c_str()));
	return 1;
}

int _httpd_getOption(HSQUIRRELVM pVM)
{
	const char *option;
	const char *value;

	mg_context *context = NULL;
	sq_getinstanceup(pVM, 1, (SQUserPointer *)&context, 0);

	if(context == NULL)
	{
		return 1;
	}

	sq_getstring(pVM, 2, &option);

	value = mg_get_option(context, option);
	sq_pushstring(pVM, value, -1);
	return 1;
}

int _httpd_setURICallback(HSQUIRRELVM pVM)
{
	const char *uri_regex;
	const char *function;

	mg_context *context = NULL;
	sq_getinstanceup(pVM, 1, (SQUserPointer *)&context, 0);

	if(context == NULL)
	{
		return 1;
	}

	sq_getstring(pVM, 2, &uri_regex);
	sq_getstring(pVM, 3, &function);

	Callbacks *callback = new Callbacks;
	callback->name = (char *)function;
	callback->vm = pVM;

	mg_set_uri_callback(context, uri_regex, Callback, (void *)callback);

	sq_pushbool(pVM, SQTrue);
	return 1;
}

int _httpd_setErrorCallback(HSQUIRRELVM pVM)
{
	int error_code;
	const char *function;

	mg_context *context = NULL;
	sq_getinstanceup(pVM, 1, (SQUserPointer *)&context, 0);

	if(context == NULL)
	{
		return 1;
	}

	sq_getinteger(pVM, 2, &error_code);
	sq_getstring(pVM, 3, &function);

	Callbacks *callback = new Callbacks;
	callback->name = (char *)function;
	callback->vm = pVM;

	mg_set_error_callback(context, error_code, Callback, (void *)callback);

	sq_pushbool(pVM, SQTrue);
	return 1;
}

int _httpd_setAuthCallback(HSQUIRRELVM pVM)
{
	const char *uri_regex;
	const char *function;

	mg_context *context = NULL;
	sq_getinstanceup(pVM, 1, (SQUserPointer *)&context, 0);

	if(context == NULL)
	{
		return 1;
	}

	sq_getstring(pVM, 2, &uri_regex);
	sq_getstring(pVM, 3, &function);

	Callbacks *callback = new Callbacks;
	callback->name = (char *)function;
	callback->vm = pVM;

	mg_set_uri_callback(context, uri_regex, Callback, (void *)callback);

	sq_pushbool(pVM, SQTrue);
	return 1;
}

int _httpd_modifyPasswordsFile(HSQUIRRELVM pVM)
{
	const char *file_name;
	const char *user_name;
	const char *password;

	mg_context *context = NULL;
	sq_getinstanceup(pVM, 1, (SQUserPointer *)&context, 0);

	if(context == NULL)
	{
		return 1;
	}

	sq_getstring(pVM, 2, &file_name);
	sq_getstring(pVM, 3, &user_name);

	if(sq_gettype(pVM, 4) == OT_STRING)
	{
		sq_getstring(pVM, 4, &password);
	}
	else if(sq_gettype(pVM, 4) == OT_NULL)
	{
		password = "";
	}

	string formated(file_name);
	formated.insert(0, HTDOCS);

	sq_pushbool(pVM, (SQBool)mg_modify_passwords_file(context, formated.c_str(), user_name, password));
	return 1;
}

int _httpd_getVar(HSQUIRRELVM pVM)
{
	struct mg_connection *conn;
	const char *var_name;
	char *value;

	sq_getstring(pVM, 2, &var_name);
	sq_getuserpointer(pVM, 3, (SQUserPointer *)&conn);

	if((value = mg_get_var(conn, var_name)) == NULL)
	{
		sq_pushnull(pVM);
	}
	else
	{
		sq_pushstring(pVM, value, -1);
	}
	mg_free(value);
	return 1;
}

int _httpd_getHeader(HSQUIRRELVM pVM)
{
	struct mg_connection *conn;
	const char *hdr_name;
	const char *value;

	sq_getstring(pVM, 2, &hdr_name);
	sq_getuserpointer(pVM, 3, (SQUserPointer *)&conn);

	if((value = mg_get_header(conn, hdr_name)) == NULL)
	{
		sq_pushnull(pVM);
	}
	else
	{
		sq_pushstring(pVM, value, -1);
	}
	return 1;
}

int _httpd_write(HSQUIRRELVM pVM)
{
	struct mg_connection *conn;
	const char *buffer;
	int bytes;

	sq_getstring(pVM, 2, &buffer);
	sq_getuserpointer(pVM, 3, (SQUserPointer *)&conn);

	if((bytes = mg_write(conn, buffer, strlen(buffer))) == -1)
	{
		sq_pushnull(pVM);
	}
	else
	{
		sq_pushinteger(pVM, bytes);
	}
	return 1;
}

int _httpd_authorize(HSQUIRRELVM pVM)
{
	struct mg_connection *conn;
	sq_getuserpointer(pVM, 2, (SQUserPointer *)&conn);

	mg_authorize(conn);
	sq_pushbool(pVM, SQTrue);
	return 1;
}

int _httpd_urlDecode(HSQUIRRELVM pVM)
{
	char *dest;
	const char *src;
	bool is_form;
	size_t len;

	sq_getstring(pVM, 2, &src);
	sq_getbool(pVM, 3, (SQBool *)&is_form);

	if((len = strlen(src)))
	{
		dest = (char *)malloc(len * sizeof(char));
		http->urlDecode(src, len, dest, len, is_form);
		sq_pushstring(pVM, dest, -1);
		free(dest);
		return 1;
	}
	sq_pushnull(pVM);
	return 1;
}

int _httpd_stop(HSQUIRRELVM pVM)
{
	mg_context *context = NULL;
	sq_getinstanceup(pVM, 1, (SQUserPointer *)&context, 0);
	mg_stop(context);
	sq_setinstanceup(pVM, 1, NULL);
	return 1;
}

int _httpd_version(HSQUIRRELVM pVM)
{
	sq_pushstring(pVM, mg_version(), -1);
	return 1;
}

int _httpd__typeof(HSQUIRRELVM pVM)
{
	sq_pushstring(pVM, "HTTPd", -1);
	return 1;
}

int RegisterHTTPFunctions(HSQUIRRELVM pVM)
{
	sq_pushstring(pVM, "HTTPd", -1);
	sq_newclass(pVM, SQFalse);

	for(int i = 0; HTTPdobj_funcs[i].name; ++i)
	{
		SQRegFunction &f = HTTPdobj_funcs[i];
		sq_pushstring(pVM, f.name, -1);
		sq_newclosure(pVM, f.f, 0);
		sq_setparamscheck(pVM, f.nparamscheck, f.typemask);
		sq_setnativeclosurename(pVM, -1, f.name);
		sq_createslot(pVM, -3);
	}
	sq_createslot(pVM, -3);
	return 1;
}
