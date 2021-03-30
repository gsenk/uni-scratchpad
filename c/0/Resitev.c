#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_VRSTIC 1000
#define MAX_STEVIL 1000 		// v vsaki vrstici
#define MAX_ZNAKOV 10000		// v vsaki vrstici
#define EPSILON 0.00000000001 	// 10E-12 kdaj lahko recemo da sta dve double stevili enaki


int main(int argc, char *argv[])
{
	FILE *dat = fopen (argv[1], "rt");
	if (dat == NULL) return 1;

	int st_vrstic=0, max_vrstic=MAX_VRSTIC, max_argumentov=MAX_STEVIL, trenutna_vrstica=0;
	double* arr_stevil=calloc(max_vrstic*max_argumentov, sizeof(double));
	char arr_besed[1000][100];

	char vrstica1[MAX_ZNAKOV];
	int offset=0;

	// preberem in prvo shranim besedo, potem se pomikam po nizu znakov in shranjujem �e stevilke
	while (1)
	{
		char *str=fgets(vrstica1, MAX_ZNAKOV, dat);
		if (str==NULL) break;
		char *vrstica=vrstica1;

		// shranim samo prvo besedo, ne rabim funkcije ker je samo ena, offset mi pove koliko znakov je beseda dolga
		sscanf(vrstica, "%s%n", &arr_besed[trenutna_vrstica], &offset);
		vrstica=vrstica+offset;
		int i=0;

		// pomikam se za offset, saj ssscanf zazna le prvo stevilo
		while (1)
		{
			// %lf je potreben za branje double stevil
			sscanf(vrstica, "%lf%n", &arr_stevil[trenutna_vrstica*max_argumentov+i], &offset);
			vrstica=vrstica+offset;
			if (fabs(arr_stevil[trenutna_vrstica*max_argumentov+i]+99999) < EPSILON) break;
			i=i+1;
		}
		trenutna_vrstica=trenutna_vrstica+1;
	}
	st_vrstic=trenutna_vrstica;
	double vhod, st; 				// stevilka ki jo vnesemo
	int max_score, score;			// score se gleda 1 ce je dovolj blizu, 0 ce ni, visji je ce je vec takih stevil
	char *rezultat, *prvi_element;	// rezultat je najbljizji prvi element
	int mesto_v_vrstici=0;

	while (1)
	{
		printf("\nVpisi neko realno stevilko, z 0 prekines vnasanje\n");
		scanf("%lf", &vhod);				// shranim podano stevilko
		if (fabs(vhod) < EPSILON) break; 	// ker == ne deluje pri double
		rezultat = "ni ujemanj";
		max_score=0;
		for (trenutna_vrstica=0; trenutna_vrstica < st_vrstic; trenutna_vrstica++)
		{
			mesto_v_vrstici=0;  			// za novo vrstico je potreben reset parametrov
			score=0;
			while(1)
			{
				st=arr_stevil[trenutna_vrstica*max_argumentov+mesto_v_vrstici];		// bolj berljivo
				if (fabs((st/vhod-1)*50)<=1) score=score+1;							// pogoj da je stevilka razliclna za manj ali enako 2%
				if (fabs(st+99999)<EPSILON) break;
				mesto_v_vrstici++;
			}
			if (score>max_score)	// ce je podana vrstica najblji�je
			{
				max_score=score;
				rezultat=arr_besed[trenutna_vrstica];
			}
		}

		printf("%s\n\n", rezultat);
	}
	fclose(dat);					// izognemo se mogocemu po�kodovanju datoteke
	return 0;
}