%%{
    machine Idle;
    import "idle_states.h";

    action start {
        printf("start");
    }

    Idle = (
        start: ( IDLE -> start )
    );

    main := ( Idle %{ printf("\n"); } )*;
}%%
