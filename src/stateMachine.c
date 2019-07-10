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
enum state_codes end_states[] = {add_tags_selected};

/* STATE: Array of function pointers containing all state functions in the state machine. */
int (* state[])(const char* s) = 
	{select_option_state, add_tag_state, add_dirs_selected_state, add_files_selected_state, add_tags_selected_state};

/* STATE TRANSITIONS: Array of transition enums containing all possible state transitions in the state machine. */
struct transition state_transitions[] = {
	{select_option, ok, add_tag},
	{add_tag, ok, add_files_selected},
	{add_files_selected, ok, add_tags_selected},
	{add_files_selected, repeat, add_files_selected},
	{add_tags_selected, repeat, add_tags_selected}};

/***********************/
/* NON-STATE FUNCTIONS */
/***********************/

int move_to_next_state(unsigned int cur_state, unsigned int ret_code) {
	int num_transitions = sizeof(state_transitions) / sizeof(state_transitions[0]);

	/* Iterate through all states until the current state and its return code are found. */
	for (int trans_count = 0; trans_count < num_transitions; trans_count++) {
		if (state_transitions[trans_count].start_state == cur_state && state_transitions[trans_count].ret_code == ret_code) {
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
int check_path(const char* string) {

	struct stat file_status;

	if (stat(string,&file_status) == 0) {

		if (file_status.st_mode & S_IFDIR) {
			printf("Directory!\n");
			return directory;
		}
		if (file_status.st_mode & S_IFREG) {
			printf("File!\n");
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
	enum ret_codes ret_code;
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

		ret_code = state_func(poped_item);

		if (ret_code == fail) {
			printf("Invalid input.\n");
			exit(EXIT_FAILURE);
		}

		if (cur_state == EXIT_STATE) {	
			exit(EXIT_SUCCESS);
		}
		cur_state = move_to_next_state(cur_state, ret_code);
	}
}


/*
 * PROBLEM: switch only accepts ints.
 * SOLUTION 1: use if else chain and hard code values.
 * SOLUTION 2: create array with values, lookup index in O(N) time max, then use index in switch case.
 * TODO: implement both and use profiler to select the best, start with SOLUTION 1.
 * */
int select_option_state(const char* s) {
	printf("select option.\n");
	
	int ret_code;

	if (strcmp(s, "-a") == 0) {
		ret_code = ok;	
	}
	else {
		ret_code = fail;
	}

	return ret_code;
}

/******************/
/* ADD TAG STATES */
/******************/

int add_tag_state(const char* s) {
	printf("add tag.\n");
	
	int ret_code;
	int path_type = check_path(s);

	if (path_type == directory) {
		ret_code = fail; //Add later.
	}
	else if (path_type == file) {
		ret_code = ok;
	}
	else {
		ret_code = fail;
	}

	return ret_code;
}

int add_files_selected_state(const char* s) {
	printf("add files.\n");
	
	int ret_code;
	int path_type = check_path(s);

	if (path_type == file) {
		ret_code = repeat;
	}
	else if (is_string_ascii(s)) {
		ret_code = ok;
	}
	else {
		ret_code = fail;
	}

	return ret_code;
}

int add_dirs_selected_state(const char* s) {
	return fail;
}

int add_tags_selected_state(const char* s) {
	printf("tags selected.\n");
	
	int ret_code;
	
	if (strcmp(s, "error") == 0) {
		ret_code = ok;
		printf("Adding tags...\n");
	}
	else if (is_string_ascii(s)) {
		ret_code = repeat;
	}
	else {
		ret_code = fail;
	}

	return ret_code;
}

