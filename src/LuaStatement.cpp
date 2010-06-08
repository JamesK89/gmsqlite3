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

#include "LuaStatement.h"
#include "database.h"
#include "statement.h"

//-----------------------------------------------------------------------------
// Statement functions
//-----------------------------------------------------------------------------

LUA_FUNCTION(StatementDelete)
{

	STATEMENT_FROM_LUA();

	ASSERT(pStatement != NULL);
	if( pStatement )
	{
		delete pStatement;
		pStatement = NULL;
	}

	return 0;

}



LUA_FUNCTION(StatementFinalize)
{

	STATEMENT_FROM_LUA();
	
	ASSERT(pStatement != NULL);
	if( pStatement )
	{
		g_pLua->Push((float)pStatement->finalize());
		return 1;
	}

	g_pLua->PushNil();
	return 1;

}


LUA_FUNCTION(StatementSqlString)
{

	STATEMENT_FROM_LUA();
	
	ASSERT(pStatement != NULL);
	if( pStatement )
	{
		g_pLua->Push((const char*)pStatement->getSql());
		return 1;
	}

	g_pLua->PushNil();
	return 1;

}


LUA_FUNCTION(StatementFetch)
{

	STATEMENT_FROM_LUA();
	
	ASSERT(pStatement != NULL);
	if( pStatement )
	{

		ILuaObject* pRow = g_pLua->GetNewTable();
		ASSERT(pRow != NULL);

		if( pRow ) {

			int retcode = pStatement->step();

			if( retcode == SQLITE_ROW ) {

				int numCols = pStatement->getNumberOfColumns();

				int numBlobBytes = 0;
				const void* pBlob = NULL;
				ILuaObject* pLBlob = NULL;

				for( int i = 0; i < numCols; i++ ) {

					const char* pszColName = pStatement->getColumnName(i);

					switch( pStatement->getColumnType(i) )
					{
						case SQLITE_INTEGER:
							pRow->SetMember(pszColName, (float)pStatement->getInteger(i));
						break;
						case SQLITE_FLOAT:
							pRow->SetMember(pszColName, pStatement->getFloat(i));
						break;
						case SQLITE_TEXT:
							pRow->SetMember(pszColName, pStatement->getText(i));
						break;
						case SQLITE_BLOB:

							numBlobBytes = 0;
							pBlob = pStatement->getBlob(i, &numBlobBytes);

							pLBlob = g_pLua->GetNewTable();
							ASSERT(pLBlob != NULL);

							if( pLBlob ) {
								for( int b = 0; b < numBlobBytes; b++ ) {
									pLBlob->SetMember((float)b, (float)((const char*)pBlob)[b]);
								}
								pRow->SetMember(pszColName, pLBlob);
							} else {
								pRow->SetMember(pszColName);
							}

							SAFE_UNREF(pLBlob);

						break;
						case SQLITE_NULL:
							pRow->SetMember(pszColName);
						break;
					}

				}

				g_pLua->Push(pRow);

			} else {

				g_pLua->PushNil();

			}

			g_pLua->Push((float)retcode);
			SAFE_UNREF(pRow);
			return 2;

		}

	}

	g_pLua->PushNil();
	return 1;

}

LUA_FUNCTION(StatementStep)
{

	STATEMENT_FROM_LUA();
	
	ASSERT(pStatement != NULL);
	if( pStatement )
	{
		g_pLua->Push((float)pStatement->step());
		return 1;
	}

	g_pLua->PushNil();
	return 1;

}

LUA_FUNCTION(StatementReset)
{

	STATEMENT_FROM_LUA();
	
	ASSERT(pStatement != NULL);
	if( pStatement )
	{
		g_pLua->Push((float)pStatement->reset());
		return 1;
	}

	g_pLua->PushNil();
	return 1;

}

LUA_FUNCTION(StatementClearBindings)
{

	STATEMENT_FROM_LUA();
	
	ASSERT(pStatement != NULL);
	if( pStatement )
	{
		g_pLua->Push((float)pStatement->clearBindings());
		return 1;
	}

	g_pLua->PushNil();
	return 1;

}


LUA_FUNCTION(StatementParameterCount)
{

	STATEMENT_FROM_LUA();
	
	ASSERT(pStatement != NULL);
	if( pStatement )
	{
		g_pLua->Push((float)pStatement->getNumberOfParameters());
		return 1;
	}

	g_pLua->PushNil();
	return 1;

}

