#define _GNU_SOURCE //for fcloseall
#include <stdio.h>//for fileno
#include <stdlib.h>
#include <unistd.h>//for read
#include <sys/socket.h>//sockets
#include <sys/types.h>//sockets
#include <netinet/in.h>//for socket structure
#include <netinet/tcp.h>//for nagle
#include <sys/sendfile.h>//for sendfile
#include <string.h>
#include <math.h>

typedef struct{
    unsigned long long primenumber;
    unsigned long long primitiveroot;
}globalPublicElements;

//SERVER CODE
void catcherror(char *message)
{
    printf("%s\n",message);
    exit(1);
}

int encodedValue(char c)
{
    switch(c)
    {
        case ' ' :  return 0;
        case 'A' :  return 1;
        case 'B' :  return 2;
        case 'C' :  return 3;
        case 'D' :  return 4;
        case 'E' :  return 5;
        case 'F' :  return 6;
        case 'G' :  return 7;
        case 'H' :  return 8;
        case 'I' :  return 9;
        case 'J' :  return 10;
        case 'K' :  return 11;
        case 'L' :  return 12;
        case 'M' :  return 13;
        case 'N' :  return 14;
        case 'O' :  return 15;
        case 'P' :  return 16;
        case 'Q' :  return 17;
        case 'R' :  return 18;
        case 'S' :  return 19;
        case 'T' :  return 20;
        case 'U' :  return 21;
        case 'V' :  return 22;
        case 'W' :  return 23;
        case 'X' :  return 24;
        case 'Y' :  return 25;
        case 'Z' :  return 26;
        case ',' :  return 27;
        case '.' :  return 28;
        case '?' :  return 29;
        case '0' :  return 30;
        case '1' :  return 31;
        case '2' :  return 32;
        case '3' :  return 33;
        case '4' :  return 34;
        case '5' :  return 35;
        case '6' :  return 36;
        case '7' :  return 37;
        case '8' :  return 38;
        case '9' :  return 39;
        case 'a' :  return 40;
        case 'b' :  return 41;
        case 'c' :  return 42;
        case 'd' :  return 43;
        case 'e' :  return 44;
        case 'f' :  return 45;
        case 'g' :  return 46;
        case 'h' :  return 47;
        case 'i' :  return 48;
        case 'j' :  return 49;
        case 'k' :  return 50;
        case 'l' :  return 51;
        case 'm' :  return 52;
        case 'n' :  return 53;
        case 'o' :  return 54;
        case 'p' :  return 55;
        case 'q' :  return 56;
        case 'r' :  return 57;
        case 's' :  return 58;
        case 't' :  return 59;
        case 'u' :  return 60;
        case 'v' :  return 61;
        case 'w' :  return 62;
        case 'x' :  return 63;
        case 'y' :  return 64;
        case 'z' :  return 65;
        case '!' :  return 66;
    }
}

char encodedChar(int c)
{
    switch(c)
    {
        case 0 : return ' ';
        case 1 : return 'A';
        case 2 : return 'B';
        case 3 : return 'C';
        case 4 : return 'D';
        case 5 : return 'E';
        case 6 : return 'F';
        case 7 : return 'G';
        case 8 : return 'H';
        case 9 : return 'I';
        case 10 : return 'J';
        case 11 : return 'K';
        case 12 : return 'L';
        case 13 : return 'M';
        case 14 : return 'N';
        case 15 : return 'O';
        case 16 : return 'P';
        case 17 : return 'Q';
        case 18 : return 'R';
        case 19 : return 'S';
        case 20 : return 'T';
        case 21 : return 'U';
        case 22 : return 'V';
        case 23 : return 'W';
        case 24 : return 'X';
        case 25 : return 'Y';
        case 26 : return 'Z';
        case 27 : return ',';
        case 28 : return '.';
        case 29 : return '?';
        case 30 : return '0';
        case 31 : return '1';
        case 32 : return '2';
        case 33 : return '3';
        case 34 : return '4';
        case 35 : return '5';
        case 36 : return '6';
        case 37 : return '7';
        case 38 : return '8';
        case 39 : return '9';
        case 40 : return 'a';
        case 41 : return 'b';
        case 42 : return 'c';
        case 43 : return 'd';
        case 44 : return 'e';
        case 45 : return 'f';
        case 46 : return 'g';
        case 47 : return 'h';
        case 48 : return 'i';
        case 49 : return 'j';
        case 50 : return 'k';
        case 51 : return 'l';
        case 52 : return 'm';
        case 53 : return 'n';
        case 54 : return 'o';
        case 55 : return 'p';
        case 56 : return 'q';
        case 57 : return 'r';
        case 58 : return 's';
        case 59 : return 't';
        case 60 : return 'u';
        case 61 : return 'v';
        case 62 : return 'w';
        case 63 : return 'x';
        case 64 : return 'y';
        case 65 : return 'z';
        case 66 : return '!';
    }
}


unsigned long long fastExponentiationAlgo(unsigned long long base, unsigned long long exp, unsigned long long prime)
{
    unsigned long long answer=base;
    int sizeOfExp=((int)(floor(log(exp)/log(2.0)))+1);
    for(int i = sizeOfExp-2;i>=0;i--)
    {
        answer = (answer*answer)%prime;
        if((exp>>i)%2==1)
            answer = (answer*base)%prime;
    }
    return answer;

}

