/*

    Gm_sqlite3 the improved sqlite library for Garry's Mod
    Copyright (C) 2010 James John Kelly Jr

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*/

#ifndef _INCLUDE_STATEMENT_H_
#define _INCLUDE_STATEMENT_H_

#include "module.h"
#include <sqlite3.h>

#ifndef CDatabase
class CDatabase;
#endif

class CStatement
{

private:

	sqlite3_stmt* m_pStmt;

public:

	CStatement(int code, sqlite3_stmt* stmt);
	~CStatement(void);

	int finalize(void);

	const char* getSql(void);

	int step(void);
	int reset(void);
	int clearBindings(void);

	int getNumberOfParameters(void);
	const char* getParameterName(int index);
	int getParameterIndex(const char* name);

	int bindNull(int index);
	int bindNull(const char* name);

	int bindInteger(int index, int value);
	int bindInteger(const char* name, int value);

	int bindInt64(int index, sqlite3_int64 value);
	int bindInt64(const char* name, sqlite3_int64 value);

	int bindFloat(int index, float value);
	int bindFloat(const char* name, float value);

	int bindDouble(int index, double value);
	int bindDouble(const char* name, double value);

	int bindText(int index, const char* value);
	int bindText(const char* name, const char* value);

	int bindBlob(int index, const char* value, int length);
	int bindBlob(const char* name, const char* value, int length);

	int getNumberOfColumns(void);
	const char* getColumnName(int index);
	int getColumnIndex(const char* name);
	int getColumnType(int index);

	int getInteger(int index);
	int getInteger(const char* name);

	sqlite3_int64 getInt64(int index);
	sqlite3_int64 getInt64(const char* name);

	float getFloat(int index);
	float getFloat(const char* name);

	double getDouble(int index);
	double getDouble(const char* name);

	const char* getText(int index);
	const char* getText(const char* name);

	const void* getBlob(int index, int* length);
	const void* getBlob(const char* name, int* length);

};

#endif
