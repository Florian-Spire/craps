//SPIRE Florian 21700144

#include "craps.h"


int des(void) {
	int lancer1 = rand() % 6 + 1;
	int lancer2 = rand() % 6 + 1;
	return lancer1 + lancer2;
}

int nombre(void) {
	int nombreJoueurs;
	FILE* fichier = NULL;
	fichier = fopen("joueurs.txt", "r");
	if (fichier == NULL) {
		do {
			printf("Merci d'entrer le nombre de joueur pour cette partie (minimum 1 joueur): ");
			if (scanf("%d", &nombreJoueurs) != 1)
			{
				printf("Nombre de joueurs invalides, merci d'entrer un nombre entier superieur a 1\n");
				fseek(stdin, 0, SEEK_END); //Réinitialise le buffer clavier
				nombreJoueurs = 0;
			}
		} while (nombreJoueurs < 1);
		fseek(stdin, 0, SEEK_END);
	}
	else {
			if (fscanf(fichier, "%d", &nombreJoueurs) != 1) {
				printf("Fichier de sauvegarde corrompu\n");
				exit(1);
		}
			fclose(fichier);
	}
	return nombreJoueurs;
}

Joueur ajouterJoueur(int n) {
	struct joueur ajout;
	FILE* fichier = NULL;
	fichier = fopen("joueurs.txt", "r");
	if (fichier == NULL) {
		printf("Merci d'entrer le nom du joueur numero %d : ", n);
		fgets(ajout.nom, TAILLE_NOM, stdin);
		for (int i = 0; ajout.nom[i] != '\0'; i++) {
			if (ajout.nom[i] == '\n') {
				ajout.nom[i] = '\0'; //Supression retour à la ligne eventuellement capturé par fgets (fin de chaine quand retour à la ligne)
			}
		}
		fseek(stdin, 0, SEEK_END); //Vide le buffer clavier (dans le cas où l'entrée clavier dépasse TAILLE_NOM)
		printf("Merci d'entrer le nombre de jetons dont vous souhaitez disposer pour cette partie : ");
		while (scanf("%d", &ajout.jetons) != 1 || ajout.jetons < 1) {
			printf("Saisie invalide, veuillez entrer un entier positif correspondant a l'argent disponible pour le joueur numero %d : ", n);
			fseek(stdin, 0, SEEK_END);
		}
		printf("\n");
		fseek(stdin, 0, SEEK_END);
		ajout.gain = 0;
	}
	else {
		for (int i = 0; i < n; i++) {
			while (fgetc(fichier) != '\n');
		}
		if (fscanf(fichier, "%d %d %s\n", &ajout.jetons, &ajout.gain, &ajout.nom)!=3) {
			printf("Fichier de sauvegarde corrompu");
			exit(1);
		}
		fclose(fichier);
	}
	return ajout;
}

Joueur* tableau(int nombreJoueurs) {
	if (nombreJoueurs < 1) { //Vérification de nombreJoueurs pour être sur que le malloc s'effectue correctement (normalement impossible que nombreJoueurs<1, si ça a pu se produire erreur et on quitte le programme)
		printf("Erreur critique nombre joueurs\n");
		exit(3);
	}
	Joueur* tableauJoueurs = (Joueur*) malloc(nombreJoueurs * sizeof(Joueur));
	if (tableauJoueurs == NULL) { // On vérifie qu'il n'y a pas eu d'erreurs avec le malloc (par exemple OS qui refuse d'accorder un emplacement mémoire)
		printf("Erreur allocation memoire");
		exit(2);
	}
	for (int i = 0; i < nombreJoueurs; i++) {
		tableauJoueurs[i] = ajouterJoueur(i+1);
	}
	return tableauJoueurs;
}

void continuer(int nombreJoueurs, Joueur* tableau) {
	FILE* fichier = NULL;
	fichier = fopen("joueurs.txt", "r");
	printf("\n");
	if (fichier != NULL) {
		fclose(fichier);
		printf("Une sauvegarde de partie a ete detecte: tapez 'N' si vous souhaitez commencer une NOUVELLE partie\nNote: la partie sauvegardee se terminera alors et vous serez redirige vers le menu d'ou vous pourrez commencer une nouvelle partie\nAppuyez sur tout autre touche pour reprendre la partie sauvegardee\n");
		char entree = fgetc(stdin);
		fseek(stdin, 0, SEEK_END);
		if (remove("joueurs.txt") != 0) {
			printf("Erreur critique lors de la suppression de la sauvegarde merci de supprimer manuellement le fichier \"joueurs.txt\" et de relancer le programme \n");
			exit(4);
		}
		if (entree == 'N' || entree == 'n') {
			printf("La partie sauvegardee va se terminer (les eventuels meilleurs gains vont etre sauvegardee)\n");
			finPartie(nombreJoueurs, tableau);
		}
		else {
			printf("La partie sauvegardee va reprendre\nRappel des donnees de la partie en cours : \n");
			for (int i = 0; i < nombreJoueurs; i++) {
				printf("Le joueur %s dispose de %d jetons\n", tableau[i].nom, tableau[i].jetons);
			}
			printf("La partie va reprendre\n");
		}
	}
}

