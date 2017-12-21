// PROJET PASSERELLE.cpp : définit le point d'entrée pour l'application console.
// LISTES_LIN_DYN.cpp : définit le point d'entrée pour l'application console.
// LES PRIMITIVES SONT CELLES DE M. C. MINICH AVEC QUELQUES LÉGÈRES MODIFS
// LES MODULES ÉLABORÉS SONT CEUX DE M. A. KACED

#include "stdafx.h" // pour Visual Studio C/C++ (inclut "stdio.h"
#include "stdlib.h" // pour malloc, free, ...
#include "string.h" // Pour strlen...

void Erreur ( char * Msg)
{// Gestion d'erreur fatale de libellé Msg
    printf ("\n\n *** ERREUR : %s\n", Msg);
    getchar (); 
    exit (1);
}

typedef int       TValeur; // Type des valeurs d'une liste non vide
typedef unsigned int TNat   ; // type entier naturel (donc entier non signé)

// Le type Maillon : c'est une structure contenant une valeur et un pointeur
typedef struct Maillon	{
		TValeur         val  ;
		struct Maillon *suiv ;
} TMaillon ;

// Le type liste noté TListe : c'est un pointeur sur un maillon
typedef TMaillon * TListe ;	

TListe TableauListe[29]; 
TListe * temppoint;


#pragma region Cons et modif

bool EstVide (TListe L)
{// Donne true ssi L est vide 
    return L == NULL;
}

TValeur Tete (TListe L)
{// Donne, ssi L est non vide, la 1ère valeur de la liste
    if (L == NULL) Erreur ("acces a la valeur d'un maillon de liste inexistant!");
    return L->val;
}

TListe Reste (TListe  L)
{// Donne, ssi L est non vide, le reste de la liste L
    if (L == NULL) Erreur ("acces au reste d'un maillon de liste inexistant!");
    return L->suiv;
}

TListe ConsVide ( )
{// Donne une liste vide
    return NULL;
}

TListe Cons (TValeur a, TListe L)
{// Donne une nouvelle liste de 1er élement a
 // et dont le reste est la liste L
    TListe R = (TListe) malloc(sizeof(TMaillon));
    R->val  = a;
    R->suiv = L;
    return R;
}

TListe Cons0 (TValeur a) // A. Kaced
{// Donne une liste à un seul élément a
    TListe R = (TListe) malloc(sizeof(TMaillon));
    R->val  = a;
    R->suiv = NULL;
    return R;
}

void ModifTete (TListe *PtL, TValeur v)
// Remplace la première valeur de la liste *PtL par v
// ATTENTION : la liste *PtL doit être NON VIDE.
{
   (*PtL)->val = v;
}

void RemplaceTete (TListe L, TValeur v) // A. Kaced
// Remplace la première valeur de la liste L par v
// ATTENTION : la liste L doit être non vide!
{  if(L == NULL) Erreur ("tentative de remplacer la valeur en tete d'une liste vide!");
   L->val = v;
}

void ModifReste (TListe *PtL, TListe NouvReste)
// Remplace le reste de la liste *PtL par NouvReste
// ATTENTION : la liste *PtL doit être NON VIDE.
{  
   (*PtL)->suiv = NouvReste ;
}

void RemplacerReste (TListe L, TListe NouvReste) // A. Kaced
// Remplace le reste de la liste *PtL par NouvReste
// ATTENTION : la liste L doit êtrenon vide!
{  if(L == NULL) Erreur ("tentative de remplacer le reste d'une liste vide!");
   L->suiv = NouvReste ;
}

void SupprimerNegatif(TListe *L){
	TListe Aux = *L;
	if(!(EstVide(*L)))
	{
		while (!(EstVide(Aux)))
		{
			if(Tete(Aux)<0){
				Aux = Reste(Aux);
				ModifReste(L,Aux);
				Aux = Reste(Aux);
			}
		}
	}
}
#pragma endregion

#pragma region Libérations
void LibererListe (TListe L)
{// Libère tous les maillons de la liste L
    TListe R ;
    while (L)  { R = L;  L = L->suiv;  free (R); }
}