void  setSeedFromCpuUtil()
{
    FILE *statptr;
    unsigned long long statArray[4];
    statptr=fopen("/proc/stat","r");
    char noUse[10];
    fscanf(statptr,"%s %llu %llu %llu %llu",noUse,&statArray[0],&statArray[1],&statArray[2],&statArray[3]);
    unsigned long long finalrandomnumber = (statArray[0]+statArray[1]+statArray[2]+statArray[3])/3;
    srand(finalrandomnumber%13121);
    fclose(statptr);
    //printf("%d\n",(int)(finalrandomnumber%13121));
    //Now rand can be used
}

int main(int argc, char **argv)
{
    struct sockaddr_in serveraddress,clientaddress;/*structure is to store addresses
                                        struct sockaddr_in
                                        { short   sin_family;
                                        u_short sin_port;
                                        struct  in_addr sin_addr;
                                        char    sin_zero[8];   };*/
    int acceptfd,socketfd;


    if(argc<2)
        catcherror("Port Not Provided");

    socketfd=socket(AF_INET,SOCK_STREAM,0);//AF_INET is address domain(for any host on internet) AF_UNIX(for 2 processes sharing common file system) search man for other 2 parameters
    int socketclosure = 1;
    setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &socketclosure, sizeof(socketclosure)); //for socket closure like nagle


    if(socketfd<0)
        catcherror("Socket cannot be created");

    memset(&serveraddress,0,sizeof(serveraddress));
    memset(&clientaddress,0,sizeof(clientaddress));

    int port=atoi(argv[1]);//convert arg[1]to integer;

    //setup address structure
    serveraddress.sin_family=AF_INET;
    serveraddress.sin_port=htons(port);//converts unsigned integer Host byte order TO Network byte order
    serveraddress.sin_addr.s_addr=INADDR_ANY;//sockaddr_contains a structure in_addr containing only one field s_addr. s_addr is address of host which is obtained by INADDR_ANY

    int bindfd=bind(socketfd,(struct sockaddr *)&serveraddress,sizeof(serveraddress));//binds socket to it's address
    if(bindfd<0)
        catcherror("Error in Binding Socket");//fails if socket is already in use;

    //connection part is over now listening part
    listen(socketfd,3);//second argument is maximum length of queue of pending connection for socketfd if >3 error connection refused for client

    socklen_t clientlen=(socklen_t)sizeof(clientaddress);
    acceptfd=accept(socketfd,(struct sockaddr *)&clientaddress,&clientlen);//blocks the process until a client connects to server.Wakes up when connection from client is done
    if(acceptfd<0)
        catcherror("Error in Accept");
    else
    {
        int nagle=1;
        setsockopt(acceptfd,IPPROTO_TCP,TCP_NODELAY,(char *) &nagle,sizeof(int));


        globalPublicElements gpe;
        char buffmsg[100];
        recv(acceptfd,buffmsg,100,0);

        printf("Received Message : %s\n",buffmsg);
        sscanf(buffmsg,"%llu:%llu",&gpe.primenumber,&gpe.primitiveroot);
        printf("%llu %llu\n",gpe.primenumber,gpe.primitiveroot);
        unsigned long long privatekey = rand()%gpe.primenumber;
        unsigned long long publickey = fastExponentiationAlgo(gpe.primitiveroot,privatekey,gpe.primenumber);
        printf("My Private key %llu\n",privatekey);
        printf("My Public key %llu\n",publickey);
        char publickeystring[100];
        sprintf(publickeystring,"%llu:%llu:%llu",publickey,gpe.primenumber,gpe.primitiveroot);
        send(acceptfd,publickeystring,100,0);
        char publickeyofclientstring[100];
        unsigned long long publickeyofclient;
        recv(acceptfd,publickeyofclientstring,100,0);
        sscanf(publickeyofclientstring,"%llu",&publickeyofclient);
        printf("Public key of Client:%llu\n",publickeyofclient);
        unsigned long long secretsharedkey = fastExponentiationAlgo(publickeyofclient,privatekey,gpe.primenumber);
        printf("Secret Shared Key is %llu\n",secretsharedkey);
        secretsharedkey = secretsharedkey%67;
        if(secretsharedkey==0)
                secretsharedkey=1;

        FILE *writeptr;
        writeptr=fopen(argv[2],"w");
        do
        {
            char receivedmsg[BUFSIZ];
            recv(acceptfd,receivedmsg,BUFSIZ,0);
            int encryptedchar;
            sscanf(receivedmsg,"%d",&encryptedchar);
            printf("Received:%d\n",encryptedchar);
            if(encryptedchar==-1)
                break;
            if(encryptedchar>=0 && encryptedchar<=67)
            {
                int decryptedchar=encryptedchar-secretsharedkey;
                if(decryptedchar<0)
                    decryptedchar=decryptedchar+67;
                printf("Inserting:%d\n",decryptedchar);
                fputc(encodedChar(decryptedchar),writeptr);
            }

        }while(1);

        printf("Done!!");

    }
    fcloseall();
}
