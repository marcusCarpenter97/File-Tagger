#include "databaseManager.h"
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

char *sql_drop_all_tables = "DROP TABLE IF EXISTS Tag;"
		      "DROP TABLE IF EXISTS Item;"
		      "DROP TABLE IF EXISTS TaggedItem;";

char *sql_create_all_tables = "CREATE TABLE Tag(Id INTEGER PRIMARY KEY, Name TEXT);"
			"CREATE TABLE Item(Id INTEGER PRIMARY KEY, Location TEXT);"
			"CREATE TABLE TaggedItem(TagId INTEGER, ItemId INTEGER, FOREIGN KEY(TagId) REFERENCES Tag(Id), FOREIGN KEY(ItemId) REFERENCES Item(Id));";

char *sql_enable_foreign_keys = "PRAGMA foreign_keys = ON";

char *db_name = "tag_db.sqlite3";

void exit_on_sql_error(int db_ret_code, char *err_msg, sqlite3 *db_object) {

	if (db_ret_code != SQLITE_OK) {

		fprintf(stderr, "SQL error: %s\n", err_msg);

		sqlite3_free(err_msg);
		sqlite3_close(db_object);

		exit(EXIT_FAILURE);
	}
}

void initialize_database(void) {

	struct stat buffer;

	//Create new database if it does not exist.
	if (stat(db_name, &buffer) != 0) {
		create_database();
	}
}

int create_database(void) {

	sqlite3 *db_object;
	
	err_msg = 0;
	int db_return_code = sqlite3_open(db_name, &db_object);

	if (db_return_code != SQLITE_OK) {

		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db_object));
		sqlite3_close(db_object);

		return 1;
	}

	db_return_code = sqlite3_exec(db_object, sql_enable_foreign_keys, 0, 0, &err_msg);
	exit_on_sql_error(db_return_code, err_msg, db_object);
	
	db_return_code = sqlite3_exec(db_object, sql_drop_all_tables, 0, 0, &err_msg);
	exit_on_sql_error(db_return_code, err_msg, db_object);

	db_return_code = sqlite3_exec(db_object, sql_create_all_tables, 0, 0, &err_msg);
	exit_on_sql_error(db_return_code, err_msg, db_object);
	
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