LUA_FUNCTION(StatementParameterName)
{

	STATEMENT_FROM_LUA();

	g_pLua->CheckType(2, GLua::TYPE_NUMBER);
	
	ASSERT(pStatement != NULL);
	if( pStatement )
	{
		g_pLua->Push((const char*)pStatement->getParameterName(g_pLua->GetInteger(2)));
		return 1;
	}

	g_pLua->PushNil();
	return 1;

}

LUA_FUNCTION(StatementParameterIndex)
{

	STATEMENT_FROM_LUA();

	g_pLua->CheckType(2, GLua::TYPE_STRING);
	
	ASSERT(pStatement != NULL);
	if( pStatement )
	{
		g_pLua->Push((float)pStatement->getParameterIndex(g_pLua->GetString(2)));
		return 1;
	}

	g_pLua->PushNil();
	return 1;

}


LUA_FUNCTION(StatementBindNull)
{

	STATEMENT_FROM_LUA();

	if( g_pLua->GetType(2) != GLua::TYPE_STRING && g_pLua->GetType(2) != GLua::TYPE_NUMBER ) {
		g_pLua->CheckType(2, GLua::TYPE_STRING);
		g_pLua->CheckType(2, GLua::TYPE_NUMBER);
	}
	
	ASSERT(pStatement != NULL);
	if( pStatement )
	{
		if( g_pLua->GetType(2) == GLua::TYPE_NUMBER ) {
			g_pLua->Push((float)pStatement->bindNull(g_pLua->GetInteger(2)));
			return 1;
		} else if( g_pLua->GetType(2) == GLua::TYPE_STRING ) {
			g_pLua->Push((float)pStatement->bindNull(g_pLua->GetString(2)));
			return 1;
		}
	}

	g_pLua->PushNil();
	return 1;

}

LUA_FUNCTION(StatementBindInteger)
{

	STATEMENT_FROM_LUA();

	if( g_pLua->GetType(2) != GLua::TYPE_STRING && g_pLua->GetType(2) != GLua::TYPE_NUMBER ) {
		g_pLua->CheckType(2, GLua::TYPE_STRING);
		g_pLua->CheckType(2, GLua::TYPE_NUMBER);
	}
	g_pLua->CheckType(3, GLua::TYPE_NUMBER);
	
	ASSERT(pStatement != NULL);
	if( pStatement )
	{
		if( g_pLua->GetType(2) == GLua::TYPE_NUMBER ) {
			g_pLua->Push((float)pStatement->bindInteger(g_pLua->GetInteger(2), g_pLua->GetInteger(3)));
			return 1;
		} else if( g_pLua->GetType(2) == GLua::TYPE_STRING ) {
			g_pLua->Push((float)pStatement->bindInteger(g_pLua->GetString(2), g_pLua->GetInteger(3)));
			return 1;
		}
	}

	g_pLua->PushNil();
	return 1;

}

LUA_FUNCTION(StatementBindFloat)
{

	STATEMENT_FROM_LUA();

	if( g_pLua->GetType(2) != GLua::TYPE_STRING && g_pLua->GetType(2) != GLua::TYPE_NUMBER ) {
		g_pLua->CheckType(2, GLua::TYPE_STRING);
		g_pLua->CheckType(2, GLua::TYPE_NUMBER);
	}
	g_pLua->CheckType(3, GLua::TYPE_NUMBER);
	
	ASSERT(pStatement != NULL);
	if( pStatement )
	{
		if( g_pLua->GetType(2) == GLua::TYPE_NUMBER ) {
			g_pLua->Push((float)pStatement->bindFloat(g_pLua->GetInteger(2), g_pLua->GetNumber(3)));
			return 1;
		} else if( g_pLua->GetType(2) == GLua::TYPE_STRING ) {
			g_pLua->Push((float)pStatement->bindFloat(g_pLua->GetString(2), g_pLua->GetNumber(3)));
			return 1;
		}
	}

	g_pLua->PushNil();
	return 1;

}

LUA_FUNCTION(StatementBindString)
{

	STATEMENT_FROM_LUA();

	if( g_pLua->GetType(2) != GLua::TYPE_STRING && g_pLua->GetType(2) != GLua::TYPE_NUMBER ) {
		g_pLua->CheckType(2, GLua::TYPE_STRING);
		g_pLua->CheckType(2, GLua::TYPE_NUMBER);
	}
	g_pLua->CheckType(3, GLua::TYPE_STRING);
	
	ASSERT(pStatement != NULL);
	if( pStatement )
	{
		if( g_pLua->GetType(2) == GLua::TYPE_NUMBER ) {
			g_pLua->Push((float)pStatement->bindText(g_pLua->GetInteger(2), g_pLua->GetString(3)));
			return 1;
		} else if( g_pLua->GetType(2) == GLua::TYPE_STRING ) {
			g_pLua->Push((float)pStatement->bindText(g_pLua->GetString(2), g_pLua->GetString(3)));
			return 1;
		}
	}

	g_pLua->PushNil();
	return 1;

}

