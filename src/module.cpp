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

#include "module.h"
#include "database.h"
#include "statement.h"

#include "LuaDatabase.h"
#include "LuaStatement.h"

ILuaInterface* g_pLua = NULL;

//-----------------------------------------------------------------------------
// Small misc functions
//-----------------------------------------------------------------------------

LUA_FUNCTION(MiscIsStatementComplete)
{

	g_pLua->CheckType(1, GLua::TYPE_STRING);

	g_pLua->Push((float)sqlite3_complete(g_pLua->GetString(1)));
	return 1;

}

LUA_FUNCTION(MiscLibVersion)
{
	
	g_pLua->Push((const char*)sqlite3_libversion());
	return 1;

}

LUA_FUNCTION(MiscLibVersionNumber)
{

	g_pLua->Push((float)sqlite3_libversion_number());
	return 1;

}

LUA_FUNCTION(MiscSourceId)
{

	g_pLua->Push((const char*)sqlite3_sourceid());
	return 1;

}

//-----------------------------------------------------------------------------
// Called when the DLL is being initialized
//-----------------------------------------------------------------------------

int Init(lua_State* L)
{


	sqlite3_initialize();
	g_pLua = Lua();

	// Database object definition
	ILuaObject* pMetaDatabase = g_pLua->GetMetaTable(META_DATABASE, TYPE_DATABASE);
	if( pMetaDatabase )
	{

		// Destructor
		pMetaDatabase->SetMember("__gc", LUA_FUNC(DatabaseDelete));

		ILuaObject* pMembersDatabase = g_pLua->GetNewTable();
		if( pMembersDatabase )
		{

			pMembersDatabase->SetMember("Open",		LUA_FUNC(DatabaseOpen));
			pMembersDatabase->SetMember("Close",	LUA_FUNC(DatabaseClose));

			pMembersDatabase->SetMember("IsOpen",	LUA_FUNC(DatabaseIsOpen));

			pMembersDatabase->SetMember("SetExtendedErrors",	LUA_FUNC(DatabaseExtendedErrors));

			pMembersDatabase->SetMember("LastError",		LUA_FUNC(DatabaseLastError));
			pMembersDatabase->SetMember("LastErrorMessage", LUA_FUNC(DatabaseLastErrorMessage));

			pMembersDatabase->SetMember("LastInsertId",	LUA_FUNC(DatabaseLastInsertId));

			pMembersDatabase->SetMember("Changes",		LUA_FUNC(DatabaseChanges));
			pMembersDatabase->SetMember("TotalChanges",	LUA_FUNC(DatabaseTotalChanges));

			pMembersDatabase->SetMember("Execute",	LUA_FUNC(DatabaseExecute));
			pMembersDatabase->SetMember("Prepare",	LUA_FUNC(DatabasePrepare));

			// Index
			pMetaDatabase->SetMember("__index", pMembersDatabase);

		}
		SAFE_UNREF(pMembersDatabase);

	}
	SAFE_UNREF(pMetaDatabase);

	// Statement object definition
	ILuaObject* pMetaStatement = g_pLua->GetMetaTable(META_STATEMENT, TYPE_STATEMENT);
	if( pMetaStatement )
	{

		// Destructor
		pMetaStatement->SetMember("__gc", LUA_FUNC(StatementDelete));

		ILuaObject* pMembersStatement = g_pLua->GetNewTable();
		if( pMembersStatement )
		{

			pMembersStatement->SetMember("Finalize", LUA_FUNC(StatementFinalize));

			pMembersStatement->SetMember("SQLString", LUA_FUNC(StatementSqlString));

			pMembersStatement->SetMember("Fetch", LUA_FUNC(StatementFetch));
			pMembersStatement->SetMember("Step", LUA_FUNC(StatementStep));
			pMembersStatement->SetMember("Reset", LUA_FUNC(StatementReset));
			pMembersStatement->SetMember("ClearBindings", LUA_FUNC(StatementClearBindings));

			pMembersStatement->SetMember("ParameterCount", LUA_FUNC(StatementParameterCount));
			pMembersStatement->SetMember("GetParameterName", LUA_FUNC(StatementParameterName));
			pMembersStatement->SetMember("GetParameterIndex", LUA_FUNC(StatementParameterIndex));

			pMembersStatement->SetMember("BindNull", LUA_FUNC(StatementBindNull));
			pMembersStatement->SetMember("BindInteger", LUA_FUNC(StatementBindInteger));
			pMembersStatement->SetMember("BindFloat", LUA_FUNC(StatementBindFloat));
			pMembersStatement->SetMember("BindString", LUA_FUNC(StatementBindString));

			pMembersStatement->SetMember("ColumnCount", LUA_FUNC(StatementColumnCount));
			pMembersStatement->SetMember("GetColumnName", LUA_FUNC(StatementColumnName));
			pMembersStatement->SetMember("GetColumnIndex", LUA_FUNC(StatementColumnIndex));
			pMembersStatement->SetMember("GetColumnType", LUA_FUNC(StatementColumnType));

			pMembersStatement->SetMember("GetInteger", LUA_FUNC(StatementGetInteger));
			pMembersStatement->SetMember("GetFloat", LUA_FUNC(StatementGetFloat));
			pMembersStatement->SetMember("GetString", LUA_FUNC(StatementGetString));

			// Index
			pMetaStatement->SetMember("__index", pMembersStatement);

		}
		SAFE_UNREF(pMembersStatement);

	}
	SAFE_UNREF(pMetaStatement);

	// Make our global table
	g_pLua->NewGlobalTable(GLOBAL_TABLE);

	// Add the members
	ILuaObject* pObject = g_pLua->GetGlobal(GLOBAL_TABLE);
	if( pObject )
	{

		// Constructor

		pObject->SetMember("New", LUA_FUNC(DatabaseNew));

		// Misc Functions

		pObject->SetMember("IsStatementComplete", LUA_FUNC(MiscIsStatementComplete));
		pObject->SetMember("LibVersion", LUA_FUNC(MiscLibVersion));
		pObject->SetMember("LibVersionNumber", LUA_FUNC(MiscLibVersionNumber));
		pObject->SetMember("SourceId", LUA_FUNC(MiscSourceId));

		// Constants

		pObject->SetMember( "SQLITE_VERSION",			SQLITE_VERSION );
		pObject->SetMember( "SQLITE_VERSION_NUMBER",	(float)SQLITE_VERSION_NUMBER );
		pObject->SetMember( "SQLITE_SOURCE_ID",			SQLITE_SOURCE_ID );

		pObject->SetMember( "SQLITE_OK",			(float)SQLITE_OK );
		pObject->SetMember( "SQLITE_ERROR",			(float)SQLITE_ERROR );
		pObject->SetMember( "SQLITE_INTERNAL",		(float)SQLITE_INTERNAL );
		pObject->SetMember( "SQLITE_PERM",			(float)SQLITE_PERM );
		pObject->SetMember( "SQLITE_ABORT",			(float)SQLITE_ABORT );
		pObject->SetMember( "SQLITE_BUSY",			(float)SQLITE_BUSY );
		pObject->SetMember( "SQLITE_LOCKED",		(float)SQLITE_LOCKED );
		pObject->SetMember( "SQLITE_NOMEM",			(float)SQLITE_NOMEM );
		pObject->SetMember( "SQLITE_READONLY",		(float)SQLITE_READONLY );
		pObject->SetMember( "SQLITE_INTERRUPT",		(float)SQLITE_INTERRUPT );
		pObject->SetMember( "SQLITE_IOERR",			(float)SQLITE_IOERR );
		pObject->SetMember( "SQLITE_CORRUPT",		(float)SQLITE_CORRUPT );
		pObject->SetMember( "SQLITE_NOTFOUND",		(float)SQLITE_NOTFOUND );
		pObject->SetMember( "SQLITE_FULL",			(float)SQLITE_FULL );
		pObject->SetMember( "SQLITE_CANTOPEN",		(float)SQLITE_CANTOPEN );
		pObject->SetMember( "SQLITE_PROTOCOL",		(float)SQLITE_PROTOCOL );
		pObject->SetMember( "SQLITE_EMPTY",			(float)SQLITE_EMPTY );
		pObject->SetMember( "SQLITE_SCHEMA",		(float)SQLITE_SCHEMA );
		pObject->SetMember( "SQLITE_TOOBIG",		(float)SQLITE_TOOBIG );
		pObject->SetMember( "SQLITE_CONSTRAINT",	(float)SQLITE_CONSTRAINT );
		pObject->SetMember( "SQLITE_MISMATCH",		(float)SQLITE_MISMATCH );
		pObject->SetMember( "SQLITE_MISUSE",		(float)SQLITE_MISUSE );
		pObject->SetMember( "SQLITE_NOLFS",			(float)SQLITE_NOLFS );
		pObject->SetMember( "SQLITE_AUTH",			(float)SQLITE_AUTH );
		pObject->SetMember( "SQLITE_FORMAT",		(float)SQLITE_FORMAT );
		pObject->SetMember( "SQLITE_RANGE",			(float)SQLITE_RANGE );
		pObject->SetMember( "SQLITE_NOTADB",		(float)SQLITE_NOTADB );
		pObject->SetMember( "SQLITE_ROW",			(float)SQLITE_ROW );
		pObject->SetMember( "SQLITE_DONE",			(float)SQLITE_DONE );

		pObject->SetMember( "SQLITE_INTEGER",	(float)SQLITE_INTEGER );
		pObject->SetMember( "SQLITE_FLOAT",		(float)SQLITE_FLOAT );
		pObject->SetMember( "SQLITE_BLOB",		(float)SQLITE_BLOB );
		pObject->SetMember( "SQLITE_NULL",		(float)SQLITE_NULL );
		pObject->SetMember( "SQLITE3_TEXT",		(float)SQLITE3_TEXT );

		pObject->SetMember( "SQLITE_ACCESS_EXISTS",		(float)SQLITE_ACCESS_EXISTS );
		pObject->SetMember( "SQLITE_ACCESS_READWRITE",	(float)SQLITE_ACCESS_READWRITE );
		pObject->SetMember( "SQLITE_ACCESS_READ",		(float)SQLITE_ACCESS_READ );

		pObject->SetMember( "SQLITE_CREATE_INDEX",			(float)SQLITE_CREATE_INDEX );
		pObject->SetMember( "SQLITE_CREATE_TABLE",			(float)SQLITE_CREATE_TABLE );
		pObject->SetMember( "SQLITE_CREATE_TEMP_INDEX",		(float)SQLITE_CREATE_TEMP_INDEX );
		pObject->SetMember( "SQLITE_CREATE_TEMP_TABLE",		(float)SQLITE_CREATE_TEMP_TABLE );
		pObject->SetMember( "SQLITE_CREATE_TEMP_TRIGGER",	(float)SQLITE_CREATE_TEMP_TRIGGER );
		pObject->SetMember( "SQLITE_CREATE_TEMP_VIEW",		(float)SQLITE_CREATE_TEMP_VIEW );
		pObject->SetMember( "SQLITE_CREATE_TRIGGER",		(float)SQLITE_CREATE_TRIGGER );
		pObject->SetMember( "SQLITE_CREATE_VIEW",			(float)SQLITE_CREATE_VIEW );
		pObject->SetMember( "SQLITE_DELETE",				(float)SQLITE_DELETE );
		pObject->SetMember( "SQLITE_DROP_INDEX",			(float)SQLITE_DROP_INDEX );
		pObject->SetMember( "SQLITE_DROP_TABLE",			(float)SQLITE_DROP_TABLE );
		pObject->SetMember( "SQLITE_DROP_TEMP_INDEX",		(float)SQLITE_DROP_TEMP_INDEX );
		pObject->SetMember( "SQLITE_DROP_TEMP_TABLE",		(float)SQLITE_DROP_TEMP_TABLE );
		pObject->SetMember( "SQLITE_DROP_TEMP_TRIGGER",		(float)SQLITE_DROP_TEMP_TRIGGER );
		pObject->SetMember( "SQLITE_DROP_TEMP_VIEW",		(float)SQLITE_DROP_TEMP_VIEW );
		pObject->SetMember( "SQLITE_DROP_TRIGGER",			(float)SQLITE_DROP_TRIGGER );
		pObject->SetMember( "SQLITE_DROP_VIEW",				(float)SQLITE_DROP_VIEW );
		pObject->SetMember( "SQLITE_INSERT",				(float)SQLITE_INSERT );
		pObject->SetMember( "SQLITE_PRAGMA",				(float)SQLITE_PRAGMA );
		pObject->SetMember( "SQLITE_READ",					(float)SQLITE_READ );
		pObject->SetMember( "SQLITE_SELECT",				(float)SQLITE_SELECT );
		pObject->SetMember( "SQLITE_TRANSACTION",			(float)SQLITE_TRANSACTION );
		pObject->SetMember( "SQLITE_UPDATE",				(float)SQLITE_UPDATE );
		pObject->SetMember( "SQLITE_ATTACH",				(float)SQLITE_ATTACH );
		pObject->SetMember( "SQLITE_DETACH",				(float)SQLITE_DETACH );
		pObject->SetMember( "SQLITE_ALTER_TABLE",			(float)SQLITE_ALTER_TABLE );
		pObject->SetMember( "SQLITE_REINDEX",				(float)SQLITE_REINDEX );
		pObject->SetMember( "SQLITE_ANALYZE",				(float)SQLITE_ANALYZE );
		pObject->SetMember( "SQLITE_CREATE_VTABLE",			(float)SQLITE_CREATE_VTABLE );
		pObject->SetMember( "SQLITE_DROP_VTABLE",			(float)SQLITE_DROP_VTABLE );
		pObject->SetMember( "SQLITE_FUNCTION",				(float)SQLITE_FUNCTION );
		pObject->SetMember( "SQLITE_SAVEPOINT",				(float)SQLITE_SAVEPOINT );
		pObject->SetMember( "SQLITE_COPY",					(float)SQLITE_COPY );

		pObject->SetMember( "SQLITE_UTF8",			(float)SQLITE_UTF8 );
		pObject->SetMember( "SQLITE_UTF16LE",		(float)SQLITE_UTF16LE );
		pObject->SetMember( "SQLITE_UTF16BE",		(float)SQLITE_UTF16BE );
		pObject->SetMember( "SQLITE_UTF16",			(float)SQLITE_UTF16 );
		pObject->SetMember( "SQLITE_ANY",			(float)SQLITE_ANY );
		pObject->SetMember( "SQLITE_UTF16_ALIGNED",	(float)SQLITE_UTF16_ALIGNED );

		pObject->SetMember( "SQLITE_CONFIG_SINGLETHREAD",	(float)SQLITE_CONFIG_SINGLETHREAD );
		pObject->SetMember( "SQLITE_CONFIG_MULTITHREAD",	(float)SQLITE_CONFIG_MULTITHREAD );
		pObject->SetMember( "SQLITE_CONFIG_SERIALIZED",		(float)SQLITE_CONFIG_SERIALIZED );
		pObject->SetMember( "SQLITE_CONFIG_MALLOC",			(float)SQLITE_CONFIG_MALLOC );
		pObject->SetMember( "SQLITE_CONFIG_GETMALLOC",		(float)SQLITE_CONFIG_GETMALLOC );
		pObject->SetMember( "SQLITE_CONFIG_SCRATCH",		(float)SQLITE_CONFIG_SCRATCH );
		pObject->SetMember( "SQLITE_CONFIG_PAGECACHE",		(float)SQLITE_CONFIG_PAGECACHE );
		pObject->SetMember( "SQLITE_CONFIG_HEAP",			(float)SQLITE_CONFIG_HEAP );
		pObject->SetMember( "SQLITE_CONFIG_MEMSTATUS",		(float)SQLITE_CONFIG_MEMSTATUS );
		pObject->SetMember( "SQLITE_CONFIG_MUTEX",			(float)SQLITE_CONFIG_MUTEX );
		pObject->SetMember( "SQLITE_CONFIG_GETMUTEX",		(float)SQLITE_CONFIG_GETMUTEX );
		pObject->SetMember( "SQLITE_CONFIG_LOOKASIDE",		(float)SQLITE_CONFIG_LOOKASIDE );
		pObject->SetMember( "SQLITE_CONFIG_PCACHE",			(float)SQLITE_CONFIG_PCACHE );
		pObject->SetMember( "SQLITE_CONFIG_GETPCACHE",		(float)SQLITE_CONFIG_GETPCACHE );
		pObject->SetMember( "SQLITE_CONFIG_LOG",			(float)SQLITE_CONFIG_LOG );

		pObject->SetMember( "SQLITE_DBCONFIG_LOOKASIDE",		(float)SQLITE_DBCONFIG_LOOKASIDE );
		pObject->SetMember( "SQLITE_DBSTATUS_LOOKASIDE_USED",	(float)SQLITE_DBSTATUS_LOOKASIDE_USED );

		pObject->SetMember( "SQLITE_FCNTL_LOCKSTATE",	(float)SQLITE_FCNTL_LOCKSTATE );
		pObject->SetMember( "SQLITE_GET_LOCKPROXYFILE",	(float)SQLITE_GET_LOCKPROXYFILE );
		pObject->SetMember( "SQLITE_SET_LOCKPROXYFILE",	(float)SQLITE_SET_LOCKPROXYFILE );
		pObject->SetMember( "SQLITE_LAST_ERRNO",		(float)SQLITE_LAST_ERRNO );

		pObject->SetMember( "SQLITE_DENY",		(float)SQLITE_DENY );
		pObject->SetMember( "SQLITE_IGNORE",	(float)SQLITE_IGNORE );

		pObject->SetMember( "SQLITE_INDEX_CONSTRAINT_EQ",		(float)SQLITE_INDEX_CONSTRAINT_EQ );
		pObject->SetMember( "SQLITE_INDEX_CONSTRAINT_GT",		(float)SQLITE_INDEX_CONSTRAINT_GT );
		pObject->SetMember( "SQLITE_INDEX_CONSTRAINT_LE",		(float)SQLITE_INDEX_CONSTRAINT_LE );
		pObject->SetMember( "SQLITE_INDEX_CONSTRAINT_LT",		(float)SQLITE_INDEX_CONSTRAINT_LT );
		pObject->SetMember( "SQLITE_INDEX_CONSTRAINT_GE",		(float)SQLITE_INDEX_CONSTRAINT_GE );
		pObject->SetMember( "SQLITE_INDEX_CONSTRAINT_MATCH",	(float)SQLITE_INDEX_CONSTRAINT_MATCH );

		pObject->SetMember( "SQLITE_IOCAP_ATOMIC",		(float)SQLITE_IOCAP_ATOMIC );
		pObject->SetMember( "SQLITE_IOCAP_ATOMIC512",	(float)SQLITE_IOCAP_ATOMIC512 );
		pObject->SetMember( "SQLITE_IOCAP_ATOMIC1K",	(float)SQLITE_IOCAP_ATOMIC1K );
		pObject->SetMember( "SQLITE_IOCAP_ATOMIC2K",	(float)SQLITE_IOCAP_ATOMIC2K );
		pObject->SetMember( "SQLITE_IOCAP_ATOMIC4K",	(float)SQLITE_IOCAP_ATOMIC4K );
		pObject->SetMember( "SQLITE_IOCAP_ATOMIC8K",	(float)SQLITE_IOCAP_ATOMIC8K );
		pObject->SetMember( "SQLITE_IOCAP_ATOMIC16K",	(float)SQLITE_IOCAP_ATOMIC16K );
		pObject->SetMember( "SQLITE_IOCAP_ATOMIC32K",	(float)SQLITE_IOCAP_ATOMIC32K );
		pObject->SetMember( "SQLITE_IOCAP_ATOMIC64K",	(float)SQLITE_IOCAP_ATOMIC64K );
		pObject->SetMember( "SQLITE_IOCAP_SAFE_APPEND",	(float)SQLITE_IOCAP_SAFE_APPEND );
		pObject->SetMember( "SQLITE_IOCAP_SEQUENTIAL",	(float)SQLITE_IOCAP_SEQUENTIAL );

		pObject->SetMember( "SQLITE_IOERR_READ",				(float)SQLITE_IOERR_READ );
		pObject->SetMember( "SQLITE_IOERR_SHORT_READ",			(float)SQLITE_IOERR_SHORT_READ );
		pObject->SetMember( "SQLITE_IOERR_WRITE",				(float)SQLITE_IOERR_WRITE );
		pObject->SetMember( "SQLITE_IOERR_FSYNC",				(float)SQLITE_IOERR_FSYNC );
		pObject->SetMember( "SQLITE_IOERR_DIR_FSYNC",			(float)SQLITE_IOERR_DIR_FSYNC );
		pObject->SetMember( "SQLITE_IOERR_TRUNCATE",			(float)SQLITE_IOERR_TRUNCATE );
		pObject->SetMember( "SQLITE_IOERR_FSTAT",				(float)SQLITE_IOERR_FSTAT );
		pObject->SetMember( "SQLITE_IOERR_UNLOCK",				(float)SQLITE_IOERR_UNLOCK );
		pObject->SetMember( "SQLITE_IOERR_RDLOCK",				(float)SQLITE_IOERR_RDLOCK );
		pObject->SetMember( "SQLITE_IOERR_DELETE",				(float)SQLITE_IOERR_DELETE );
		pObject->SetMember( "SQLITE_IOERR_BLOCKED",				(float)SQLITE_IOERR_BLOCKED );
		pObject->SetMember( "SQLITE_IOERR_NOMEM",				(float)SQLITE_IOERR_NOMEM );
		pObject->SetMember( "SQLITE_IOERR_ACCESS",				(float)SQLITE_IOERR_ACCESS );
		pObject->SetMember( "SQLITE_IOERR_CHECKRESERVEDLOCK",	(float)SQLITE_IOERR_CHECKRESERVEDLOCK );
		pObject->SetMember( "SQLITE_IOERR_LOCK",				(float)SQLITE_IOERR_LOCK );
		pObject->SetMember( "SQLITE_IOERR_CLOSE",				(float)SQLITE_IOERR_CLOSE );
		pObject->SetMember( "SQLITE_IOERR_DIR_CLOSE",			(float)SQLITE_IOERR_DIR_CLOSE );
		pObject->SetMember( "SQLITE_LOCKED_SHAREDCACHE",		(float)SQLITE_LOCKED_SHAREDCACHE );

		pObject->SetMember( "SQLITE_LIMIT_LENGTH",				(float)SQLITE_LIMIT_LENGTH );
		pObject->SetMember( "SQLITE_LIMIT_SQL_LENGTH",			(float)SQLITE_LIMIT_SQL_LENGTH );
		pObject->SetMember( "SQLITE_LIMIT_COLUMN",				(float)SQLITE_LIMIT_COLUMN );
		pObject->SetMember( "SQLITE_LIMIT_EXPR_DEPTH",			(float)SQLITE_LIMIT_EXPR_DEPTH );
		pObject->SetMember( "SQLITE_LIMIT_COMPOUND_SELECT",		(float)SQLITE_LIMIT_COMPOUND_SELECT );
		pObject->SetMember( "SQLITE_LIMIT_VDBE_OP",				(float)SQLITE_LIMIT_VDBE_OP );
		pObject->SetMember( "SQLITE_LIMIT_FUNCTION_ARG",		(float)SQLITE_LIMIT_FUNCTION_ARG );
		pObject->SetMember( "SQLITE_LIMIT_ATTACHED",			(float)SQLITE_LIMIT_ATTACHED );
		pObject->SetMember( "SQLITE_LIMIT_LIKE_PATTERN_LENGTH",	(float)SQLITE_LIMIT_LIKE_PATTERN_LENGTH );
		pObject->SetMember( "SQLITE_LIMIT_VARIABLE_NUMBER",		(float)SQLITE_LIMIT_VARIABLE_NUMBER );
		pObject->SetMember( "SQLITE_LIMIT_TRIGGER_DEPTH",		(float)SQLITE_LIMIT_TRIGGER_DEPTH );

		pObject->SetMember( "SQLITE_LOCK_NONE",			(float)SQLITE_LOCK_NONE );
		pObject->SetMember( "SQLITE_LOCK_SHARED",		(float)SQLITE_LOCK_SHARED );
		pObject->SetMember( "SQLITE_LOCK_RESERVED",		(float)SQLITE_LOCK_RESERVED );
		pObject->SetMember( "SQLITE_LOCK_PENDING",		(float)SQLITE_LOCK_PENDING );
		pObject->SetMember( "SQLITE_LOCK_EXCLUSIVE",	(float)SQLITE_LOCK_EXCLUSIVE );

		pObject->SetMember( "SQLITE_MUTEX_FAST",			(float)SQLITE_MUTEX_FAST );
		pObject->SetMember( "SQLITE_MUTEX_RECURSIVE",		(float)SQLITE_MUTEX_RECURSIVE );
		pObject->SetMember( "SQLITE_MUTEX_STATIC_MASTER",	(float)SQLITE_MUTEX_STATIC_MASTER );
		pObject->SetMember( "SQLITE_MUTEX_STATIC_MEM",		(float)SQLITE_MUTEX_STATIC_MEM );
		pObject->SetMember( "SQLITE_MUTEX_STATIC_MEM2",		(float)SQLITE_MUTEX_STATIC_MEM2 );
		pObject->SetMember( "SQLITE_MUTEX_STATIC_OPEN",		(float)SQLITE_MUTEX_STATIC_OPEN );
		pObject->SetMember( "SQLITE_MUTEX_STATIC_PRNG",		(float)SQLITE_MUTEX_STATIC_PRNG );
		pObject->SetMember( "SQLITE_MUTEX_STATIC_LRU",		(float)SQLITE_MUTEX_STATIC_LRU );
		pObject->SetMember( "SQLITE_MUTEX_STATIC_LRU2",		(float)SQLITE_MUTEX_STATIC_LRU2 );

		pObject->SetMember( "SQLITE_OPEN_READONLY",			(float)SQLITE_OPEN_READONLY );
		pObject->SetMember( "SQLITE_OPEN_READWRITE",		(float)SQLITE_OPEN_READWRITE );
		pObject->SetMember( "SQLITE_OPEN_CREATE",			(float)SQLITE_OPEN_CREATE );
		pObject->SetMember( "SQLITE_OPEN_DELETEONCLOSE",	(float)SQLITE_OPEN_DELETEONCLOSE );
		pObject->SetMember( "SQLITE_OPEN_EXCLUSIVE",		(float)SQLITE_OPEN_EXCLUSIVE );
		pObject->SetMember( "SQLITE_OPEN_AUTOPROXY",		(float)SQLITE_OPEN_AUTOPROXY );
		pObject->SetMember( "SQLITE_OPEN_MAIN_DB",			(float)SQLITE_OPEN_MAIN_DB );
		pObject->SetMember( "SQLITE_OPEN_TEMP_DB",			(float)SQLITE_OPEN_TEMP_DB );
		pObject->SetMember( "SQLITE_OPEN_TRANSIENT_DB",		(float)SQLITE_OPEN_TRANSIENT_DB );
		pObject->SetMember( "SQLITE_OPEN_MAIN_JOURNAL",		(float)SQLITE_OPEN_MAIN_JOURNAL );
		pObject->SetMember( "SQLITE_OPEN_TEMP_JOURNAL",		(float)SQLITE_OPEN_TEMP_JOURNAL );
		pObject->SetMember( "SQLITE_OPEN_SUBJOURNAL",		(float)SQLITE_OPEN_SUBJOURNAL );
		pObject->SetMember( "SQLITE_OPEN_MASTER_JOURNAL",	(float)SQLITE_OPEN_MASTER_JOURNAL );
		pObject->SetMember( "SQLITE_OPEN_NOMUTEX",			(float)SQLITE_OPEN_NOMUTEX );
		pObject->SetMember( "SQLITE_OPEN_FULLMUTEX",		(float)SQLITE_OPEN_FULLMUTEX );
		pObject->SetMember( "SQLITE_OPEN_SHAREDCACHE",		(float)SQLITE_OPEN_SHAREDCACHE );
		pObject->SetMember( "SQLITE_OPEN_PRIVATECACHE",		(float)SQLITE_OPEN_PRIVATECACHE );

		pObject->SetMember( "SQLITE_STATUS_MEMORY_USED",		(float)SQLITE_STATUS_MEMORY_USED );
		pObject->SetMember( "SQLITE_STATUS_PAGECACHE_USED",		(float)SQLITE_STATUS_PAGECACHE_USED );
		pObject->SetMember( "SQLITE_STATUS_PAGECACHE_OVERFLOW",	(float)SQLITE_STATUS_PAGECACHE_OVERFLOW );
		pObject->SetMember( "SQLITE_STATUS_SCRATCH_USED",		(float)SQLITE_STATUS_SCRATCH_USED );
		pObject->SetMember( "SQLITE_STATUS_SCRATCH_OVERFLOW",	(float)SQLITE_STATUS_SCRATCH_OVERFLOW );
		pObject->SetMember( "SQLITE_STATUS_MALLOC_SIZE",		(float)SQLITE_STATUS_MALLOC_SIZE );
		pObject->SetMember( "SQLITE_STATUS_PARSER_STACK",		(float)SQLITE_STATUS_PARSER_STACK );
		pObject->SetMember( "SQLITE_STATUS_PAGECACHE_SIZE",		(float)SQLITE_STATUS_PAGECACHE_SIZE );
		pObject->SetMember( "SQLITE_STATUS_SCRATCH_SIZE",		(float)SQLITE_STATUS_SCRATCH_SIZE );

		pObject->SetMember( "SQLITE_STMTSTATUS_FULLSCAN_STEP",	(float)SQLITE_STMTSTATUS_FULLSCAN_STEP );
		pObject->SetMember( "SQLITE_STMTSTATUS_SORT",			(float)SQLITE_STMTSTATUS_SORT );

		pObject->SetMember( "SQLITE_SYNC_NORMAL",	(float)SQLITE_SYNC_NORMAL );
		pObject->SetMember( "SQLITE_SYNC_FULL",		(float)SQLITE_SYNC_FULL );
		pObject->SetMember( "SQLITE_SYNC_DATAONLY",	(float)SQLITE_SYNC_DATAONLY );

		pObject->SetMember( "SQLITE_TESTCTRL_FIRST",				(float)SQLITE_TESTCTRL_FIRST );
		pObject->SetMember( "SQLITE_TESTCTRL_PRNG_SAVE",			(float)SQLITE_TESTCTRL_PRNG_SAVE );
		pObject->SetMember( "SQLITE_TESTCTRL_PRNG_RESTORE",			(float)SQLITE_TESTCTRL_PRNG_RESTORE );
		pObject->SetMember( "SQLITE_TESTCTRL_PRNG_RESET",			(float)SQLITE_TESTCTRL_PRNG_RESET );
		pObject->SetMember( "SQLITE_TESTCTRL_BITVEC_TEST",			(float)SQLITE_TESTCTRL_BITVEC_TEST );
		pObject->SetMember( "SQLITE_TESTCTRL_FAULT_INSTALL",		(float)SQLITE_TESTCTRL_FAULT_INSTALL );
		pObject->SetMember( "SQLITE_TESTCTRL_BENIGN_MALLOC_HOOKS",	(float)SQLITE_TESTCTRL_BENIGN_MALLOC_HOOKS );
		pObject->SetMember( "SQLITE_TESTCTRL_PENDING_BYTE",			(float)SQLITE_TESTCTRL_PENDING_BYTE );
		pObject->SetMember( "SQLITE_TESTCTRL_ASSERT",				(float)SQLITE_TESTCTRL_ASSERT );
		pObject->SetMember( "SQLITE_TESTCTRL_ALWAYS",				(float)SQLITE_TESTCTRL_ALWAYS );
		pObject->SetMember( "SQLITE_TESTCTRL_RESERVE",				(float)SQLITE_TESTCTRL_RESERVE );
		pObject->SetMember( "SQLITE_TESTCTRL_OPTIMIZATIONS",		(float)SQLITE_TESTCTRL_OPTIMIZATIONS );
		pObject->SetMember( "SQLITE_TESTCTRL_ISKEYWORD",			(float)SQLITE_TESTCTRL_ISKEYWORD );
		pObject->SetMember( "SQLITE_TESTCTRL_LAST",					(float)SQLITE_TESTCTRL_LAST );

	}
	SAFE_UNREF(pObject);

	return 0;

}

//-----------------------------------------------------------------------------
// Called when the DLL is being unloaded. 
//-----------------------------------------------------------------------------

int Shutdown(lua_State* L)
{

	sqlite3_shutdown();

	return 0;

}

//-----------------------------------------------------------------------------
// Module initialization
//-----------------------------------------------------------------------------

GMOD_MODULE( Init, Shutdown );
