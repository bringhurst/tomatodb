# Haven External Protocol v1

This document describes the protocol used to control and manage data by
applications external to the haven daemon. It is assumed that all messages
are using the appropriate constants and processed with MessagePack.

All messages may contain credentials relevant to the current security scheme.

## Overview
There are currently 4 primary message types with the following purposes:

### CONNECT
This is a message sent by the client at the beginning of every session. It may
contain version and authentation information.

### CONTROL
This is a message to instruct the havend to move between operational states.
For example, it may be used to tell a havend to become a location service, a
processor, or to leave or join a specific quorum.

### DATA
This is used for CRUD style queries of operational and configuration data.

### DISCOVER
This is used to discover the location of a quorum or location service.

### HEARTBEAT
This is used to gather the current state of processors and to handle the
propogation of watch events.

## Details for CONNECT Message

From client to havend:
````
{
    action : connect,
    version : string,
}
````

From havend to client:
````
{
    result : boolean,
    error  : detail
}
````

## CONTROL Message

From client to havend:
````
{
    action : become,
    type   : server_type,

}
````

From havend to client:
````
{
    result : boolean,
    error  : detail
}
````

## HEARTBEAT Message

From client to havend:
````
{
    action : heartbeat,
    type   : ping,
}
````

From havend to client:
````
{
    action : heartbeat,
    type   : pong,
    result : watchlist
}
````

## DATA Message

From client to havend:
````
{
    action : query,
    type   : crud,
}
````

From havend to client:
````
{
    result : boolean,
    error  : detail
}
````

## DISCOVER Message

From client to havend:
````
{
    action : discover,
    type   : uuid or location or quorum,
    path   : path string,
    uuid   : uuid_value
}
````

From havend to client:
````
{
    result   : boolean,
    location : address,
    error    : detail
}
