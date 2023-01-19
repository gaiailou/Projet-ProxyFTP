/*
*
* Proxy réalisé par :
* - Nicolas Rousseau
* - Gaïa Ducournau
*
* Dans le cadre du projet de R3.05
*
*/
#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/socket.h>
#include  <netdb.h>
#include  <string.h>
#include  <unistd.h>
#include  <stdbool.h>
#include "./simpleSocketAPI.h"


#define SERVADDR "127.0.0.1"        // Définition de l'adresse IP d'écoute
#define SERVPORT "0"                // Définition du port d'écoute, si 0 port choisi dynamiquement
#define LISTENLEN 1                 // Taille de la file des demandes de connexion
#define MAXBUFFERLEN 1024           // Taille du tampon pour les échanges de données
#define MAXHOSTLEN 64               // Taille d'un nom de machine
#define MAXPORTLEN 64               // Taille d'un numéro de port


int main(){
    int ecode;                       // Code retour des fonctions
    char serverAddr[MAXHOSTLEN];     // Adresse du serveur
    char serverPort[MAXPORTLEN];     // Port du server
    int descSockRDV;                 // Descripteur de socket de rendez-vous
    int descSockCOM;                 // Descripteur de socket de communication
    struct addrinfo hints;           // Contrôle la fonction getaddrinfo
    struct addrinfo *res;            // Contient le résultat de la fonction getaddrinfo
    struct sockaddr_storage myinfo;  // Informations sur la connexion de RDV
    struct sockaddr_storage from;    // Informations sur le client connecté
    socklen_t len;                   // Variable utilisée pour stocker les 
                                     // longueurs des structures de socket
    char buffer[MAXBUFFERLEN];       // Tampon de communication entre le client et le serveur
    char us[50];                     // Contient l'utilisateur
    char serv[50];                   // Contient le serveur
    
    // Initialisation de la socket de RDV IPv4/TCP
    descSockRDV = socket(AF_INET, SOCK_STREAM, 0);
    if (descSockRDV == -1) {
         perror("Erreur création socket RDV\n");
         exit(2);
    }
    // Publication de la socket au niveau du système
    // Assignation d'une adresse IP et un numéro de port
    // Mise à zéro de hints
    memset(&hints, 0, sizeof(hints));
    // Initialisation de hints
    hints.ai_flags = AI_PASSIVE;      // mode serveur, nous allons utiliser la fonction bind
    hints.ai_socktype = SOCK_STREAM;  // TCP
    hints.ai_family = AF_INET;        // seules les adresses IPv4 seront présentées par 
                                      // la fonction getaddrinfo

     // Récupération des informations du serveur
     ecode = getaddrinfo(SERVADDR, SERVPORT, &hints, &res);
     if (ecode) {
         fprintf(stderr,"getaddrinfo: %s\n", gai_strerror(ecode));
         exit(1);
     }
     // Publication de la socket
     ecode = bind(descSockRDV, res->ai_addr, res->ai_addrlen);
     if (ecode == -1) {
         perror("Erreur liaison de la socket de RDV");
         exit(3);
     }
     // Nous n'avons plus besoin de cette liste chainée addrinfo
     freeaddrinfo(res);

     // Récuppération du nom de la machine et du numéro de port pour affichage à l'écran
     len=sizeof(struct sockaddr_storage);
     ecode=getsockname(descSockRDV, (struct sockaddr *) &myinfo, &len);
     if (ecode == -1)
     {
         perror("SERVEUR: getsockname");
         exit(4);
     }
     ecode = getnameinfo((struct sockaddr*)&myinfo, sizeof(myinfo), serverAddr,MAXHOSTLEN, 
                         serverPort, MAXPORTLEN, NI_NUMERICHOST | NI_NUMERICSERV);
     if (ecode != 0) {
             fprintf(stderr, "error in getnameinfo: %s\n", gai_strerror(ecode));
             exit(4);
     }
     printf("L'adresse d'ecoute est: %s\n", serverAddr);
     printf("Le port d'ecoute est: %s\n", serverPort);
     printf("Je suis en attente d'une connexion, veuillez ouvrir un deuxième terminale \n");
     printf("et vous connectez avec ftp -d [adresse] [port]\n");

     // Definition de la taille du tampon contenant les demandes de connexion
     ecode = listen(descSockRDV, LISTENLEN);
     if (ecode == -1) {
         perror("Erreur initialisation buffer d'écoute");
         exit(5);
     }

    len = sizeof(struct sockaddr_storage);
     // Attente connexion du client
     // Lorsque demande de connexion, creation d'une socket de communication avec le client
     descSockCOM = accept(descSockRDV, (struct sockaddr *) &from, &len);
     if (descSockCOM == -1){
         perror("Erreur accept\n");
         exit(6);
     }
    // Echange de données avec le client connecté

    /*****
     * Testez de mettre 220...
     * **/
    strcpy(buffer, "220 Bienvenue\n");
    write(descSockCOM, buffer, strlen(buffer));

    /*******
     * 
     * A vous de continuer !
     * 
     * *****/


    //Echange de données avec le serveur
    ecode = read(descSockCOM, buffer, MAXBUFFERLEN - 1);
    if (ecode == -1) {perror("Problème de lecture\n"); exit(3);}
    buffer[ecode] = '\0';
    printf("MESSAGE RECU DU SERVEUR: \"%s\".\n",buffer);
    //Découpage de l'entrée [login]@[serveur]
    sscanf(buffer,"USER %[^@]@%s", us, serv);

    strcat(us, "\r\n");//ajout de \r\n nécéssaire pour la connection au serveur

    printf("Utilisateur: %s \n", us);
    printf("Serveur : %s \n", serv);

    //Connection au serveur sur le port 21

    int SockCOMServ;
    char *port="21";
printf("----Tentative de connection au serveur sur le port 21----\n");
    ecode=connect2Server(serv, port, &SockCOMServ); 
if (ecode==-1){
        perror("Problème de lecture\n");
    }else{ 
        printf("Connexion établie\n"); 
    
 //recuperer la socket demande user
    printf("----Tentative de recuperation la socket demande user----\n");
    ecode = read(SockCOMServ, buffer, MAXBUFFERLEN - 1);
    if (ecode == -1) {perror("Problème de lecture\n"); exit(4);}
    buffer[ecode] = '\0';
    printf("MESSAGE RECU DU SERVEUR: \"%s\".\n",buffer);
    
    // envoie du login au serveur
printf("----Tentative d'envoie de l'utilisateur au serveur----\n");
    char cmdSRV[60] = "USER "; 
    strcat(cmdSRV, us);// commande (user login) afin de se connecter

    printf(buffer);
    write(SockCOMServ, cmdSRV, strlen(cmdSRV));

    //écoute et recuperer demande password (code:331)
    printf("----Tentative de recuperation la socket demande password----\n");
    ecode = read(SockCOMServ, buffer, MAXBUFFERLEN - 1);
    if (ecode == -1)
    {
        perror("probleme de lecture");
        exit(1);
    }
    buffer[ecode] = '\0';
    printf("MESSAGE RECU DU SERVEUR: \"%s\".\n",buffer);


    //transfert demande password au client
    printf("----Je transfert au client la demande----\n");
    write(descSockCOM, buffer, strlen(buffer));


    // ecoute la reponse du client
printf("----je recupère le mot de passe----\n");
    ecode = read(descSockCOM, buffer, MAXBUFFERLEN - 1);
    if (ecode == -1)
    {
        perror("probleme de lecture");
        exit(1);
    }
    buffer[ecode] = '\0';
    printf("MESSAGE RECU DU CLIENT: \"%s\".\n", buffer);


    // renvoie le mdp au serveur (Code:PASS)
printf("----Je transfert le mot de passe au serveur----\n");
    write(SockCOMServ, buffer, strlen(buffer));

    // attente de la reponse du serveur (Code:230)
printf("----J'attend la reponse du serveur----\n");
    ecode = read(SockCOMServ, buffer, MAXBUFFERLEN - 1);
    if (ecode == -1)
    {
        perror("probleme de lecture");
        exit(1);
    }
    buffer[ecode] = '\0';
    printf("MESSAGE RECU DU SERVEUR: \"%s\".\n",buffer);


    // envoie la confirmation au client (code:230)
printf("----Je transfert la confirmation serveur au client----\n");
    write(descSockCOM, buffer, strlen(buffer));

//faire le "ping pong" entre serveur et client
    //------------------------------------------
    // ecoute la reponse du client (code:SYST)
    ecode = read(descSockCOM, buffer, MAXBUFFERLEN - 1);
    if (ecode == -1)
    {
        perror("probleme de lecture");
        exit(1);
    }
    buffer[ecode] = '\0';
    printf("MESSAGE RECU DU CLIENT: \"%s\".\n",buffer);

    // envoie au serveur (code:SYST)
    write(SockCOMServ, buffer, strlen(buffer));

    // ecoute la reponse du serveur (code:215)
    ecode = read(SockCOMServ, buffer, MAXBUFFERLEN - 1);
    if (ecode == -1)
    {
        perror("probleme de lecture");
        exit(1);
    }
    buffer[ecode] = '\0';
    printf("MESSAGE RECU DU SERVEUR: \"%s\".\n",buffer);

    // envoie au client (code:215)
    write(descSockCOM, buffer, strlen(buffer));

    // ecoute la reponse du client
    ecode = read(descSockCOM, buffer, MAXBUFFERLEN - 1);
    if (ecode == -1)
    {
        perror("probleme de lecture");
        exit(1);
    }
    buffer[ecode] = '\0';
    printf("MESSAGE RECU DU CLIENT: \"%s\".\n",buffer);

    // decoupage du PORT
    printf("----Je fais du découpage *couic couic*----\n");
    int n1, n2, n3, n4, n5, n6;
    sscanf(buffer, "PORT %d,%d,%d,%d,%d,%d", &n1, &n2, &n3, &n4, &n5, &n6); // decoupage du message buffer recu

    // calcul de pasv (conversion du message recu pour obtenir l'ip dans le bon format et le calcul du port car on le recoit en octet)
    char ipClient[50], portClient[10];
    sprintf(ipClient, "%d.%d.%d.%d", n1, n2, n3, n4);
    sprintf(portClient, "%d", n5 * 256 + n6);
    printf("IP : %s , PORT : %s \n",ipClient,portClient);

    // connection avec le client
    printf("----Connexion avec le Client----\n");
    int actif;
    ecode = connect2Server(ipClient, portClient, &actif);

    // envoie de PASV au serveur
    char command[50] = "PASV\r\n";
    write(SockCOMServ, command, strlen(command));

    // ecoute du serveur
    ecode = read(SockCOMServ, buffer, MAXBUFFERLEN - 1);
    if (ecode == -1)
    {
        perror("probleme de lecture");
        exit(1);
    }
    buffer[ecode] = '\0';
    printf("MESSAGE RECU DU SERVEUR: \"%s\".\n",buffer);

    // decoupage de l'ip et du port
    printf("----Je fais du découpage *couic couic*x2----\n");
    sscanf(buffer, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)", &n1, &n2, &n3, &n4, &n5, &n6);

    char ipServeur[50], portServeur[10];
    sprintf(ipServeur, "%d.%d.%d.%d", n1, n2, n3, n4);
    sprintf(portServeur, "%d", n5 * 256 + n6);
    printf("IP : %s , PORT : %s \n",ipServeur,portServeur);

    // connection au serveur
    printf("----Connexion en passif ----\n");
    int passif;
    ecode = connect2Server(ipServeur, portServeur, &passif);

    // envoie de 200 au client (connexion est ok)
    char message[50] = "200 OK\n";
    write(descSockCOM, message, strlen(message));

    // ecoute de LIST au client
    ecode = read(descSockCOM, buffer, MAXBUFFERLEN - 1);
    if (ecode == -1)
    {
        perror("probleme de lecture");
        exit(1);
    }
    buffer[ecode] = '\0';
    printf("MESSAGE RECU DU CLIENT: \"%s\".\n",buffer);


    // envoie au serveur (code:LIST)
    write(SockCOMServ, buffer, strlen(buffer));

    // ecoute du serveur
    ecode = read(SockCOMServ, buffer, MAXBUFFERLEN - 1);
    if (ecode == -1)
    {
        perror("probleme de lecture");
        exit(1);
    }
    buffer[ecode] = '\0';
    printf("MESSAGE RECU DU SERVEUR: \"%s\".\n",buffer);

    // envoie au client (150)
    write(descSockCOM, buffer, strlen(buffer));


    printf("MESSAGE RECU DU SERVEUR: \"%s\".\n",buffer);
    do // boucle afin de lire l'entierete du ls
    {
        // lecture de donnees du serveur
        ecode = read(passif, buffer, MAXBUFFERLEN - 1);
        if (ecode == -1)
        {
            perror("probleme de lecture");
            exit(1);
        }
        buffer[ecode] = '\0';
        printf("%s", buffer);

        // envoie des donnees au client
        write(actif, buffer, strlen(buffer));
    } while (read(passif, buffer, MAXBUFFERLEN - 1) != 0);

    close(actif);
    close(passif);

    // ecoute pour confirmation du transfert (226)
    ecode = read(SockCOMServ, buffer, MAXBUFFERLEN - 1);
    if (ecode == -1)
    {
        perror("probleme de lecture");  
        exit(1);
    }
    buffer[ecode] = '\0';
    printf("MESSAGE RECU DU SERVEUR: \"%s\".\n",buffer);

    // envoie de la confirmation du transfert au client (226)
    write(descSockCOM, buffer, strlen(buffer));

    //Fermeture de la connexion
    printf("----Je ferme les connexions----\n");
    close(descSockCOM);
    close(descSockRDV);
    }
}
    
