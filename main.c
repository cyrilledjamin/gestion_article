#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>
#include <io.h>
#include <string.h>
#define TVA 0.21
#define FICHIER_STOCK "C:Djamin.dat"


// définition des structures
typedef struct
{ int Jour, Mois, Annee;} date;
typedef struct
{ date DateEntree; int TailleS, TailleM, TailleL;} stock;
typedef struct
{ int Reference; char Designation[20], Categorie;
  float PrixHT; stock Stock;} article;

typedef struct element element;
struct element
{
    article information;
    struct element* suite;
};

// structure de controle
typedef struct liste liste;
struct liste
{
    element *premier;
};

/*
struct tmp
{
    article information;
    struct tmp *suite;
};
typedef struct tmp liste;*/

// prototype des fonctions Labos2.1
void EncoderDate(date *ptr);
double JoursEcoules(date p);
void EncoderArticle(article *ptr);
void AfficherArticle(article Art);

// prototype des fonctions Labos2.2
void AjouterArticle(article,char []);
int RecupererStock(article** ptr,char []);
int SupprimerArticle(char[]);
long RechercherArticle(article* ptr, FILE *);
void ModifierStock(char []);

// prototype des fonctions Labos2.3
void TrierDesignation(article *brut, article **tri,int nbre);
void TrierPrix(article *brut, article **tri,int nb);
void VoirArticle(article ARTICLE);

// prototype des fonctions Labos2.4

void AjouterArticlePanier(char fichier[], liste *liste_initiale);
void VoirPanier (liste *liste_initiale);
void SupprimerArticlePanier(char fichier[], liste** ptr_debut);
void EcrireBonDeCommande(char fichier[], liste *ptr);
void ViderPanier();

//debut du programme
int main()
{
   // Initialisation du panier
  liste *initial_liste = malloc(sizeof(liste));
  element *element_liste = malloc(sizeof(element));

  if(initial_liste == NULL || element_liste == NULL)
  {
    exit(EXIT_FAILURE);
  }

  article dummy_article;
  dummy_article.Reference = -10;

  element_liste->information = dummy_article;
  element_liste->suite = NULL;
  initial_liste->premier = element_liste;

  char choixMenu, choix_panier; int choix,i;
  article larticle;
  article *ptr;
  article** tri;
  int N;

 do{
    printf("\n\n--------------------------------------");
    printf("\nA. Stock\n"
           "B. Vente\n"
           "Q. Quitter\n\n\n"
           "Votre choix: ");
    fflush(stdin);scanf("%c", &choixMenu);
   switch (choixMenu)
   {
       do
       {
        case 'A': printf("\n\n-------------------------------------------\n");
                  printf("\n1. Ajouter un nouvel article \n"
                         "2. Afficher tous les articles \n"
                         "3. Supprimer tous les article \n"
                         "4. Modifier le stock \n"
                         "5. Retourner au menu principal \n\n\n"
                         "Votre choix:  ");
            fflush(stdin);      scanf("%d", &choix);
                  switch (choix)
                  {
                        case 1:  AjouterArticle(larticle, FICHIER_STOCK);
                                 break;

                        case 2: N=RecupererStock(&ptr, FICHIER_STOCK);
                                tri=(article **)malloc(N*sizeof(article *));
                                TrierDesignation(ptr,tri,N);
                                for(i=0; i<N; i++)
                                    {
                                        AfficherArticle(**(tri+i));
                                    }
                                free(ptr);free(tri);
                                break;

                        case 3: SupprimerArticle(FICHIER_STOCK);
                                break;

                        case 4: ModifierStock(FICHIER_STOCK);
                                break;

                        default:  break;
                  }
       }while(choix!=5); break;

       do
       {
        case 'B': printf("\n\n---------------------------------------------\n");
                  printf("\n1. collection homme \n"
                         "2. collection femme \n"
                         "3. Panier \n"
                         "4. Retourner au menu principal \n\n\n"
                         "Votre choix:  ");
            fflush(stdin);      scanf("%d", &choix);
            switch (choix)
                {
                        case 1: N=RecupererStock(&ptr, FICHIER_STOCK);
                                tri=(article **)malloc(N*sizeof(article *));
                                TrierPrix(ptr,tri,N);


                                     for(i=0; i<N; i++)
                                       {

                                           if(((*(tri+i))->Categorie)=='H')
                                            VoirArticle(**(tri+i));
                                       }


                                        printf("Ajouter un article au panier 'O' ou 'N': ");
                                        fflush(stdin);  scanf("%c", &choix_panier);
                                        switch (choix_panier)
                                        {
                                            case 'O':  printf("\n hello!");
                                                       AjouterArticlePanier(FICHIER_STOCK, initial_liste);

                                                                    break;
                                        }


                                    free(ptr);free(tri);
                                break;

                        case 2: N=RecupererStock(&ptr, FICHIER_STOCK);
                                tri=(article **)malloc(N*sizeof(article *));
                                TrierPrix(ptr,tri,N);


                                     for(i=0; i<N; i++)
                                       {

                                           if(((*(tri+i))->Categorie)=='F')
                                            VoirArticle(**(tri+i));
                                       }

                                  printf("Ajouter un article au panier 'O' ou 'N': ");
                                        fflush(stdin);  scanf("%c", &choix_panier);
                                        switch (choix_panier)
                                        {
                                            case 'O':  printf("\n Ajout en cours...");
                                                       AjouterArticlePanier(FICHIER_STOCK, initial_liste);

                                                                    break;
                                        }
                                    free(ptr);free(tri);
                                break;


                        case 3:
                                VoirPanier(initial_liste);
                                break;

                }
       }while(choix!=4); break;

        case 'Q': printf("\n\nMerci et a bientot..."); break;
        default: break;
   }
}while((choixMenu!='Q'));
getch();
    return 0;
}

