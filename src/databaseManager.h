extern char *sql_drop_all_tables;

extern char *sql_create_all_tables;

extern char *sql_enable_foreign_keys;

extern char *db_name;

char *err_msg;

int create_database(void);
void initialize_database(void);
//int exit_on_sql_error(int db_ret_code, char *err_msg, sqlite3 db_object);
