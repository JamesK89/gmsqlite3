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

#include "statement.h"
#include "database.h"

#define VALIDATE_STATEMENT(ret) if( !this->m_pStmt ) { return ret; }

CStatement::CStatement(int code, sqlite3_stmt* stmt)
{
	Msg("CStatement\n");
	ASSERT( stmt != NULL );
	this->m_pStmt = stmt;
}

CStatement::~CStatement(void)
{
	// I had to disable this because the garbage collector would cause a crash right here, so be sure to finalize your statements!
	//this->finalize();
}

int CStatement::finalize(void)
{
	VALIDATE_STATEMENT(SQLITE_ERROR);
	return sqlite3_finalize(this->m_pStmt);
}


const char* CStatement::getSql(void)
{
	VALIDATE_STATEMENT(NULL);
	return sqlite3_sql(this->m_pStmt);
}


int CStatement::step(void)
{
	VALIDATE_STATEMENT(SQLITE_ERROR);
	return sqlite3_step(this->m_pStmt);
}

int CStatement::reset(void)
{
	VALIDATE_STATEMENT(SQLITE_ERROR);
	return sqlite3_reset(this->m_pStmt);
}

int CStatement::clearBindings(void)
{
	VALIDATE_STATEMENT(SQLITE_ERROR);
	return sqlite3_clear_bindings(this->m_pStmt);
}


int CStatement::getNumberOfParameters(void)
{
	VALIDATE_STATEMENT(SQLITE_ERROR);
	return sqlite3_bind_parameter_count(this->m_pStmt);
}

const char* CStatement::getParameterName(int index)
{
	VALIDATE_STATEMENT(NULL);
	return sqlite3_bind_parameter_name(this->m_pStmt, index);
}

int CStatement::getParameterIndex(const char* name)
{
	VALIDATE_STATEMENT(0);
	return sqlite3_bind_parameter_index(this->m_pStmt, name);
}


int CStatement::bindNull(int index)
{
	VALIDATE_STATEMENT(SQLITE_ERROR);
	return sqlite3_bind_null(this->m_pStmt, index);
}

int CStatement::bindNull(const char* name)
{
	return this->bindNull(this->getParameterIndex(name));
}


int CStatement::bindInteger(int index, int value)
{
	VALIDATE_STATEMENT(SQLITE_ERROR);
	return sqlite3_bind_int(this->m_pStmt, index, value);
}

int CStatement::bindInteger(const char* name, int value)
{
	return this->bindInteger(this->getParameterIndex(name), value);
}


int CStatement::bindInt64(int index, sqlite3_int64 value)
{
	VALIDATE_STATEMENT(SQLITE_ERROR);
	return sqlite3_bind_int64(this->m_pStmt, index, value);
}

int CStatement::bindInt64(const char* name, sqlite3_int64 value)
{
	return this->bindInt64(this->getParameterIndex(name), value);
}


int CStatement::bindFloat(int index, float value)
{
	VALIDATE_STATEMENT(SQLITE_ERROR);
	return sqlite3_bind_double(this->m_pStmt, index, (double)value);
}

int CStatement::bindFloat(const char* name, float value)
{
	return this->bindFloat(this->getParameterIndex(name), value);
}


int CStatement::bindDouble(int index, double value)
{
	VALIDATE_STATEMENT(SQLITE_ERROR);
	return sqlite3_bind_double(this->m_pStmt, index, value);
}

int CStatement::bindDouble(const char* name, double value)
{
	return this->bindDouble(this->getParameterIndex(name), value);
}


int CStatement::bindText(int index, const char *value)
{
	VALIDATE_STATEMENT(SQLITE_ERROR);
	return sqlite3_bind_text(this->m_pStmt, index, value, strlen(value)*sizeof(char), SQLITE_TRANSIENT);
}

int CStatement::bindText(const char* name, const char* value)
{
	return this->bindText(this->getParameterIndex(name), value);
}


int CStatement::bindBlob(int index, const char *value, int length)
{
	VALIDATE_STATEMENT(SQLITE_ERROR);
	return sqlite3_bind_blob(this->m_pStmt, index, value, length, SQLITE_TRANSIENT);
}

int CStatement::bindBlob(const char* name, const char *value, int length)
{
	return this->bindBlob(this->getParameterIndex(name), value, length);
}


int CStatement::getNumberOfColumns(void)
{
	VALIDATE_STATEMENT(0);
	return sqlite3_column_count(this->m_pStmt);
}

const char* CStatement::getColumnName(int index)
{
	VALIDATE_STATEMENT(NULL);
	return sqlite3_column_name(this->m_pStmt, index);
}

// SQLite3 doesn't have a sqlite3_column_index function, so we have to do it ourselves for now
int CStatement::getColumnIndex(const char *name)
{
	VALIDATE_STATEMENT(-1);
	int index = -1;
	int length = this->getNumberOfColumns();
	for( int i = 0; i < length; i++ ) {
		if( stricmp(name, this->getColumnName(i)) == 0 ) {
			index = i;
			break;
		}
	}
	return index;
}

int CStatement::getColumnType(int index)
{
	VALIDATE_STATEMENT(0);
	return sqlite3_column_type(this->m_pStmt, index);
}


int CStatement::getInteger(int index)
{
	VALIDATE_STATEMENT(0);
	return sqlite3_column_int(this->m_pStmt, index);
}

int CStatement::getInteger(const char* name)
{
	return this->getInteger(this->getColumnIndex(name));
}


sqlite3_int64 CStatement::getInt64(int index)
{
	VALIDATE_STATEMENT(0);
	return sqlite3_column_int64(this->m_pStmt, index);
}

sqlite3_int64 CStatement::getInt64(const char *name)
{
	return this->getInt64(this->getColumnIndex(name));
}


float CStatement::getFloat(int index)
{
	VALIDATE_STATEMENT(0);
	return (float)sqlite3_column_double(this->m_pStmt, index);
}

float CStatement::getFloat(const char *name)
{
	return this->getFloat(this->getColumnIndex(name));
}


double CStatement::getDouble(int index)
{
	VALIDATE_STATEMENT(0);
	return sqlite3_column_double(this->m_pStmt, index);
}

double CStatement::getDouble(const char *name)
{
	return this->getDouble(this->getColumnIndex(name));
}


const char* CStatement::getText(int index)
{
	VALIDATE_STATEMENT(NULL);
	return (const char*)sqlite3_column_text(this->m_pStmt, index);
}

const char* CStatement::getText(const char *name)
{
	return this->getText(this->getColumnIndex(name));
}


const void* CStatement::getBlob(int index, int* length)
{
	VALIDATE_STATEMENT(NULL);
	if( length ) {
		*length = sqlite3_column_bytes(this->m_pStmt, index);
	}
	return sqlite3_column_blob(this->m_pStmt, index);
}

const void* CStatement::getBlob(const char* name, int* length)
{
	return this->getBlob(this->getColumnIndex(name), length);
}
