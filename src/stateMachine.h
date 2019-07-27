#ifndef STATEMACHINE_H
#define STATEMACHINE_H

int state_machine(void);
int move_to_next_state(unsigned int cur_state, unsigned int ret_code);

/* Helper functions */
int check_path(const char* string);
int is_string_ascii(const char* string);
int is_file_type_valid(const char* string);

/* State functions */
int select_option_state(const char* s);
int add_tag_state(const char* s);
int add_files_selected_state(const char* s);
int add_dirs_selected_state(const char* s);
int add_tags_selected_state(const char* s);
int reading_file_type_state(const char* s);
int file_type_selected_state(const char* s);
int delete_tag_state(const char* s);
int delete_all_tags_state(const char* s);
int delete_files_selected_state(const char* s);
int delete_dirs_selected_state(const char* s);
int delete_all_files_selected_state(const char* s);
int delete_all_dirs_selected_state(const char* s);
int delete_tags_selected_state(const char* s);

/* Data structures */
extern int (* state[])(const char* s);

enum path_types {directory, file, invalid_path};

enum state_codes {select_option, add_tag, add_dirs_selected, add_files_selected, add_tags_selected, reading_file_type, 
	file_type_selected, delete_tag, delete_all_tags, delete_files_selected, delete_dirs_selected, delete_all_files_selected, 
	delete_all_dirs_selected, delete_tags_selected};

extern enum state_codes final_states[];

enum transition_values {add, path_to_file, path_to_dir, type, tag_name, type_name, del, recursive, search, no_tag, tag_only, fail, end};

struct transition {
	enum state_codes start_state;
	enum transition_values next_transition;
	enum state_codes dest_state;
};

extern struct transition state_transitions[];

int is_final_state(enum state_codes state_to_check);

#define START_STATE select_option 

#endif /* STATEMACHINE_H */
