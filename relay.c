#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>   
#include <pthread.h> 

#include "multicast.h"

mcast_t *receiver_a; //  receiver multicast for LAN-A
mcast_t *receiver_b; //  receiver multicast for LAN-A
mcast_t *sender_a; //  sender multicast for LAN-A
mcast_t *sender_b; //  sender multicast for LAN-B


pthread_t listening_a; // LAN A listening thread id
pthread_t listening_b; // LAN B listening thread id
pthread_mutex_t mutex;

// thread routine functions
void *listenA(void *arg){
    char buffer[500];
    
    while (1) {
        // listening from LAN-A
        while (multicast_check_receive(receiver_a) == 0) {
            printf("Relay listening for messages from LAN-A...\n");
        }

        multicast_receive(receiver_a, buffer, 500);

        // printf("Received: %s\n", buffer);

        // relay message to LAN-b
        multicast_send(sender_b, buffer, strlen(buffer));

        // clear buffer
        memset(buffer,0,sizeof(buffer));
    }
}

void *listenB(void *arg){
    char buffer[500];    
    
    while (1) {
    
        // listening from LAN-B
        while (multicast_check_receive(receiver_b) == 0) {
            printf("Relay listening for messages from LAN-B...\n");
        }

        multicast_receive(receiver_b, buffer, 500);

        // printf("Received: %s\n", buffer);

        // relay message to LAN-A
        multicast_send(sender_a, buffer, strlen(buffer));

        // clear buffer
        memset(buffer,0,sizeof(buffer));
    }
}

void relay_init(char *mcast_addr_a, char *mcast_addr_b, int sport_a, int sport_b, int rport_a, int rport_b) {
    //Relay *new_relay = (Relay *)malloc(sizeof(Relay));

    sender_a = multicast_init(mcast_addr_a, rport_a, sport_a);
    sender_b = multicast_init(mcast_addr_b, rport_b, sport_b);
    
    receiver_a = multicast_init(mcast_addr_a, sport_a, rport_a);
    receiver_b = multicast_init(mcast_addr_b, sport_b, rport_b);

    // Setup reception for LAN-A and LAN-B
    multicast_setup_recv(receiver_a);
    multicast_setup_recv(receiver_b);
}

int relay_run() {
    // creating LAN A message receiving thread
    int rc = pthread_create(&listening_a, NULL, listenA, NULL);
    if (rc){
         printf("Error:unable to create listening thread, %d", rc);
         return -1;
    }

    // creating LAN B message receiving thread
    rc = pthread_create(&listening_b, NULL, listenB, NULL);
    if (rc){
         printf("Error:unable to create listening thread, %d", rc);
         return -1;
    }

    pthread_join(listening_a, NULL);
    pthread_join(listening_b, NULL);

    return 1;
}

void relay_destroy() {
    multicast_destroy(receiver_a);
    multicast_destroy(receiver_b);
    multicast_destroy(sender_a);
    multicast_destroy(sender_b);
}

int main() {
    char *mcast_addr_a = "224.1.10.1"; // multicast address for LAN-A
    char *mcast_addr_b = "224.1.20.2"; // multicast address for LAN-B
    
    int sport_a = 5555; // source port for LAN-A
    int rport_a = 7777; // receive port for LAN-A
   
    int sport_b = 6666; // source port for LAN-B
    int rport_b = 8888; // receive port for LAN-B

    relay_init(mcast_addr_a, mcast_addr_b, sport_a, sport_b, rport_a, rport_b);
    
    relay_run();
    
    //relay_destroy(relay);

    return 0;
}
