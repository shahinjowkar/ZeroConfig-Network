#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zcs.h"

void hello(char *s, char *r) {
    printf("Ad received: %s, with value: %s\n", s, r);
    zcs_log();
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        return -1;
    }
    int lan_type = atoi(argv[1]);
    int rv;
    rv = zcs_init(ZCS_APP_TYPE,lan_type);
    sleep(10);
    char *names[10];
    rv = zcs_query("type", "speaker", names, 10);
    if (rv > 0) {
        zcs_attribute_t attrs[5];
	int anum = 5;
        rv = zcs_get_attribs(names[0], attrs, &anum);
        if ((strcmp(attrs[0].attr_name, "location") == 0) &&
            (strcmp(attrs[0].value, "kitchen") == 0)) {
                rv = zcs_listen_ad(names[0], hello);
        }
        // rv = zcs_listen_ad(names[0], hello);
    }
}


