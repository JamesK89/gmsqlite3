require("sqlite3")

function sqlite3callback(numColumns, columns, values)
	print("== sqlite3callback: "..numColumns.." columns")
	PrintTable(columns)
	print("values:")
	PrintTable(values)
	return sqlite3.SQLITE_OK
end

function doSQLiteTest(player, command, arguments)

	print("Running SQLite version "..sqlite3.LibVersion())
	PrintTable(sqlite3)
	
	local db = sqlite3.New()
	local rt = nil;
	
	-- The question mark will prepend the base folder path
	if db:Open("?sqlite3test.db", sqlite3.SQLITE_OPEN_READWRITE + sqlite3.SQLITE_OPEN_CREATE) == sqlite3.SQLITE_OK then
		print("Opened database")
	else
		print("Failed to open database")
		return
	end

	if db:Execute("CREATE TABLE IF NOT EXISTS test ( s TEXT, x INTEGER );") == sqlite3.SQLITE_OK  then
		print("Successfully created table")
	else
		print("Failed to created table")
		return
	end
	
	print("== INSERT ==") -- Insert rows using parameterized queries
	
	db:Execute("BEGIN;") -- If you are going to do a large amount of inserts, then a transaction can speed it up
	local stmt, rt = db:Prepare("INSERT INTO test ( s, x ) VALUES ( ?, ? );")
	for i=0,50 do
	
		local s = ""
		local v = math.random(1,2)
		
		if v == 1 then
			s = "Test1"
		else
			s = "Test2"
		end
		
		v = math.random(0,1000)
		
		stmt:BindString(1, s)
		stmt:BindInteger(2, v)
		
		print("I. Value: "..s..", "..v)
		
		stmt:Step() -- Execute the query
		stmt:Reset() -- Reset the query back to its initial state
		
	end
	stmt:Finalize() -- ALWAYS FINALIZE your statements when you are finished with them, or suffer memory leaks and crashes
	db:Execute("END;") -- End Transaction

	print("== SELECT ==") -- Process rows as tables, using parameterized queries
	
	stmt = db:Prepare("SELECT * FROM test WHERE s = $STRING ORDER BY x DESC;")
	stmt:BindString("$STRING", "Test1") -- You can also use paramterized queries using names instead of indexes, unlike the previous example
	while true do
		local row = stmt:Fetch()
		if not row then break end 
		print("S. Value: "..row["s"]..", "..row["x"])
	end
	stmt:Finalize() -- ALWAYS FINALIZE your statements when you are finished with them, or suffer memory leaks and crashes
	
	print("== SELECT #2 ==") -- Or process them the SQLite way
	
	stmt = db:Prepare("SELECT s, x FROM test ORDER BY x DESC;")
	while stmt:Step() == sqlite3.SQLITE_ROW do
		-- You can retrieve results by either column name or index
		print("S. Value: "..stmt:GetString("s")..", "..stmt:GetInteger(1))
	end
	print("Original SQL statement: "..stmt:SQLString())
	stmt:Finalize() -- ALWAYS FINALIZE your statements (Trying to make a point here)

	print("== Callback Test==")
	db:Execute("SELECT * FROM test WHERE s=\"Test1\";", sqlite3callback)
	
	print("== Total Changes: "..db:TotalChanges())
	
	db:Close()
	
end
concommand.Add("sqlite3test", doSQLiteTest)