// definition des fonctions
void EncoderDate(date* ptr)
{
    printf("\n entrez la date au format jj/mm/aaaa:  ");
    fflush(stdin);
    scanf("%d/%d/%d", &(ptr->Jour),&(ptr->Mois),&(ptr->Annee));
}
double JoursEcoules(date p)
{
  double diffTemps;
  time_t tsec=time(NULL);
//tsec est initialisé à la date et l'heure courante
struct tm t1;
t1=*localtime(&tsec);
//conversion de la date saisi par l'utilisateur
   t1.tm_mday = p.Jour;
   t1.tm_mon  = p.Mois - 1;
   t1.tm_year = p.Annee - 1900;
//tsec= time(NULL);
diffTemps= difftime(tsec,mktime(&t1))/86400;
return diffTemps;
}
void EncoderArticle(article* ptr)
{
 do{
      printf("\n entrer la reference de l'article:   ");
      scanf("%d", &(ptr->Reference));
   }while(((ptr->Reference)<1000) || ((ptr->Reference)>9999));

     fflush(stdin);
        printf("\n entrer la Designation:  ");
      gets(ptr->Designation);

 do{
      fflush(stdin);
      printf("\n entrer la categorie ( F ou H ):   ");
      scanf("%c",&(ptr->Categorie));
   }while(((ptr->Categorie)!='F') && ((ptr->Categorie)!='H'));
     fflush(stdin);
      printf("\n entrer le PrixHT:  ");
      scanf("%f",&(ptr->PrixHT));
   fflush(stdin);
   printf("\n entrer la taille S,M,L:  ");
   scanf("%d,%d,%d", &(ptr->Stock.TailleS), &(ptr->Stock.TailleM), &(ptr->Stock.TailleL));
 do{
// la date d'entrer en stock
      EncoderDate(&(ptr->Stock.DateEntree));
   }while(JoursEcoules(ptr->Stock.DateEntree)<0);//verifier que la date n'ai pas dans le futur
  printf("\n\nArticle en stock\n\n\n");
}
void AfficherArticle(article Art)
 {
    printf("\n\n---------------------------------------------\n");
    printf("Reference:%d\n",(Art.Reference));
    printf("Designation:%s\n",(Art.Designation));
    printf("Categorie: %c\n",(Art.Categorie));
    printf("Prix HT: %.2f\n",(Art.PrixHT));
    printf("Prix TTC: %.2f\n",((Art.PrixHT)*TVA)+(Art.PrixHT));
    printf("Date d'entree en stock: %d/%d/%d\n",(Art.Stock.DateEntree.Jour), (Art.Stock.DateEntree.Mois), (Art.Stock.DateEntree.Annee));
    printf("jours de stockage: %.0lf\n",JoursEcoules(Art.Stock.DateEntree));
    printf("Quantite disponible: Taille S= %d, Taille M= %d, Taille L= %d\n\n\n",(Art.Stock.TailleS), (Art.Stock.TailleM), (Art.Stock.TailleL));
 }


 //Labos 2.2 *****************************

