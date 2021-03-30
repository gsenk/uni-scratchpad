#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define MAX_STUDENT 250       // max stevilos tudentev
#define MAX_OCEN 12         // stevilo cen v datoteki ocena1.txt
#define DOLZ_STUD_STEV 12     // dol�ina studentskih �tevilk v datoteki ocena1.txt
#define ST_PARAM_STUDENT1 2     // �tevilo �tevilk v vrstici za izpis v datoteki ocena2.txt
#define MAX_DOLZ_OCENE (2+1)    // dol�ine ocene za zapis v string+1 za znak '\0'
#define NI_DOBRIH_NALOG 20      // neka �tevilka vi�ja od �tevila nalog, saj bo tako sort deloval pravilno

#define ST_PARAM_STUDENT2 3     // stevilo stevilk v vrstici za vhod v datoteki ocena2.txt
#define ST_PARAM_STUDENT3 2     // �tevilo parametrov za zapis v ocena3.txt dobljenih z datoteke ocene1.txt

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// izbere random nalogo ocenjeno z 0 ali 1
void RAND_NALOGA(char *ocene, int *studenti, int student)
{
  int naloga, i, st_dobrih=0;
  while(1)                // �elimo zbirati random nalogo dokler ne dobimo take ki ustreza kriterijem
  {
    naloga = (rand() % MAX_OCEN);     // stevilka med 0 in 11
    for (i = 0; i < MAX_OCEN; i++)    // hocemo preveriti ce obtajajo ugodne re�itve
    {
      if ((ocene[MAX_OCEN*MAX_DOLZ_OCENE*student+i*MAX_DOLZ_OCENE] == '0') || (ocene[MAX_OCEN*MAX_DOLZ_OCENE*student+i*MAX_DOLZ_OCENE] == '1'))
      {
        st_dobrih++;
        break;
      }
    }
    if (st_dobrih==0)
    {
      studenti[student*ST_PARAM_STUDENT1+1]=NI_DOBRIH_NALOG;
      break;
    }
    // ce obstajajo ugodne re�itve, zacnemo primerjat iz�rebano nalogo z ugodnimi
    else if ((ocene[MAX_OCEN*MAX_DOLZ_OCENE*student+naloga*MAX_DOLZ_OCENE] == '0') || (ocene[MAX_OCEN*MAX_DOLZ_OCENE*student+naloga*MAX_DOLZ_OCENE] == '1'))
    {
      studenti[student*ST_PARAM_STUDENT1+1]=(naloga+1);
      break;
    }
  }
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// posortiramo 2D array, na� #define NI_DOBRIH_NALOG nam omogoca sortiranje tudi teh, ki so ne�tevilse,
// saj jim pripi�emo vrednost vi�jo od mogoci� pri nalogah
void SelectionSort(int *a, int array_size)
{
     int i;
     for (i = 0; i < array_size - 1; i++)
     {
          int j, min, temp[ST_PARAM_STUDENT1];
          min = i;
          for (j = i+1; j < array_size; j++)
          {
               if (a[j*ST_PARAM_STUDENT1+1] < a[min*ST_PARAM_STUDENT1+1])
                    min = j;
          }

          temp[0] = a[i*ST_PARAM_STUDENT1];
          temp[1] = a[i*ST_PARAM_STUDENT1+1];
          a[i*ST_PARAM_STUDENT1] = a[min*ST_PARAM_STUDENT1];
          a[i*ST_PARAM_STUDENT1+1] = a[min*ST_PARAM_STUDENT1+1];
          a[min*ST_PARAM_STUDENT1] = temp[0];
          a[min*ST_PARAM_STUDENT1+1] = temp[1];
     }
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int main(int argc, char *argv[])
{
  if (argc == 3)
  {
    FILE *ocene1 = fopen (argv[1], "rt");
    if (ocene1 == NULL) return 1;

    int student=0;                              // �tevilo vrstic v vhodni datoteki
    int *studenti=calloc(MAX_STUDENT*ST_PARAM_STUDENT1, sizeof(int));   // shranim podatke za izpis
    char *grades=calloc(MAX_STUDENT*MAX_OCEN*MAX_DOLZ_OCENE, sizeof(char)); // shranim pdoatke z datoteke ocene1.txt
    srand (time(NULL));

    char vrstica[DOLZ_STUD_STEV+MAX_OCEN*MAX_DOLZ_OCENE*2];
    while(fgets(vrstica, sizeof(vrstica), ocene1) != NULL)
    {
        // ker je format trdno dolocen lahko to naredimo kar naenkrat, druga mo�nost je znaka for
      sscanf(vrstica, "%d %s %s %s %s %s %s %s %s %s %s %s %s", &studenti[student*ST_PARAM_STUDENT1],
        &grades[MAX_OCEN*MAX_DOLZ_OCENE*student+0*MAX_DOLZ_OCENE], &grades[MAX_OCEN*MAX_DOLZ_OCENE*student+1*MAX_DOLZ_OCENE], &grades[MAX_OCEN*MAX_DOLZ_OCENE*student+2*MAX_DOLZ_OCENE],
        &grades[MAX_OCEN*MAX_DOLZ_OCENE*student+3*MAX_DOLZ_OCENE], &grades[MAX_OCEN*MAX_DOLZ_OCENE*student+4*MAX_DOLZ_OCENE], &grades[MAX_OCEN*MAX_DOLZ_OCENE*student+5*MAX_DOLZ_OCENE],
        &grades[MAX_OCEN*MAX_DOLZ_OCENE*student+6*MAX_DOLZ_OCENE], &grades[MAX_OCEN*MAX_DOLZ_OCENE*student+7*MAX_DOLZ_OCENE], &grades[MAX_OCEN*MAX_DOLZ_OCENE*student+8*MAX_DOLZ_OCENE],
        &grades[MAX_OCEN*MAX_DOLZ_OCENE*student+9*MAX_DOLZ_OCENE], &grades[MAX_OCEN*MAX_DOLZ_OCENE*student+10*MAX_DOLZ_OCENE], &grades[MAX_OCEN*MAX_DOLZ_OCENE*student+11*MAX_DOLZ_OCENE]);

      RAND_NALOGA(grades, studenti, student);
      fflush(stdin);
      student++;
    }
    fclose(ocene1);

    SelectionSort(studenti, student);

    FILE *ocene2 = fopen (argv[2], "wt");
    if (ocene2 == NULL) return 1;
    char ocen[3];
    int i;
    for (i = 0; i < student; i++)
    {
      if (studenti[ST_PARAM_STUDENT1*i+1] == NI_DOBRIH_NALOG)
      {
        strcpy(ocen, "-");
      }
      else
      {
        sprintf(ocen, "%d", studenti[ST_PARAM_STUDENT1*i+1]);
      }
      fprintf(ocene2, "%d %s\n", studenti[ST_PARAM_STUDENT1*i], ocen);

    }
    fclose(ocene2);
    return 0;
  }

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  else if (argc == 4)
  {
    FILE *ocene1 = fopen (argv[1], "rt");
    if (ocene1 == NULL) return 1;
    FILE *ocene2 = fopen (argv[2], "rt");
    if (ocene2 == NULL) return 1;
    FILE *ocene3 = fopen (argv[3], "wt");
    if (ocene3 == NULL) return 1;

    int student=0, i, ocena;                // �tevilo vrstic v vhodni datoteki
    int *studenti=calloc(MAX_STUDENT*ST_PARAM_STUDENT3, sizeof(int));   // shranim podatke za izpis
    char *grades=calloc(MAX_STUDENT*MAX_OCEN*MAX_DOLZ_OCENE, sizeof(char)); // shranim pdoatke z datoteke ocene1.txt

    char vrstica[DOLZ_STUD_STEV+MAX_OCEN*MAX_DOLZ_OCENE];

    while(fgets(vrstica, sizeof(vrstica), ocene1) != NULL)
    {
        // ker je format trdno dolocen lahko to naredimo kar naenkrat, druga mo�nost je znaka for
      sscanf(vrstica, "%d %s %s %s %s %s %s %s %s %s %s %s %s", &studenti[student*ST_PARAM_STUDENT3],
        &grades[MAX_OCEN*MAX_DOLZ_OCENE*student+0*MAX_DOLZ_OCENE], &grades[MAX_OCEN*MAX_DOLZ_OCENE*student+1*MAX_DOLZ_OCENE], &grades[MAX_OCEN*MAX_DOLZ_OCENE*student+2*MAX_DOLZ_OCENE],
        &grades[MAX_OCEN*MAX_DOLZ_OCENE*student+3*MAX_DOLZ_OCENE], &grades[MAX_OCEN*MAX_DOLZ_OCENE*student+4*MAX_DOLZ_OCENE], &grades[MAX_OCEN*MAX_DOLZ_OCENE*student+5*MAX_DOLZ_OCENE],
        &grades[MAX_OCEN*MAX_DOLZ_OCENE*student+6*MAX_DOLZ_OCENE], &grades[MAX_OCEN*MAX_DOLZ_OCENE*student+7*MAX_DOLZ_OCENE], &grades[MAX_OCEN*MAX_DOLZ_OCENE*student+8*MAX_DOLZ_OCENE],
        &grades[MAX_OCEN*MAX_DOLZ_OCENE*student+9*MAX_DOLZ_OCENE], &grades[MAX_OCEN*MAX_DOLZ_OCENE*student+10*MAX_DOLZ_OCENE], &grades[MAX_OCEN*MAX_DOLZ_OCENE*student+11*MAX_DOLZ_OCENE]);

      ocena=0;
      for (i=0; i<MAX_OCEN; i++)
      {
        ocena = ocena + atoi(&grades[MAX_OCEN*MAX_DOLZ_OCENE*student+i*MAX_DOLZ_OCENE]);
        if (ocena>=10) break;
      }
      studenti[student*ST_PARAM_STUDENT3+1]=ocena;
      fflush(stdin);
      student++;
    }

    student=0;
    char vrstica1[DOLZ_STUD_STEV+ST_PARAM_STUDENT2*MAX_DOLZ_OCENE];
    int *studenti1=calloc(MAX_STUDENT*(ST_PARAM_STUDENT2-1), sizeof(int));    // shranim podatke za izpis
    char *grades1=calloc(MAX_STUDENT*MAX_DOLZ_OCENE, sizeof(char));       // shranim pdoatke z datoteke ocene1.txt
    while(fgets(vrstica1, sizeof(vrstica1), ocene2) != NULL)
    {
        // ker je format trdno dolocen lahko to naredimo kar naenkrat, druga mo�nost je znaka for
      sscanf(vrstica1, "%d %s %d", &studenti1[student*(ST_PARAM_STUDENT2-1)], &grades1[student*MAX_DOLZ_OCENE], &studenti1[student*(ST_PARAM_STUDENT2-1)+1]);

      fflush(stdin);
      student++;
    }

    int j;
    for (i=0; i<student; i++)
    {
      for (j=0; j<student; j++){
        if (studenti[i*ST_PARAM_STUDENT3]==studenti1[j*(ST_PARAM_STUDENT2-1)]) break;
      }
      fprintf(ocene3, "%d %d %d(%s) %d\n", studenti[i*ST_PARAM_STUDENT3], studenti[i*ST_PARAM_STUDENT3+1],
      studenti1[j*(ST_PARAM_STUDENT2-1)+1], &grades1[j*MAX_DOLZ_OCENE], studenti[i*ST_PARAM_STUDENT3+1] + studenti1[j*(ST_PARAM_STUDENT2-1)+1]);

    }

    fclose(ocene1);
    fclose(ocene2);
    fclose(ocene3);
    return 0;
  }

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  else
  {
    printf("Napacno �tevilo parametrov");
    return 0;
  }
}
