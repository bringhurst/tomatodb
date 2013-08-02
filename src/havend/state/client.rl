%%{
    machine Client;
    import "client_states.h";

    Client = (
        start: ( IDLE -> start )
    );

    main := ( Client %{ printf("\n"); } )*;
}%%
