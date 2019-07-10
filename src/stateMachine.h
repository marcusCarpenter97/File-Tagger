#ifndef STATEMACHINE_H
#define STATEMACHINE_H

int state_machine(void);
int move_to_next_state(unsigned int cur_state, unsigned int ret_code);

/* Helper functions */
int check_path(const char* string);
int is_string_ascii(const char* string);

/* State functions */
int select_option_state(const char* s);
int add_tag_state(const char* s);
int add_files_selected_state(const char* s);
int add_dirs_selected_state(const char* s);
int add_tags_selected_state(const char* s);

/* Data structures */
extern int (* state[])(const char* s); /* Array of function pointers where all functions take a cons char* as a parameter. */

enum path_types {directory, file, invalid_path};

enum state_codes {select_option, add_tag, add_dirs_selected, add_files_selected, add_tags_selected};

extern enum state_codes final_states[];

enum ret_codes {ok, fail, repeat};

struct transition {
	enum state_codes start_state;
	enum ret_codes ret_code;
	enum state_codes dest_state;
};

extern struct transition state_transitions[];

int is_final_state(enum state_codes state_to_check);

#define START_STATE select_option 
#define EXIT_STATE add_tags_selected

#define PROGRAMME_NAME "./tag"

#endif /* STATEMACHINE_H */
