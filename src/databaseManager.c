#include "databaseManager.h"
#include <sqlite3.h>
#include <stdio.h>

int setup_database(void) {

	sqlite3 *db_object;
	
	err_msg = 0;
	int db_return_code = sqlite3_open(db_name, &db_object);

	if (db_return_code != SQLITE_OK) {

		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db_object));
		sqlite3_close(db_object);

		return 1;
	}

	db_return_code = sqlite3_exec(db_object, sql_enable_foreign_keys, 0, 0, &err_msg);

	if (db_return_code != SQLITE_OK) {

		fprintf(stderr, "SQL error: %s\n", err_msg);

		sqlite3_free(err_msg);
		sqlite3_close(db_object);

		return 1;
	}
	
	db_return_code = sqlite3_exec(db_object, sql_drop_all_tables, 0, 0, &err_msg);

	if (db_return_code != SQLITE_OK) {

		fprintf(stderr, "SQL error: %s\n", err_msg);

		sqlite3_free(err_msg);
		sqlite3_close(db_object);

		return 1;
	}

	db_return_code = sqlite3_exec(db_object, sql_create_all_tables, 0, 0, &err_msg);
	
	if (db_return_code != SQLITE_OK) {

		fprintf(stderr, "SQL error: %s\n", err_msg);

		sqlite3_free(err_msg);
		sqlite3_close(db_object);

		return 1;
	}
	
	sqlite3_close(db_object);
	
	return 0;
}
/*
int main(void) {

	if (setup_database()) {
		return 1;
	}
	return 0;
}*/
