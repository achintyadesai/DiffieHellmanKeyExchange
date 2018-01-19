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

typedef struct{
    unsigned long long primenumber;
    unsigned long long primitiveroot;
}globalPublicElements;

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
        default : return -1;
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

int primeFactorization(unsigned long long no,unsigned long long primefactors[])
{
    int index=0;
    if(no%2==0)
    {
        primefactors[index]=2;
        index++;
    }
    while(no%2==0)
    {
        no=no/2;
    }

        printf("Inside\n");
    for(int i=3;i<=sqrt(no);i++)
    {
        if(no%i==0)
        {
            primefactors[index]=i;
            index++;
        }
        while(no%i==0)
        {
            no=no/i;
        }

    }
    if(no>2)
    {
        primefactors[index]=no;
        index++;
    }

    return index;
}

int main(int argc, char **argv)
{
    setSeedFromCpuUtil();
    printf("%llu\n",fastExponentiationAlgo(5,7,11));
    unsigned long long number,primenumber;
    int flag;
    do
    {
        flag=0;
        number = rand();
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
    printf("\nPrime Found %llu\n",number);

    primenumber=number;
    unsigned long long totientvalue = primenumber-1;
    unsigned long long primitiveroot;

    unsigned long long *primefactors;
    primefactors = malloc((unsigned int)totientvalue);
    int numberofdistinctprimefactors = primeFactorization(totientvalue,primefactors);
    for(int i=0;i<numberofdistinctprimefactors;i++)
    {
        primefactors[i]=totientvalue/primefactors[i];
    }

    for(primitiveroot=2;primitiveroot<totientvalue;primitiveroot++)
    {
        flag=0;

        for(unsigned long long j=0;j<numberofdistinctprimefactors;j++)
        {
            if(fastExponentiationAlgo(primitiveroot,primefactors[j],primenumber)==1)
            {
                flag=1;
                break;
            }

        }
        if(flag==1)
            continue;
        else
        {
            break;
        }
    }
    printf("Primitive root found:%llu",primitiveroot);

    globalPublicElements gpe;
    gpe.primenumber=primenumber;
    gpe.primitiveroot=primitiveroot;

    char gpestring[100];
    sprintf(gpestring,"%llu:%llu",gpe.primenumber,gpe.primitiveroot);
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
        send(socketfd,gpestring,100,0);
        char publickeyofserverstring[100];
        recv(socketfd,publickeyofserverstring,100,0);
        unsigned long long publickeyofserver,receivedprimenumber,receivedprimitiveroot;
        sscanf(publickeyofserverstring,"%llu:%llu:%llu",&publickeyofserver,&receivedprimenumber,&receivedprimitiveroot);
        if(receivedprimenumber==gpe.primenumber && receivedprimitiveroot==gpe.primitiveroot)
        {
            printf("Public key of Server %llu\n",publickeyofserver);
            unsigned long long privatekey = rand()%gpe.primenumber;
            unsigned long long publickey = fastExponentiationAlgo(gpe.primitiveroot,privatekey,gpe.primenumber);
            char publickeystring[100];
            sprintf(publickeystring,"%llu",publickey);
            send(socketfd,publickeystring,100,0);
            printf("My Private key %llu\n",privatekey);
            printf("My Public key %llu\n",publickey);
            unsigned long long secretsharedkey = fastExponentiationAlgo(publickeyofserver,privatekey,gpe.primenumber);
            printf("Secret Shared Key is %llu\n",secretsharedkey);
            secretsharedkey = secretsharedkey%67;
            if(secretsharedkey==0)
                secretsharedkey=1;

            FILE* readptr;
            readptr=fopen(argv[3],"rt");
            int unencrpytedchar;
            while((unencrpytedchar=fgetc(readptr))!=EOF)
            {
                printf("Unencrypted char:%d\n",unencrpytedchar);
                int encryptedcharacter=(encodedValue(unencrpytedchar)+secretsharedkey)%67;
                char sendcharstring[BUFSIZ];
                sprintf(sendcharstring,"%d",encryptedcharacter);
                send(socketfd,sendcharstring,BUFSIZ,0);
            }
            char sendcharstring[BUFSIZ];
            sprintf(sendcharstring,"%d",-1);
            send(socketfd,sendcharstring,BUFSIZ,0);


        }
        else
        {
            catcherror("Error in Communication");
        }
    }
    fcloseall();
    free(primefactors);
}
