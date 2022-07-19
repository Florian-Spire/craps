//SPIRE Florian 21700144

#pragma warning(disable : 4996) //Permet l'utilisation de scanf avec Visual Studio (on peut aussi remplacer les "scanf" par "scanf_s" mais "scanf_s" n'est pas une fonctions standard de C
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define TAILLE_NOM 25 //Paramètrer la taille maximum du nom
#define NOMBRE_GAINS 15 //Paramètrer le nombre de plus hauts gains mémorisés

//Garder les high scores
//Optionnel : Sauvegarde par lecture/ecriture de fichier
struct joueur {
	char nom[TAILLE_NOM];
	int jetons;
	int pass; //Mémorise si le joueur pass ou don't pass le prochain tour
	int mise;
	int gain;
};
typedef struct joueur Joueur;

int des(void);
int nombre(void);
Joueur ajouterJoueur(int n);
Joueur* tableau(int nombreJoueurs);
void continuer(int nombreJoueurs, Joueur* tableau);
void passe(int nombreJoueurs, Joueur* Joueurs, int nul);
void gagne(Joueur* joueur);
void perd(Joueur* joueur);
int phase1(int nombreJoueurs, Joueur* Joueurs, int nul);
void changementPhase(int nombreJoueurs, Joueur* Joueurs, int point);
void augmenter(Joueur* joueur);
void diminuer(Joueur* joueur);
void doubler(Joueur* joueur);
void diviser(Joueur* joueur);
void phase2(int nombreJoueurs, Joueur* Joueurs, int point, int suspension);
int choix(int nombreJoueurs, Joueur* Joueurs);
int finPartie(int nombreJoueurs, Joueur* Joueurs);
int sauvegardeJoueurs(int nombreJoueurs, Joueur* Joueurs);
int* triGains(int* tableauGains, int nombreEnregistrement);
int indiceMin(int* tableau);
int sauvegardeGains(int nombreJoueurs, Joueur* Joueurs, int mode);