#ifndef STATEMACHINE_H
#define STATEMACHINE_H

int verify_input(void);
int move_to_next_state(unsigned int cur_state, unsigned int ret_code);

/* Helper function */
int check_path(const char* string);
int is_tag(const char* string); /* Depreceted. */

int waiting_for_input_state(const char* s); /* Depreceted. */
int select_option_state(const char* s);
int add_tag_state(const char* s);
int add_files_selected_state(const char* s);
int add_tags_selected_state(const char* s);
int add_tags_state(const char* s);

extern int (* state[])(const char* s); /* Array of function pointers where all functions take a cons char* as a parameter. */

enum path_types {directory, file, invalid_path};

enum state_codes {waiting_for_input, select_option, add_tag, add_files_selected, add_tags_selected, add_tags};

enum ret_codes {ok, fail, repeat};

struct transition {
	enum state_codes start_state;
	enum ret_codes ret_code;
	enum state_codes dest_state;
};

extern struct transition state_transitions[];

#define START_STATE select_option 
#define EXIT_STATE add_tags

#define PROGRAMME_NAME "./tag"

#endif /* STATEMACHINE_H */
