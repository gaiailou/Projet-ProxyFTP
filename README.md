# Projet de ProxyFTP

Un proxy FTP est un logiciel qui agit comme intermédiaire (on dit aussi mandataire) entre un client FTP et un serveur FTP. Celui-ci se trouve à la frontière de 2 domaines, le domaine local et le reste du monde. Ainsi lorsque l’on veut accéder à un serveur FTP sur Internet, on peut être amené à utiliser un logiciel proxy FTP. 
L’identification de l’utilisateur et du site serveur doit être communiquée au proxy sous la forme suivante : 
nom_login@nom_serveur

## Sujet

A partir d’une base donnée par les professeurs, nous devions programmer la connexion entre le client et le serveur avec le proxy.

## Instructions

Pour demarrer le proxy :

On se place dans le dossier ProxyFTP et on fait ```make``` pour executer le makefile.

Puis on lance le proxy avec ```./proxy```.

Ensuite il suffit de suivre l’instruction qui vous dis d’ouvrir une seconde fenêtre pour se connecter avec ```ftp -d [adresse] [port]```.

## Auteurs

- [Nicolas Rousseau](https://github.com/NeikoStream)
- [Gaïa Ducournau](https://github.com/gaiailou)
