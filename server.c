#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFF_SIZE 1024

int main(void)
{
    unsigned int port;

    char bufor[BUFF_SIZE],nazwa_pliku[BUFF_SIZE];
    char liczba_bajtow_str[BUFF_SIZE];
    unsigned int rozmiar_pliku, liczba_bajtow,i,j;
    int gniazdo, gniazdo2,odebrane,bajty_odebrane_lacznie,odebrane_bajty;
    FILE *wskaznik_na_plik;
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

    printf("Czekam na polaczenie ...\n");
    if ((gniazdo2 = accept(gniazdo,(struct sockaddr *)&nadawca,&dl)) > 0){
        memset(bufor, 0, BUFF_SIZE);

        liczba_bajtow=recv(gniazdo2, bufor, BUFF_SIZE, 0);
        printf("Wiadomosc: %s\n",bufor);
        printf("Liczba bajtow: %d\n",liczba_bajtow);

        i=0;
        for(i;i<liczba_bajtow;i++){
            if(bufor[i]!=' '){
                liczba_bajtow_str[i]=bufor[i];
            }
            else{
                break;
            }
        }

        rozmiar_pliku=atoi(liczba_bajtow_str);
        printf("Rozmiar pliku: %d\n",rozmiar_pliku);
        i=i+1;
        j=0;
        /*
        for(i;i<liczba_bajtow;i++){
            nazwa_pliku[j]=bufor[i];
            j++;
        }
        */
        printf("Podaj nazwe pliku, ktora ma zostac zapisana: ");
        scanf("%s", nazwa_pliku);

        //tworzenie pliku jesli nie istnieje, w trybie append and reading binarnie
        wskaznik_na_plik = fopen(nazwa_pliku,"ab+");
           if(wskaznik_na_plik == NULL)
        {
            printf("Blad tworzenia/otwartcia pliku!\n");
            exit(-1);
        }

        while(bajty_odebrane_lacznie<rozmiar_pliku){
            odebrane_bajty=recv(gniazdo2, bufor, BUFF_SIZE, 0);
            bajty_odebrane_lacznie+=odebrane_bajty;
            //printf("Odebrane bajty w jednej iteracji: %d\n",odebrane_bajty);
            fwrite( bufor, 1 , odebrane_bajty, wskaznik_na_plik );
            memset(bufor, 0, BUFF_SIZE);
        }

        printf("Liczba odebranych bajtow: %d\n",bajty_odebrane_lacznie);
        close(gniazdo2);
        fclose(wskaznik_na_plik);
    }
    close(gniazdo);
    return 0;
}
