# Haven External Protocol v1

This document describes the protocol used to control and manage data by
applications external to the haven daemon. It is assumed that all messages
are using the appropriate constants and processed with MessagePack.

All messages may contain credentials relevant to the current security scheme.

## CONNECT Message

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

## BECOME Message

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
