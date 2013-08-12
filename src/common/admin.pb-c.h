/* Generated by the protocol buffer compiler.  DO NOT EDIT! */

#ifndef PROTOBUF_C_admin_2eproto__INCLUDED
#define PROTOBUF_C_admin_2eproto__INCLUDED

#include <google/protobuf-c/protobuf-c.h>

PROTOBUF_C_BEGIN_DECLS


typedef struct _Haven__Admin__ModifyPeers Haven__Admin__ModifyPeers;
typedef struct _Haven__Admin__ModifyPeers__Peer Haven__Admin__ModifyPeers__Peer;
typedef struct _Haven__Admin__Result Haven__Admin__Result;


/* --- enums --- */

typedef enum _Haven__Admin__ModifyPeers__ModifyType {
    HVN__ADMIN__MODIFY_PEERS__MODIFY_TYPE__ADD = 1,
    HVN__ADMIN__MODIFY_PEERS__MODIFY_TYPE__REMOVE = 2
} Haven__Admin__ModifyPeers__ModifyType;

/* --- messages --- */

struct  _Haven__Admin__ModifyPeers__Peer {
    ProtobufCMessage base;
    char* hostname;
    int32_t port;
};
#define HVN__ADMIN__MODIFY_PEERS__PEER__INIT \
    { PROTOBUF_C_MESSAGE_INIT (&haven__admin__modify_peers__peer__descriptor) \
        , NULL, 0 }


struct  _Haven__Admin__ModifyPeers {
    ProtobufCMessage base;
    Haven__Admin__ModifyPeers__ModifyType type;
    size_t n_peers;
    Haven__Admin__ModifyPeers__Peer** peers;
};
#define HVN__ADMIN__MODIFY_PEERS__INIT \
    { PROTOBUF_C_MESSAGE_INIT (&haven__admin__modify_peers__descriptor) \
        , 0, 0,NULL }


struct  _Haven__Admin__Result {
    ProtobufCMessage base;
    protobuf_c_boolean success;
};
#define HVN__ADMIN__RESULT__INIT \
    { PROTOBUF_C_MESSAGE_INIT (&haven__admin__result__descriptor) \
        , 0 }


/* Haven__Admin__ModifyPeers__Peer methods */
void   haven__admin__modify_peers__peer__init
(Haven__Admin__ModifyPeers__Peer*         message);
/* Haven__Admin__ModifyPeers methods */
void   haven__admin__modify_peers__init
(Haven__Admin__ModifyPeers*         message);
size_t haven__admin__modify_peers__get_packed_size
(const Haven__Admin__ModifyPeers*   message);
size_t haven__admin__modify_peers__pack
(const Haven__Admin__ModifyPeers*   message,
 uint8_t*             out);
size_t haven__admin__modify_peers__pack_to_buffer
(const Haven__Admin__ModifyPeers*   message,
 ProtobufCBuffer*     buffer);
Haven__Admin__ModifyPeers*
haven__admin__modify_peers__unpack
(ProtobufCAllocator*  allocator,
 size_t               len,
 const uint8_t*       data);
void   haven__admin__modify_peers__free_unpacked
(Haven__Admin__ModifyPeers* message,
 ProtobufCAllocator* allocator);
/* Haven__Admin__Result methods */
void   haven__admin__result__init
(Haven__Admin__Result*         message);
size_t haven__admin__result__get_packed_size
(const Haven__Admin__Result*   message);
size_t haven__admin__result__pack
(const Haven__Admin__Result*   message,
 uint8_t*             out);
size_t haven__admin__result__pack_to_buffer
(const Haven__Admin__Result*   message,
 ProtobufCBuffer*     buffer);
Haven__Admin__Result*
haven__admin__result__unpack
(ProtobufCAllocator*  allocator,
 size_t               len,
 const uint8_t*       data);
void   haven__admin__result__free_unpacked
(Haven__Admin__Result* message,
 ProtobufCAllocator* allocator);
/* --- per-message closures --- */

typedef void (*Haven__Admin__ModifyPeers__Peer_Closure)
(const Haven__Admin__ModifyPeers__Peer* message,
 void* closure_data);
typedef void (*Haven__Admin__ModifyPeers_Closure)
(const Haven__Admin__ModifyPeers* message,
 void* closure_data);
typedef void (*Haven__Admin__Result_Closure)
(const Haven__Admin__Result* message,
 void* closure_data);

/* --- services --- */

typedef struct _Haven__Admin__Admin_Service Haven__Admin__Admin_Service;
struct _Haven__Admin__Admin_Service {
    ProtobufCService base;
    void (*peers)(Haven__Admin__Admin_Service* service,
                  const Haven__Admin__ModifyPeers* input,
                  Haven__Admin__Result_Closure closure,
                  void* closure_data);
};
typedef void (*Haven__Admin__Admin_ServiceDestroy)(Haven__Admin__Admin_Service*);
void haven__admin__admin__init(Haven__Admin__Admin_Service* service,
                               Haven__Admin__Admin_ServiceDestroy destroy);
#define HVN__ADMIN__ADMIN__BASE_INIT \
    { &haven__admin__admin__descriptor, protobuf_c_service_invoke_internal, NULL }
#define HVN__ADMIN__ADMIN__INIT(function_prefix__) \
    { HVN__ADMIN__ADMIN__BASE_INIT,\
        function_prefix__ ## peers  }
void haven__admin__admin__peers(ProtobufCService* service,
                                const Haven__Admin__ModifyPeers* input,
                                Haven__Admin__Result_Closure closure,
                                void* closure_data);

/* --- descriptors --- */

extern const ProtobufCMessageDescriptor haven__admin__modify_peers__descriptor;
extern const ProtobufCMessageDescriptor haven__admin__modify_peers__peer__descriptor;
extern const ProtobufCEnumDescriptor    haven__admin__modify_peers__modify_type__descriptor;
extern const ProtobufCMessageDescriptor haven__admin__result__descriptor;
extern const ProtobufCServiceDescriptor haven__admin__admin__descriptor;

PROTOBUF_C_END_DECLS


#endif  /* PROTOBUF_admin_2eproto__INCLUDED */
