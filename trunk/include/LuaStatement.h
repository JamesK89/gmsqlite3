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

#ifndef _INCLUDE_LUA_STATEMENT_H_
#define _INCLUDE_LUA_STATEMENT_H_

#include "module.h"

#define STATEMENT_FROM_LUA() \
	if( g_pLua->GetType(1) != TYPE_STATEMENT ) g_pLua->TypeError(META_STATEMENT, 1); \
	CStatement* pStatement = (CStatement*)g_pLua->GetUserData(1);

//-----------------------------------------------------------------------------
// Statement functions
//-----------------------------------------------------------------------------

LUA_PROTOTYPE(StatementDelete);

LUA_PROTOTYPE(StatementFinalize);

LUA_PROTOTYPE(StatementSqlString);

LUA_PROTOTYPE(StatementFetch);
LUA_PROTOTYPE(StatementStep);
LUA_PROTOTYPE(StatementReset);
LUA_PROTOTYPE(StatementClearBindings);

LUA_PROTOTYPE(StatementParameterCount);
LUA_PROTOTYPE(StatementParameterName);
LUA_PROTOTYPE(StatementParameterIndex);

LUA_PROTOTYPE(StatementBindNull);
LUA_PROTOTYPE(StatementBindInteger);
LUA_PROTOTYPE(StatementBindFloat);
LUA_PROTOTYPE(StatementBindString);

LUA_PROTOTYPE(StatementColumnCount);
LUA_PROTOTYPE(StatementColumnName);
LUA_PROTOTYPE(StatementColumnIndex);
LUA_PROTOTYPE(StatementColumnType);

LUA_PROTOTYPE(StatementGetInteger);
LUA_PROTOTYPE(StatementGetFloat);
LUA_PROTOTYPE(StatementGetString);

#endif