void LibererElement (TListe *PtL) // A. Kaced
{// Libère l'élément référencé par L et met L à vide
 // ATTENTION: ne fait rien si L vide!
	if (*PtL != NULL)  { free (PtL);  PtL = NULL; }
}

#pragma endregion


   /////////////////////////////////////////////////////////////
   //  MODULES ELABORES POUR LES LISTES LINEAIRES DYNAMIQUES  //
   /////////////////////////////////////////////////////////////
#pragma region Consultations, opérations particulières
void AfficherListe (TListe L) // itérative
{// Affiche les valeurs de la liste L 
 // au format : v1, v2, ..., vn
	TListe R = L;
	while (!EstVide(R))
	{ printf (" %d", Tete(R));
	  R = Reste(R);
	  if (!EstVide(R)) putchar(',');
	}
}

TNat Long (TListe L) // récursive
{// Donne le nombre de valeurs présentes dans L
	return EstVide(L)? 0u: 1u + Long(Reste(L));
}

TListe CopieListe (TListe L) // récursive
{// Donne une copie conforme et distincte de L
	if (EstVide(L)) return L;
	else            return Cons(Tete(L), CopieListe(Reste(L)));
}
TListe Concat(TListe L1, TListe L2) //Concatène la liste L1 et L2
{
	if (EstVide(L1)) return L2;
	else return Cons(Tete(L1), Concat(Reste(L1), L2));
}

void Concat2Listes (TListe *L1, TListe * L2) // récursive
{// Concatène L2 au bout de L1 et met L2 à vide
	TListe R;
	
	if (EstVide(*L1))
	{
		*L1 = *L2;
		*L2 = ConsVide();
	}
	else
	{
		R = Reste(*L1),
		LibererListe(*L2);
		Concat2Listes(&R, L2);
		ModifReste(L1, R);
	}
}

void RenverserListe (TListe *L) // itérative
{// Renverse, sur place, l'ordre des éléments de L
	bool Trouve = true;
	int i;
	int fin = Long(*L);
	TListe Res = ConsVide();

	while (!Trouve) 
	{
		TListe Inter = *L;

		for (i = 0; i < fin; i++)
			Inter = Reste(Inter);
		Res = Cons(Tete(Inter), Res);
		fin = fin - 1;

		if (fin = 0)
			Trouve = false;
	}
	*L = Res;
}

bool EstDans (TValeur v, TListe L) // récursive
{// Donne true ssi v est une valeur présente dans L
	if (EstVide(L)) return false;
	else if (Tete(L) == v) return true;
	else EstDans(v, Reste(L));
}

void CouperEn2 (TListe *L1, TListe *L2) // récursive
{// Laisse en L1 les éléments de rang pair et
 // met les autres en L2.
	TListe L3;

	if (EstVide(*L1))
		*L2 = ConsVide();
	else
	{
		L3 = Reste(*L1);
		CouperEn2(&L3, L1);
		ModifReste(L2, *L1);
		*L1 = L3;
	}
}

void Fusionner (TListe L1, TListe L2, TListe *L) // récursive
{// Donne la fusion des deux listes ordonnées L1 et L2
	if (EstVide(L1))
		*L = L2;
	else if (EstVide(L2))
		*L = L1;
	else if (Tete(L1) <= Tete(L2))
	{
		Fusionner(Reste(L1), L2, L);
		ModifReste(&L1, *L);
		*L = L1;
	}
	else
	{
		Fusionner(Reste(L2), L1, L);
		ModifReste(&L2, *L);
		*L = L2;
	}
}
void EchangeDeuxListes(int i1, int i2, TListe* L){
	TListe LTemp;
	LTemp = L[i1];
	L[i1] = L[i2];
	L[i2] = LTemp;
}
#pragma endregion

