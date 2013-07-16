/* Generated by the protocol buffer compiler.  DO NOT EDIT! */

#ifndef PROTOBUF_C_database_2eproto__INCLUDED
#define PROTOBUF_C_database_2eproto__INCLUDED

#include <google/protobuf-c/protobuf-c.h>

PROTOBUF_C_BEGIN_DECLS


typedef struct _Haven__Database__LogCommand Haven__Database__LogCommand;
typedef struct _Haven__Database__AppendEntry Haven__Database__AppendEntry;
typedef struct _Haven__Database__AppendEntries Haven__Database__AppendEntries;
typedef struct _Haven__Database__CatchUp Haven__Database__CatchUp;
typedef struct _Haven__Database__CatchUpResult Haven__Database__CatchUpResult;


/* --- enums --- */

typedef enum _Haven__Database__LogCommand__CommandType {
    HAVEN__DATABASE__LOG_COMMAND__COMMAND_TYPE__PUT = 4,
    HAVEN__DATABASE__LOG_COMMAND__COMMAND_TYPE__GET = 5,
    HAVEN__DATABASE__LOG_COMMAND__COMMAND_TYPE__DELETE = 6
} Haven__Database__LogCommand__CommandType;
typedef enum _Haven__Database__CatchUpResult__CatchUpType {
    HAVEN__DATABASE__CATCH_UP_RESULT__CATCH_UP_TYPE__LOG_ENTRIES = 1,
    HAVEN__DATABASE__CATCH_UP_RESULT__CATCH_UP_TYPE__SNAPSHOT_REPLACEMENT = 2
} Haven__Database__CatchUpResult__CatchUpType;

/* --- messages --- */

struct  _Haven__Database__LogCommand {
    ProtobufCMessage base;
    Haven__Database__LogCommand__CommandType type;
    ProtobufCBinaryData key;
    protobuf_c_boolean has_revision;
    int64_t revision;
    protobuf_c_boolean has_value;
    ProtobufCBinaryData value;
};
#define HAVEN__DATABASE__LOG_COMMAND__INIT \
    { PROTOBUF_C_MESSAGE_INIT (&haven__database__log_command__descriptor) \
        , 0, {0,NULL}, 0,0, 0,{0,NULL} }


struct  _Haven__Database__AppendEntry {
    ProtobufCMessage base;
    int64_t term;
    int64_t index;
    Haven__Database__LogCommand* command;
};
#define HAVEN__DATABASE__APPEND_ENTRY__INIT \
    { PROTOBUF_C_MESSAGE_INIT (&haven__database__append_entry__descriptor) \
        , 0, 0, NULL }


struct  _Haven__Database__AppendEntries {
    ProtobufCMessage base;
    protobuf_c_boolean is_transaction;
    size_t n_entries;
    Haven__Database__AppendEntry** entries;
};
#define HAVEN__DATABASE__APPEND_ENTRIES__INIT \
    { PROTOBUF_C_MESSAGE_INIT (&haven__database__append_entries__descriptor) \
        , 0, 0,NULL }


struct  _Haven__Database__CatchUp {
    ProtobufCMessage base;
    int64_t candidate_id;
    int64_t last_log_term;
    int64_t last_log_index;
};
#define HAVEN__DATABASE__CATCH_UP__INIT \
    { PROTOBUF_C_MESSAGE_INIT (&haven__database__catch_up__descriptor) \
        , 0, 0, 0 }


struct  _Haven__Database__CatchUpResult {
    ProtobufCMessage base;
    int64_t current_term;
    Haven__Database__CatchUpResult__CatchUpType type;
    Haven__Database__AppendEntries* entries;
    size_t n_snapshot;
    ProtobufCBinaryData* snapshot;
};
#define HAVEN__DATABASE__CATCH_UP_RESULT__INIT \
    { PROTOBUF_C_MESSAGE_INIT (&haven__database__catch_up_result__descriptor) \
        , 0, 0, NULL, 0,NULL }


/* Haven__Database__LogCommand methods */
void   haven__database__log_command__init
(Haven__Database__LogCommand*         message);
size_t haven__database__log_command__get_packed_size
(const Haven__Database__LogCommand*   message);
size_t haven__database__log_command__pack
(const Haven__Database__LogCommand*   message,
 uint8_t*             out);
size_t haven__database__log_command__pack_to_buffer
(const Haven__Database__LogCommand*   message,
 ProtobufCBuffer*     buffer);
Haven__Database__LogCommand*
haven__database__log_command__unpack
(ProtobufCAllocator*  allocator,
 size_t               len,
 const uint8_t*       data);
void   haven__database__log_command__free_unpacked
(Haven__Database__LogCommand* message,
 ProtobufCAllocator* allocator);
/* Haven__Database__AppendEntry methods */
void   haven__database__append_entry__init
(Haven__Database__AppendEntry*         message);
size_t haven__database__append_entry__get_packed_size
(const Haven__Database__AppendEntry*   message);
size_t haven__database__append_entry__pack
(const Haven__Database__AppendEntry*   message,
 uint8_t*             out);
size_t haven__database__append_entry__pack_to_buffer
(const Haven__Database__AppendEntry*   message,
 ProtobufCBuffer*     buffer);
Haven__Database__AppendEntry*
haven__database__append_entry__unpack
(ProtobufCAllocator*  allocator,
 size_t               len,
 const uint8_t*       data);
