// we write a 3D PML solver on FDTD grid where in the center of a box, with a=MAX is no conduction but in it's shell, which is BARRIER think there is. Conduction is displayed by SIGMA_# in various directions.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TIME 300
#define MAX 70

// sigma is size of initial gauss
#define SIGMA_X 10
#define SIGMA_Y 1000
#define SIGMA_Z 1000


#define DELTA_X 1
#define DELTA_Y 1
#define DELTA_Z 1
// barrier je thickenss of cube which has ressistance, buffer is useful for border conditions
#define BARRIER_X 15
#define BARRIER_Y 15
#define BARRIER_Z 15
#define BUFFER 1
// ressistance of material. 1 is the center, 2 is around it with thickness of BARRIER
#define RESSISTANCE1 0
#define RESSISTANCE2 5


#include <time.h>

/////////////////////////////////////////////////////////////////////////////////////////
//
// initial gauss condition with sine function in x direction
//
/////////////////////////////////////////////////////////////////////////////////////////
void initial_gauss3D(double *ValX, double *ValY, double *ValZ, int X, int Y, int Z, double sigma_x, double sigma_y, double sigma_z, double resolution, double delay){
  int z,y,x;
  double z_axis, y_axis, x_axis;
  for (z=0; z<Z; z++){
    for (y=0; y<Y; y++){
      for (x=0; x<X; x++){
        x_axis=fabs((x+delay-X/2.0)/(resolution*sigma_x));
        y_axis=fabs((y+delay-Y/2.0)/(resolution*sigma_y));
        z_axis=fabs((z+delay-Z/2.0)/(resolution*sigma_z));
        ValX[Y*X*z+X*y+x]=10*exp(-(x_axis*x_axis+y_axis*y_axis+z_axis*z_axis))*sin((1./3.)*x/(double)resolution);
        ValY[Y*X*z+X*y+x]=10*exp(-(x_axis*x_axis+y_axis*y_axis+z_axis*z_axis))*sin((1./3.)*x/(double)resolution);
        ValZ[Y*X*z+X*y+x]=10*exp(-(x_axis*x_axis+y_axis*y_axis+z_axis*z_axis))*sin((1./3.)*x/(double)resolution);
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// Check if this element is in BARRIER or inside cube
//
/////////////////////////////////////////////////////////////////////////////////////////
// if this function return 1, then our point is inside barrier, so it should fell ressistance
int is_inside_BARRIER(int x, int y, int z, int X, int Y, int Z, int barrier_x, int barrier_y, int barrier_z, int buffer){
  if ((x<=(buffer-1+barrier_x)) || (x>=(X-buffer-barrier_x)) || (y<=(buffer-1+barrier_y)) || (y>=(Y-buffer-barrier_y)) || (z<=(buffer-1+barrier_z)) || (z>=(Z-buffer-barrier_z))){
    return 1;
  }
  return 0;
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
  clock_t startClock = clock();
  int K=MAX, J=MAX, I=MAX, k, j, i, buffer=BUFFER, barrier_k=BARRIER_Z, barrier_j=BARRIER_Y, barrier_i=BARRIER_X, t;

// delta has to be around 0.5, because of dispersion
  int resolution=1;
  double delta_x=DELTA_X, delta_y=DELTA_Y, delta_z=DELTA_Z;
  double delta_t=sqrt(delta_x*delta_x+delta_y*delta_y+delta_z*delta_z), delta=0.25;//delta=0.5*delta_t;
  int K_max=((K+2*barrier_k)*resolution+2*buffer), J_max=((J+2*barrier_j)*resolution+2*buffer), I_max=((I+2*barrier_i)*resolution+2*buffer);

// declare arrays
  double* Ex=malloc((K_max)*(J_max)*(I_max)*sizeof(double));
  double* Ey=malloc((K_max)*(J_max)*(I_max)*sizeof(double));
  double* Ez=malloc((K_max)*(J_max)*(I_max)*sizeof(double));
  double* Bx=malloc((K_max)*(J_max)*(I_max)*sizeof(double));
  double* By=malloc((K_max)*(J_max)*(I_max)*sizeof(double));
  double* Bz=malloc((K_max)*(J_max)*(I_max)*sizeof(double));
// important values for our conditions
  double upor, upor1=RESSISTANCE1, upor2=RESSISTANCE2, sigma_x=SIGMA_X, sigma_y=SIGMA_Y, sigma_z=SIGMA_Z, delayE=0, delayB=0.5;
//printf("naredil bom zacetne pogoje\n");

// intital conditions
  initial_gauss3D(Ex, Ey, Ez, I_max, J_max, K_max, sigma_x, sigma_y, sigma_z, (double)resolution, delayE);
  initial_gauss3D(Bx, By, Bz, I_max, J_max, K_max, sigma_x, sigma_y, sigma_z, (double)resolution, delayB);
//printf("sem naredil zacetne pogoje\n");
/////////////////////////////////////////////////////////////////////////////////////////
//
// starting time propagation
//
/////////////////////////////////////////////////////////////////////////////////////////
  FILE *yeeE1 = fopen("Maxwell_PMLE.txt", "wt");
  FILE *yeeB1 = fopen("Maxwell_PMLB.txt", "wt");
for (t=0; t<=TIME; t++){
    double placementE, placementB;
    /*char fname1[20];
    char fname2[20];
    sprintf(fname1, "Maxwell_PMLE%d.txt", t);
    sprintf(fname2, "Maxwell_PMLB%d.txt", t);

    FILE *yeeE = fopen(fname1, "wt");
    FILE *yeeB = fopen(fname2, "wt");*/
//printf("opened the files at time %d\n", t);
    for (k=buffer; k<(K_max-buffer); k++){
      for (i=buffer; i<(I_max-buffer); i++){
        placementE = Ex[(J_max*I_max*k+I_max*(int)(J_max/2-1)+i)];
        placementB = Bx[(J_max*I_max*k+I_max*(int)(J_max/2-1)+i)];
        //fprintf(yeeE, "%.4f    %.4f  %.15f\n", (double)i/(double)resolution, (double)k/(double)resolution, placementE);
        //fprintf(yeeB, "%.4f    %.4f  %.15f\n", (double)i/(double)resolution, (double)k/(double)resolution, placementB);
        fprintf(yeeE1, "%.15f\n", placementE);
        fprintf(yeeB1, "%.15f\n", placementB);
      }
      //fprintf(yeeE, "\n");
      //fprintf(yeeB, "\n");
    }
    //fclose(yeeE);
    //fclose(yeeB);
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
          if (is_inside_BARRIER(i, j, k, I_max, J_max, K_max, barrier_i, barrier_j, barrier_k, buffer)) upor=upor2;
          else upor=upor1;
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
    exchange_x(Ex, I_max, J_max, K_max);
    exchange_x(Ey, I_max, J_max, K_max);
    exchange_x(Ez, I_max, J_max, K_max);
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
  fclose(yeeE1);
  fclose(yeeB1);
  clock_t endClock = clock();
  printf("%ld\n", (endClock - startClock) / CLOCKS_PER_SEC);
  return 0;
}
