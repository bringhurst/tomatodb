%%{
    machine Consensus;

    import "consensus_states.h";

    Following := (

    )

    Campaigning := (


    Role := (
        start: ( HANDLE_LOG_APPEND -> Following ),

        Following: (
            HEARTBEAT_TIMEOUT -> Campaigning |
            HANDLE_LOG_APPEND -> Following
        ),

        Campaigning: (
            /* TODO */
            ELECTION_TIMEOUT -> Campaigning |
            ELECTION_SUCCESS -> Coordinating |
            DISCOVERED_HIGHER_TERM -> Following
        ),

        Coordinating: (
            DISCOVERED_HIGHER_TERM -> Following
        ),

    ) <err(error);




main := (Role)*;

}%%