void passe(int nombreJoueurs, Joueur* Joueurs, int nul) {
	for (int i = 0; i < nombreJoueurs; i++) {
		printf("\n");
		if (!(Joueurs[i].pass) && nul) {
			printf("%s : coup nul : vous ne pouvez effectuer aucune action durant ce tour \n", Joueurs[i].nom);
			continue;
		}
		printf("%s : souhaitez vous miser sur pass ou don't pass ? Entrez P pour pass ou D pour don't pass  : ", Joueurs[i].nom);
		char entree = fgetc(stdin);
		fseek(stdin, 0, SEEK_END);
		if (entree == 'P' || entree == 'p') {
			Joueurs[i].pass = 1;
			printf("Combien souhaitez vous miser sur pass ? ");
			while (scanf_s("%d", &Joueurs[i].mise) != 1 || Joueurs[i].jetons - Joueurs[i].mise < 0 || Joueurs[i].mise < 0) {
				printf("RAPPEL : vous disposez de %d jetons \n", Joueurs[i].jetons);
				printf("Entree invalide, votre mise doit etre un entier positif non nul superieur ou egal au nombre de jetons qu'il vous reste : ");
				fseek(stdin, 0, SEEK_END);
			}
		}
		else if (entree == 'D' || entree == 'd') {
			Joueurs[i].pass = 0;
			printf("Combien souhaitez vous miser sur don't pass ? ");
			while (scanf_s("%d", &Joueurs[i].mise) != 1 || Joueurs[i].jetons - Joueurs[i].mise < 0 || Joueurs[i].mise < 0) {
				printf("RAPPEL : vous disposez de %d jetons \n", Joueurs[i].jetons);
				printf("Entree invalide, votre mise doit etre un entier positif non nul superieur ou egal au nombre de jetons qu'il vous reste: ");
				fseek(stdin, 0, SEEK_END);
			}
		}
		else {
			printf("Entree invalide, merci de saisir une entree correcte \n");
			i--;
		}
		fseek(stdin, 0, SEEK_END);
		}
	}


void gagne(Joueur* joueur) {
	joueur->gain += joueur->mise;
	joueur->jetons += joueur->mise;
	printf("%s : bravo vous recuperez votre mise et gagnez %d jetons ! \n", joueur->nom, joueur->mise);
	joueur->mise = 0;
}

void perd(Joueur* joueur) {
	joueur->gain -= joueur->mise;
	joueur->jetons -= joueur->mise; 
	printf("%s : vous avez perdu votre mise de %d jetons \n", joueur->nom, joueur->mise); 
	if (joueur->jetons < 1) {
		printf("%s : vous n'avez plus assez de jetons pour jouer, merci de definir un nouveau nombre de jetons pour ce joueur: ", joueur->nom);
			while (scanf("%d", &joueur->jetons) != 1 || joueur->jetons<1) {
				printf("Saisie invalide, veuillez entrer un entier positif non nul correspondant au nombre de jetons pour %s : ", joueur->nom);
				fseek(stdin, 0, SEEK_END);
			}
			fseek(stdin, 0, SEEK_END);
		}
	joueur->mise = 0;
}

int phase1(int nombreJoueurs, Joueur* Joueurs, int nul) {
	int lancer = des();
	passe(nombreJoueurs, Joueurs, nul);
	printf("\nLe %d a ete tire ! \n", lancer);
	for (int i = 0; i < nombreJoueurs; i++) {
		if (lancer == 2) {
			if (Joueurs[i].pass) {
				perd(&Joueurs[i]);
			}
			else {
				gagne(&Joueurs[i]);
			}
		}
		else if (lancer == 12) {
			if (Joueurs[i].pass) {
				perd(&Joueurs[i]);
			}
			else {
				printf("%s : coup nul : rien ne bouge pour vous (vous ne pourrez pas jouer au prochain tour) \n", Joueurs[i].nom);
			}
		}
		else if (lancer == 7) {
			if (Joueurs[i].pass) {
				gagne(&Joueurs[i]);
			}
			else {
				perd(&Joueurs[i]);
			}
		}
		else {
			printf("%d est donc le point \n\n", lancer);
			changementPhase(nombreJoueurs, Joueurs, lancer);
			return 0;
		}
	}
	if (lancer == 12) {
		phase1(nombreJoueurs, Joueurs, 1);
	}
	else {
		phase1(nombreJoueurs, Joueurs, 0);
	}
	return 0;
}

