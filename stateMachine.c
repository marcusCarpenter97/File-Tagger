#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stateMachine.h"
#include "inputStack.h"

int (* state[])(const char* s) = 
	{waiting_for_input_state, select_option_state, add_tag_state, add_files_selected_state, add_tags_selected_state, add_tags_state};

struct transition state_transitions[] = {
	{waiting_for_input, ok, select_option},
	{waiting_for_input, fail, waiting_for_input},
	{select_option, ok, add_tag},
	{select_option, fail, waiting_for_input},
	{add_tag, ok, add_files_selected},
	{add_tag, fail, waiting_for_input},
	{add_files_selected, ok, add_tags_selected},
	{add_files_selected, repeat, add_files_selected},
	{add_files_selected, fail, waiting_for_input},
	{add_tags_selected, ok, add_tags},
	{add_tags_selected, repeat, add_tags_selected},
	{add_tags_selected, fail, waiting_for_input}};

int move_to_next_state(unsigned int cur_state, unsigned int ret_code) {
	int num_transitions = sizeof(state_transitions) / sizeof(state_transitions[0]);

	/* Iterate through all states until the current state and its return code are found. */
	for (int trans_count = 0; trans_count < num_transitions; trans_count++) {
		if (state_transitions[trans_count].start_state == cur_state && state_transitions[trans_count].ret_code == ret_code) {
			return state_transitions[trans_count].dest_state;
		}
	}

	/* Could not move to next state. */
	return -1;
}

int verify_input(void) {
	enum state_codes cur_state = START_STATE;
	enum ret_codes ret_code;
	int (* state_func) (const char* s);
	
	while (1) {
		state_func = state[cur_state];
		
		/* Give next item in the stack as parameter to state function. */
		ret_code = state_func(pop());

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

/* Deprecated. */
int waiting_for_input_state(const char* s) {
	printf("waiting for input.\n%s\n",s);
	if (strcmp(s, PROGRAMME_NAME) == 0) {
		return ok;
	}
	return fail;
}

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

/*--------------
 *  Add states
 *--------------*/

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

	return ok;
}

int add_tags_selected_state(const char* s) {
	printf("tags selected.\n");
	return ok;
}

int add_tags_state(const char* s) {
	printf("adding tags.\n");
	return ok;
}
