# APDU

## Gestion de la session de connection

- Connexion : 
Le client envoie un CNX pour se connecter au serveur.
Celui-ci enregistrera le client dans une liste.
Puis il renverra COK pour avertir que la connexion est effective

- Deconnexion
Le client envoie un DNX pour se déconnecter du serveur.
Celui-ci supprimera le client de la liste.
Puis il renverra DOK pour avertir de la bonne fin de déconnexion.

## Gestion des nombres

- Demande d'un nombre
Pour demander un nombre le client enverra GEN.
Le serveur répondra avec NBR suivie du nombre en question.

- Proposition d'une solution
Pour envoyer ca solution le client enverra RES suivi du nombre et de la solution.
S'il n'y a pas de solution pour le nombre, alors le champ solution sera un #

## Gestion de la vie des clients

- Vérification
Le serveur envoie un PIN(G) en multicast aux clients
S'il sont en vies, ils répondront PON(G)
Lorsqu'un client répond, ton timestamp sera mis à jour.
Sinon, si le timestamp est trop long, le client sera supprimé de la liste des clients