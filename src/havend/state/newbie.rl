%%{
    machine Newbie;
    import "newbie_events.h";

    action newbie_setup {
        HAVEN_newbie_setup_cb();
    }

    action newbie_listen {
        HAVEN_newbie_listen_cb();
    }

    action newbie_handle_cmd {
        HAVEN_newbie_handle_cmd_cb();
    }

    action newbie_become_client {
        fgoto Client::RegisteringWithLocationService;
    }

    action newbie_become_location {
        fgoto Consensus::StartingLocationService;
    }

    action newbie_join_existing {
        fgoto Consensus::JoiningExistingQuorum;
    }

    action newbie_exit {
        HAVEN_newbie_exit_cb();
    }

    action newbie_error {
        HAVEN_newbie_error_cb();
    }

Newbie := (
        start: (
            CONFIGURE @newbie_setup -> Listening
        ),

        Listening: (
            RECEIVED_CMD @newbie_handle_cmd -> Handling
        ),

        Handling: (
            JOIN_EXISTING_CMD @newbie_join_existing |
            BECOME_CLIENT_CMD @newbie_become_client |
            BECOME_LOCATION_CMD @newbie_become_location
        )

    ) <err(newbie_error);

    include Client "client.rl";
    include Consensus "consensus.rl";
}%%

%% write data;
