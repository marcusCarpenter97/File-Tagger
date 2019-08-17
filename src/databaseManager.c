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

char *sql_enable_foreign_keys = "PRAGMA foreign_keys = ON;";

char *sql_insert_into_tag = "INSERT INTO Tag(Name) VALUES(?);";

char *sql_insert_into_item = "INSERT INTO Item(Location) VALUES(?);";

char *sql_insert_into_taggedItem = "INSERT INTO TaggedItem(TagId, ItemId) VALUES(?, ?);";

char *db_name = "tag_db.sqlite3";

Sql_prep_stmt_input sql_prep_stmt_input; 

void exit_on_sql_error(int db_ret_code, char *err_msg, sqlite3 *db_object, int err_line, char *err_file) {

	if (db_ret_code != SQLITE_OK) {

		fprintf(stderr, "SQL error: %s on line number %d in file %s\n", err_msg, err_line, err_file);

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
	exit_on_sql_error(db_return_code, err_msg, db_object, __LINE__, __FILE__);
	
	db_return_code = sqlite3_exec(db_object, sql_drop_all_tables, 0, 0, &err_msg);
	exit_on_sql_error(db_return_code, err_msg, db_object, __LINE__, __FILE__);

	db_return_code = sqlite3_exec(db_object, sql_create_all_tables, 0, 0, &err_msg);
	exit_on_sql_error(db_return_code, err_msg, db_object, __LINE__, __FILE__);
	
	sqlite3_close(db_object);
}

void append_path_to_sql_stmt(const char* path) { //TODO Check if array is full.
	sql_prep_stmt_input.paths[sql_prep_stmt_input.path_index] = path;
	sql_prep_stmt_input.path_index++;
}

void append_tag_name_to_sql_stmt(const char* tag) { //TODO Check if array is full.
	sql_prep_stmt_input.tags[sql_prep_stmt_input.tag_index] = tag;
	sql_prep_stmt_input.tag_index++;
}

int insert_tags(void) {
	
//input:
//any amount of tags
//any amount of paths
//start
//for each tag in struct
//  add tag to db
//  save Last Insert Rowid in struct
//for each path in struct
//  add path to db
//  save Last Insert Rowid in struct
//for each tagID in struct
//  for each itemID in struct
//   add both IDs to taggedItem

	sqlite3 *db_object;
	sqlite3_stmt *sql_insert_into_tag_stmt;
	sqlite3_stmt *sql_insert_into_item_stmt;
	sqlite3_stmt *sql_insert_into_taggedItem_stmt;
	
	err_msg = 0;
	int db_return_code = sqlite3_open(db_name, &db_object);

	if (db_return_code != SQLITE_OK) {

		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db_object));
		sqlite3_close(db_object);

		return EXIT_FAILURE;
	}

	db_return_code = sqlite3_prepare_v2(db_object, sql_insert_into_tag , -1, &sql_insert_into_tag_stmt, 0);
	exit_on_sql_error(db_return_code, err_msg, db_object, __LINE__, __FILE__);

	db_return_code = sqlite3_prepare_v2(db_object, sql_insert_into_item, -1, &sql_insert_into_item_stmt, 0);
	exit_on_sql_error(db_return_code, err_msg, db_object, __LINE__, __FILE__);

	db_return_code = sqlite3_prepare_v2(db_object, sql_insert_into_taggedItem, -1, &sql_insert_into_taggedItem_stmt, 0); 
	exit_on_sql_error(db_return_code, err_msg, db_object, __LINE__, __FILE__);

	db_return_code = sqlite3_exec(db_object, "BEGIN TRANSACTION", 0, 0, 0);

	for (int next_tag = 0; next_tag < sql_prep_stmt_input.tag_index; next_tag++) { 

		db_return_code = sqlite3_bind_text(sql_insert_into_tag_stmt, 1, sql_prep_stmt_input.tags[next_tag], -1, SQLITE_TRANSIENT);
		db_return_code = sqlite3_step(sql_insert_into_tag_stmt);

		int last_row_id = sqlite3_last_insert_rowid(db_object);
		sql_prep_stmt_input.tags_id[next_tag] = last_row_id;
	
		sqlite3_reset(sql_insert_into_tag_stmt);
		sqlite3_clear_bindings(sql_insert_into_tag_stmt);
	}	

	for (int next_path = 0; next_path < sql_prep_stmt_input.path_index; next_path++) {

		db_return_code = sqlite3_bind_text(
				sql_insert_into_item_stmt, 1, sql_prep_stmt_input.paths[next_path], -1, SQLITE_TRANSIENT);
		db_return_code = sqlite3_step(sql_insert_into_item_stmt);

		int last_row_id = sqlite3_last_insert_rowid(db_object);
		sql_prep_stmt_input.paths_id[next_path] = last_row_id;
		
		sqlite3_reset(sql_insert_into_item_stmt);
		sqlite3_clear_bindings(sql_insert_into_item_stmt);
	}	
	
	for (int next_tag = 0; next_tag < sql_prep_stmt_input.tag_index; next_tag++) {
		for (int next_path = 0; next_path < sql_prep_stmt_input.path_index; next_path++) {
			
			db_return_code = sqlite3_bind_int(
					sql_insert_into_taggedItem_stmt, 1, sql_prep_stmt_input.tags_id[next_tag]);
			db_return_code = sqlite3_bind_int(
					sql_insert_into_taggedItem_stmt, 2, sql_prep_stmt_input.paths_id[next_path]);
			db_return_code = sqlite3_step(sql_insert_into_taggedItem_stmt);
			
			sqlite3_reset(sql_insert_into_taggedItem_stmt);
			sqlite3_clear_bindings(sql_insert_into_taggedItem_stmt);
		}
	}

	db_return_code = sqlite3_exec(db_object, "COMMIT", 0, 0, 0);
	
	sqlite3_finalize(sql_insert_into_tag_stmt);
	sqlite3_finalize(sql_insert_into_item_stmt);
	sqlite3_close(db_object);

	return EXIT_SUCCESS;
}

//For testing.
//int main(void) {
//
//	initialize_database();
//
//	append_tag_name_to_sql_stmt("tagName1");
//	append_path_to_sql_stmt("locationName1");
//	
//	append_tag_name_to_sql_stmt("tagName2");
//	append_path_to_sql_stmt("locationName2");
//
//	insert_tags();
//
//	return 0;
//}
