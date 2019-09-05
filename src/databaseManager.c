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

char *sql_select_all_locations_for_tag = "SELECT Item.Location, Tag.Name FROM Item JOIN TaggedItem ON TaggedItem.itemID = Item.ID JOIN Tag ON TaggedItem.tagID = Tag.ID WHERE Tag.Name = (?);";

char *sql_select_all_tags = "SELECT *FROM Tag;";

char *sql_select_all_locations = "SELECT * FROM Item";

char *db_name = "tag_db.sqlite3";

Sql_prep_stmt_input sql_prep_stmt_input; 

void exit_on_sql_error(int db_ret_code, const char *err_msg, sqlite3 *db_object, int err_line, char *err_file) {

	if (db_ret_code != SQLITE_OK) {

		fprintf(stderr, "SQL error: %s on line number %d in file %s\n", err_msg, err_line, err_file);

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

sqlite3* open_database(void) {

	sqlite3 *db_object;
	
	int db_return_code = sqlite3_open(db_name, &db_object);
	
	if (db_return_code != SQLITE_OK) {

		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db_object));
		sqlite3_close(db_object);

		exit(EXIT_FAILURE);
	}

	return db_object;
}

void create_database(void) {

	char *err_msg = 0;

	int db_return_code;

	sqlite3* db_object = open_database();

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

void prepare_sql_statement(sqlite3 *db_object, char *sql, sqlite3_stmt *stmt_to_prep) {
//TODO debugging message wont help anymore. Does not work. Find way to pass prep_stmt pointer properly.	
	int db_return_code = sqlite3_prepare_v2(db_object, sql, -1, &stmt_to_prep, 0);
	exit_on_sql_error(db_return_code, sqlite3_errmsg(db_object), db_object, __LINE__, __FILE__);
}

void print_returned_rows(int db_ret_code, sqlite3_stmt *prep_stmt) {

	do {
		db_ret_code = sqlite3_step(prep_stmt);

		if (db_ret_code == SQLITE_ROW) {

			printf("%s: ", sqlite3_column_text(prep_stmt, 0));
			printf("%s\n", sqlite3_column_text(prep_stmt, 1));

		}

	} while (db_ret_code != SQLITE_DONE);
}

int insert_tags(void) {

	sqlite3* db_object = open_database();
	
	sqlite3_stmt *sql_insert_into_tag_stmt;
	sqlite3_stmt *sql_insert_into_item_stmt;
	sqlite3_stmt *sql_insert_into_taggedItem_stmt;

	int db_return_code;
	int tag_pair_counter = 0;

	//Prepare sql statements
	db_return_code = sqlite3_prepare_v2(db_object, sql_insert_into_tag , -1, &sql_insert_into_tag_stmt, 0);
	exit_on_sql_error(db_return_code, sqlite3_errmsg(db_object), db_object, __LINE__, __FILE__);

	db_return_code = sqlite3_prepare_v2(db_object, sql_insert_into_item, -1, &sql_insert_into_item_stmt, 0);
	exit_on_sql_error(db_return_code, sqlite3_errmsg(db_object), db_object, __LINE__, __FILE__);

	db_return_code = sqlite3_prepare_v2(db_object, sql_insert_into_taggedItem, -1, &sql_insert_into_taggedItem_stmt, 0); 
	exit_on_sql_error(db_return_code, sqlite3_errmsg(db_object), db_object, __LINE__, __FILE__);

	//Insert everything as one transaction
	db_return_code = sqlite3_exec(db_object, "BEGIN TRANSACTION", 0, 0, 0);

	//For each tag
	for (int next_tag = 0; next_tag < sql_prep_stmt_input.tag_index; next_tag++) { 

		//Add to db
		db_return_code = sqlite3_bind_text(sql_insert_into_tag_stmt, 1, sql_prep_stmt_input.tags[next_tag], -1, SQLITE_TRANSIENT);
		db_return_code = sqlite3_step(sql_insert_into_tag_stmt);

		//Remember row id
		int last_row_id = sqlite3_last_insert_rowid(db_object);
		sql_prep_stmt_input.tags_id[next_tag] = last_row_id;
	
		sqlite3_reset(sql_insert_into_tag_stmt);
		sqlite3_clear_bindings(sql_insert_into_tag_stmt);
	}	

	//For each path
	for (int next_path = 0; next_path < sql_prep_stmt_input.path_index; next_path++) {

		//Add to db
		db_return_code = sqlite3_bind_text(
				sql_insert_into_item_stmt, 1, sql_prep_stmt_input.paths[next_path], -1, SQLITE_TRANSIENT);
		db_return_code = sqlite3_step(sql_insert_into_item_stmt);

		//Remember row id
		int last_row_id = sqlite3_last_insert_rowid(db_object);
		sql_prep_stmt_input.paths_id[next_path] = last_row_id;
		
		sqlite3_reset(sql_insert_into_item_stmt);
		sqlite3_clear_bindings(sql_insert_into_item_stmt);
	}	
	
	//For each tag-path pair add them to db
	for (int next_tag = 0; next_tag < sql_prep_stmt_input.tag_index; next_tag++) {
		for (int next_path = 0; next_path < sql_prep_stmt_input.path_index; next_path++) {
			
			db_return_code = sqlite3_bind_int(
					sql_insert_into_taggedItem_stmt, 1, sql_prep_stmt_input.tags_id[next_tag]);
			
			db_return_code = sqlite3_bind_int(
					sql_insert_into_taggedItem_stmt, 2, sql_prep_stmt_input.paths_id[next_path]);
			
			db_return_code = sqlite3_step(sql_insert_into_taggedItem_stmt);
			
			sqlite3_reset(sql_insert_into_taggedItem_stmt);
			sqlite3_clear_bindings(sql_insert_into_taggedItem_stmt);

			tag_pair_counter++;
		}
	}

	db_return_code = sqlite3_exec(db_object, "COMMIT", 0, 0, 0);
	
	sqlite3_finalize(sql_insert_into_tag_stmt);
	sqlite3_finalize(sql_insert_into_item_stmt);
	sqlite3_finalize(sql_insert_into_taggedItem_stmt);
	sqlite3_close(db_object);

	return tag_pair_counter;
}

int select_all_locations_for_tags() {
	
	sqlite3* db_object = open_database();
	sqlite3_stmt *sql_select_all_locations_for_tag_stmt;
	
	int db_return_code;
	db_return_code = sqlite3_prepare_v2(db_object, sql_select_all_locations_for_tag, -1, &sql_select_all_locations_for_tag_stmt, 0);
	exit_on_sql_error(db_return_code, sqlite3_errmsg(db_object), db_object, __LINE__, __FILE__);

	for (int curr_tag = 0; curr_tag < sql_prep_stmt_input.tag_index; curr_tag++) {

		db_return_code = sqlite3_bind_text(sql_select_all_locations_for_tag_stmt, 1, sql_prep_stmt_input.tags[0], -1, SQLITE_TRANSIENT);
		print_returned_rows(db_return_code, sql_select_all_locations_for_tag_stmt);
		
		sqlite3_reset(sql_select_all_locations_for_tag_stmt);
		sqlite3_clear_bindings(sql_select_all_locations_for_tag_stmt);
	}

	sqlite3_finalize(sql_select_all_locations_for_tag_stmt);
	sqlite3_close(db_object);
	
	return EXIT_SUCCESS;
}

int select_all_locations_for_tag() {
	
	sqlite3* db_object = open_database();
	sqlite3_stmt *sql_select_all_locations_for_tag_stmt;
	
	int db_return_code;
	db_return_code = sqlite3_prepare_v2(db_object, sql_select_all_locations_for_tag, -1, &sql_select_all_locations_for_tag_stmt, 0);
	exit_on_sql_error(db_return_code, sqlite3_errmsg(db_object), db_object, __LINE__, __FILE__);

	db_return_code = sqlite3_bind_text(sql_select_all_locations_for_tag_stmt, 1, sql_prep_stmt_input.tags[0], -1, SQLITE_TRANSIENT);

	print_returned_rows(db_return_code, sql_select_all_locations_for_tag_stmt);

	sqlite3_finalize(sql_select_all_locations_for_tag_stmt);
	sqlite3_close(db_object);
	
	return EXIT_SUCCESS;
}

int select_all_tags(void) {
	
	sqlite3* db_object = open_database();
	sqlite3_stmt *sql_select_all_tags_stmt;
	
	int db_return_code;
	db_return_code = sqlite3_prepare_v2(db_object, sql_select_all_tags, -1, &sql_select_all_tags_stmt, 0);
	exit_on_sql_error(db_return_code, sqlite3_errmsg(db_object), db_object, __LINE__, __FILE__);

	print_returned_rows(db_return_code, sql_select_all_tags_stmt);

	sqlite3_finalize(sql_select_all_tags_stmt);
	sqlite3_close(db_object);
	
	return EXIT_SUCCESS;
}

int select_all_locations(void) {
	
	sqlite3* db_object = open_database();
	sqlite3_stmt *sql_select_all_locations_stmt;
	
	int db_return_code;
	db_return_code = sqlite3_prepare_v2(db_object, sql_select_all_locations, -1, &sql_select_all_locations_stmt, 0);
	exit_on_sql_error(db_return_code, sqlite3_errmsg(db_object), db_object, __LINE__, __FILE__);

	print_returned_rows(db_return_code, sql_select_all_locations_stmt);
	
	sqlite3_finalize(sql_select_all_locations_stmt);
	sqlite3_close(db_object);
	
	return EXIT_SUCCESS;
}

int delete_tags(void) {
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
