
#include "actions.h"

%%{
    machine State;
    import "consensus.h";

    alphtype int;
    access machine->;

    action create_location_quorum {
        HAVEN_create_location_quorum(server);
    }

    action consensus_error {
        HAVEN_consensus_error_cb();
    }

Consensus := (
        start: (
            CREATE_REGULAR -> CreateNewQuorum  |
            CREATE_LOCATION @create_location_quorum -> StartingLocationService  |
            JOIN -> JoiningExistingQuorum
        ),

        StartingLocationService: (
            SETUP_LOCATION_SERVICE -> Coordinating
        ),

        CreateNewQuorum: (
            CREATE_NEW_QUORUM -> Coordinating
        ),

        JoiningExistingQuorum: (
            JOIN_EXISTING_QUORUM -> Following
        ),

        Following: (
            HEARTBEAT_TIMEOUT -> Campaigning |
            HANDLE_LOG_APPEND -> Following
        ),

        Campaigning: (
            ELECTION_TIMEOUT -> Campaigning |
            ELECTION_SUCCESS -> Coordinating |
            DISCOVERED_HIGHER_TERM -> Following
        ),

        Coordinating: (
            DISCOVERED_HIGHER_TERM -> Following
        )

    ) <err(consensus_error);
}%%

%% write data;

int HAVEN_init_consensus_state(HAVEN_state_t *state, HAVEN_state_actions_t *actions)
{
    int cs;
    %% write init;
}

int HAVEN_exec_consensus_state(HAVEN_state_t *state, HAVEN_server_t* server)
{
    int *p = NULL;
    int *pe = NULL;
    int *eof = NULL;
    int cs = machine->cs;

    %% write exec;
}

/* EOF */
