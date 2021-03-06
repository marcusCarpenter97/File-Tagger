#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stateMachine.h"
#include "inputStack.h"

/************************************/
/* TABLES USED BY THE STATE MACHINE */
/************************************/

/* END STATES: Array of state codes enums which defines the end states for the state machine. */
enum state_codes end_states[] = {add_tags_selected, delete_all_files_selected, delete_all_dirs_selected, delete_tags_selected};

/* STATE: Array of function pointers containing all state functions in the state machine. */
int (* state[])(const char* s) = 
	{select_option_state, add_tag_state, add_dirs_selected_state, add_files_selected_state, add_tags_selected_state, 
		reading_file_type_state, file_type_selected_state, delete_tag_state, delete_all_tags_state, delete_files_selected_state, 
		delete_dirs_selected_state, delete_all_files_selected_state, delete_all_dirs_selected_state, delete_tags_selected_state, 
		tags_selected_state, reading_file_types_state, search_dir_selected_state, search_all_tags_state, 
		file_types_selected_state, search_no_tags_state, search_tags_only_state};

/* STATE TRANSITIONS: Array of transition enums containing all possible state transitions in the state machine. */
struct transition state_transitions[] = {
	{select_option, add, add_tag},
	{add_tag, path_to_file, add_files_selected},
	{add_tag, path_to_dir, add_dirs_selected},
	{add_files_selected, tag_name, add_tags_selected},
	{add_files_selected, path_to_file, add_files_selected},
	{add_dirs_selected, tag_name, add_tags_selected},
	{add_dirs_selected, path_to_dir, add_dirs_selected},
	{add_dirs_selected, type, reading_file_type},
	{reading_file_type, type_name, file_type_selected},
	{file_type_selected, type_name, file_type_selected},
	{file_type_selected, tag_name, add_tags_selected},
	{add_tags_selected, tag_name, add_tags_selected},
	{delete_tag, tag_name, delete_tags_selected},
	{delete_tag, path_to_file, delete_files_selected},
	{delete_tag, path_to_dir, delete_dirs_selected},
	{delete_all_tags, path_to_file, delete_all_files_selected},
	{delete_all_tags, path_to_dir, delete_all_dirs_selected},
	{delete_files_selected, path_to_file, delete_files_selected},
	{delete_files_selected, tag_name, delete_tags_selected},
	{delete_dirs_selected, path_to_dir, delete_dirs_selected},
	{delete_dirs_selected, tag_name, delete_tags_selected},
	{delete_all_files_selected, path_to_file, delete_all_files_selected},
	{delete_all_dirs_selected, path_to_dir, delete_all_dirs_selected},
	{delete_tags_selected, tag_name, delete_tags_selected},
	{tags_selected, tag_name, tags_selected},
	{reading_file_types, type_name, file_types_selected},
	{search_dir_selected, path_to_dir, search_dir_selected},
	{search_dir_selected, type, reading_file_types},
	{search_dir_selected, tag_name, tags_selected},
	{search_all_tags, no_tag, search_no_tags},
	{search_all_tags, tag_only, search_tags_only},
	{file_types_selected, type_name, file_types_selected},
	{file_types_selected, tag_name, tags_selected}};

/***********************/
/* NON-STATE FUNCTIONS */
/***********************/

int move_to_next_state(unsigned int cur_state, unsigned int next_transition) {
	int num_transitions = sizeof(state_transitions) / sizeof(state_transitions[0]);

	/* Iterate through all states until the current state and its return code are found. */
	for (int trans_count = 0; trans_count < num_transitions; trans_count++) {
		if (state_transitions[trans_count].start_state == cur_state && 
				state_transitions[trans_count].next_transition == next_transition) {
			return state_transitions[trans_count].dest_state;
		}
	}

	/* Could not move to next state. */
	return -1; //If this is returned the programme will crash!
}

int is_final_state(enum state_codes state_to_check) {
	
	for (int i = 0; i < sizeof(end_states)/sizeof(end_states[0]); i++) {
		if (state_to_check == end_states[i]) {
			return 1;
		}
	}
	return 0;
}

/* Only accepts png and jpeg. TODO: Add more types and improve how the checks are made. */
int is_file_type_valid(const char* string) {
	if (strcmp(string, "png") == 0) {
		return 1;
	}
	else if (strcmp(string, "jpeg") == 0) {
		return 1;
	}
	else {
		return 0;
	}
}

/* Change name to is_tag_valid and create regex to validate tag names. */
int is_string_ascii(const char* string) {

	char zero_ascii = '0';
	char nine_ascii = '9';
	char big_a_ascii = 'A';
	char big_z_ascii = 'Z';
	char small_a_ascii = 'a';
	char small_z_ascii = 'z';

	for (int i = 0; i < strlen(string); i++) {
		if (!((zero_ascii <= string[i] && string[i] <= nine_ascii) || 
		    (big_a_ascii <= string[i] && string[i] <= big_z_ascii) || 
		    (small_a_ascii <= string[i] && string[i] <= small_z_ascii))) {
			return 0;
		}
	}
	return 1;
}

