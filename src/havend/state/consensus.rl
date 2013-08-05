%%{
    machine Consensus;
    import "consensus_events.h";

    action consensus_error {
        HAVEN_consensus_error_cb();
    }

Consensus := (
        start: (
            CREATE_REGULAR -> CreateNewQuorum  |
            CREATE_LOCATION -> StartingLocationService  |
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