void AjouterArticle(article Art,char fichier[])
{
    FILE *fic;
    fic=fopen(fichier,"ab");
    if(fic==NULL)
        printf("Erreur d'ouverture\n");
    else
    {
        printf("Fichier cree\n");
        EncoderArticle(&Art);
        fwrite(&Art,sizeof(article),1,fic);
        fclose(fic);
    }
}

int RecupererStock(article** ptr,char fichier[])
{
    int cb=-1;
    FILE *fic;
    fic=fopen(fichier,"rb");
    if(fic==NULL)
        printf("Erreur d'ouverture\n");
    else
    {
        cb=filelength(fileno(fic))/sizeof(article);
        *ptr=(article*)malloc(cb*sizeof(article));
        fread(*ptr,sizeof(article),cb,fic);
        fclose(fic);
    }
    return cb;
}

int SupprimerArticle(char fichier[])
{
    FILE* fic;
    int i,j,k=0; char choix;
    long int position, size;
    article ARTICLE;
    article* initial;
    article* _final;
    fic=fopen(fichier,"rb+");
    if(fic==NULL){
        printf("Erreur le fichier n'existe pas\n");
        return 1;
    }
    fseek(fic, 0, SEEK_END);
    size = ftell(fic);
    fseek(fic, 0, 0);
    initial=malloc(size);
    _final=malloc(size);

/**** Recherche de l'article ****/
    position=RechercherArticle(&ARTICLE,fic);
    fseek(fic,0,0);
    fread(initial,size,1,fic);
 if(position>-1)
 {

/**** affiche de l'article a supprimer ****/

    fseek(fic, position, 0);
    fread(&ARTICLE, sizeof(article), 1, fic);
    AfficherArticle(ARTICLE);

 do{
            printf("\nVoulez-vous le supprimer? 'O' ou 'N' : ");
            fflush(stdin);
            scanf("%c", &choix);
    switch(choix)
    {
        case 'O':
      /*** deplacer la tete de lecture dans le fichier au debut ***/
                fseek(fic,0,0);
                if(position == 0 && size == sizeof(article))
                {
                    fclose(fic);
                    fopen(fichier,"wb");//dans le cas ou il n'y a qu'un article dans le fichier
                    fclose(fic);
                }
                else
                {
                    j=position/sizeof(article);
                    for(i=0;i<size/sizeof(article);i++)
                    {
                        if(i != j)
                        {
                            _final[k]= initial[i];
                            k++;
                        }
                    }
                    fclose(fic);
                    fopen(fichier,"wb");
                    fseek(fic,0,0);
                    fwrite(_final,size-sizeof(article),1,fic);
                }

             break;
    }
 }while((choix!='N') && (choix!='O'));
 }
 else{
                return 1;
            }
        free(_final);
        free(initial);
        fclose(fic);
        return 0;
}

long RechercherArticle(article* ptr, FILE *fichier)
{
    int reference;
    fichier=fopen(FICHIER_STOCK,"r+b");

    if(fichier==NULL)
    {
        printf("Erreur d'ouverture");
    }
    else
    {
        printf("\n\nReference de l'article a rechercher: ");
        fflush(stdin); scanf("%d",&reference);
        ptr =(article*)malloc(sizeof(article));
        while(fread(ptr, sizeof(article), 1, fichier))
        {
            if((ptr->Reference) == reference)
            {
                return (ftell(fichier) - sizeof(article));
            }
        }

        printf("\n Article Introuvable !!!");
    }
    return -1;
}

