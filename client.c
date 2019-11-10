#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h> 

//nanananannananannndasndbuajn
char buf[512];

int main(int argc, char **argv)
{
    struct sockaddr_in adr;
    int gniazdo, r;
    unsigned int port;
    char abcd[512];    

    printf("Podaj adres IP odbiorcy: ");
    scanf("%s", abcd);
    printf("Podaj numer portu odbiorcy: ");
    scanf("%u", &port);
    gniazdo = socket(AF_INET, SOCK_DGRAM, 0);
    adr.sin_family = AF_INET;
    adr.sin_port = htons(port);
    adr.sin_addr.s_addr = inet_addr(abcd);
    printf("Podaj wiadomosc: ");
    fflush(stdout); fgetc(stdin);
    fgets(buf, 512, stdin);
           
    r = sendto(gniazdo,
               buf,
               512,
               0,
               (struct sockaddr*) &adr,
               sizeof(adr));
    if (r != 512) printf("sendto() nie powiodl sie\n");
    else printf("Wiadomosc wyslana.\n");
    close(gniazdo);
    return 0;
}
