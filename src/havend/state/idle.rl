%%{
    machine Idle;
    import "idle_states.h";

    Idle = (
        start: ( IDLE -> start )
    );

    main := ( Idle %{ printf("\n"); } )*;
}%%
