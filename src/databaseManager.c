#include "databaseManager.h"
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

char *sql_drop_all_tables = "DROP TABLE IF EXISTS Tag;"
		      	    "DROP TABLE IF EXISTS Item;"
		      	    "DROP TABLE IF EXISTS TaggedItem;";

char *sql_create_all_tables = "CREATE TABLE Tag(Id INTEGER PRIMARY KEY, Name TEXT);"
			      "CREATE TABLE Item(Id INTEGER PRIMARY KEY, Location TEXT);"
			      "CREATE TABLE TaggedItem(TagId INTEGER, ItemId INTEGER,"
			      "FOREIGN KEY(TagId) REFERENCES Tag(Id), FOREIGN KEY(ItemId) REFERENCES Item(Id));";

char *sql_enable_foreign_keys = "PRAGMA foreign_keys = ON";

char *sql_insert_into_tag = "INSERT INTO Tag(Name) VALUES(?);";

char *sql_insert_into_item = "INSERT INTO Item(Location) VALUES(?)";

char *sql_insert_into_taggedItem = "INSERT INTO TaggegItem(TagId, ItemId) VALUES(?, ?)";

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

void create_database(void) {

	sqlite3 *db_object;
	
	err_msg = 0;
	int db_return_code = sqlite3_open(db_name, &db_object);

	if (db_return_code != SQLITE_OK) {

		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db_object));
		sqlite3_close(db_object);

		exit(EXIT_FAILURE);
	}

	db_return_code = sqlite3_exec(db_object, sql_enable_foreign_keys, 0, 0, &err_msg);
	exit_on_sql_error(db_return_code, err_msg, db_object);
	
	db_return_code = sqlite3_exec(db_object, sql_drop_all_tables, 0, 0, &err_msg);
	exit_on_sql_error(db_return_code, err_msg, db_object);

	db_return_code = sqlite3_exec(db_object, sql_create_all_tables, 0, 0, &err_msg);
	exit_on_sql_error(db_return_code, err_msg, db_object);
	
	sqlite3_close(db_object);
}

void insert_tag(char *tag_name, char *file_location) {
	
	sqlite3 *db_object;
	sqlite3_stmt *prep_stmt1;
	sqlite3_stmt *prep_stmt2;
	
	err_msg = 0;
	int db_return_code = sqlite3_open(db_name, &db_object);

	if (db_return_code != SQLITE_OK) {

		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db_object));
		sqlite3_close(db_object);

		exit(EXIT_FAILURE);
	}

	db_return_code = sqlite3_prepare_v2(db_object, sql_insert_into_tag , -1, &prep_stmt1, 0);
	exit_on_sql_error(db_return_code, err_msg, db_object);

	db_return_code = sqlite3_prepare_v2(db_object, sql_insert_into_item, -1, &prep_stmt2, 0);
	exit_on_sql_error(db_return_code, err_msg, db_object);

	//TODO Add tagged item with triggers.
//	db_return_code = sqlite3(db_object, sql_insert_into_taggedItem,  0, 0, &err_msg);
//	exit_on_sql_error(db_return_code, err_msg, db_object);

	db_return_code = sqlite3_exec(db_object, "BEGIN TRANSACTION", 0, 0, 0);
	
	db_return_code = sqlite3_bind_text(prep_stmt1, 1, tag_name, -1, SQLITE_TRANSIENT);
	db_return_code = sqlite3_bind_text(prep_stmt2, 1, file_location, -1, SQLITE_TRANSIENT);


	db_return_code = sqlite3_step(prep_stmt1);
	db_return_code = sqlite3_step(prep_stmt2);

	db_return_code = sqlite3_exec(db_object, "COMMIT", 0, 0, 0);
	
	sqlite3_finalize(prep_stmt1);
	sqlite3_finalize(prep_stmt2);
	sqlite3_close(db_object);
}

//For testing.
//int main(void) {
//
//	initialize_database();
//
//	insert_tag("tagName2", "locationName2");
//
//	return 0;
//}
