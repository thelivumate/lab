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
int datalen,divlen,i,j,len,flag=1;
void check(){
    for(j=0;j<divlen;j++){
        data[i]=total[i];
    }
    while(j<=len){
        if(data[0]=='1'){
            for(i=1;i<divlen;i++){
                data[i]=((data[i]==div1[i])?'0':'1');
            }
        }
        for(int i=0;i<divlen-1;i++){
            data[i]=data[i+1];
        }
        data[i]=total[j++];
    }
}
int main(){
    struct sockaddr_in server_address;
    server_address.sin_family=AF_INET;
    server_address.sin_addr.s_addr=INADDR_ANY;
    server_address.sin_port=htons(PORT);

    int server_socket=socket(AF_INET,SOCK_STREAM,0);
    bind(server_socket,(struct sockaddr *)&server_address,sizeof(server_address));
    listen(server_socket,568);
    socklen_t siz=sizeof(server_address);
    int client_socket=accept(server_socket,(struct sockaddr *)&server_address,&siz);

    printf("Enter the total bit of data:");    
 	scanf("%d",&datalen);
 	printf("\nEnter the total bit of divisor");
 	scanf("%d",&divlen);
 	len=datalen+divlen-1;	
 	printf("\nEnter the data:");
 	scanf("%s",data);		      
    printf("\nEnter the divisor");
    scanf("%s",div1); 

    char temp[MAX_SIZE];
    recv(client_socket,temp,sizeof(temp),0);
    printf("%s",temp);
    for(i=0;i<len;i++){
        total[i]=temp[i];
    }
    check();
    for(i=0;i<divlen-1;i++){
		if(data[i]=='1')
		{
			flag=0;
			break;
		}
	}
	if(flag==1)
	printf("\nsuccessful!!");
	else
	printf("\nreceived code word contains errors...\n");
    

    
}