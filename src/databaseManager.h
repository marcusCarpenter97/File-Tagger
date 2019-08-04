#include <sqlite3.h>

typedef struct sql_prep_stmt_input {
	char *locations[5];
	char *tags[5];
/*	char *types[5];*/
} db_input;

extern char *sql_drop_all_tables;
extern char *sql_create_all_tables;
extern char *sql_enable_foreign_keys;
extern char *sql_insert_into_tag;
extern char *sql_insert_into_item;
extern char *sql_insert_into_taggedItem;
extern char *sql_search_tag;
extern char *sql_delete_tag;

extern char *db_name;

char *err_msg;

void create_database(void);
void initialize_database(void);
void exit_on_sql_error(int db_ret_code, char *err_msg, sqlite3 *db_object);
void insert_tag(char *tag_name, char *file_location);
