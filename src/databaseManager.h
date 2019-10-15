#include <sqlite3.h>

#define MAX_TAGS_PER_TRANSACTION 20 //better name

typedef struct { //better name
	const char *paths[MAX_TAGS_PER_TRANSACTION];
	const char *tags[MAX_TAGS_PER_TRANSACTION];
	int paths_id[MAX_TAGS_PER_TRANSACTION];
	int tags_id[MAX_TAGS_PER_TRANSACTION];
	int tag_index;
	int path_index;
/*	char *types[MAX_TAGS_PER_TRANSACTION];*/
} Sql_prep_stmt_input;// sql_prep_stmt_input; //db_input; ???

//typedef struct { //TODO: Merge into one 2D array for result table.
//	const unsigned char *column[2]; //All tables only have 2 columns
//} Db_row;

typedef struct {
	const unsigned char *table[MAX_TAGS_PER_TRANSACTION][2]; //TODO: make infinite array
} Result_table;

extern char *sql_drop_all_tables;
extern char *sql_create_all_tables;
extern char *sql_enable_foreign_keys;
extern char *sql_insert_into_tag;
extern char *sql_insert_into_item;
extern char *sql_insert_into_taggedItem;
extern char *sql_select_all_locations_for_tag;
extern char *sql_select_all_tags;
extern char *sql_select_all_locations;
extern char *sql_delete_tag;

extern char *db_name;

char *err_msg;//delete???

void initialize_database(void);
void create_database(void);
sqlite3* open_database(void);
void append_path_to_sql_stmt(const char* path);
void append_tag_name_to_sql_stmt(const char* tag);
void prepare_sql_statement(sqlite3 *db_object, char *sql, sqlite3_stmt *stmt_to_prep);
Result_table exe_stmt_and_save_rows(sqlite3_stmt *prep_stmt);

void exit_on_sql_error(int db_ret_code, const char *err_msg, sqlite3 *db_object, int err_line, char *err_file);
int insert_tags(void);
Result_table select_all_locations_for_tag(void);
Result_table select_all_locations_for_tags(void);
Result_table select_all_locations(void);
Result_table select_all_tags(void);
int delete_tags(void);
