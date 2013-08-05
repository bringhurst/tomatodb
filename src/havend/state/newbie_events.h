#ifndef __HAVEN_HAVEND_NEWBIE_EVENTS_H
#define __HAVEN_HAVEND_NEWBIE_EVENTS_H

typedef enum HAVEN_newbie_states {
    CONFIGURE = 0,
    RECEIVED_CMD = 1,
    JOIN_EXISTING_CMD = 2,
    BECOME_CLIENT_CMD = 3,
    BECOME_LOCATION_CMD = 4
} HAVEN_newbie_states;

#endif /* __HAVEN_HAVEND_NEWBIE_STATES_H */
