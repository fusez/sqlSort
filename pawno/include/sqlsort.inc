#define sqlSort_Ascending \
	(true)

#define sqlSort_Descending \
	(false)

new
	bool:	sqlSort_dbFirstOpen = true,
	bool:	sqlSort_dbOpen,
	DB:	sqlSort_dbHandle
;

sqlSort_OpenDB()
{
	if(!sqlSort_dbOpen)
	{
		if((sqlSort_dbHandle = db_open("sqlSort.db")) == DB:0)
			return 0;

		if(sqlSort_dbFirstOpen)
		{
			db_query(sqlSort_dbHandle, "PRAGMA synchronous = OFF");
			db_query(sqlSort_dbHandle, "CREATE TABLE IF NOT EXISTS array (array_element INTEGER)");

			sqlSort_dbFirstOpen = false;
		}
		sqlSort_dbOpen = true;
	}
	return 1;
}

sqlSort_CloseDB()
{
	if(sqlSort_dbOpen)
	{
		db_close(sqlSort_dbHandle);
		sqlSort_dbOpen = false;
	}
}

sqlSort(array[], bool:order = sqlSort_Ascending, array_size = sizeof array)
{
	// open db
	if(!sqlSort_OpenDB())
		return 0;

	// remove any previous array from db
	db_query(sqlSort_dbHandle, "DELETE FROM array");

	// insert array into db
	for(new index, query[54]; index < array_size; index ++)
	{
		format(query, 54, "INSERT INTO array (array_element) VALUES (%i)", array[index]);
		db_query(sqlSort_dbHandle, query);
	}

	// sort array inside db
	new DBResult:result;
	if(order == sqlSort_Ascending)
		result = db_query(sqlSort_dbHandle, "SELECT array_element FROM array ORDER BY array_element ASC");
	else if(order == sqlSort_Descending)
		result = db_query(sqlSort_dbHandle, "SELECT array_element FROM array ORDER BY array_element DESC");

	// return 0 if no rows were found
	if(db_num_rows(result) == 0)
		return db_free_result(result), 0;

	// retrieve sorted array from db
	new
		result_string[11],
		array_index
	;

	do
	{
		db_get_field(result, 0, result_string, 11);
		array[array_index ++] = strval(result_string);
	}
	while
		db_next_row(result);

	// free result
	db_free_result(result);

	// close db
	sqlSort_CloseDB();
	return 1;
}
