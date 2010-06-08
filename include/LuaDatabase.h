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

#ifndef _INCLUDE_LUA_DATABASE_H_
#define _INCLUDE_LUA_DATABASE_H_

#include "module.h"

#define DATABASE_FROM_LUA() \
	if( g_pLua->GetType(1) != TYPE_DATABASE ) g_pLua->TypeError(META_DATABASE, 1); \
	CDatabase* pDatabase = (CDatabase*)g_pLua->GetUserData(1);

//-----------------------------------------------------------------------------
// Database functions
//-----------------------------------------------------------------------------

LUA_PROTOTYPE(DatabaseNew);
LUA_PROTOTYPE(DatabaseDelete);

LUA_PROTOTYPE(DatabaseOpen);
LUA_PROTOTYPE(DatabaseClose);

LUA_PROTOTYPE(DatabaseIsOpen);

LUA_PROTOTYPE(DatabaseExtendedErrors);

LUA_PROTOTYPE(DatabaseLastError);
LUA_PROTOTYPE(DatabaseLastErrorMessage);

LUA_PROTOTYPE(DatabaseLastInsertId);

LUA_PROTOTYPE(DatabaseChanges);
LUA_PROTOTYPE(DatabaseTotalChanges);

LUA_PROTOTYPE(DatabaseExecute);
LUA_PROTOTYPE(DatabasePrepare);

#endif
