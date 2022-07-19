//SPIRE Florian 21700144

#include "craps.h"

int main(void) {
	srand((unsigned int)time(NULL));
	char entree;
	do {
		printf("MENU PRINCIPAL : tapez la lettre J pour commencer a jouer, la lettre H pour acceder aux Highscores ou la lettre Q pour quitter: ");
		entree = fgetc(stdin);
		fseek(stdin, 0, SEEK_END); //Permet de ne pas reporter le contenu du buffer clavier à plus tard
		if (entree == 'h' || entree == 'H') {
			sauvegardeGains(0, NULL, 1);
		}
		else if (entree == 'q' || entree == 'Q') {
			exit(0);
		}
		else if (entree != 'j' && entree != 'J') {
			printf("Entree incorrecte\n");
		}
	} while (entree != 'j' && entree != 'J');
	int nombreJoueurs = nombre();
	Joueur* tableauJoueurs = tableau(nombreJoueurs);
	continuer(nombreJoueurs, tableauJoueurs);
	phase1(nombreJoueurs, tableauJoueurs,0);
	free(tableauJoueurs);
	return 0;
}