/* Find better name. */
/* Check whether a string is a path to a directory or a file. */
int check_path_type(const char* string) {

	struct stat file_status;

	if (stat(string,&file_status) == 0) {

		if (file_status.st_mode & S_IFDIR) {
			return directory;
		}
		if (file_status.st_mode & S_IFREG) {
			return file;
		}
	}
		printf("Error: Not a file or directory.\n");
		return invalid_path;
}

/*******************************/
/* MAIN STATE MACHINE FUNCTION */
/*******************************/

int state_machine(void) {
	enum state_codes cur_state = START_STATE;
	enum transition_values next_transition;
	int (* state_func) (const char* s);
	const char* poped_item;

	while (1) {
		state_func = state[cur_state];
		
		poped_item = pop();
	
		/* If a stack related error occurs and cur_state requires a parameter (e.g. not a final state) exit with error. */	
		if (strcmp(poped_item, "error") == 0 && !is_final_state(cur_state)) {
			printf("%s", poped_item);
			exit(EXIT_FAILURE);
		}

		next_transition = state_func(poped_item);

		if (next_transition == fail) {
			printf("Invalid input.\n");
			exit(EXIT_FAILURE);
		}

		if (next_transition == end) {	
			exit(EXIT_SUCCESS);
		}
		cur_state = move_to_next_state(cur_state, next_transition);
	}
}


/*
 * PROBLEM: switch only accepts ints.
 * SOLUTION 1: use if else chain and hard code values.
 * SOLUTION 2: create array with values, lookup index in O(N) time max, then use index in switch case.
 * TODO: implement both and use profiler to select the best, start with SOLUTION 1.
 * */
int select_option_state(const char* s) {
	
	int next_transition;
	int path_type = check_path_type(s);
	
	if (strcmp(s, "-a") == 0) {
		next_transition = add;	
	}
	else if (strcmp(s, "-d") == 0) {
		next_transition = del; 
	}
	else if (is_string_ascii(s)) {
		next_transition = tag_name;
	}
	else if (strcmp(s, "-t") == 0) {
		next_transition = type;
	}
	else if (path_type == directory) {
		next_transition = path_to_dir;
	}
	else if (strcmp(s, "-s") == 0) {
		next_transition = search; 
	}
	else {
		next_transition = fail;
	}

	return next_transition;
}

/******************/
/* ADD TAG STATES */
/******************/

int add_tag_state(const char* s) {
	
	int next_transition;
	int path_type = check_path_type(s);

	if (path_type == directory) {
		next_transition = path_to_dir;
	}
	else if (path_type == file) {
		next_transition = path_to_file;
	}
	else {
		next_transition = fail;
	}

	return next_transition;
}

int add_files_selected_state(const char* s) {
	
	int next_transition;
	int path_type = check_path_type(s);

	if (path_type == file) {
		next_transition = path_to_file;
	}
	else if (is_string_ascii(s)) {
		next_transition = tag_name;
	}
	else {
		next_transition = fail;
	}

	return next_transition;
}

int add_dirs_selected_state(const char* s) {

	int next_transition;
	int path_type = check_path_type(s);
	
	if (path_type == directory) {
		next_transition = path_to_dir;
	}
	else if (is_string_ascii(s)) {
		next_transition = tag_name;
	}
	else if (strcmp(s, "-t") == 0) {
		next_transition = type;
	}
	else {
		next_transition = fail;
	}

	return next_transition;
}

int reading_file_type_state(const char* s) {

	int next_transition;
	
	if (is_file_type_valid(s)) {
		next_transition = type_name;
	}
	else {
		next_transition = fail;
	}
	return next_transition;
}

int file_type_selected_state(const char* s) {

	int next_transition;

	if (is_file_type_valid(s)) {
		next_transition = type_name;
	}
	else if (is_string_ascii(s)) {
		next_transition = tag_name;
	}
	else {
		next_transition = fail;
	}

	return next_transition;
}

int add_tags_selected_state(const char* s) {
	
	int next_transition;
	
	if (strcmp(s, "error") == 0) {
		next_transition = end;
		printf("Adding tags...\n");
	}
	else if (is_string_ascii(s)) {
		next_transition = tag_name;
	}
	else {
		next_transition = fail;
	}

	return next_transition;
}

/*********************/
/* DELETE TAG STATES */
/*********************/

