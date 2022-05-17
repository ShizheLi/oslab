# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/time.h>
# include <sys/select.h>

void myfun (char *a) {
    a[0] = 'a';
    a[1] = 'b';
}

int main () {
    char a[2];
    myfun(a);
    printf("%c\n", a[0]);
}