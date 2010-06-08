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

#include "LuaDatabase.h"
#include "database.h"
#include "statement.h"

//-----------------------------------------------------------------------------
// Database functions
//-----------------------------------------------------------------------------

static int DatabaseExecCallback(void* usrPtr, int numColumns, char** values, char** columns)
{

	ASSERT(usrPtr != NULL);
	if( !usrPtr ) return SQLITE_ABORT; // THIS SHOULD NOT BE HAPPENING AAAAAHHHH!!!

	ILuaObject* pColumns = g_pLua->GetNewTable();

	ASSERT(pColumns != NULL);
	if( !pColumns ) { return SQLITE_ABORT; }

	ILuaObject* pValues = g_pLua->GetNewTable();
	
	ASSERT(pValues != NULL);
	if( !pValues ) { SAFE_UNREF(pColumns); return SQLITE_ABORT; }

	for( int i = 0; i < numColumns; i++ ) {
		pColumns->SetMember((float)i, columns[i]);
		pValues->SetMember((float)i, values[i]);
	}

	((ILuaObject*)usrPtr)->Push();
	g_pLua->Push((float)numColumns);
	pColumns->Push();
	pValues->Push();
	g_pLua->Call(3, 1);

	SAFE_UNREF(pColumns);
	SAFE_UNREF(pValues);

	return (int)(g_pLua->GetInteger(1));

}

LUA_FUNCTION(DatabaseNew)
{

	CDatabase* pDatabase = new CDatabase();

	ASSERT(pDatabase != NULL);
	if( pDatabase ) {

		ILuaObject* pMeta = g_pLua->GetMetaTable(META_DATABASE, TYPE_DATABASE);

		ASSERT(pMeta != NULL);
		if( pMeta ) {
			g_pLua->PushUserData(pMeta, pDatabase);
			SAFE_UNREF(pMeta);
			return 1;
		}

		SAFE_UNREF(pMeta);

	}

	if( pDatabase ) delete pDatabase;

	g_pLua->PushNil();
	return 1;

}

LUA_FUNCTION(DatabaseDelete)
{

	DATABASE_FROM_LUA();

	ASSERT(pDatabase != NULL);
	if( pDatabase ) {
		delete pDatabase;
		pDatabase = NULL;
	}

	return 0;

}


LUA_FUNCTION(DatabaseOpen)
{

	DATABASE_FROM_LUA();

	g_pLua->CheckType(2, GLua::TYPE_STRING);
	g_pLua->CheckType(3, GLua::TYPE_NUMBER);

	ASSERT(pDatabase != NULL);
	if( pDatabase ) {

		const char* pszDbName = g_pLua->GetString(2);
		int flags = g_pLua->GetInteger(3);
		char pszNewDbName[MAX_PATH];

		int length = strlen(pszDbName);
		if(  length < 1 || ( length == 1 && pszDbName[0] == '?' ) ) {
			g_pLua->PushNil();
			return 1;
		}

		// For security reasons, don't allow clients to create databases outside memory or game folder just in case they download a malicious script
		if( g_pLua->IsClient() ) { 
			if( pszDbName[0] != '?' && stricmp(pszDbName,":memory:") != 0 ) {
				sprintf(pszNewDbName, "%s%s", modulemanager->GetBaseFolder(), &pszDbName[1]);
			} else {
				sprintf(pszNewDbName, "%s", pszDbName);
			}
		} else {
			if( pszDbName[0] == '?' ) {
				sprintf(pszNewDbName, "%s%s", modulemanager->GetBaseFolder(), &pszDbName[1]);
			} else {
				sprintf(pszNewDbName, "%s", pszDbName);
			}
		}

		g_pLua->Push((float)pDatabase->open(pszNewDbName, flags, NULL));
		return 1;

	}

	g_pLua->PushNil();
	return 1;

}

LUA_FUNCTION(DatabaseClose)
{

	DATABASE_FROM_LUA();

	ASSERT(pDatabase != NULL);
	if( pDatabase ) {
		g_pLua->Push((float)pDatabase->close());
		return 1;
	}

	g_pLua->PushNil();
	return 1;

}


