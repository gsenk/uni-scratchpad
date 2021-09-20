// make .plt files for ploting
#include <stdio.h>
#include <stdlib.h>

#define TIME 50
int main(){
  int i;
  FILE *vn = fopen("plot.plt", "wt");

  fprintf(vn, "reset\n");
  fprintf(vn, "set key right top\n");
  fprintf(vn, "set zrange[-10:10]\n");
  for (i=0; i<=TIME; i++){
    fprintf(vn, "\n");
    fprintf(vn, "\n");
    fprintf(vn, "\n");
    fprintf(vn, "\n");
    fprintf(vn, "\n");
    fprintf(vn, "\n");
    fprintf(vn, "set terminal png size 600,600\n");
    fprintf(vn, "set output \"graf_E%d.png\"\n", i);
    fprintf(vn, "\n");
    fprintf(vn, "set autoscale xy\n");
    fprintf(vn, "set xlabel \"x\"\n");
    fprintf(vn, "set ylabel \"z\"\n");
    fprintf(vn, "\n");
    fprintf(vn, "set pm3d map\n");
    fprintf(vn, "set title \"E time = %d\"\n", i);
    fprintf(vn, "splot \"Maxwell_PMLE%d.txt\" title\"\"\n", i);
    fprintf(vn, "\n");
    fprintf(vn, "set terminal windows\n");
    fprintf(vn, "replot\n");
    fprintf(vn, "\n");
    fprintf(vn, "\n");
    fprintf(vn, "\n");
    fprintf(vn, "set terminal png size 600,600\n");
    fprintf(vn, "set output \"graf_B%d.png\"\n", i);
    fprintf(vn, "\n");
    fprintf(vn, "set autoscale xy\n");
    fprintf(vn, "set xlabel \"x\"\n");
    fprintf(vn, "set ylabel \"z\"\n");
    fprintf(vn, "\n");
    fprintf(vn, "set pm3d map\n");
    fprintf(vn, "set title \"B time = %d\"\n", i);
    fprintf(vn, "splot \"Maxwell_PMLB%d.txt\" title\"\"\n", i);
    fprintf(vn, "\n");
    fprintf(vn, "set terminal windows\n");
    fprintf(vn, "replot\n");
  }
  return 0;
}