#pragma region Ajout/suppression
void AjouterEnPlace (TListe * L, TValeur v, int n) // récursive
{// Ajouter la valeur v au rang n dans la liste L
 // Si le rang n > Long(n) alors on mettra la valeur en queue de liste
 // Si le rang n <= 1 alors on mettra la valeur en tête de liste
	TListe R;

	if (EstVide(*L) && n <= 1)
		*L = Cons(v, ConsVide());
	else
	{
		R = Reste(*L);
		AjouterEnPlace(&R, v, n - 1);
		ModifReste(L, R);
	}
}


void AjouterEnFin(TListe * L, TValeur v) // récursive
{// Ajouter la valeur v en fin de la liste L
	TListe Aux = *L;

	if (!EstDans(v, *L) || EstVide(*L))
		Aux = Cons(v, Aux);
	else
	{
		Aux = Reste(Aux);
		AjouterEnFin(&Aux, v);
	}
}


void SupprimerEnPlace (TListe * L, int n) // récursive
{// Supprimer la valeur au rang n, si elle existe, dans la liste L
	TListe Prec = ConsVide();
	TListe Aux = *L;

	if(!EstVide(*L))
	{
		if (EstVide(Reste(*L)) || n == 1)
			*L = Reste(*L);
		else
		{
			Prec = Aux;
			Aux = Reste(Aux);
			SupprimerEnPlace(&Aux, n - 1);
			Aux = Reste(Aux);
			ModifReste(&Prec, Aux);
		}
	}
}

void Supprimer1ereOcc (TListe * L, TValeur v) // récursive
{// Supprimer, si elle existe, la 1ère valeur v de la liste L
	TListe Aux = *L;

	if (!EstVide(*L))
	{
		if (Tete(Aux) == v)
		{
			Aux = Reste(Aux);
			ModifReste(L, Aux);
		}
		else
		{
			Aux = Reste(Aux);
			Supprimer1ereOcc(&Aux, v);
		}
	}
}

void SupprimerTouteOcc (TListe * L, TValeur v) // récursive
{// Supprimer toute occurrence de la valeur v de la liste L
	TListe Aux = *L;

	if (!EstVide(*L))
	{
		if (Tete(Aux) == v)
		{
			Aux = Reste(Aux);
			ModifReste(L, Aux);
			Aux = Reste(Aux);
			SupprimerTouteOcc(&Aux, v);
		}
	}
}

void OterDoublons (TListe * L) // récursive
{// Ôter les doublons de la liste L
	TListe Aux = ConsVide();

	if (EstVide(*L))
		*L = ConsVide();
	else
	{
		if (!EstDans(Tete(*L), Aux))
		{
			Cons(Tete(*L), Aux);
			*L = Reste(*L);
			OterDoublons(L);
		}
		else
		{
			*L = Reste(*L);
			OterDoublons(L);
		}			
	}
	*L = Aux;
}

#pragma endregion



void Partitionner (TValeur pivot, TListe* L1,TListe* L2) // récursive
{// Partitionner L1 à l'aide du pivot en
 // mettant les valeurs de L1 > pivot dans L2 et en laissant les autres dans L1
	TListe Aux = CopieListe(*L1);
	*L1 = ConsVide();

	if (EstVide(*L1))
		*L2 = *L1;
	else
	{
		if (Tete(Aux) > pivot)
		{
			Cons(Tete(Aux), *L2);
			Aux = Reste(Aux);
			Partitionner(pivot, &Aux, L2);
		}
		else
		{
			Cons(Tete(Aux), *L1);
			Aux = Reste(Aux);
			Partitionner(pivot, &Aux, L2);
		}
	}
}

void TrierRapidement (TListe *L1) // doublement récursive
{// Effectuer le tri rapide de la liste linéaire L
	TListe L2 = ConsVide();
	TValeur Pivot1 = Long(*L1) / 2;
	TValeur Pivot2 = Pivot1;

	if (Long(*L1) < 0)
	{
		Partitionner(Pivot1, L1, &L2);
		Pivot1 = Pivot1 - 1;
		TrierRapidement(L1);
		Pivot2 = Pivot2 + 1;
		TrierRapidement(&L2);
	}
	Concat2Listes(L1, &L2);
}



