#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFF_SIZE 1024

int main(void)
{
    unsigned int port;
    char bufor[BUFF_SIZE];
    int rozmiar_pliku;
    char nazwa_pliku[BUFF_SIZE];

    //char sciezka_do_pliku[BUFF_SIZE];

    int gniazdo, gniazdo2,odebrane;
    struct sockaddr_in adr, nadawca;
    socklen_t dl = sizeof(struct sockaddr_in);

    printf("Na ktorym porcie mam sluchac? : ");
    scanf("%u", &port);
    gniazdo = socket(PF_INET, SOCK_STREAM, 0);
    adr.sin_family = AF_INET;
    adr.sin_port = htons(port);
    adr.sin_addr.s_addr = INADDR_ANY;
    if (bind(gniazdo, (struct sockaddr *)&adr,
             sizeof(adr)) < 0)
    {
        printf("Bind nie powiodl sie.\n");
        return 1;
    }
    if (listen(gniazdo, 10) < 0)
    {
        printf("Listen nie powiodl sie.\n");
        return 1;
    }
    int liczba_bajtow;

    printf("Czekam na polaczenie ...\n");
    if ((gniazdo2 = accept(gniazdo,(struct sockaddr *)&nadawca,&dl)) > 0){
        memset(bufor, 0, BUFF_SIZE);
        
        liczba_bajtow=recv(gniazdo2, bufor, BUFF_SIZE, 0);
        printf("Wiadomosc: %s\n",bufor);
        printf("Bajty: %d\n",liczba_bajtow);

        int i = 0;
        char liczba_bajtow_str[BUFF_SIZE];
        for(i;i<liczba_bajtow;i++){
            if(bufor[i]!=' '){
                liczba_bajtow_str[i]=bufor[i];
            }
            else{
                break;
            }
        }
        printf("elo\n");
        rozmiar_pliku=atoi(liczba_bajtow_str);
        i=i+1;
        int j=0;
        for(i;i<liczba_bajtow;i++){
            nazwa_pliku[j]=bufor[i];
            j++;
    
        }
        printf("%d %s\n", rozmiar_pliku, nazwa_pliku);

        // while(odebrane<rozmiar_pliku){
        //     odebrane+=recv(gniazdo2,|bufor|,BUFF_SIZE,0);
        // }
        //rozmiar
        // memset(bufor, 0, BUFF_SIZE);
        // recv(gniazdo2, bufor, BUFF_SIZE, 0);
        // printf("Wiadomosc od %s: %s\n",
        //        inet_ntoa(nadawca.sin_addr),
        //        bufor);
        //send(gniazdo2,bufor,strlen(bufor),0);
        close(gniazdo2);
    }
    close(gniazdo);
    return 0;
}