void changementPhase(int nombreJoueurs, Joueur* Joueurs, int point) {
	for (int i = 0; i < nombreJoueurs; i++) {
		if (Joueurs[i].pass && Joueurs[i].jetons-Joueurs[i].mise>0) {
			augmenter(&Joueurs[i]);
		}
		else if (!(Joueurs[i].pass)) {
			diminuer(&Joueurs[i]);
		}
		else {
			printf("%s : vous n'avez pas assez de jetons pour pouvoir augmenter votre mise \n\n", Joueurs[i].nom);
		}
	}
	phase2(nombreJoueurs, Joueurs, point, 0);
}

void augmenter(Joueur* joueur) {
	int ajouter = 0;
	printf("%s : votre mise sur Pass est de %d jetons \n", joueur->nom, joueur->mise);
	fseek(stdin, 0, SEEK_END);
	printf("De combien souhaitez-vous augmenter votre mise (vous pouvez ne pas augmenter en tapant 0) : ");
	while (scanf("%d", &ajouter) != 1 || ajouter < 0 || joueur->jetons - joueur->mise - ajouter < 0) {
		printf("RAPPEL : vous disposez de %d jetons et votre mise actuelle est de %d\n", joueur->jetons, joueur->mise);
		printf("Entree invalide, l'augmentation de votre mise doit etre un entier positif superieur ou egal au nombre de jetons qu'il vous reste : ");
		fseek(stdin, 0, SEEK_END);
	}
	fseek(stdin, 0, SEEK_END);
	joueur->mise += ajouter;
	if (ajouter == 0) {
		printf("%s : vous n'avez pas augmente votre mise \n\n", joueur->nom);
	}
	else {
		printf("%s : votre mise est desormais de %d jetons\n\n", joueur->nom, joueur->mise);
	}
}

void diminuer(Joueur* joueur) {
	int soustraire = 0;
	printf("%s : votre mise sur don't pass est de %d jetons \n", joueur->nom, joueur->mise);
	fseek(stdin, 0, SEEK_END);
	printf("De combien souhaitez-vous reduire votre mise (vous pouvez ne pas reduire en tapant 0) : ");
	while (scanf("%d", &soustraire) != 1 || soustraire<0 || joueur->mise - soustraire < 0) {
		printf("RAPPEL : votre mise actuelle est de %d \n", joueur->mise);
		printf("Entree invalide, la reduction de votre mise doit etre un entier positif inferieur ou egal a votre mise actuelle : ");
		fseek(stdin, 0, SEEK_END);
	}
	fseek(stdin, 0, SEEK_END);
	joueur->mise -= soustraire;
	if (soustraire == 0) {
		printf("%s : vous n'avez pas reduit votre mise \n\n", joueur->nom);
	}
	else {
		printf("%s : votre mise est desormais de %d jetons\n\n", joueur->nom, joueur->mise);
	}
}

void doubler(Joueur* joueur) {
	printf("%s : votre mise est de %d jetons \nSouhaitez-vous doubler cette mise ?\n Tapez la lettre O si vous souhaitez doubler la mise ou tout autre touche si vous souhaitez conserver votre mise actuelle : ", joueur->nom, joueur->mise);
	char entree = fgetc(stdin);
	fseek(stdin, 0, SEEK_END);
	if (entree == 'O' || entree == 'o') {
		joueur->mise *= 2;
		printf("%s : votre mise a bien ete double, elle est desormais de %d jetons\n", joueur->nom, joueur->mise);
	}
	else {
		printf("%s : vous n'avez pas double votre mise \n", joueur->nom);
	}
	fseek(stdin, 0, SEEK_END);
}

void diviser(Joueur* joueur) {
	printf("%s : votre mise est de %d jetons \nSouhaitez-vous diviser par 2 cette mise ?\n Tapez la lettre O si vous souhaitez reduire de moitie votre mise ou tout autre touche si vous souhaitez conserver votre mise actuelle : ", joueur->nom, joueur->mise);
	char entree = fgetc(stdin);
	fseek(stdin, 0, SEEK_END);
	if (entree == 'O' || entree == 'o') {
		joueur->mise /= 2;
		printf("%s : votre mise a bien ete reduite de moitie, elle est desormais de %d jetons\n", joueur->nom, joueur->mise);
	}
	else {
		printf("%s : vous n'avez pas reduit votre mise \n", joueur->nom);
	}
	fseek(stdin, 0, SEEK_END);
}

