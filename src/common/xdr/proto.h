/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _PROTO_H_RPCGEN
#define _PROTO_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


enum proto_msg_type {
	BOOTSTRAP = 1,
	CONNECT = 2,
	CONSENSUS = 3,
	CONTROL = 4,
	DATA = 5,
	BOOTSTRAP_R = 6,
	CONNECT_R = 7,
	CONSENSUS_R = 8,
	CONTROL_R = 9,
	DATA_R = 10,
};
typedef enum proto_msg_type proto_msg_type;

struct replica_info {
	char uuid[16];
	int port;
	char *address;
};
typedef struct replica_info replica_info;

struct msg_bootstrap {
	proto_msg_type type;
	char uuid[16];
	int port;
	char *address;
};
typedef struct msg_bootstrap msg_bootstrap;

struct msg_resp_bootstrap {
	proto_msg_type type;
	bool_t success;
	struct {
		u_int known_replicas_len;
		replica_info *known_replicas_val;
	} known_replicas;
};
typedef struct msg_resp_bootstrap msg_resp_bootstrap;

struct msg_connect {
	char magic[4];
	char api_version[4];
	proto_msg_type type;
};
typedef struct msg_connect msg_connect;

struct msg_resp_connect {
	char api_version[4];
	proto_msg_type type;
	bool_t success;
};
typedef struct msg_resp_connect msg_resp_connect;

struct msg_consensus_log_entry {
	struct {
		u_int data_len;
		char *data_val;
	} data;
};
typedef struct msg_consensus_log_entry msg_consensus_log_entry;

struct msg_consensus {
	proto_msg_type type;
	bool_t is_vote;
	char uuid_id[16];
	u_long term;
	u_long log_index;
	u_long log_term;
	u_long commit_index;
	struct {
		u_int log_entries_len;
		msg_consensus_log_entry *log_entries_val;
	} log_entries;
};
typedef struct msg_consensus msg_consensus;

struct msg_resp_consensus {
	u_long term;
	bool_t success;
};
typedef struct msg_resp_consensus msg_resp_consensus;

enum msg_control_action_type {
	ATTACH_APPEND = 1,
	ATTACH_DATA = 2,
	ATTACH_VOTE = 3,
	IDLE_FOLLOWER = 4,
	DESTROY = 5,
	EXIT = 6,
};
typedef enum msg_control_action_type msg_control_action_type;

struct msg_control {
	msg_control_action_type action;
	char uuid[16];
};
typedef struct msg_control msg_control;

struct msg_resp_control {
	bool_t success;
};
typedef struct msg_resp_control msg_resp_control;

enum msg_data_transaction {
	SINGLE = 1,
	BEGIN = 2,
	COMMIT = 3,
	ABORT = 4,
};
typedef enum msg_data_transaction msg_data_transaction;

enum msg_data_verb {
	PUT = 1,
	GET = 2,
	DELETE = 3,
	WATCH = 4,
	UNWATCH = 5,
};
typedef enum msg_data_verb msg_data_verb;

enum msg_data_mode {
	READ_WRITE = 1,
	READ_ONLY = 2,
	READ_BOUNDED = 3,
	READ_TIMESTAMP = 4,
};
typedef enum msg_data_mode msg_data_mode;

struct msg_data_op {
	msg_data_verb verb;
	struct {
		u_int data_len;
		char *data_val;
	} data;
};
typedef struct msg_data_op msg_data_op;

struct msg_data {
	msg_data_mode mode;
	msg_data_transaction transaction;
	struct {
		u_int ops_len;
		msg_data_op *ops_val;
	} ops;
};
typedef struct msg_data msg_data;

struct msg_resp_data {
	bool_t success;
};
typedef struct msg_resp_data msg_resp_data;

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_proto_msg_type (XDR *, proto_msg_type*);
extern  bool_t xdr_replica_info (XDR *, replica_info*);
extern  bool_t xdr_msg_bootstrap (XDR *, msg_bootstrap*);
extern  bool_t xdr_msg_resp_bootstrap (XDR *, msg_resp_bootstrap*);
extern  bool_t xdr_msg_connect (XDR *, msg_connect*);
extern  bool_t xdr_msg_resp_connect (XDR *, msg_resp_connect*);
extern  bool_t xdr_msg_consensus_log_entry (XDR *, msg_consensus_log_entry*);
extern  bool_t xdr_msg_consensus (XDR *, msg_consensus*);
extern  bool_t xdr_msg_resp_consensus (XDR *, msg_resp_consensus*);
extern  bool_t xdr_msg_control_action_type (XDR *, msg_control_action_type*);
extern  bool_t xdr_msg_control (XDR *, msg_control*);
extern  bool_t xdr_msg_resp_control (XDR *, msg_resp_control*);
extern  bool_t xdr_msg_data_transaction (XDR *, msg_data_transaction*);
extern  bool_t xdr_msg_data_verb (XDR *, msg_data_verb*);
extern  bool_t xdr_msg_data_mode (XDR *, msg_data_mode*);
extern  bool_t xdr_msg_data_op (XDR *, msg_data_op*);
extern  bool_t xdr_msg_data (XDR *, msg_data*);
extern  bool_t xdr_msg_resp_data (XDR *, msg_resp_data*);

#else /* K&R C */
extern bool_t xdr_proto_msg_type ();
extern bool_t xdr_replica_info ();
extern bool_t xdr_msg_bootstrap ();
extern bool_t xdr_msg_resp_bootstrap ();
extern bool_t xdr_msg_connect ();
extern bool_t xdr_msg_resp_connect ();
extern bool_t xdr_msg_consensus_log_entry ();
extern bool_t xdr_msg_consensus ();
extern bool_t xdr_msg_resp_consensus ();
extern bool_t xdr_msg_control_action_type ();
extern bool_t xdr_msg_control ();
extern bool_t xdr_msg_resp_control ();
extern bool_t xdr_msg_data_transaction ();
extern bool_t xdr_msg_data_verb ();
extern bool_t xdr_msg_data_mode ();
extern bool_t xdr_msg_data_op ();
extern bool_t xdr_msg_data ();
extern bool_t xdr_msg_resp_data ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_PROTO_H_RPCGEN */