void ModifierStock(char fichier[])
{
    FILE *fic;
    fic = fopen(FICHIER_STOCK, "r+b");
    article ptr;
    long position=-1;
    position=RechercherArticle(&ptr, fic);

    if (position >- 1)
    {
        fseek(fic, position, 0);
        fread(&ptr, sizeof(article), 1, fic);
        AfficherArticle(ptr);
        fflush(stdin);
        printf("\n Reencoder les quantites \n"
               " Taille S,M,L:  ");
        scanf("%d,%d,%d", &(ptr.Stock.TailleS), &(ptr.Stock.TailleM), &(ptr.Stock.TailleL));
        fseek(fic, position, 0);
        fwrite(&ptr, sizeof(article), 1, fic);
        fclose(fic);
    }
    else ;
}

// labos 2.3

void TrierDesignation(article *brut,article **tri,int  nbre)
{
    int i,k,j,lflag;

    for(i=0;i<nbre;i++)
        {
           // *(tri+i)=(brut+i);
            lflag=1;

            for(j=0;j<i;j++)
                {
                    if(strcmpi((brut+i)->Designation,((*(tri)+j))->Designation)<0)
                        {
                            for(k=i-1;k>=j;k--) *(tri+k+1)=*(tri+k);
                            *(tri+j)=(brut+i);
                            lflag=0;
                            j=i;
                        }
                }
            if(lflag) *(tri+i)=(brut+i);
        }
}

void TrierPrix(article *brut, article **tri,int nb)
{
     int i,k,j,lflag;

    for(i=0;i<nb;i++)
        {
            *(tri+i)=(brut+i);
            lflag=1;

            for(j=0;j<i;j++)
                {
                    if(((brut+i)->PrixHT)<((*(tri)+j)->PrixHT))
                        {
                            for(k=i-1;k>=j;k--) *(tri+k+1)=*(tri+k);
                            *(tri+j)=(brut+i);
                            lflag=0;
                            j=i;
                        }
                }
            if(lflag) *(tri+i)=(brut+i);
        }

}

void VoirArticle(article Art)
{
   printf("\n\n---------------------------------------------\n");
    printf("Reference:%d\n",(Art.Reference));
    printf("Designation:%s\n",(Art.Designation));
    printf("Prix TTC: %.2f\n",((Art.PrixHT)*TVA)+(Art.PrixHT));
    printf("Quantite disponible: Taille S= %d, Taille M= %d, Taille L= %d\n\n\n",(Art.Stock.TailleS), (Art.Stock.TailleM), (Art.Stock.TailleL));
}

// fonction labos 2.4

