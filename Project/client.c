#include <stdio.h>  //printf
#include <string.h> //strlen
#include <sys/socket.h> //socket
#include <arpa/inet.h>  //inet_addr

#define PORT 60000

int main(){
    int sock,i;
    char name[80],response[80],command[80],ip[80],port[10];
    struct sockaddr_in server,new_server,client, server_client,new_client;
    char remote_name[80],nIp[80],nPort[80];
    char client_msg[2000],message[1000];
    char complete[2000];
    int size_new,new_client_socket;

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock < 0 )
    {
        printf("Could not create socket");
    }

    printf("Socket created; sock = %d\n", sock);

    memset(&server, 0, sizeof (server));   /* zero out structure */
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( PORT );

    i = connect(sock, (struct sockaddr *) &server, sizeof(server));
    if(i<0)
    {
        perror("connect");
        return 1;
    }

    printf("Connected\n");
    // while(1)
    // {
        printf("Enter username: ");
        scanf("%s",name);

        i = send(sock,name,strlen(name),0);
        if(i<0)
        {
            printf("Could not send your name\n");
            return 1;
        }

        i = recv(sock,response,80,0);
        response[strlen(response) -1] = '\0';
        if(i<0)
        {
            printf("Server did not acknowledge your name\n");
            return 1;
        }
        else
        {
            printf("%s\n",response );
        }

        printf("\t\t\t%s\t\t\t\n",name );
        
        
        printf("Command List:\n\\l - view available clients\n");
        printf("\\c - to communicate with a client\n");
        printf("\\w - to subscribe to working group\n");
        printf("\\f - to subscribe to fun group\n");
        printf("\\sw - to send a broadcast to working group\n");
        printf("\\sf - to send a broadcast to fun group\n");

        // printf("->)");
        // fflush(stdout);
        fd_set readfs;
        FD_ZERO(&readfs);
        while(1)
        {
            printf("->)");
            fflush(stdout);
            FD_SET(sock, &readfs);
            FD_SET(0, &readfs);

            memset(response,'\0',2000);
            memset(command,'\0',1000);

            select(sock+1, &readfs, NULL, NULL, NULL);

            if(FD_ISSET(sock,&readfs))
            {
                //printf("Server here first\n");
                i = recv(sock,response,2000,0);
                if(i<0){
                    printf("recv failed\n");
                    return 1;
                }
                //printf("Server reply: \n");
                if(strcmp(response,"n") ==0)
                {
                    printf("Server could not find user\n");
                }
                else if(!strncmp(response, "@",1))
                {
                    memset(nIp,'\0',30);
                    memset(nPort,'\0',10);

                    i=1;

                    while(response[i] != ',')
                    {
                        nIp[i-1] = response[i];
                        i++;
                    }
                    i++;
                    int j=0;
                    while(response[i] != '\0')
                    {
                        nPort[j] = response[i];
                        i++;
                        j++;
                    }

                    //printf("\nnew_ip : %s  ;  new_port = %s\nForking new process...\n", nIp,nPort);
                    // if((i=fork())==0)
                    // {
                        new_client_socket = socket(AF_INET , SOCK_STREAM , 0);
                        if(new_client_socket<0)
                        {
                            perror("socket");
                        }

                        memset(&server_client, 0, sizeof (server_client)); 
                        server_client.sin_addr.s_addr = (long int)atoi(nIp);
                        server_client.sin_family = AF_INET;
                        server_client.sin_port = htons( atoi(nPort) );

                        printf("Attempting to connect\n");

                        i = connect(new_client_socket,(struct sockaddr *) &server_client, sizeof(server_client));
                        if(i<0)
                        {
                            perror("Connect");
                            return 1;
                        }

                        //select(new_client_socket+1,convofds,NULL,NULL,NULL);

                        printf("Converstion begun\nEnter \\x at any time to end.\n");
                        fd_set readfs_new; 
                        FD_ZERO(&readfs_new);
                        //printf("Enter message : ");
                        while(1)
                        {
                            FD_SET(new_client_socket, &readfs_new);
                            FD_SET(0, &readfs_new);
            
                            memset(response,'\0',2000);
                            memset(message,'\0',1000);
                            
                            //printf("\nEnter message : ");
                            select(new_client_socket+1, &readfs_new, NULL, NULL, NULL);
                            
                            
                            //If new server gets first
                            if(FD_ISSET(new_client_socket, &readfs_new)) 
                            {
                                recv(new_client_socket, response, 2000, 0);
                                if(strcmp(response,"yes")==0)
                                {
                                    printf("Msg received? : %s \n", response);
                                }
                                else if(strcmp(response,"\\x")==0)
                                {
                                    printf("Msg received? : %s \n", response);
                                    printf("ENDING CONVERSATION\n");
                                    
                                    //send(new_client_socket,message,strlen(message), 0);
                                    //close(new_client_socket);
                                    FD_CLR(new_client_socket, &readfs_new);
                                    FD_CLR(0, &readfs_new);
                                    send(sock,"\\x",strlen("\\x"),0);
                                    break;
                                }
                                else
                                    printf("%s\n", response);
                                
                            }


                            
                            //If stdin gets first
                            if(FD_ISSET(0, &readfs_new)) 
                            {
    /*                              printf("Enter message : ");*/
                                //scanf("%s", message);
                                fgets(message,sizeof(message),stdin);
                                message[strlen(message)-1] = '\0';
                                if(strlen(message)>0)
                                {
                                    if(strcmp(message,"\\x")==0)
                                    {
                                        printf("ENDING CONVERSATION\n");

                                        send(new_client_socket,message,strlen(message), 0);
                                        //close(new_client_socket);
                                        FD_CLR(new_client_socket, &readfs_new);
                                        FD_CLR(0, &readfs_new);
                                        send(sock,"\\x",strlen("\\x"),0);
                                        break;
                                    }
                                    else
                                    {
                                        strcpy(complete,name);
                                        strcat(complete,":");
                                        strcat(complete,message);
                                        send(new_client_socket  , complete , strlen(complete) , 0);
                                        memset(complete,'\0',2000);
                                    }
                                }
                            }
                            // FD_CLR(new_client_socket, &readfs_new);
                            // FD_CLR(0, &readfs_new);
                        }
                    //}
                    
                }
                else
                {
                    printf("%s\n",response );
                    //send(sock,response,strlen(response),0);
                }
                    
            }

            if(FD_ISSET(0, &readfs)) {

                scanf("%s",command);

                i = send(sock,command,strlen(command),0);
                if(strcmp(command,"\\q")==0)
                {
                    close(sock);
                    FD_CLR(sock, &readfs);
                    FD_CLR(0, &readfs);
                    FD_ZERO(&readfs);   
                }
                else if(strcmp(command, "\\w") == 0)
                {
                    //this breaks everything
                    char * message = "Requesting to join the working group..";
                    send(sock, message, strlen(message), 0);
                    memset(command,'\0',2000);
                }
 
                else if(strcmp(command, "\\f") == 0)
                {
                    //this also breaks everything
                    char * message = "Requesting to join the fun group..";
                    send(sock, message, strlen(message), 0);
                    memset(command,'\0',2000);
                }

                else if (strcmp(command, "\\sw") == 0)
                {
                    printf("Enter message to send to working group..\n");
                    char message[1024];
                    fgets(message, sizeof(message), stdin); // duplicated to flush buffer
                    fgets(message, sizeof(message), stdin); 
                    message[strlen(message) - 1] = 0;
                    send(sock, message, strlen(message), 0);
                    memset(command,'\0',2000);
                }

                else if (strcmp(command, "\\sf") == 0)
                {
                    printf("Enter message to send to fun group..\n");
                    char message[1024];
                    fgets(message, sizeof(message), stdin); // duplicated to flush buffer
                    fgets(message, sizeof(message), stdin); 
                    message[strlen(message) - 1] = 0;
                    send(sock, message, strlen(message), 0);
                    memset(command,'\0',2000);
                }
                else if(strcmp(command,"\\c") == 0)
                {
                    printf("Please enter the name of the client you wish to chat with\n");
                    scanf("%s",remote_name);
                    send(sock,remote_name,strlen(remote_name),0);

                    recv(sock,response,80,0);
                    //response[strlen(response) -1] = '\0';
                    //printf("Reply:%s\n",response );
                    if(strcmp(response,"n") ==0)
                    {
                        printf("Invalid name\n");
                    }
                    else
                    {
                        int port,new_sock;
                        printf("%s\n",response );
                        memset(response,'\0',2000);
                        recv(sock,response,2000,0);

                        printf("new port:%s\n", response);
                        port = atoi(response);
                        if(port>0)
                        {
                        if((i = fork()) == 0)
                        {
                            new_sock = socket(AF_INET,SOCK_STREAM,0);
                            if(new_sock<0)
                            {
                                perror("socket");
                            }

                            printf("new socket created:%d\n",new_sock );

                            new_server.sin_family = AF_INET; 
                            new_server.sin_addr.s_addr = INADDR_ANY;
                            
                            new_server.sin_port = htons( port );

                            i = bind(new_sock, (struct sockaddr *) &new_server, sizeof(new_server));
                            if(i<0)
                            {
                                perror("bind");
                            }

                            listen(new_sock,1);

                            printf("Waiting for client to connect.\n");
                            
                            size_new = sizeof(struct sockaddr_in);

                            new_client_socket = accept(new_sock,(struct sockaddr *) &new_client, (socklen_t *)&size_new);

                            if(new_client_socket<0)
                            {
                                perror("accept");
                            }

                            printf("New connection accepted from %d\n", new_client_socket);
                            
                            memset(client_msg,'\0',2000);
                            int read_size;

                            printf("Conversation begun\nEnter \\x at any time to end.\n");
                            
                            fd_set readfs_new; 
                            FD_ZERO(&readfs_new);
                            //printf("Enter message : ");
                            while(1) 
                            {
    
                                FD_SET(new_client_socket , &readfs_new);
                                FD_SET(0, &readfs_new);

                                memset(client_msg,'\0',2000);
                                memset(message,'\0',1000);
    
                                //printf("\nEnter message : ");
                                select(FD_SETSIZE, &readfs_new, NULL, NULL, NULL);
                                
                                
                                //If new client gets first
                                if(FD_ISSET(new_client_socket , &readfs_new)) 
                                {
                                    recv(new_client_socket, client_msg , 2000 , 0);
                                    //printf("\n\nwaiting for client to type something...");
                                    printf("%s\n",client_msg);
                                    if(strcmp(client_msg,"\\x")==0)
                                    {
                                        printf("ENDING CONVERSATION\n");

                                        //send(new_client_socket,message,strlen(message), 0);
                                        //close(new_client_socket);
                                        FD_CLR(new_client_socket, &readfs_new);
                                        FD_CLR(0, &readfs_new);
                                        send(sock,"\\x",strlen("\\x"),0);
                                        break;
                                    }
                                    else
                                        send(new_client_socket , "yes" , strlen("yes") , 0);
                                }
                                    
                                //If stdin gets first
                                if(FD_ISSET(0, &readfs_new)) 
                                {
/*                                          printf("\nEnter message : ");*/
                                    //scanf("%s", message);
                                    fgets(message,sizeof(message),stdin);
                                    message[strlen(message)-1] = '\0';

                                    if(strlen(message)>0)
                                    {
                                        if(strcmp(message,"\\x")==0)
                                        {
                                            printf("ENDING CONVERSATION\n");

                                            send(new_client_socket,message,strlen(message), 0);
                                            //close(new_client_socket);
                                            FD_CLR(new_client_socket, &readfs_new);
                                            FD_CLR(0, &readfs_new);
                                            send(sock,"\\x",strlen("\\x"),0);
                                            break;
                                        }
                                        else
                                        {
                                            strcpy(complete,name);
                                            strcat(complete,":");
                                            strcat(complete,message);
                                            strcat(complete,"\n");
                                            send(new_client_socket  , complete , strlen(complete) , 0);
                                            //send(new_client_socket  , complete , strlen(complete) , 0);
                                            memset(complete,'\0',2000);
                                        }
                                    }
                                }
                                FD_CLR(new_client_socket , &readfs_new);
                                FD_CLR(0, &readfs_new);       
                
                            }
                        }while(wait(NULL) != -1);
                        }
                        else
                        {
                            printf("Client is unavailable for personal chat at the mometn\n");
                        }

                    }
                }
            }
        }
    //}
}