LUA_FUNCTION(StatementColumnCount)
{

	STATEMENT_FROM_LUA();
	
	ASSERT(pStatement != NULL);
	if( pStatement )
	{
		g_pLua->Push((float)pStatement->getNumberOfColumns());
		return 1;
	}

	g_pLua->PushNil();
	return 1;

}

LUA_FUNCTION(StatementColumnName)
{

	STATEMENT_FROM_LUA();

	g_pLua->CheckType(2, GLua::TYPE_NUMBER);
	
	ASSERT(pStatement != NULL);
	if( pStatement )
	{
		g_pLua->Push((const char*)pStatement->getColumnName(g_pLua->GetInteger(2)));
		return 1;
	}

	g_pLua->PushNil();
	return 1;

}

LUA_FUNCTION(StatementColumnIndex)
{

	STATEMENT_FROM_LUA();

	g_pLua->CheckType(2, GLua::TYPE_STRING);
	
	ASSERT(pStatement != NULL);
	if( pStatement )
	{
		g_pLua->Push((float)pStatement->getColumnIndex(g_pLua->GetString(2)));
		return 1;
	}

	g_pLua->PushNil();
	return 1;

}

LUA_FUNCTION(StatementColumnType)
{

	STATEMENT_FROM_LUA();

	g_pLua->CheckType(2, GLua::TYPE_NUMBER);
	
	ASSERT(pStatement != NULL);
	if( pStatement )
	{
		g_pLua->Push((float)pStatement->getColumnType(g_pLua->GetInteger(2)));
		return 1;
	}

	g_pLua->PushNil();
	return 1;

}

LUA_FUNCTION(StatementGetInteger)
{

	STATEMENT_FROM_LUA();

	if( g_pLua->GetType(2) != GLua::TYPE_STRING && g_pLua->GetType(2) != GLua::TYPE_NUMBER ) {
		g_pLua->CheckType(2, GLua::TYPE_STRING);
		g_pLua->CheckType(2, GLua::TYPE_NUMBER);
	}
	
	ASSERT(pStatement != NULL);
	if( pStatement )
	{
		if( g_pLua->GetType(2) == GLua::TYPE_NUMBER ) {
			g_pLua->Push((float)pStatement->getInteger(g_pLua->GetInteger(2)));
			return 1;
		} else if( g_pLua->GetType(2) == GLua::TYPE_STRING ) {
			g_pLua->Push((float)pStatement->getInteger(g_pLua->GetString(2)));
			return 1;
		}
	}

	g_pLua->PushNil();
	return 1;

}

LUA_FUNCTION(StatementGetFloat)
{

	STATEMENT_FROM_LUA();

	if( g_pLua->GetType(2) != GLua::TYPE_STRING && g_pLua->GetType(2) != GLua::TYPE_NUMBER ) {
		g_pLua->CheckType(2, GLua::TYPE_STRING);
		g_pLua->CheckType(2, GLua::TYPE_NUMBER);
	}
	
	ASSERT(pStatement != NULL);
	if( pStatement )
	{
		if( g_pLua->GetType(2) == GLua::TYPE_NUMBER ) {
			g_pLua->Push((float)pStatement->getFloat(g_pLua->GetInteger(2)));
			return 1;
		} else if( g_pLua->GetType(2) == GLua::TYPE_STRING ) {
			g_pLua->Push((float)pStatement->getFloat(g_pLua->GetString(2)));
			return 1;
		}
	}

	g_pLua->PushNil();
	return 1;

}

LUA_FUNCTION(StatementGetString)
{

	STATEMENT_FROM_LUA();

	if( g_pLua->GetType(2) != GLua::TYPE_STRING && g_pLua->GetType(2) != GLua::TYPE_NUMBER ) {
		g_pLua->CheckType(2, GLua::TYPE_STRING);
		g_pLua->CheckType(2, GLua::TYPE_NUMBER);
	}

	ASSERT(pStatement != NULL);
	if( pStatement )
	{
		if( g_pLua->GetType(2) == GLua::TYPE_NUMBER ) {
			g_pLua->Push((const char*)pStatement->getText(g_pLua->GetInteger(2)));
			return 1;
		} else if( g_pLua->GetType(2) == GLua::TYPE_STRING ) {
			g_pLua->Push((const char*)pStatement->getText(g_pLua->GetString(2)));
			return 1;
		}
	}

	g_pLua->PushNil();
	return 1;

}