void AjouterArticlePanier(char fichier[], liste* liste_initiale)
{
    FILE *fic;
    fic = fopen(FICHIER_STOCK, "r+b");
    article ptr;
    long position=-1;
    char taille;
    position=RechercherArticle(&ptr, fic);fflush(stdin);

    printf("\n Entrer la taille de l'article: ");
    scanf("%c",&taille);fflush(stdin);
    if (position >- 1)
    {
        fseek(fic, position, 0);
        fread(&ptr, sizeof(article), 1, fic);
        switch(taille) {
            case 'L':
                if(ptr.Stock.TailleL > 0) {
                    ptr.Stock.TailleL -= 1;
                    fseek(fic, position, 0);
                    fwrite(&ptr, sizeof(article), 1, fic);
                    fclose(fic);

                    if(liste_initiale->premier->information.Reference == -10) {
                        liste_initiale->premier->information.Reference = ptr.Reference;
                        strcpy(liste_initiale->premier->information.Designation, ptr.Designation);
                        liste_initiale->premier->information.PrixHT = ptr.PrixHT;
                        liste_initiale->premier->information.Stock.TailleL = 1;
                        liste_initiale->premier->information.Stock.TailleS = 0;
                        liste_initiale->premier->information.Stock.TailleM = 0;
                    }
                    else{
                        element *cur_element = liste_initiale->premier;
                        int mise_a_jour_reussie = 0;
                        while(1 == 1) {
                            if(ptr.Reference == cur_element->information.Reference) {
                                if(cur_element->information.Stock.TailleL > 0) {
                                    cur_element->information.Stock.TailleL += 1;
                                    mise_a_jour_reussie = 1;
                                    break;
                                }
                            }
                            if(cur_element->suite != NULL) {
                                cur_element = cur_element->suite;
                            } else {
                                break;
                            }
                        }

                         // L'article qu'on veut ajouter n'est pas present dans le panier << Non vide >>
                        if(mise_a_jour_reussie == 0) {
                            element *nouvel_element = malloc(sizeof(element));
                            nouvel_element->information.Reference = ptr.Reference;
                            strcpy(nouvel_element->information.Designation, ptr.Designation);
                            nouvel_element->information.PrixHT = ptr.PrixHT;
                            nouvel_element->information.Stock.TailleL = 1;
                            nouvel_element->information.Stock.TailleS = 0;
                            nouvel_element->information.Stock.TailleM = 0;
                            nouvel_element->suite = liste_initiale->premier;
                            liste_initiale->premier = nouvel_element;
                        } break;
                    }
                } else {
                        printf("\n Article non disponible !");
                }
                break;
            case 'M':
                if(ptr.Stock.TailleM > 0) {
                    ptr.Stock.TailleM -= 1;
                    fseek(fic, position, 0);
                    fwrite(&ptr, sizeof(article), 1, fic);
                    fclose(fic);

                    if(liste_initiale->premier->information.Reference == -10) {
                        liste_initiale->premier->information.Reference = ptr.Reference;
                        strcpy(liste_initiale->premier->information.Designation, ptr.Designation);
                        liste_initiale->premier->information.PrixHT = ptr.PrixHT;
                        liste_initiale->premier->information.Stock.TailleM = 1;
                        liste_initiale->premier->information.Stock.TailleS = 0;
                        liste_initiale->premier->information.Stock.TailleL = 0;
                    }
                    else{
                        element *cur_element = liste_initiale->premier;
                        int mise_a_jour_reussie = 0;
                        while(1 == 1) {
                            if(ptr.Reference == cur_element->information.Reference) {
                                if(cur_element->information.Stock.TailleM > 0) {
                                    cur_element->information.Stock.TailleM += 1;
                                    mise_a_jour_reussie = 1;
                                    break;
                                }
                            }
                            if(cur_element->suite != NULL) {
                                cur_element = cur_element->suite;
                            } else {
                                break;
                            }
                        }

                         // L'article qu'on veut ajouter n'est pas present dans le panier << Non vide >>
                        if(mise_a_jour_reussie == 0) {
                            element *nouvel_element = malloc(sizeof(element));
                            nouvel_element->information.Reference = ptr.Reference;
                            strcpy(nouvel_element->information.Designation, ptr.Designation);
                            nouvel_element->information.PrixHT = ptr.PrixHT;
                            nouvel_element->information.Stock.TailleM = 1;
                            nouvel_element->information.Stock.TailleS = 0;
                            nouvel_element->information.Stock.TailleL = 0;
                            nouvel_element->suite = liste_initiale->premier;
                            liste_initiale->premier = nouvel_element;
                        } break;
                    }
                } else {
                        printf("\n Article non disponible !");
                }
                break;
            case 'S':
                if(ptr.Stock.TailleS > 0) {
                    ptr.Stock.TailleS -= 1;
                    fseek(fic, position, 0);
                    fwrite(&ptr, sizeof(article), 1, fic);
                    fclose(fic);

                    if(liste_initiale->premier->information.Reference == -10) {
                        liste_initiale->premier->information.Reference = ptr.Reference;
                        strcpy(liste_initiale->premier->information.Designation, ptr.Designation);
                        liste_initiale->premier->information.PrixHT = ptr.PrixHT;
                        liste_initiale->premier->information.Stock.TailleS = 1;
                        liste_initiale->premier->information.Stock.TailleL = 0;
                        liste_initiale->premier->information.Stock.TailleM = 0;
                    }
                    else{
                        element *cur_element = liste_initiale->premier;
                        int mise_a_jour_reussie = 0;
                        while(1 == 1) {
                            if(ptr.Reference == cur_element->information.Reference) {
                                if(cur_element->information.Stock.TailleS > 0) {
                                    cur_element->information.Stock.TailleS += 1;
                                    mise_a_jour_reussie = 1;
                                    break;
                                }
                            }
                            if(cur_element->suite != NULL) {
                                cur_element = cur_element->suite;
                            } else {
                                break;
                            }
                        }

                         // L'article qu'on veut ajouter n'est pas present dans le panier << Non vide >>
                        if(mise_a_jour_reussie == 0) {
                            element *nouvel_element = malloc(sizeof(element));
                            nouvel_element->information.Reference = ptr.Reference;
                            strcpy(nouvel_element->information.Designation, ptr.Designation);
                            nouvel_element->information.PrixHT = ptr.PrixHT;
                            nouvel_element->information.Stock.TailleS = 1;
                            nouvel_element->information.Stock.TailleM = 0;
                            nouvel_element->information.Stock.TailleL = 0;
                            nouvel_element->suite = liste_initiale->premier;
                            liste_initiale->premier = nouvel_element;
                        } break;
                    }
                } else {
                    printf("\n Article non disponible !");
                }
                break;
            }
        }
}

