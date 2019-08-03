
char *sql_drop_all_tables = "DROP TABLE IF EXISTS Tag;"
			    "DROP TABLE IF EXISTS Item;"
			    "DROP TABLE IF EXISTS TaggedItem;";

char *sql_create_all_tables = "CREATE TABLE Tag(Id INTEGER PRIMARY KEY, Name TEXT);"
			      "CREATE TABLE Item(Id INTEGER PRIMARY KEY, Location TEXT);"
			      "CREATE TABLE TaggedItem(TagId INTEGER, ItemId INTEGER, FOREIGN KEY(TagId) REFERENCES Tag(Id), FOREIGN KEY(ItemId) REFERENCES Item(Id));";

char *sql_enable_foreign_keys = "PRAGMA foreign_keys = ON";

char *db_name = "tag_db.sqlite3";

char *err_msg;

int setup_database(void);
int exit_on_sql_error(int db_ret_code, char *err_msg, sqlite3 db_object);
