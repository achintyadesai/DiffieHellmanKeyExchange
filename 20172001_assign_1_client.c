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
#include <limits.h>
//CLIENT CODE

void catcherror(char *message)
{
    printf("%s\n",message);
    exit(1);
}

char* integerToBinary(int a)
{
    char* binaryNumber;
    int sizeOfNumber = ((int)(floor(log(a)/log(2.0)))+1);
    binaryNumber = (char*)malloc(sizeOfNumber);
    int index = sizeOfNumber;
    while(a!=0)
    {
        binaryNumber[index-1] = (a%2)+'0';
        a = a/2;
        index--;
    }
    return binaryNumber;
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


int millerRabinPrimality(unsigned long long number,unsigned long long q,unsigned long long k)
{
    unsigned long long base = rand();
    base = base % number-1;
    if(base==0)
        base=2;
    if(fastExponentiationAlgo(base,q,number)==1)
        return -1;//inconclusive

    for(unsigned long long j = 0;j<k;j++)
        if(fastExponentiationAlgo(base,(1<<j)*q,number)==number-1)
    {
            return -1;
    }
    return 0;
}


int main(int argc, char **argv)
{
    setSeedFromCpuUtil();
    printf("%llu\n",fastExponentiationAlgo(5,7,11));
    int flag;
    do
    {
        flag=0;
        unsigned long long number = rand();
        if(number%2==0)
        {
            flag=1;
            continue;
        }
        unsigned long long k;
        unsigned long long q;
        printf("Number is %llu",number);
        for(k=1;k<=(int)(floor(log(number-1)/log(2.0)));k++)
        {
            if((number-1)%(1<<k)==0)
            {
                q=(number-1)/(1<<k);
                if(q%2==1)
                    break;
            }
        }

        printf("Atlast found a q and k %llu %llu",q,k);

        for(int i=0;i<10;i++)
            if(millerRabinPrimality(number,q,k)==0)
                flag=1;
        if(flag==0)
                printf("Prime with high probability");
        else    printf("Not Prime");
    }while(flag==1);
    printf("\nPrime Found");
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
