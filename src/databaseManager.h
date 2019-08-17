#include <sqlite3.h>

#define MAX_TAGS_PER_TRANSACTION 5 //better name

typedef struct { //better name
	const char *paths[MAX_TAGS_PER_TRANSACTION];
	const char *tags[MAX_TAGS_PER_TRANSACTION];
	int paths_id[MAX_TAGS_PER_TRANSACTION];
	int tags_id[MAX_TAGS_PER_TRANSACTION];
	int tag_index;
	int path_index;
/*	char *types[MAX_TAGS_PER_TRANSACTION];*/
} Sql_prep_stmt_input;// sql_prep_stmt_input; //db_input; ???

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

/* Public functions */
void initialize_database(void);
void append_path_to_sql_stmt(const char* path);
void append_tag_name_to_sql_stmt(const char* tag);
int add_tags_to_db(void);

/* Private functions */
void create_database(void);
void exit_on_sql_error(int db_ret_code, char *err_msg, sqlite3 *db_object, int err_line, char *err_file);
int insert_tags(void);
