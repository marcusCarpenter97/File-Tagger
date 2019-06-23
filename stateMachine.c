#include "stateMachine.h"
#include "inputStack.h"

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

//int num_of_transitions, char* transitions[]
int verify_input(void) {
	enum state_codes cur_state = START_STATE;
	enum ret_codes ret_code;
	int (* state_func) (void);
	
	while (1) {
		state_func = state[cur_state];
		ret_code = state_func();
		if (cur_state == EXIT_STATE) {	
			break;
		}
		cur_state = move_to_next_state(cur_state, ret_code);
	}
}

int waiting_for_input_state(void) {
	printf("waiting for input.\n");
	return ok;
}

int select_option_state(void) {
	printf("select option.\n");
	return ok;
}

/*--------------
 *  Add states
 *--------------*/

int add_tag_state(void) {
	printf("add tag.\n");
	return ok;
}

int add_files_selected_state(void) {
	printf("add files.\n");
	return ok;
}

int add_tags_selected_state(void) {
	printf("tags selected.\n");
	return ok;
}

int add_tags_state(void) {
	printf("adding tags.\n");
	return ok;
}
