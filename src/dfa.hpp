int is_state(int state) {
    if (0 <= state && state <= 4)
        return 1;

    std::cout << "Error: Invalid state (" << state << ")\n";
    return -1;
}

int is_action(char act) {
    if(act == 'a' || act == 'b' || act == 'c' || act == 'd')
        return 1;

    std::cout << "Error: Invalid action (" << act << ")\n";
    return -1;
}

int get_next_state(int state, char action) {

    if (!is_action(action) || !is_state(state))
        return -1;

    int s = state;
    char a = action;

    if (s == 0) {

        if (a == 'a')
            return 1;
        else if (a == 'b' || a == 'c' || a == 'd')
            return 4;

    } else if (s == 1) {

        if (a == 'a')
            return 1;
        else if (a == 'b')
            return 2;
        else if (a == 'c' || a == 'd')
            return 4;

    } else if (s == 2) {

        if (a == 'a')
            return 4;
        else if (a == 'b')
            return 2;
        else if (a == 'c' || a == 'd')
            return 3;

    } else if (s == 3) {

        if (a == 'a')
            return 1;
        else if (a == 'b')
            return 4;
        else if (a == 'c' || a == 'd')
            return 3;

    } else if (s == 4) {

        if (a == 'a' || a == 'b' || a == 'c' || a == 'd')
            return 4;
    }
}