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


enum TDB_proto_msg_type {
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
typedef enum TDB_proto_msg_type TDB_proto_msg_type;

struct TDB_replica_info {
	char uuid[16];
	int port;
	char *address;
};
typedef struct TDB_replica_info TDB_replica_info;

struct TDB_msg_bootstrap {
	TDB_proto_msg_type type;
	char uuid[16];
	int port;
	char *address;
};
typedef struct TDB_msg_bootstrap TDB_msg_bootstrap;

struct TDB_msg_resp_bootstrap {
	TDB_proto_msg_type type;
	bool_t success;
	struct {
		u_int known_replicas_len;
		TDB_replica_info *known_replicas_val;
	} known_replicas;
};
typedef struct TDB_msg_resp_bootstrap TDB_msg_resp_bootstrap;

struct TDB_msg_connect {
	TDB_proto_msg_type type;
	char magic[4];
	char api_version[4];
};
typedef struct TDB_msg_connect TDB_msg_connect;

struct TDB_msg_resp_connect {
	TDB_proto_msg_type type;
	char api_version[4];
	bool_t success;
};
typedef struct TDB_msg_resp_connect TDB_msg_resp_connect;

struct TDB_msg_consensus_log_entry {
	struct {
		u_int data_len;
		char *data_val;
	} data;
};
typedef struct TDB_msg_consensus_log_entry TDB_msg_consensus_log_entry;

struct TDB_msg_consensus {
	TDB_proto_msg_type type;
	bool_t is_vote;
	char uuid_id[16];
	u_long term;
	u_long log_index;
	u_long log_term;
	u_long commit_index;
	struct {
		u_int log_entries_len;
		TDB_msg_consensus_log_entry *log_entries_val;
	} log_entries;
};
typedef struct TDB_msg_consensus TDB_msg_consensus;

struct TDB_msg_resp_consensus {
	TDB_proto_msg_type type;
	u_long term;
	bool_t success;
};
typedef struct TDB_msg_resp_consensus TDB_msg_resp_consensus;

enum TDB_msg_control_action_type {
	ATTACH_APPEND = 1,
	ATTACH_DATA = 2,
	ATTACH_VOTE = 3,
	IDLE_FOLLOWER = 4,
	DESTROY = 5,
	EXIT = 6,
};
typedef enum TDB_msg_control_action_type TDB_msg_control_action_type;

struct TDB_msg_control {
	TDB_proto_msg_type type;
	TDB_msg_control_action_type action;
	char uuid[16];
};
typedef struct TDB_msg_control TDB_msg_control;

struct TDB_msg_resp_control {
	TDB_proto_msg_type type;
	bool_t success;
};
typedef struct TDB_msg_resp_control TDB_msg_resp_control;

enum TDB_msg_data_transaction {
	SINGLE = 1,
	BEGIN = 2,
	COMMIT = 3,
	ABORT = 4,
};
typedef enum TDB_msg_data_transaction TDB_msg_data_transaction;

enum TDB_msg_data_verb {
	PUT = 1,
	GET = 2,
	DELETE = 3,
	WATCH = 4,
	UNWATCH = 5,
};
typedef enum TDB_msg_data_verb TDB_msg_data_verb;

enum TDB_msg_data_mode {
	READ_WRITE = 1,
	READ_ONLY = 2,
	READ_BOUNDED = 3,
	READ_TIMESTAMP = 4,
};
typedef enum TDB_msg_data_mode TDB_msg_data_mode;

struct TDB_msg_data_op {
	TDB_msg_data_verb verb;
	struct {
		u_int data_len;
		char *data_val;
	} data;
};
typedef struct TDB_msg_data_op TDB_msg_data_op;

struct TDB_msg_data {
	TDB_proto_msg_type type;
	TDB_msg_data_mode mode;
	TDB_msg_data_transaction transaction;
	struct {
		u_int ops_len;
		TDB_msg_data_op *ops_val;
	} ops;
};
typedef struct TDB_msg_data TDB_msg_data;

struct TDB_msg_resp_data {
	TDB_proto_msg_type type;
	bool_t success;
};
typedef struct TDB_msg_resp_data TDB_msg_resp_data;

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_TDB_proto_msg_type (XDR *, TDB_proto_msg_type*);
extern  bool_t xdr_TDB_replica_info (XDR *, TDB_replica_info*);
extern  bool_t xdr_TDB_msg_bootstrap (XDR *, TDB_msg_bootstrap*);
extern  bool_t xdr_TDB_msg_resp_bootstrap (XDR *, TDB_msg_resp_bootstrap*);
extern  bool_t xdr_TDB_msg_connect (XDR *, TDB_msg_connect*);
extern  bool_t xdr_TDB_msg_resp_connect (XDR *, TDB_msg_resp_connect*);
extern  bool_t xdr_TDB_msg_consensus_log_entry (XDR *, TDB_msg_consensus_log_entry*);
extern  bool_t xdr_TDB_msg_consensus (XDR *, TDB_msg_consensus*);
extern  bool_t xdr_TDB_msg_resp_consensus (XDR *, TDB_msg_resp_consensus*);
extern  bool_t xdr_TDB_msg_control_action_type (XDR *, TDB_msg_control_action_type*);
extern  bool_t xdr_TDB_msg_control (XDR *, TDB_msg_control*);
extern  bool_t xdr_TDB_msg_resp_control (XDR *, TDB_msg_resp_control*);
extern  bool_t xdr_TDB_msg_data_transaction (XDR *, TDB_msg_data_transaction*);
extern  bool_t xdr_TDB_msg_data_verb (XDR *, TDB_msg_data_verb*);
extern  bool_t xdr_TDB_msg_data_mode (XDR *, TDB_msg_data_mode*);
extern  bool_t xdr_TDB_msg_data_op (XDR *, TDB_msg_data_op*);
extern  bool_t xdr_TDB_msg_data (XDR *, TDB_msg_data*);
extern  bool_t xdr_TDB_msg_resp_data (XDR *, TDB_msg_resp_data*);

#else /* K&R C */
extern bool_t xdr_TDB_proto_msg_type ();
extern bool_t xdr_TDB_replica_info ();
extern bool_t xdr_TDB_msg_bootstrap ();
extern bool_t xdr_TDB_msg_resp_bootstrap ();
extern bool_t xdr_TDB_msg_connect ();
extern bool_t xdr_TDB_msg_resp_connect ();
extern bool_t xdr_TDB_msg_consensus_log_entry ();
extern bool_t xdr_TDB_msg_consensus ();
extern bool_t xdr_TDB_msg_resp_consensus ();
extern bool_t xdr_TDB_msg_control_action_type ();
extern bool_t xdr_TDB_msg_control ();
extern bool_t xdr_TDB_msg_resp_control ();
extern bool_t xdr_TDB_msg_data_transaction ();
extern bool_t xdr_TDB_msg_data_verb ();
extern bool_t xdr_TDB_msg_data_mode ();
extern bool_t xdr_TDB_msg_data_op ();
extern bool_t xdr_TDB_msg_data ();
extern bool_t xdr_TDB_msg_resp_data ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_PROTO_H_RPCGEN */
