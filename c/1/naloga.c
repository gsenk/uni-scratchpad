#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_IME 500
#define MAX_PICTURES 1000

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

struct PNG_FILE								// na� database
{
	char *name;
	int x;
	int y;
} png_files[MAX_PICTURES];

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Razclenimo za vsak del posebej za la�ji opis PNG formata.
int IS_PNG_FILE(char *ime, int k)
{
	FILE *dat = fopen (ime, "rt");			// odpremo kot tekstovno datoteko za branje, r=branje, w=pisanje;
											// t=tekstovna, b=binarna; ce dodamo na konec + odpremo datoteko znamenom posodabljanja
	if (dat == NULL) return 1;

	int i, data_point;
	int is_png[]={137,80,78,71,13,10,26,10};// Decimalni zapis magicnega �tevila pri formatu PNG
	for (i=0; i<8; i++)
	{
		fscanf(dat, "%02x", &data_point);	// vsako heksagonalno stevilo zasede dve polji
		if ((data_point-is_png[i])!=0)		// primerjamo tabele, ce sta enaki, z 0 lahko enacimo, saj sta oba naravni stevili
		{
			printf("File %s in not of PNG format!", ime);
			return 2;
		}
	}

	for (i=0; i<4; i++) 					// skocimo cez kjer je zapisana dolzina bloka
	{
		fscanf(dat, "%02x", &data_point);
	}
	int is_ihdr[]={73,72,68,82};
	for (i=0; i<4; i++)						// preverimo IHDR flag
	{
		fscanf(dat, "%02x", &data_point);
		if ((data_point-is_ihdr[i])!=0)
		{
			printf("File %s in not of PNG format!", ime);
			return 2;
		}
	}

	int sirina, visina;						// razberemo �e vi�ino in �irino
	fscanf(dat, "%08x", &sirina);
	fscanf(dat, "%08x", &visina);
	//printf("sirina je %d, visina pa %d\n", sirina, visina);


											// preverimo �e konec datoteke
	int konec[8], koncni_st, pravi_konec[]={73,69,78,68,174,66,96,130};
	while(fscanf(dat, "%02x", &koncni_st)!=EOF)
	{
		for (i=0;i<7;i++)
		{
			konec[i]=konec[i+1];
		}
		konec[7]=koncni_st;

	}
	for (i=0; i<8; i++)
	{
		if ((konec[i]-pravi_konec[i])!=0)
		{
			printf("File %s in not of PNG format!", ime);
			return 2;
		}
	}


	png_files[k].name=strdup(ime);				// ce je vse vredu zapi�emo vrednost v struct, NUJNO strdup, saj drugace shrani le pointer
	png_files[k].x=sirina;
	png_files[k].y=visina;

	fclose(dat);
	return 0;									// flag avtenticnosti PNG datoteke
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void CHOP_NEWLINE(char *ime)				// Vsaka prebrana vrstica ima na koncu znak za novo vrstico, nadomestimo
{											// ga z znakom za konec stringa, saj se tako program ustavi ko bere besedo
	int len = strlen(ime);
	if(ime[len-1] == '\n')
		ime[len-1] = '\0';
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int main(int argc, char *argv[])
{
	FILE *dat = fopen (argv[1], "rt");
	if (dat == NULL) return 1;

	int i, st_vrstic = 0, st_png=0;
	char vrstica[MAX_IME];


	while (1)							// Prvo preberem na�po datoteko ki vsebuje imena slik
	{
		if (fgets(vrstica, MAX_IME, dat) == NULL) break;
		CHOP_NEWLINE(vrstica); 			// stringu moramo odstraniti newline character
		//printf("%s\n", vrstica);
		if (IS_PNG_FILE(vrstica, st_png) == 0)
		{
			printf("Nasa slika po imenu %s je siroka %d in visoka %d pikslov.\n", png_files[st_png].name, png_files[st_png].x, png_files[st_png].y);
			st_png++;
		}

		printf("\n");
		st_vrstic++;
	}
										// statistika datotek
	printf("st png filov %d, st vrstic %d\n", st_png, st_vrstic-1);

	while(1)							// sedaj omogocimo poizvedbo o datotekah
	{
		char ime_poizvedbe[500];
		printf("Vnesi kater file bi rad pogledal, ne pozabi na koncnico! 0 konca proces\n");
		scanf("%s", &ime_poizvedbe);
		//printf("%s\n", ime_poizvedbe);
		if (strcmp(ime_poizvedbe,"0")==0) break;

		for (i=0; i<=st_png; i++)
		{
			if (i==st_png) printf("Ta datoteka ne obstaja oziroma ne ustreza zakonitostim PNG formata, oz NULL\n");
			else if (strcmp((png_files[i].name),ime_poizvedbe)==0)
			{
				printf("Iskali ste %s, njegove dimenzije so je %d x %d\n", ime_poizvedbe, png_files[i].x, png_files[i].y);
				break;
			}
		}


	}
	fclose(dat);
	return 0;
}