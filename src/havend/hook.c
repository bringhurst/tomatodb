
// for each replica,
//    create a hook task for each remote replica
//    each hook creates two sockets to the remote replica, one for vote and for append.
//    each hook creates two channels that allow the replica to send to the two sockets.

typedef struct HVN_hook_t {
    uuid_t remote_uuid;
    int report_port;
    char* remote_address;
    int fd_append;
    int fd_vote;
    Channel* append_chan;
    Channel* vote_chan;
    Channel* exit_chan;
} HVN_hook_t;
