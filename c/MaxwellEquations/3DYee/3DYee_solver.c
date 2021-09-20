// we write a 3D yee solver on FDTD grid with a source current j_z(x,t)=\sigma E_y(x,t)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TIME 50
#define MAX 98
#define SIGMA 15

/////////////////////////////////////////////////////////////////////////////////////////
//
// initial gauss condition with sine function in x direction
//
/////////////////////////////////////////////////////////////////////////////////////////
void initial_gauss3D(double *ValX, double *ValY, double *ValZ, int X, int Y, int Z, double sigma_x, double sigma_y, double sigma_z, double resolution, double delay){
  int z,y,x;
  for (z=0; z<Z; z++){
    for (y=0; y<Y; y++){
      for (x=0; x<X; x++){
        ValX[Y*X*z+X*y+x]=10*exp(-((z+delay-Z/2.0)*(z+delay-Z/2.0)/(resolution*resolution)+(y+delay-Y/2.0)*(y+delay-Y/2.0)/(resolution*resolution)+(x+delay-X/2.0)*(x+delay-X/2.0)/(resolution*resolution))/(sigma_x*sigma_x))*sin(x/(double)resolution/4);
        ValY[Y*X*z+X*y+x]=10*exp(-((z+delay-Z/2.0)*(z+delay-Z/2.0)/(resolution*resolution)+(y+delay-Y/2.0)*(y+delay-Y/2.0)/(resolution*resolution)+(x+delay-X/2.0)*(x+delay-X/2.0)/(resolution*resolution))/(sigma_y*sigma_y))*sin(x/(double)resolution/4);
        ValZ[Y*X*z+X*y+x]=10*exp(-((z+delay-Z/2.0)*(z+delay-Z/2.0)/(resolution*resolution)+(y+delay-Y/2.0)*(y+delay-Y/2.0)/(resolution*resolution)+(x+delay-X/2.0)*(x+delay-X/2.0)/(resolution*resolution))/(sigma_z*sigma_z))*sin(x/(double)resolution/4);
      }
    }
  }
}
/////////////////////////////////////////////////////////////////////////////////////////
//
// if you search 50 element on right is in index 49, so second last is 48.
//
/////////////////////////////////////////////////////////////////////////////////////////
// first on left is the second last on right, and last on right is the same as second on left.
void exchange_x(double *Val, int X, int Y, int Z){
  int z, y;
  for (z=1; z<(Z-1); z++){
    for (y=1; y<(Y-1); y++){
      Val[Y*X*z+X*y+0]=Val[Y*X*z+X*y+(X-2)];
      Val[Y*X*z+X*y+(X-1)]=Val[Y*X*z+X*y+1];
    }
  }
}
// first in the back is the second last in front, and last in front is the same as second in the back.
void exchange_y(double *Val, int X, int Y, int Z){
  int z, x;
  for (z=1; z<(Z-1); z++){
    for (x=1; x<(X-1); x++){
      Val[Y*X*z+X*0+x]=Val[Y*X*z+X*(Y-2)+x];
      Val[Y*X*z+X*(Y-1)+x]=Val[Y*X*z+X*1+x];
    }
  }
}
// first on bottom is the second last on top, and last on top is the same as second on bottom.
void exchange_z(double *Val, int X, int Y, int Z){
  int y, x;
  for (y=1; y<(Y-1); y++){
    for (x=1; x<(X-1); x++){
      Val[Y*X*0+X*y+x]=Val[Y*X*(Z-2)+X*y+x];
      Val[Y*X*(Z-1)+X*y+x]=Val[Y*X*1+X*y+x];
    }
  }
}
/////////////////////////////////////////////////////////////////////////////////////////
//
// start main function
//
/////////////////////////////////////////////////////////////////////////////////////////
int main(){
  int K=MAX, J=MAX, I=MAX, k, j, i, buffer=1, t;

// delta has to be around 0.5, because of dispersion
  int resolution=1;
  double delta_x=1/(double)resolution, delta_y=1/(double)resolution, delta_z=1/(double)resolution;
  double delta_t=sqrt(delta_x*delta_x+delta_y*delta_y+delta_z*delta_z), delta=0.65;//delta=0.5*delta_t;
  int K_max=(K*resolution+2*buffer), J_max=(J*resolution+2*buffer), I_max=(I*resolution+2*buffer);

// declare arrays
  double* Ex=malloc((K_max)*(J_max)*(I_max)*sizeof(double));
  double* Ey=malloc((K_max)*(J_max)*(I_max)*sizeof(double));
  double* Ez=malloc((K_max)*(J_max)*(I_max)*sizeof(double));
  double* Bx=malloc((K_max)*(J_max)*(I_max)*sizeof(double));
  double* By=malloc((K_max)*(J_max)*(I_max)*sizeof(double));
  double* Bz=malloc((K_max)*(J_max)*(I_max)*sizeof(double));
// important values for our conditions
  double upor=0.1, sigma=SIGMA, delayE=0, delayB=0.5;
//printf("naredil bom zacetne pogoje\n");

// intital conditions
  initial_gauss3D(Ex, Ey, Ez, I_max, J_max, K_max, sigma, sigma, sigma, (double)resolution, delayE);
  initial_gauss3D(Bx, By, Bz, I_max, J_max, K_max, sigma, sigma, sigma, (double)resolution, delayB);
//printf("sem naredil zacetne pogoje\n");
/////////////////////////////////////////////////////////////////////////////////////////
//
// starting time propagation
//
/////////////////////////////////////////////////////////////////////////////////////////
for (t=0; t<=TIME; t++){
    double placementE, placementB;
    char fname1[20];
    char fname2[20];
    sprintf(fname1, "Maxwell_yeeE%d.txt", t);
    sprintf(fname2, "Maxwell_yeeB%d.txt", t);

    FILE *yeeE = fopen(fname1, "wt");
    FILE *yeeB = fopen(fname2, "wt");
//printf("opened the files at time %d\n", t);
    for (k=buffer; k<(K_max-buffer-1); k++){
      for (i=buffer; i<(I_max-buffer-1); i++){
        placementE = Ex[(J_max*I_max*k+I_max*(int)(J*resolution/2+1)+i)];
        placementB = Bx[(J_max*I_max*k+I_max*(int)(J*resolution/2+1)+i)];
        fprintf(yeeE, "%.4f    %.4f  %.15f\n", (double)i/(double)resolution, (double)k/(double)resolution, placementE);
        fprintf(yeeB, "%.4f    %.4f  %.15f\n", (double)i/(double)resolution, (double)k/(double)resolution, placementB);
      }
      fprintf(yeeE, "\n");
      fprintf(yeeB, "\n");
    }
    fclose(yeeE);
    fclose(yeeB);
//printf("wrote to file at time %d\n", t);
/////////////////////////////////////////////////////////////////////////////////////////
//
// Propagation of E in 3D
//
/////////////////////////////////////////////////////////////////////////////////////////
// in 1D; E[j]=(delta*(B[j+1]-B[j])+(1.0-upor*delta_t/2.0)*E[j])/(1.0+upor*delta_t/2.0);
    for (k=buffer; k<(K_max-buffer); k++){ //do +1 gre vse ker je vse skp večje za 1 na obeh straneh, torej je tale +1 zarad leve strani
      for (j=buffer; j<(J_max-buffer); j++){
        for (i=buffer; i<(I_max-buffer); i++){
          Ex[J_max*I_max*k+I_max*j+i]=(
            +delta/delta_y*(Bz[J_max*I_max*k+I_max*j+i]-Bz[J_max*I_max*k+I_max*(j-1)+i])
            -delta/delta_z*(By[J_max*I_max*k+I_max*j+i]-By[J_max*I_max*k+I_max*j+(i-1)])
            +(1-delta_t*upor/2.0)*Ex[J_max*I_max*k+I_max*j+i])/(1+delta_t*upor/2.0); // i is x direction
          Ey[J_max*I_max*k+I_max*j+i]=(
            +delta/delta_z*(Bx[J_max*I_max*k+I_max*j+i]-Bx[J_max*I_max*k+I_max*j+(i-1)])
            -delta/delta_x*(Bz[J_max*I_max*k+I_max*j+i]-Bz[J_max*I_max*(k-1)+I_max*j+i])
            +(1-delta_t*upor/2.0)*Ey[J_max*I_max*k+I_max*j+i])/(1+delta_t*upor/2.0); // j is y direction
          Ez[J_max*I_max*k+I_max*j+i]=(
            +delta/delta_x*(By[J_max*I_max*k+I_max*j+i]-By[J_max*I_max*(k-1)+I_max*j+i])
            -delta/delta_y*(Bx[J_max*I_max*k+I_max*j+i]-Bx[J_max*I_max*k+I_max*(j-1)+i])
            +(1-delta_t*upor/2.0)*Ez[J_max*I_max*k+I_max*j+i])/(1+delta_t*upor/2.0); // k is z direction
        }
      }
    }
/////////////////////////////////////////////////////////////////////////////////////////
//
// periodic boundaries of E
//
/////////////////////////////////////////////////////////////////////////////////////////
    //printf("delam meje E\n");
//printf("changing borders\n");
    exchange_x(Ex, I_max, J_max, K_max);
//printf("changed Ex\n");
    exchange_x(Ey, I_max, J_max, K_max);
//printf("changed Ey\n");
    exchange_x(Ez, I_max, J_max, K_max);
//printf("changed Ez\n");
    exchange_y(Ex, I_max, J_max, K_max);
    exchange_y(Ey, I_max, J_max, K_max);
    exchange_y(Ez, I_max, J_max, K_max);
    exchange_z(Ex, I_max, J_max, K_max);
    exchange_z(Ey, I_max, J_max, K_max);
    exchange_z(Ez, I_max, J_max, K_max);
/////////////////////////////////////////////////////////////////////////////////////////
//
// Propagation of B in 3D
//
/////////////////////////////////////////////////////////////////////////////////////////
    for (k=buffer; k<(K_max-buffer); k++){ //do +1 gre vse ker je vse skp večje za 1 na obeh straneh, torej je tale +1 zarad leve strani
      for (j=buffer; j<(J_max-buffer); j++){
        for (i=buffer; i<(I_max-buffer); i++){
          Bx[J_max*I_max*k+I_max*j+i]=
            -delta/delta_y*(Ez[J_max*I_max*k+I_max*(j+1)+i]-Ez[J_max*I_max*k+I_max*j+i])
            +delta/delta_z*(Ey[J_max*I_max*k+I_max*j+(i+1)]-Ey[J_max*I_max*k+I_max*j+i])
            +Bx[J_max*I_max*k+I_max*j+i];
          By[J_max*I_max*k+I_max*j+i]=
            -delta/delta_z*(Ex[J_max*I_max*k+I_max*j+(i+1)]-Ex[J_max*I_max*k+I_max*j+i])
            +delta/delta_x*(Ez[J_max*I_max*(k+1)+I_max*j+i]-Ez[J_max*I_max*k+I_max*j+i])
            +By[J_max*I_max*k+I_max*j+i];
          Bz[J_max*I_max*k+I_max*j+i]=
            -delta/delta_x*(Ey[J_max*I_max*(k+1)+I_max*j+i]-Ey[J_max*I_max*k+I_max*j+i])
            +delta/delta_y*(Ex[J_max*I_max*k+I_max*(j+1)+i]-Ex[J_max*I_max*k+I_max*j+i])
            +Bz[J_max*I_max*k+I_max*j+i];
        }
      }
    }
/////////////////////////////////////////////////////////////////////////////////////////
//
// periodic boundaries of B
//
/////////////////////////////////////////////////////////////////////////////////////////
    //printf("delam meje B\n");
    exchange_x(Bx, I_max, J_max, K_max);
    exchange_x(By, I_max, J_max, K_max);
    exchange_x(Bz, I_max, J_max, K_max);
    exchange_y(Bx, I_max, J_max, K_max);
    exchange_y(By, I_max, J_max, K_max);
    exchange_y(Bz, I_max, J_max, K_max);
    exchange_z(Bx, I_max, J_max, K_max);
    exchange_z(By, I_max, J_max, K_max);
    exchange_z(Bz, I_max, J_max, K_max);
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
  }
  free(Ex);
  free(Ey);
  free(Ez);
  free(Bx);
  free(By);
  free(Bz);
  return 0;
}