void VoirPanier(liste *liste_initiale)
{
    printf("Votre panier\n-------------\n");
    printf("Reference                      Designation                  Taille          Quantite          Prix\n");

    element *element_courant = liste_initiale->premier;

    if(element_courant->information.Reference != -10) {
        while(1 == 1) {

            char taille = 'Z';
            int qte = 0;

            if(element_courant->information.Stock.TailleS > 0 ) {;
                taille = 'S'; qte = element_courant->information.Stock.TailleS;
            } else if(element_courant->information.Stock.TailleM > 0 ) {
                taille = 'M'; qte = element_courant->information.Stock.TailleM;
            } else if(element_courant->information.Stock.TailleL > 0 ) {
                taille = 'L'; qte = element_courant->information.Stock.TailleL;
            }

            printf("%0d %34s %21c %15d %22.2f\n", element_courant->information.Reference, element_courant->information.Designation, taille, qte, ((((element_courant->information.PrixHT)*TVA)+(element_courant->information.PrixHT))*qte));

            if(element_courant->suite == NULL){
                break;
            } else {
                element_courant = element_courant->suite;
            }
        }
    } else {
        printf("\nCe panier est vide, RIEN a Afficher !");
    }

}

    /*
    void AjouterArticlePanier(char fichier[], liste* ptr_debut)
    {
         int trouve=0;
        liste *inserer, *parcours, *precedent;
        parcours= ptr_debut;
        precedent=NULL;
        inserer= (liste*)malloc(sizeof(liste));
    //    EncoderInfo(&(inserer->information));
        while (parcours!=NULL && !trouve)
        {
            if(inserer->information.Reference == parcours->information.Reference)
                trouve++;
            else
            {
                precedent=parcours;
                parcours=parcours->suite;
            }
        }

    inserer->suite = parcours;
    if(precedent==NULL)
        *ptr_debut=*inserer;
    else
        precedent->suite=inserer;
    return;
}

void VoirPanier (liste lis)
{

}

void SupprimerArticlePanier(char fichier[], liste** ptr_debut)
{
    int trouve=0;
    int supprimer;
    liste *parcours, *precedent;
    parcours= *ptr_debut;
    precedent=NULL;
    printf("Reference: ");
    fflush(stdin); scanf("%d", &supprimer);
    while (parcours!=NULL && !trouve)
    {
        if(supprimer == parcours->information.Reference)
            parcours=NULL;
        else
        {
            precedent=parcours;
            parcours=parcours->suite;
        }
    }
    if(trouve)
    {
        if(precedent==NULL)
            *ptr_debut=parcours->suite;
        else
            precedent->suite=parcours->suite;
        free(parcours);
    }
    return;
}

void EcrireBonDeCommande(char fichier[], liste *ptr)
{

}*/