void Tri_Fusion (TListe * L) // doublement récursive
{// Trier la liste L par fusions récursives 
	TListe R, R2;
	
	if (!EstVide(*L) && !EstVide(Reste(*L)))
	{
		CouperEn2(L, &R);
		Tri_Fusion(L);
		Fusionner(*L, R, &R2);
		*L = R2;
	}
}

void SaisirListe(TListe * L)
{
	
	bool fini = false;
	int i = 2;
	TValeur V;
	printf("Saisir le premier element :");
	scanf("%d", &V);
	AjouterEnPlace(L, V, 1);
	
	do{
		static char choix[1]="";
		printf("Inserer un nouvel élement ? O/N");
		scanf("%s", &choix);
		if (choix[0]=='o'||choix[0]=='O')
		{
			printf("Inserer le %d eme élément:", i);
			scanf("%d", &V);
			AjouterEnPlace(L, V, i);
			i++;
			if(i == 30) {fini = true;}
		}
		else if (choix[0] == 'N'||choix[0] == 'n') {fini = true;}
		else {Erreur("Mauvaise commande, recommencez !");}
	}while(!fini);

}

void AfficherCar(int n, char c)
{
	int i;
	for (i = 0; i <= n; i++)
	{
		printf("%c", c);
	}
}
int LongueurListe(TListe L)
{
	int i=0;
	while (!EstVide(L))
	{ i++;
	  L = Reste(L);
	}
	return i;

}
char OrdreListe(TListe L)
{
	if(EstVide(L)||EstVide(Reste(L)))return'=';
	else
	{
		char Ordre = OrdreListe(Reste(L));
		switch(Ordre)
		{
		case '<':
			if(Tete(L)>Tete(Reste(L)))return'?';
			else return '<';
		case '>':
			if(Tete(L)<Tete(Reste(L)))return'?';
			else return '>';
		case '=':
			if(Tete(L)>Tete(Reste(L)))return '>';
			else if(Tete(L)<Tete(Reste(L)))return '<';
			else return '=';
		default:
			return '?';
		}
	}
}
void Titrer(char *s, int largeur) //affiche le titre au milieu de la fenêtre
{
	int i;
	printf("\n");
	int d = ((largeur - strlen(s) - 6) / 2);
	AfficherCar(d, ' ');
	printf("\xC9");
	AfficherCar(strlen(s) + 5, '\xCD');
	printf("\xBB\n");
	AfficherCar(d, ' ');
	printf("\xBA   %s   \xBA\n", s);
	AfficherCar(d, ' ');
	printf("\xC8");
	AfficherCar(strlen(s) + 5, '\xCD');
	printf("\xBC\n");
}
void AfficherMenu()
{
		Titrer("MENU DES COMMANDES", 110);
		printf("\n\n\n>L1 a >L30 : Saisir une des 30 listes lineaires.\n\n"
			   "<L1 a <L30 : Afficher une des 30 listes lineaires.\n\n"
			   "<L*	   : Afficher toutes les listes lineaires non vides.\n\n"
			   "+L1 a +L30 : Trier, si necessaire,  une des 30 listes lineaires \n"	
			   "-L1 a -L30 : Renverser, si necessaire, l'ordre d'une des 30 listes lineaires\n\n"
			   "^L1 a ^L30 : Melanger, si necessaire,  une des 30 listes lineaires\n\n"
			   "L1|L2	   : Fusionner deux listes (qui doivent etre ordonnees!) dans la 1ere citee\n\n"
			   "L1\L2	   : Concatener deux listes dans la premiere citee\n\n"
			   "?L1 a L30  : Donner des infos sur la liste : * longueur, * ordre (<, =, >, ?)\n\n"
			   "&L1 a &L30 : Liberer une des 30 listes lineaires\n\n"
			   "&*	   : Liberer toutes les listes lineaires non vides du tableau\n\n"
			   "L1#L2,...  : Echanger deux listes quelconques du tableau\n\n"
			   "!M         : Afficher le menu des commandes possibles\n\n"
			   "!L1		   : Supprime valeurs negatives\n\n"
			   "!Q         : Quitter l'interpreteur de commande.\n\n"
		);
}
void ChoisirChoix()
{
	bool fini = false;
	int num;
	do
	{
	int i;
	char choix[7] = "";
	printf ("Votre choix : ");
	scanf("%s",&choix);
	char choixtri[1]="";
		switch(choix[0])
		{
			case '?' :
					switch(strlen(choix))
				{
					case 3:
					{
					int choixcase = choix[2]- '0';
					printf("Longueur de la liste n° %c : %d \n\n",choix[2],LongueurListe(TableauListe[choixcase- 1]));
					printf("Ordre de la liste n° %c : %c \n\n",choix[2],OrdreListe(TableauListe[choixcase- 1]));
					choixcase = NULL;
					break;
					}
					case 4:
					{
					int choixcase2 = (choix[2] - '0')*10;
					int choixcase3 = choix[3]-'0';
					printf("Longueur de la liste n° %c%c : %d \n\n",choix[2],choix[3],LongueurListe(TableauListe[choixcase3-1]));
					printf("Ordre de la liste n° %c%c : %c \n\n",choix[2],choix[3],OrdreListe(TableauListe[choixcase3-1]));
					choixcase2 = choixcase3 = NULL;
					break;
					}
					default:
					Erreur("Mauvaise commande, recommencez!");
					system("pause");
					break;
				}
			break;
			case '&' :
					switch(strlen(choix))
				{
					case 2:
						{
						int i = 0;
						if(choix[1] =='*')
						{
							for(i=0;i<29;i++)
							{
								if(EstVide(TableauListe[i]))
									{
										printf("Liste numero %d vide !\n",i);
										break;
									}
								else
									{
										LibererListe(TableauListe[i]);
										if(EstVide(TableauListe[i])){printf("Liste numero %d vidée avec succès !\n",i);}
										break;
									}
							}
						}
						else {Erreur("Faute de frappe, recommencez !");break;}
						}
				}
					break;
			case 'L' :
				{
					switch(strlen(choix))
				{
					case 5:
						{
						switch(choix[2])
						{	
						case '/':
							{
							int choix1 = choix[1]-'0'-1;
							int choix2 = choix[4]-'0'-1;
							Concat2Listes(&TableauListe[choix1],&TableauListe[choix2]);
							choix1 = choix2 = NULL;
							break;
							}
						case '|':
							{
							int choix1 = choix[1]-'0'-1;
							int choix2 = choix[4]-'0'-1;
							Fusionner(TableauListe[(choix[1])-'0'-1],TableauListe[(choix[4])-'0'-1],temppoint);
							choix1 = choix2 = NULL;
							break;
							}
						case '#':
							{
							int choix1 = choix[1]-'0'-1;
							int choix2 = choix[4]-'0'-1;
							EchangeDeuxListes(choix1,choix2,TableauListe);
							choix1 = choix2 = NULL;
							break;
							}
						default:
							Erreur("Faute de frappe, recommencez !");
							break;
						}
						}
					case 6:
						{
							if(choix[2]=='/'||choix[2]=='|'||choix[2]=='#')
						{
						switch(choix[2])
						{
						case '/':
							{
							int choix1 = choix[1]-'0'-1;
							int choix2 = (choix[4] - '0')*10+(choix[5])-'0'-1;
							Concat2Listes(&TableauListe[choix1],&TableauListe[choix2]);
							choix1 = choix2 = NULL;
							break;
							}
						case '|':
							{
							int choix1 = choix[1]-'0'-1;
							int choix2 = (choix[4] - '0')*10+(choix[5])-'0'-1;
							Fusionner(TableauListe[choix1],TableauListe[choix2],temppoint);
							choix1 = choix2 = NULL;
							break;
							}
						case '#':
							{
							int choix1 = choix[1]-'0'-1;
							int choix2 = (choix[4] - '0')*10+(choix[5])-'0'-1;
							EchangeDeuxListes(choix1,choix2,TableauListe);
							choix1 = choix2 = NULL;
							break;
							}
						default:
							Erreur("Faute de frappe, recommencez !");
							break;
						}
						}
							else if (choix[3]=='/'||choix[3]=='|'||choix[3]=='#')
						{
							switch(choix[3])
						{
						case '/':
							{
							int choix1 = (choix[1] - '0')*10+(choix[2])-'0'-1;
							int choix2 = (choix[5])-'0'-1;
							Concat2Listes(&TableauListe[choix1],&TableauListe[choix2]);
							choix1=choix2=NULL;
							break;
							}
						case '|':
							{
							int choix1 = (choix[1] - '0')*10+(choix[2])-'0'-1;
							int choix2 = (choix[5])-'0'-1;
							Fusionner(TableauListe[choix1],TableauListe[choix2],temppoint);
							choix1 = choix2 = NULL;
							break;
							}
						case '#':
							{
							int choix1 = (choix[1] - '0')*10+(choix[2])-'0'-1;
							int choix2 = (choix[5])-'0'-1;
							EchangeDeuxListes(choix1,choix2,TableauListe);
							choix1 = choix2 = NULL;
							break;
							}
						default:
							Erreur("Faute de frappe, recommencez !");
							break;
						}
						}
						else{Erreur("Mauvaise commande, recommencez!");system("pause");break;}
						}
					case 7:
						{
						switch(choix[3])
						{
						case '/':
							{
							int choix1 = (choix[1] - '0')*10+(choix[2])-'0'-1;
							int choix2 = (choix[5] - '0')*10+(choix[6])-'0'-1;
							Concat2Listes(&TableauListe[choix1],&TableauListe[choix2]);
							choix1 = choix2 = NULL;
							break;
							}
						case '|':
							{
							int choix1 = (choix[1] - '0')*10+(choix[2])-'0'-1;
							int choix2 = (choix[5] - '0')*10+(choix[6])-'0'-1;
							Fusionner(TableauListe[choix1],TableauListe[choix2],temppoint);
							choix1 = choix2 = NULL;
							break;
							}
						case '#':
							{
							int choix1 = (choix[1] - '0')*10+(choix[2])-'0'-1;
							int choix2 = (choix[5] - '0')*10+(choix[6])-'0'-1;
							EchangeDeuxListes(choix1,choix2,TableauListe);
							choix1 = choix2 = NULL;
							break;
							}
						default:
							Erreur("Faute de frappe, recommencez !");
							break;
						}
					}
				}
			}
			case '+' :
				{
					switch(strlen(choix))
				{
					
					case 3:
						{
						printf("Tri rapide ou par fusion ?R/F");
						scanf("%c",choixtri);
						int choixcase = choix[2]- '0'- 1;
						if(choixtri[0] == 'R' || choixtri[0] == 'r')
						{
							TrierRapidement(&TableauListe[choixcase]);
						}
						else if (choixtri[0] == 'F' || choixtri[0] == 'f')
						{
							Tri_Fusion(&TableauListe[choixcase]);
						}
						else
						{
						Erreur("Mauvaise commande, recommencez !");
						system("pause");
						}
						break;
						}
						
					case 4:
						{
						if (((choix[2] - '0')*10+(choix[3])-'0'-1) < 30)
						{
						printf("Tri rapide ou par fusion ?R/F");
						scanf("%c",choixtri);
						if(choixtri[0] == 'R' || choixtri[0] == 'r')
						{
							TrierRapidement(&TableauListe[(choix[2] - '0')*10+(choix[3])-'0'-1]);
						}
						else if (choixtri[0] == 'F' || choixtri[0] == 'f')
						{
							Tri_Fusion(&TableauListe[(choix[2] - '0')*10+(choix[3])-'0'-1]);
						}
						break;
						}
						else { Erreur("Vous ne pouvez pas saisir de liste dans cette case du tableau, recommencez !");system("pause");break;}		
						}
					}
				}
			case '-' :
				{
				switch(strlen(choix))
				{
					case 3:
						{
						int choixcase = choix[2]- '0'- 1;
						RenverserListe(&TableauListe[choixcase]);
						break;
						}
					case 4:
						{
						int choixcase1 = (choix[2] - '0')*10+(choix[3]-'0'-1);
						if(choixcase1<30)
						{
						RenverserListe(&TableauListe[choixcase1]);
						}
						else
						{
							Erreur("Vous ne pouvez pas saisir de liste dans cette case du tableau, recommencez !");
						}
						break;
						}
					default:
				Erreur("Mauvaise commande, recommencez");
				system("pause");
				break;
				}
				}
			case '>' :
				{
				switch(strlen(choix))
				{
					case 3:
						{
							int choixcase = choix[2]- '0'- 1;
							SaisirListe(&TableauListe[choixcase]);
							break;
						}
					case 4:
						{
						int choixcase = (choix[2] - '0')*10+((choix[3])-'0'-1);
						if(choixcase<30)
						{
						SaisirListe(&TableauListe[choixcase]);
						break;
						}
						else
						{
							Erreur("Vous ne pouvez pas saisir de liste dans cette case du tableau, recommencez !");
							system("pause");
							break;
						}
						}
					default:
					Erreur("Mauvaise commande, recommencez");
					break;
						
				}break;
				}
			case '<' :
				switch(strlen(choix))
				{
					case 3:
						{
						int choixcase1 = choix[2]-'0';
						int choixcase2 = choix[3]-'0';
						if(choix[2]=='*')
						{
							for(i=0;i<30;i++)
							{
								if(!EstVide(TableauListe[i])) 
								{
									printf("Liste n°%d :",i+1);
									AfficherListe(TableauListe[i]);
									printf("\n\n");
								}
								else {printf("La liste n°%d est vide !\n\n",i+1);}
								
							}
						}
						else if (choixcase1>0 && choixcase1<30)
						{
							if(EstVide(TableauListe[choixcase1-1]))
							{
						Erreur("Liste vide, donc impossible de l'afficher");
						system("pause");
						
						break;
							}
							else
							{
							printf("Liste n°%d :",choixcase1);
							AfficherListe(TableauListe[choixcase1-1]);
							printf("\n\n");
							break;
							}
							break;
						}
						else { Erreur("Erreur de commande, recommencez !");system("pause");}
						break;
						}
					case 4:
						{
						int choixcase = (choix[2] - '0')*10+((choix[3])-'0'-1);
						if(choixcase<30)
						{
							AfficherListe(TableauListe[choixcase]);
						}
						else
						{
							Erreur("Vous ne pouvez pas afficher de liste dans cette case du tableau, recommencez !");
							system("pause");
						}
						break;
						}
					default:
						int choixcase = (choix[2] - '0')*10+((choix[3])-'0'-1);
						Erreur("Mauvaise commande, recommencez");
						break;
						
				}break;
			case '!':
				{
				if (choix[1] == 'm' || choix[1] == 'M')
				{
					AfficherMenu();
				}
				else if (choix[1] =='q'|| choix[1] == 'Q')
				{
					fini = true;
					printf("Fermeture du programme\n");
					system("pause");
					exit(0);
				}
				else {Erreur("Mauvaise commande, recommencez !");system("pause");}
				break;
				}break;
				}
}while (true);
}



int main()
{   
	/*TListe L1 = Cons(5.0, Cons( 2.0, Cons( 3.0, Cons0( 7.0))));
    TListe L2 = Cons(17.0, Cons(19.0, Cons(11.0, Cons0(13.0))));
	TListe L3;*/
	Titrer("GESTION D'UN TABLEAU DE LISTES LINEAIRES CHAINEES DYNAMIQUES", 110);	
	printf ("\n\n\n");
	int i;
	for(i=0;i<30;i++){TableauListe[i] = ConsVide();}
	AfficherMenu();
	ChoisirChoix();
	getchar();  
	return 0;


}