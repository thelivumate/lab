#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <arpa/inet.h>

#define MAX_SIZE 234
#define PORT 8080
char data[20],div1[20],temp[4],total[40];
int datalen,divlen,i,j,len;
void check(){
    for(j=0;j<divlen;j++){
        data[j]=total[j];
    }
    while(j<=len){
        if(data[0]=='1'){
            for(i=1;i<divlen;i++){
                data[i]=((data[i]==div1[i])?'0':'1');
            }
        }
        for(i=0;i<divlen-1;i++){
            data[i]=data[i+1];
        }
        data[i]=total[j++];
    }
}
int main(int argc,char* argv[]){
    struct sockaddr_in server_address;
    server_address.sin_family=AF_INET;
    server_address.sin_addr.s_addr=inet_addr(argv[1]);
    server_address.sin_port=htons(PORT);

    int client_socket=socket(AF_INET,SOCK_STREAM,0);

    connect(client_socket,(struct sockaddr *)&server_address,sizeof(server_address));


 	printf("Enter the total bit of data:");    
 	scanf("%d",&datalen);
 	printf("\nEnter the total bit of divisor");
 	scanf("%d",&divlen);
 	len=datalen+divlen-1;	
 	printf("\nEnter the data:");
 	scanf("%s",data);		      
    printf("\nEnter the divisor");
    scanf("%s",div1); 

    for(i=0;i<datalen;i++){
        total[i]=data[i];
        temp[i]=data[i];
    }

    for(i=datalen;i<len;i++){
        total[i]='0';
    }
    check();
    for(i=0;i<divlen;i++){
        temp[i+datalen]=data[i];
    }

    printf("transmitted code: %s",temp);


    send(client_socket,temp,strlen(temp),0);







    
    
}