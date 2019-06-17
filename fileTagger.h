int verify_input(int num_of_transitions, char* transitions[]);

int waiting_for_input_state(void);
int select_option_state(void);
int add_tag_state(void);
int add_files_selected_state(void);
int add_tags_selected_state(void);
int add_tags(void);

int (* state[])(void) = {waiting_for_input_state, select_option_state, add_tag_state, add_files_selected_state, add_tags_selected_state,
       	add_tags};
enum state_codes {waiting_for_input, select_option, add_tag, add_files_selected, add_tags_selected, add_tags};

enum ret_codes {ok, fail, repeat};

struct transition {
	enum state_codes start_state;
	enum ret_codes ret_code;
	enum start_codes dest_state;
};

struct transition state_transitions[] = {
	{waiting_for_input, ok, select_option},
	{waiting_for_input, fail, witing_for_input},
	{select_option, ok, add_tag},
	{select_option, fail, witing_for_input},
	{add_tag, ok, add_filess_selected},
	{add_tag, fail, witing_for_input},
	{add_files_selected, ok, add_tags_selected},
	{add_files_selected, repeat, add_files_selected},
	{add_files_selected, fail, waiting_for_input},
	{add_tags_selected, ok, add_tags},
	{add_tags_selected, repeat, add_tags_selected},
	{add_tags_selected, fail, waiting_for_input}};

#define START_STATE waiting_for_input
#define EXIT_STATE add_tags

