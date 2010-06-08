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

#include "database.h"
#include "statement.h"

#define VALIDATE_DATABASE(ret) if( !this->m_pDatabase ) { return ret; }

CDatabase::CDatabase()
{
	Msg("CDatabase\n");
	this->m_pDatabase = NULL;
}

CDatabase::~CDatabase()
{
	this->close();
}


sqlite3* CDatabase::getDatabase(void)
{
	return this->m_pDatabase;
}


int CDatabase::open(const char* dbName, int flags, const char* zVfs) {
	if( this->m_pDatabase ) {
		return SQLITE_ERROR;
	}
	return sqlite3_open_v2(dbName, &this->m_pDatabase, flags, zVfs);
}

int CDatabase::close(void)
{
	VALIDATE_DATABASE(SQLITE_ERROR);
	int retcode = sqlite3_close(this->m_pDatabase);
	this->m_pDatabase = NULL;
	return retcode;
}


bool CDatabase::isOpen(void)
{
	return ( this->m_pDatabase != NULL );
}


int CDatabase::setExtendedErrors(bool onoff)
{
	VALIDATE_DATABASE(SQLITE_ERROR);
	return sqlite3_extended_result_codes(this->m_pDatabase, onoff);
}


int CDatabase::getErrorCode(void)
{
	VALIDATE_DATABASE(SQLITE_ERROR);
	return sqlite3_errcode(this->m_pDatabase);
}

const char* CDatabase::getErrorMessage(void)
{
	VALIDATE_DATABASE(NULL);
	return sqlite3_errmsg(this->m_pDatabase);
}


sqlite3_int64 CDatabase::getLastInsertId(void)
{
	VALIDATE_DATABASE(0);
	return sqlite3_last_insert_rowid(this->m_pDatabase);
}


int CDatabase::getChanges(void)
{
	VALIDATE_DATABASE(0);
	return sqlite3_changes(this->m_pDatabase);
}

int CDatabase::getTotalChanges(void)
{
	VALIDATE_DATABASE(0);
	return sqlite3_total_changes(this->m_pDatabase);
}


int CDatabase::execute(const char* sql, sqlite3_callback callback, void* usrPtr)
{
	VALIDATE_DATABASE(SQLITE_ERROR);
	return sqlite3_exec(this->m_pDatabase, sql, callback, usrPtr, NULL);
}

int CDatabase::prepare(CStatement** stmt, const char* sql)
{

	VALIDATE_DATABASE(NULL);

	if( !stmt ) return SQLITE_ERROR;

	if( *stmt ) {
		(*stmt)->finalize();
		delete (*stmt);
	}

	*stmt = NULL;
	sqlite3_stmt* pStmt = NULL;

	int retcode = sqlite3_prepare_v2(this->m_pDatabase, sql, -1, &pStmt, NULL);

	if( retcode == SQLITE_OK ) {
		*stmt = new CStatement(retcode, pStmt);
	}

	return retcode;

}
