
#include "client_machine.h"

%%{
    machine Client;
    import "client_events.h";

    action client_error {
        HAVEN_client_error_cb();
    }

Client := (
        RegisteringWithLocationService: (
            REGISTERED @client_error
        )

    ) <err(client_error);
}%%
