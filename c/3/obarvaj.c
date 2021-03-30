#include <stdio.h>
#include <stdlib.h>

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define MAX_VRSTICA 999

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int* PREBERI_FILE(char *ime, int dimenzije[3])
{
  int *place = malloc(sizeof(int));
  *place=1;
  FILE *pic = fopen (ime, "rt");
  if (pic == NULL) return place;

  int num_stevilk=0, v_stevilki=0, stevka=0, stevilo, index;  // potrebujemo 4 stevilke, prva pride z P2, druga �irina,
                // tretja vi�ina, cetrta navi�ja barva predvidevamo da so vrstice dejanske slike po vrsticah locene od teh podatkov
  char stevilka[]={"0"}, vrstica[MAX_VRSTICA], znak;          // najvecja stevilka je vi�ina barve, predpostavimo da je najvec trimestna
  while(num_stevilk<4)
  {
    if (fgets(vrstica, MAX_VRSTICA, pic) == NULL) break;
                                                        // preverjamo na�o vrstico za �tevilke
    index = 0;
    while(1){
      sscanf(&vrstica[index], "%c", &znak);
      // te pogoje ustrezni �tevilki bi lahko dali v svojo funkcijo
      if((v_stevilki == 1) && (znak <= '0' || znak >= '9'))
      {                         // pomeni: smo v �tevilki, a je je konec
        stevilka[stevka] = '\0';
        v_stevilki = 0;
        stevka = 0;
        num_stevilk++;
        if (num_stevilk > 1)    // nocemo shranit �tevilke s P2
        {
          sscanf(stevilka, "%d", &dimenzije[num_stevilk-2]);
        }
      }
      if (znak == '#' || znak == '\n')
      {                         // pri�li smo na konec uporabne vrstice
        v_stevilki = 0;
        stevka = 0;
        break;
      }
      if (znak >= '0' && znak <= '9')
      {                         // naslednji znak je stevka, oznacimo da smo v �tevilu
        stevilka[stevka] = znak;
        v_stevilki = 1;
        stevka++;
      }
      index++;
    }
  }
  int *slika=calloc(dimenzije[0]*dimenzije[1], sizeof(int));  // shranim na�i sliko, katero  potem modificiram
  stevka=0;
  while(fscanf(pic, "%d", &stevilo) != EOF)
  {
    slika[stevka]=stevilo;
    stevka++;
  }
  fclose(pic);
  return slika;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ZAPISI_FILE(char *ime, int *slika, int dimenzije[3])
{
  FILE *pic = fopen (ime, "wt");
  if (pic == NULL) return;

  fprintf(pic, "P2\n# pobarvano z mojim programom\n%d %d %d\n", dimenzije[0], dimenzije[1], dimenzije[2]);
  int i, velikost=dimenzije[0]*dimenzije[1];
  for (i = 0; i < velikost; i++)
  {
    fprintf(pic, "%d ", slika[i]);
    if (((i+1)%dimenzije[0]) == 0) fprintf(pic, "\n");
  }
  fclose(pic);
  return;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void PREBARVAJ(int *slika, int dimenzije[3], int nova, int original, int x, int y)
{
  if (slika[dimenzije[0]*y+x] == original)
  {
    slika[dimenzije[0]*y+x] = nova;
    if ((x+1) <= dimenzije[0]) PREBARVAJ(slika, dimenzije, nova, original, x+1, y);
    if ((x-1) >= 0) PREBARVAJ(slika, dimenzije, nova, original, x-1, y);
    if ((y+1) <= dimenzije[1]) PREBARVAJ(slika, dimenzije, nova, original, x, y+1);
    if ((y-1) >= 0) PREBARVAJ(slika, dimenzije, nova, original, x, y-1);
    return;
  }
  return;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int main(int argc, char *argv[])
{
  if (argc != 6)
  {
    printf("Vnesel si napacno stevilo spremenljivk\n");
    return 1;
  }

  int tocka_x=((*argv[3])-'0'), tocka_y=((*argv[4])-'0'), nova_barva, stara_barva;
  sscanf((argv[5]), "%d", &nova_barva);
  int dimenzije[3];           // dimenzije si seledijo: �irina, vi�ina, maksimalna_barva

  int *slika = PREBERI_FILE(argv[1], dimenzije);
  if (*slika == 1)
  {
    printf("Nisem mogel prebrati vhodne datoteke");
    return 0;
  }
  stara_barva=slika[dimenzije[0]*tocka_y+tocka_x];
  PREBARVAJ(slika, dimenzije, nova_barva, stara_barva, tocka_x, tocka_y);
  ZAPISI_FILE(argv[2], slika, dimenzije);
  return 0;
}