void phase2(int nombreJoueurs, Joueur* Joueurs, int point, int suspension) { 
	int lancer = des();
	printf("\nLe %d a ete tire ! \n", lancer);
	for (int i = 0; i < nombreJoueurs; i++) {
		printf("\n");
		if (lancer == point) {
			if (Joueurs[i].pass) {
				gagne(&Joueurs[i]);
			}
			else {
				perd(&Joueurs[i]);
			}
		}
		else if (lancer == 7) {
			if (Joueurs[i].pass) {
				perd(&Joueurs[i]);
			}
			else {
				gagne(&Joueurs[i]);
			}
		}
		else {
			printf("Le resultat est suspendu \n");
			if (suspension && Joueurs[i].mise > 1) {
				diviser(&Joueurs[i]);
			}
			else if (suspension==0 && (Joueurs[i].mise * 2 <= Joueurs[i].jetons) && Joueurs[i].mise>0) {
				doubler(&Joueurs[i]);
			}
			else {
				printf("%s : vous ne pouvez pas effectuer d'action pour le prochain tour \n", Joueurs[i].nom);
			}
		}
	}
	if (lancer == point || lancer == 7) {
		printf("Le tour est termine\nSouhaitez-vous recommencer un nouveau tour ?\nTapez O si oui ou tout autre touche si vous voulez arreter : ");
		char entree = fgetc(stdin);
		fseek(stdin, 0, SEEK_END);
		if (entree == 'O' || entree == 'o') {
			phase1(nombreJoueurs, Joueurs,0);
		}
		else {
			choix(nombreJoueurs,  Joueurs);
		}
	}
	else {
		phase2(nombreJoueurs, Joueurs, point, 1);
	}
}

int choix(int nombreJoueurs, Joueur* Joueurs) {
	while(1) {
		printf("\nSouhaitez-vous sauvegarder votre partie pour la reprendre ulterieurement ? Tapez 'O' si vous souhaitez sauvegarder ou 'N' si vous souhaitez terminer definitivement la partie et afficher les resultats (les plus hauts gains seront sauvegardes) : \n");
		char entree = fgetc(stdin);
		fseek(stdin, 0, SEEK_END);
		if (entree == 'O' || entree == 'o') {
			sauvegardeJoueurs(nombreJoueurs, Joueurs);
			return 0;
		}
		else if (entree == 'N' || entree == 'n') {
			finPartie(nombreJoueurs, Joueurs);
			return 0;
		}
		printf("Entree incorrecte\n");
	}
} 


int finPartie(int nombreJoueurs, Joueur* Joueurs) {
	printf("\nLes resulats pour cette partie sont : \n");
	for (int i = 0; i < nombreJoueurs; i++) {
		printf("%s : %d jetons\n", Joueurs[i].nom, Joueurs[i].gain);
	}
	sauvegardeGains(nombreJoueurs, Joueurs, 0);
	main();
	return 0;
} 

int sauvegardeJoueurs(int nombreJoueurs, Joueur* Joueurs) {
	FILE* fichier = NULL;
	fichier = fopen("joueurs.txt", "w+");
	if (fichier != NULL) {
		fprintf(fichier, "%d\n", nombreJoueurs);
		for (int i = 0; i < nombreJoueurs; i++) {
			fprintf(fichier, "%d %d %s\n", Joueurs[i].jetons, Joueurs[i].gain, Joueurs[i].nom);
		}
		fclose(fichier);
		printf("\nVotre partie a bien ete sauvegardee\n");
		return 0;
	}
	else
	{
		printf("Impossible d'acceder au fichier de sauvegarde (joueurs.txt) : la partie ne peut etre sauvegardee\n");
		return 1;
	}
}



int* triGains(int* tableauGains, int nombreEnregistrement) {
	int indicage = 0, egalite = 0;
	if (nombreEnregistrement < 1) {
		exit(3);
	}
	int* tableauTri = malloc(nombreEnregistrement * sizeof(int));
	if (tableauTri == NULL) {
		printf("Erreur allocation memoire");
		exit(2);
	}
	for (int i = 0; i < nombreEnregistrement; i++) {
		for (int j = 0; j < nombreEnregistrement; j++) {
			if (tableauGains[i] > tableauGains[j]) {
				indicage++;
			}
			else if (tableauGains[i] == tableauGains[j] && i != j) {
				egalite = 1;
			}
		}
		tableauTri[i] = indicage;
		indicage = 0;
	}
	while (egalite) { //Si égalité on veut que chaque élément de notre tableau ait une valeur unique
		egalite = 0;
		for (int k = 0; k < nombreEnregistrement; k++) {
			for (int l = 0; l < nombreEnregistrement; l++) {
				if (tableauTri[l] == tableauTri[k] && l != k) {
					tableauTri[k]++;
					egalite = 1;
				}
			}
		}
	}
	return tableauTri;
}

