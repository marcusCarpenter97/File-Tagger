#include "fileTagger.h"

int verify_input(int num_of_transitions, char* transitions[]) {
	enum state_codes cur_state = IDLE_STATE;
	enum ret_codes ret_code;
	int (* state_func) (void);
	
	while (1) {
		state_func = state[cur_state];
		ret_code = state_func();
		if (ret_code == EXIT_STATE) {	
			break;
		}
		cur_state = move_to_next_state(cur_state, ret_code);
	}
}