int delete_tag_state(const char* s) {
	
	int next_transition;
	int path_type = check_path_type(s);
	
	if (strcmp(s, "-r") == 0) {
		next_transition = recursive;
	}
	else if (is_string_ascii(s)) {
		next_transition = tag_name;
	}
	else if (path_type == file) {
		next_transition = path_to_file;
	}
	else if (path_type == directory) {
		next_transition = path_to_dir;
	}
	else {
		next_transition = fail;
	}	

	return next_transition;
}

int delete_all_tags_state(const char* s) {
	
	int next_transition;
	int path_type = check_path_type(s);

	if (path_type == file) {
		next_transition = path_to_file;
	}
	else if (path_type == directory) {
		next_transition = path_to_dir;
	}
	else {
		next_transition = fail;
	}

	return next_transition;
}

int delete_files_selected_state(const char* s) {
	
	int next_transition;
	int path_type = check_path_type(s);

	if (path_type == file) {
		next_transition = path_to_file;
	}
	else if (is_string_ascii(s)) {
		next_transition = tag_name;
	}
	else {
		next_transition = fail;
	}

	return next_transition;
}

int delete_dirs_selected_state(const char* s) {
	
	int next_transition;
	int path_type = check_path_type(s);

	if (path_type == directory) {
		next_transition = path_to_dir;
	}
	else if (is_string_ascii(s)) {
		next_transition = tag_name;
	}
	else {
		next_transition = fail;
	}

	return next_transition;
}

int delete_all_files_selected_state(const char* s) {
	
	int next_transition;
	int path_type = check_path_type(s);

	if (strcmp(s, "error") == 0) {
		next_transition = end;
		printf("Deleting tags...\n");
	}
	else if (path_type == file) {
		next_transition = path_to_file;
	}
	else {
		next_transition = fail;
	}

	return next_transition;
}

int delete_all_dirs_selected_state(const char* s) {
	
	int next_transition;
	int path_type = check_path_type(s);

	if (strcmp(s, "error") == 0) {
		next_transition = end;
		printf("Deleting tags...\n");
	}
	else if (path_type == directory) {
		next_transition = path_to_dir;
	}
	else {
		next_transition = fail;
	}

	return next_transition;
}

int delete_tags_selected_state(const char* s) {
	
	int next_transition = fail;
	
	if (strcmp(s, "error") == 0) {
		next_transition = end;
		printf("Deleting tags...\n");
	}
	else if (is_string_ascii(s)) {
		next_transition = tag_name;
	}
	else {
		next_transition = fail;
	}
	
	return next_transition;
}

/*********************/
/* SEARCH TAG STATES */
/*********************/

int tags_selected_state(const char* s) {
	
	int next_transition;
	
	if (strcmp(s, "error") == 0) {
		next_transition = end;
		printf("Searching tags...\n");
	}
	else if (is_string_ascii(s)) {
		next_transition = tag_name;
	}
	else {
		next_transition = fail;
	}
	
	return next_transition;
}

int reading_file_types_state(const char* s) {
	
	int next_transition;
	
	if (is_file_type_valid(s)) {
		next_transition = type_name;
	}
	else {
		next_transition = fail;
	}
	return next_transition;
}

int search_dir_selected_state(const char* s) {
	
	int next_transition = fail;
	int path_type = check_path_type(s);
	
	if (strcmp(s, "-t") == 0) {
		next_transition = type_name;
	}
	else if (path_type == directory) {
		next_transition = path_to_dir;
	}
	else if (is_string_ascii(s)) {
		next_transition = tag_name;
	}
	else {
		next_transition = fail;
	}
	
	return next_transition;
}

int search_all_tags_state(const char* s) {

	int next_transition = fail;
	
	if (strcmp(s, "error") == 0) {
		next_transition = end;
		printf("Searching tags...\n");
	}
	else if (strcmp(s, "-n") == 0) {
		next_transition = no_tag;
	}
	else if (strcmp(s, "-tg") == 0) {
		next_transition = tag_only;
	}
	else {
		next_transition = fail;
	}

	return next_transition;
}

int file_types_selected_state(const char* s) {
	
	int next_transition = fail;
	
	if (is_file_type_valid(s)) {
		next_transition = type_name;
	}
	else if (is_string_ascii(s)) {
		next_transition = tag_name;
	}
	else {
		next_transition = fail;
	}
	
	return next_transition;
}

int search_no_tags_state(const char* s) {
	
	int next_transition = fail;
	
	if (strcmp(s, "error") == 0) {
		next_transition = end;
		printf("Searching tags...\n");
	}
	else {
		next_transition = fail;
	}
	
	return next_transition;
}

int search_tags_only_state(const char* s) {
	
	int next_transition = fail;
	
	if (strcmp(s, "error") == 0) {
		next_transition = end;
		printf("Searching tags...\n");
	}
	else {
		next_transition = fail;
	}
	
	return next_transition;
}

