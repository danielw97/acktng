#ifndef DEC_SOCKET_H
#define DEC_SOCKET_H 1

/*
 * Public interface for socket.c — OS-dependent socket/network code.
 */

extern int global_port;
extern const char echo_off_str[];
extern const char echo_on_str[];
extern const char go_ahead_str[];

int init_socket(int port);
void game_loop(int control);
bool write_to_descriptor(int desc, char *txt, int length);
void write_to_buffer(DESCRIPTOR_DATA *d, const char *txt, int length);
bool process_output(DESCRIPTOR_DATA *d, bool fPrompt);
void read_from_buffer(DESCRIPTOR_DATA *d);
void free_desc(DESCRIPTOR_DATA *d);
void queue_login_greeting(DESCRIPTOR_DATA *d);

#endif /* DEC_SOCKET_H */
