#include <stdio.h>

//#pragma weak bar = foo

int bar(int lol)
{  
    printf("CICI %s", __FUNCTION__);
}

int plop(int plot)
{

}
