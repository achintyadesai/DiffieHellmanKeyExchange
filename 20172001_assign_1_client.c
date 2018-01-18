#define _GNU_SOURCE //for fcloseall
#include <unistd.h>//for read
#include <sys/socket.h>//sockets
#include <sys/types.h>//sockets
#include <netinet/in.h>//for socket structure
#include <netdb.h>//for hostnet
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


//CLIENT CODE

void catcherror(char *message)
{
    printf("%s\n",message);
    exit(1);
}

char* integerToBinary(int a)
{
    char* binaryNumber;
    int sizeOfNumber=((int)(floor(log(a)/log(2.0)))+1);
    binaryNumber=(char*)malloc(sizeOfNumber);
    int index=sizeOfNumber;
    while(a!=0)
    {
        binaryNumber[index-1]=(a%2)+'0';
        a=a/2;
        index--;
    }
    return binaryNumber;
}

int fastExponentiationAlgo(int base, int exp, int prime)
{
    int answer;
    answer=base;
    char* expinbinary;
    int sizeOfExp=((int)(floor(log(exp)/log(2.0)))+1);
    for(int i=sizeOfExp-2;i>=0;i--)
    {
        answer=(answer*answer)%prime;
        if((exp>>i)%2==1)
            answer=(answer*base)%prime;
    }
    return answer;

}


int main(int argc, char **argv)
{
    printf("%d\n",fastExponentiationAlgo(45,99,29));

    struct sockaddr_in serveraddress;/*structure is to store addresses
                                        struct sockaddr_in
                                        { short   sin_family;
                                        u_short sin_port;
                                        struct  in_addr sin_addr;
                                        char    sin_zero[8];   };*/
    int socketfd;

    struct hostent *serverhost;/*
                                struct  hostent
                                char *h_name;                    official name of host
                                char **h_aliases;                alias list
                                int h_addrtype;                  host address type
                                int h_length;                    length of address
                                char **h_addr_list;              list of addresses from name server in c99 all pointers were char * hence char **
                                #define h_addr  h_addr_list[0]   address, for backward compatiblity
                                */

    if(argc<3)
        catcherror("Wrong Usage");

    socketfd=socket(AF_INET,SOCK_STREAM,0);//AF_INET is address domain(for any host on internet) AF_UNIX(for 2 processes sharing common file system) search man for other 2 parameters
    if(socketfd<0)
        catcherror("Socket cannot be created");

    memset(&serveraddress,0,sizeof(serveraddress));

    serverhost=gethostbyname(argv[1]);//name of host is converted to structure hostent containing information about host. Performs lookup in the configuration files /etc/host.conf and /etc/nsswitch.conf.
    if(serverhost==NULL)
        catcherror("No Such Host");

    int port=atoi(argv[2]);//convert arg[1]to integer;

    //setup address structure
    serveraddress.sin_family=AF_INET;
    serveraddress.sin_port=htons(port);//converts unsigned integer Host byte order TO Network byte order
    bcopy((char *)serverhost->h_addr,(char *)&serveraddress.sin_addr.s_addr,serverhost->h_length);//copies serverhost address to s_addr

    //connect
    socklen_t serverlen=(socklen_t)sizeof(serveraddress);
    int connectifd=connect(socketfd,(struct sockaddr *)&serveraddress,serverlen);//connects to server
    if(connectifd<0)
        catcherror("Error in Connecting");
    else
    {
        send(socketfd,"Hello Server",20,0);

    }
    fcloseall();
}