void   haven__database__append_entry__free_unpacked
(Haven__Database__AppendEntry* message,
 ProtobufCAllocator* allocator);
/* Haven__Database__AppendEntries methods */
void   haven__database__append_entries__init
(Haven__Database__AppendEntries*         message);
size_t haven__database__append_entries__get_packed_size
(const Haven__Database__AppendEntries*   message);
size_t haven__database__append_entries__pack
(const Haven__Database__AppendEntries*   message,
 uint8_t*             out);
size_t haven__database__append_entries__pack_to_buffer
(const Haven__Database__AppendEntries*   message,
 ProtobufCBuffer*     buffer);
Haven__Database__AppendEntries*
haven__database__append_entries__unpack
(ProtobufCAllocator*  allocator,
 size_t               len,
 const uint8_t*       data);
void   haven__database__append_entries__free_unpacked
(Haven__Database__AppendEntries* message,
 ProtobufCAllocator* allocator);
/* Haven__Database__CatchUp methods */
void   haven__database__catch_up__init
(Haven__Database__CatchUp*         message);
size_t haven__database__catch_up__get_packed_size
(const Haven__Database__CatchUp*   message);
size_t haven__database__catch_up__pack
(const Haven__Database__CatchUp*   message,
 uint8_t*             out);
size_t haven__database__catch_up__pack_to_buffer
(const Haven__Database__CatchUp*   message,
 ProtobufCBuffer*     buffer);
Haven__Database__CatchUp*
haven__database__catch_up__unpack
(ProtobufCAllocator*  allocator,
 size_t               len,
 const uint8_t*       data);
void   haven__database__catch_up__free_unpacked
(Haven__Database__CatchUp* message,
 ProtobufCAllocator* allocator);
/* Haven__Database__CatchUpResult methods */
void   haven__database__catch_up_result__init
(Haven__Database__CatchUpResult*         message);
size_t haven__database__catch_up_result__get_packed_size
(const Haven__Database__CatchUpResult*   message);
size_t haven__database__catch_up_result__pack
(const Haven__Database__CatchUpResult*   message,
 uint8_t*             out);
size_t haven__database__catch_up_result__pack_to_buffer
(const Haven__Database__CatchUpResult*   message,
 ProtobufCBuffer*     buffer);
Haven__Database__CatchUpResult*
haven__database__catch_up_result__unpack
(ProtobufCAllocator*  allocator,
 size_t               len,
 const uint8_t*       data);
void   haven__database__catch_up_result__free_unpacked
(Haven__Database__CatchUpResult* message,
 ProtobufCAllocator* allocator);
/* --- per-message closures --- */

typedef void (*Haven__Database__LogCommand_Closure)
(const Haven__Database__LogCommand* message,
 void* closure_data);
typedef void (*Haven__Database__AppendEntry_Closure)
(const Haven__Database__AppendEntry* message,
 void* closure_data);
typedef void (*Haven__Database__AppendEntries_Closure)
(const Haven__Database__AppendEntries* message,
 void* closure_data);
typedef void (*Haven__Database__CatchUp_Closure)
(const Haven__Database__CatchUp* message,
 void* closure_data);
typedef void (*Haven__Database__CatchUpResult_Closure)
(const Haven__Database__CatchUpResult* message,
 void* closure_data);

/* --- services --- */

typedef struct _Haven__Database__Database_Service Haven__Database__Database_Service;
struct _Haven__Database__Database_Service {
    ProtobufCService base;
    void (*follower_catch_up)(Haven__Database__Database_Service* service,
                              const Haven__Database__CatchUp* input,
                              Haven__Database__CatchUpResult_Closure closure,
                              void* closure_data);
};
typedef void (*Haven__Database__Database_ServiceDestroy)(Haven__Database__Database_Service*);
void haven__database__database__init(Haven__Database__Database_Service* service,
                                     Haven__Database__Database_ServiceDestroy destroy);
#define HAVEN__DATABASE__DATABASE__BASE_INIT \
    { &haven__database__database__descriptor, protobuf_c_service_invoke_internal, NULL }
#define HAVEN__DATABASE__DATABASE__INIT(function_prefix__) \
    { HAVEN__DATABASE__DATABASE__BASE_INIT,\
        function_prefix__ ## follower_catch_up  }
void haven__database__database__follower_catch_up(ProtobufCService* service,
        const Haven__Database__CatchUp* input,
        Haven__Database__CatchUpResult_Closure closure,
        void* closure_data);

/* --- descriptors --- */

extern const ProtobufCMessageDescriptor haven__database__log_command__descriptor;
extern const ProtobufCEnumDescriptor    haven__database__log_command__command_type__descriptor;
extern const ProtobufCMessageDescriptor haven__database__append_entry__descriptor;
extern const ProtobufCMessageDescriptor haven__database__append_entries__descriptor;
extern const ProtobufCMessageDescriptor haven__database__catch_up__descriptor;
extern const ProtobufCMessageDescriptor haven__database__catch_up_result__descriptor;
extern const ProtobufCEnumDescriptor    haven__database__catch_up_result__catch_up_type__descriptor;
extern const ProtobufCServiceDescriptor haven__database__database__descriptor;

PROTOBUF_C_END_DECLS


#endif  /* PROTOBUF_database_2eproto__INCLUDED */