int indiceMin(int* tableau) {
	int min = 0;
	for (int i = 1; i < NOMBRE_GAINS; i++) {
		if (tableau[i] < tableau[min]) {
			min = i;
		}
	}
	return min;
}

int sauvegardeGains(int nombreJoueurs, Joueur* Joueurs, int mode) {
	char nomEnregistre[TAILLE_NOM];
	int gainEnregistre;
	int nombreEnregistrement = 0;
	int min;
	FILE* fichier = NULL;
	fichier = fopen("gains.txt", "r");
	char tableauNom[NOMBRE_GAINS][TAILLE_NOM];
	int tableauGains[NOMBRE_GAINS];
	if (fichier != NULL)
	{
		for (int i = 0; i < NOMBRE_GAINS; i++) {
			if ((fscanf(fichier, "%s\n%d\n", nomEnregistre, &gainEnregistre)) != 2) {
				nombreEnregistrement = i;
				break;
			}
			else {
				//La boucle for ici remplace ce qu'on aurait pu faire avec un "strcpy" (il aurait alors fallu inclure "string.h") mais comme cela ne prends que 3 lignes de code on le fait à la main ici
				for (int j = 0; nomEnregistre[j] != '\0'; j++) {
					tableauNom[i][j] = nomEnregistre[j];
					tableauNom[i][j + 1] = '\0'; //On s'assure que la chaine se finisse bien par \0 : si la boucle se termine le dernier caractère sera \0 (sinon il sera immédiatement remplacé par une autre valeur si la boucle continue)
				}
				tableauGains[i] = gainEnregistre;
				nombreEnregistrement++;
			}
		}
		fclose(fichier);
		if (mode) {
			printf("LES MEILLEURS SCORES SONT :\n");
			for (int i = 0; i < nombreEnregistrement; i++) {
				printf("%s : %d\n", tableauNom[i], tableauGains[i]);
			}
		}
	} 
	else
	{
		if (mode) {
			printf("Aucun score n'a ete enregistre pour le moment : vous devez terminer une partie avant pour voir les scores s'afficher ici\n");
			printf("NOTE: Si vous avez sauvegarde une partie elle est toujours consideree comme \"en cours\" : vous devez mettre fin a cette partie pour voir les meilleurs scores s'afficher ici\n");
			return 0;
		}
		else {
			nombreEnregistrement = 0;
		}
	}
	if(mode==0) {
		for (int i = 0; i < nombreJoueurs; i++) {
			if (nombreEnregistrement < NOMBRE_GAINS) {
				tableauGains[nombreEnregistrement] = Joueurs[i].gain;
				for (int j = 0; Joueurs[i].nom[j] != '\0'; j++) {
					tableauNom[nombreEnregistrement][j] = Joueurs[i].nom[j];
					tableauNom[nombreEnregistrement][j + 1] = '\0';
				}
				nombreEnregistrement++;
			}
			else { 
				min = indiceMin(tableauGains);
				if (Joueurs[i].gain > tableauGains[min]) {
					tableauGains[min] = Joueurs[i].gain; //On remplace le gain minimum par le nouveau gain si celui-ci est plus grand (celui-ci n'est pas forcément le minimum donc on le recherche à chaque tour)
					for (int j = 0; Joueurs[i].nom[j] != '\0'; j++) {
						tableauNom[min][j] = Joueurs[i].nom[j];
						tableauNom[min][j + 1] = '\0';
					}
				}
			}
		}
		int* tri = triGains(tableauGains, nombreEnregistrement); //Renvoi un tableau qui nous permet de savoir pour chaque élément sa position chronologique
		fichier = fopen("gains.txt", "w+");
		for (int k = nombreEnregistrement - 1; k >= 0; k--) {
			for (int n = 0; n < nombreEnregistrement; n++) {
				if (tri[n] == k) {
					fprintf(fichier, "%s\n%d\n", tableauNom[n], tableauGains[n]); //On écrit dans le fichier dans l'ordre décroissant des scores
					break;
				}
			}
		}
		fclose(fichier);
		free(tri);
	}
	return 0;
} 