LUA_FUNCTION(DatabaseIsOpen)
{

	DATABASE_FROM_LUA();

	ASSERT(pDatabase != NULL);
	if( pDatabase ) {
		g_pLua->Push((bool)pDatabase->isOpen());
		return 1;
	}

	g_pLua->PushNil();
	return 1;

}


LUA_FUNCTION(DatabaseExtendedErrors)
{

	DATABASE_FROM_LUA();

	g_pLua->CheckType(2, GLua::TYPE_BOOL);

	ASSERT(pDatabase != NULL);
	if( pDatabase ) {
		g_pLua->Push((float)pDatabase->setExtendedErrors(g_pLua->GetBool(2)));
		return 1;
	}

	g_pLua->PushNil();
	return 1;

}


LUA_FUNCTION(DatabaseLastError)
{

	DATABASE_FROM_LUA();

	ASSERT(pDatabase != NULL);
	if( pDatabase ) {
		g_pLua->Push((float)pDatabase->getErrorCode());
		return 1;
	}

	g_pLua->PushNil();
	return 1;

}

LUA_FUNCTION(DatabaseLastErrorMessage)
{
	
	DATABASE_FROM_LUA();

	ASSERT(pDatabase != NULL);
	if( pDatabase ) {
		g_pLua->Push((const char*)pDatabase->getErrorMessage());
		return 1;
	}

	g_pLua->PushNil();
	return 1;

}


LUA_FUNCTION(DatabaseLastInsertId)
{

	DATABASE_FROM_LUA();

	ASSERT(pDatabase != NULL);
	if( pDatabase ) {
		g_pLua->Push((float)pDatabase->getLastInsertId());
		return 1;
	}

	g_pLua->PushNil();
	return 1;

}


LUA_FUNCTION(DatabaseChanges)
{

	DATABASE_FROM_LUA();

	ASSERT(pDatabase != NULL);
	if( pDatabase ) {
		g_pLua->Push((float)pDatabase->getChanges());
		return 1;
	}

	g_pLua->PushNil();
	return 1;

}

LUA_FUNCTION(DatabaseTotalChanges)
{

	DATABASE_FROM_LUA();

	ASSERT(pDatabase != NULL);
	if( pDatabase ) {
		g_pLua->Push((float)pDatabase->getTotalChanges());
		return 1;
	}

	g_pLua->PushNil();
	return 1;

}


LUA_FUNCTION(DatabaseExecute)
{

	DATABASE_FROM_LUA();

	g_pLua->CheckType(2, GLua::TYPE_STRING);

	ILuaObject* pCallback = NULL;

	if( g_pLua->GetType(3) == GLua::TYPE_FUNCTION ) {
		pCallback = g_pLua->GetObject(3);
	} else if( g_pLua->GetType(3) == GLua::TYPE_NIL ) {
	} else {
		g_pLua->CheckType(3, GLua::TYPE_FUNCTION);
	}

	ASSERT(pDatabase != NULL);
	if( pDatabase ) {
		int retcode = SQLITE_ERROR;
		if( pCallback != NULL ) {
			retcode = pDatabase->execute(g_pLua->GetString(2), DatabaseExecCallback, (void*)pCallback);
		} else {
			retcode = pDatabase->execute(g_pLua->GetString(2));
		}
		g_pLua->Push((float)retcode);
		SAFE_UNREF(pCallback);
		return 1;
	}

	SAFE_UNREF(pCallback);

	g_pLua->PushNil();
	return 1;

}

LUA_FUNCTION(DatabasePrepare)
{

	DATABASE_FROM_LUA();

	g_pLua->CheckType(2, GLua::TYPE_STRING);

	CStatement* pStatement = NULL;

	ASSERT(pDatabase != NULL);
	if( pDatabase ) {

		int retcode = pDatabase->prepare(&pStatement, g_pLua->GetString(2));

		ASSERT(pStatement != NULL);
		if( pStatement ) {

			ILuaObject* pMeta = g_pLua->GetMetaTable(META_STATEMENT, TYPE_STATEMENT);

			ASSERT(pMeta != NULL);
			if( pMeta ) {
				g_pLua->PushUserData(pMeta, pStatement);
				g_pLua->Push((float)retcode);
				SAFE_UNREF(pMeta);
				return 2;
			}

			SAFE_UNREF(pMeta);

		}

	}

	if( pStatement ) delete pStatement;

	g_pLua->PushNil();
	return 1;

}
