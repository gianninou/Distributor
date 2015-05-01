# Distributor

Projet de RSA de Jean-Phillipe EISENBARTH et Valentin GIANNINI

Ce projet a pour but de chercher les nombres de Friedman de manière distribuée sur un réseau.

Ce projet est composé de 4 composants à savoir :

- Generator : permet de distribuer des nombre de manière séquentielle, d'enregistrer des résultats et de l'inscrire dans un fichier

- ClientExec : permet de lancer le script Python qui calcule le cherche une solution pour un nombre donné. S'il trouve alors le résultat est du type "nb:resultat" sinon une chaine vide.

- Server : permet de gérer des clients qui sont sur le réseaux, c'est lui qui va les inscrire dans une liste et envoyer les données du generator. Il vérifie également que les clients sont toujours en ligne via des PING-PONG, si le client ne répond pas, alors il sera supprimé de la liste des clients et le nombre sera donné au prochain client.

- Client : permet de demander au serveur un nombre et renvoie les solutions. C'est lui qui lance dialogue avec le clientExec.

Pour lancer la procédure il faut lancer le serveur et récupérer son adresse IP.

Ensuite, lancer des clients en spécifiant l'adresse IP du serveur.

De ce fait les clients calculeront des nombres de Friedman de manière distribuée.


