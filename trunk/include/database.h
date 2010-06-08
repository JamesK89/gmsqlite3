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

#ifndef _INCLUDE_DATABASE_H_
#define _INCLUDE_DATABASE_H_

#include "module.h"
#include <sqlite3.h>

#ifndef CStatement
class CStatement;
#endif

#ifndef sqlite3_callback
typedef int (*sqlite3_callback)(void*,int,char**,char**);
#endif

class CDatabase
{

private:

	sqlite3* m_pDatabase;

public:

	CDatabase(void);
	~CDatabase(void);

	sqlite3* getDatabase(void);

	int open(const char* dbName, int flags=0, const char* zVfs=NULL);
	int close(void);

	bool isOpen(void);

	int setExtendedErrors(bool onoff);

	int getErrorCode(void);
	const char* getErrorMessage(void);

	sqlite3_int64 getLastInsertId(void);

	int getChanges(void);
	int getTotalChanges(void);

	int execute(const char* sql, sqlite3_callback callback=NULL, void* usrPtr=NULL);
	int prepare(CStatement** stmt, const char* sql);

};

#endif
