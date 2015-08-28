//TODO:

char numberToDial[25];

int rn52_init(void)
{

    return 0;
}

int rn52_getStatus(void)
{
    puts("Q\r");

    return rn52_getErr();
}

int rn52_getErr(void)
{
    return 0;
}

int rn52_dial(char* str)
{

    return 0;
}

int rn52_accept(void)
{
    return 0;
}

int rn52_reject(void)
{
    return 0;
}

int rn52_pairStart(void)
{
    return 0;
}

int rn52_pairStop(void)
{
    return 0;
}