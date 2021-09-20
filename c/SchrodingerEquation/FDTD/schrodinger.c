// we write a 3D PML solver on FDTD grid where in the center of a box, with a=MAX is no conduction but in it's shell,
// which is BARRIER think there is. Conduction is displayed by SIGMA_# in various directions.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#ifndef M_PI
  #define M_PI 3.14159265359
#endif

#define TIME 7000
#define MAX 300

// sigma is size of initial gauss
#define SIGMA_X 70
#define SIGMA_Y 70
#define SIGMA_Z 70

#define DELTA_X 1
#define DELTA_Y 1
#define DELTA_Z 1
#define DELTA 0.4
// barrier je thickenss of cube which has ressistance, buffer is useful for border conditions
#define BUFFER 1
// Potential generation Box, Atom, wall & stuff
#define POTENTIAL_VALUE 0.1
// 1  Wall, 2 Box, 3 atom, 4 harmonical potential, 4 lattice
#define POT_FORM 5
// position of intial gauss from 0 to 1
#define DELAY (M_PI/2)
#define POSITION 0.2
#define PRINT_PAGES 100

/////////////////////////////////////////////////////////////////////////////////////////
//
// Check if our point is inside sphere
//
/////////////////////////////////////////////////////////////////////////////////////////
int Inside_Sphere(int x, int y, int z, double sx, double sy, double sz, double r){
  if (sqrt(((double)x-sx)*((double)x-sx)+((double)y-sy)*((double)y-sy)+((double)z-sz)*((double)z-sz))<=r){
    //printf("%f\n", sqrt((x-sx)*(x-sx)+(y-sy)*(y-sy)+(z-sz)*(z-sz)));
    return 1;
  }
  else {return 0;}
}
/////////////////////////////////////////////////////////////////////////////////////////
//
// generate the potential we wish for
//
/////////////////////////////////////////////////////////////////////////////////////////
void Generate_Potential(double *Val, int X, int Y, int Z, double value, int name){
  int x, y, z;
  int i, j, k;
  for (z=0; z<Z; z++){
    for (y=0; y<Y; y++){
      for (x=0; x<X; x++){
        Val[X*Y*z+X*y+x]=0;
      }
    }
  }
  switch(name){
    case 1:{ // Wall potential
      int wall_start=X*0.5;
      int wall_thicknes=2;
      for (z=0; z<Z; z++){
        for (y=0; y<Y; y++){
          for (x=wall_start; x<(wall_start+wall_thicknes); x++){
            Val[X*Y*z+X*y+x]=value;
            }
          }
        }
      break;
    }
    case 2:{ // BOX potential
      int A=X/3; // size of box in x direction
      int B=3*Y/4; // size of box in y direction
      int C=3*Z/4; // size of box in z direction
      int D=3*X/4; // center of the box in x direction
      for (z=(int)(Z/2-C/2); z<=(int)(Z/2+C/2); z++){
        for (y=(int)(Y/2-B/2); y<=(int)(Y/2+B/2); y++){
          for (x=(int)(D-A/2); x<=(int)(D+A/2); x++){
            Val[X*Y*z+X*y+x]=0.5*value;
          }
        }
      }
      break;
    }
    case 3:{ // center of the atom potential
      double C=3*X/4;
      double r0=15;
      double r, dx, dy, dz;
      double r6;
      double pot;
      for (z=0; z<Z; z++){
        for (y=0; y<Y; y++){
          for (x=0; x<X; x++){
            dx=(C-x);
            dy=(y-Y/2);
            dz=(z-Z/2);
            r=sqrt(dx*dx+dy*dy+dz*dz);
            r6=(r0/r);
            pot=value*(r6*r6-2*r6);
            if (r<r0/4) pot=value;
            Val[X*Y*z+X*y+x]=pot;
          }
        }
      }
      break;
    }
    case 4:{ // Harmonic potential
      double kx=0.01*0.01;    // 1/100
      double ky=0.025*0.025;  // 1/50
      double kz=0.025*0.025;  // 1/50
      for (z=0; z<Z; z++){
        for (y=0; y<Y; y++){
          for (x=0; x<X; x++){
            Val[X*Y*z+X*y+x]=3*value*(kx*(x-X/2)*(x-X/2)+ky*(y-Y/2)*(y-Y/2)+kz*(z-Z/2)*(z-Z/2));
          }
        }
      }
    break;
    }
    case 5:{
      double r=7;
      double spherex[]={6*X/12, 7*X/12, 8*X/12, 9*X/12, 10*X/12, 11*X/12};
      double spherey[]={1*Y/6, 2*Y/6, 3*Y/6, 4*Y/6, 5*Y/6};
      double spherez[]={1*Z/6, 2*Z/6, 3*Z/6, 4*Z/6, 5*Z/6};
      for (z=0; z<Z; z++){
        for (y=0; y<Y; y++){
          for (x=0; x<X; x++){
            for (k=0; k<5; k++){
              for (j=0; j<5; j++){
                for (i=0; i<6; i++){
                  if (Inside_Sphere(x, y, z, spherex[i], spherey[j], spherez[k], r)){
                    Val[X*Y*z+X*y+x]=6*value;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}
/////////////////////////////////////////////////////////////////////////////////////////
//
// initial gauss condition
//
/////////////////////////////////////////////////////////////////////////////////////////
void initial_gauss3D(double *Val, int X, int Y, int Z, double sigma_x, double sigma_y, double sigma_z, double delay, double position){
  int z, y, x;
  double z_axis, y_axis, x_axis;
  for (z=0; z<Z; z++){
    for (y=0; y<Y; y++){
      for (x=0; x<X; x++){
        x_axis=fabs((x-X*position)/(sigma_x));
        y_axis=fabs((y-Y/2.0)/(sigma_y));
        z_axis=fabs((z-Z/2.0)/(sigma_z));
        Val[X*Y*z+X*y+x]=10*exp(-(x_axis*x_axis+y_axis*y_axis+z_axis*z_axis))*sin(2*M_PI*x/30-delay); // 10 is for slowing down the fluctuations
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
// start MAIN function
//
/////////////////////////////////////////////////////////////////////////////////////////
int main(){
  clock_t startClock = clock();
  int K=MAX, J=MAX, I=2*MAX, k, j, i, buffer=BUFFER, t;
  int Pot_form=POT_FORM;
  double delayRe=0, delayIm=DELAY;

//  has to be less then 0.5, because of dispersion
  double delta_x=DELTA_X, delta_y=DELTA_Y, delta_z=DELTA_Z;
  double delta_t=sqrt(delta_x*delta_x+delta_y*delta_y+delta_z*delta_z), delta=(DELTA/delta_t);
  int K_max=(K+2*buffer), J_max=(J+2*buffer), I_max=(I+2*buffer);

// declare arrays
  double* PsiRe=malloc((K_max)*(J_max)*(I_max)*sizeof(double));
  double* PsiIm=malloc((K_max)*(J_max)*(I_max)*sizeof(double));
  double* Potential=malloc((K_max)*(J_max)*(I_max)*sizeof(double));
// important values for our conditions
  double sigma_x=SIGMA_X, sigma_y=SIGMA_Y, sigma_z=SIGMA_Z;//, delayRe=0, delayIm=DELAYIM;

// intital conditions
  initial_gauss3D(PsiRe, I_max, J_max, K_max, sigma_x, sigma_y, sigma_z, delayRe, POSITION);
  initial_gauss3D(PsiIm, I_max, J_max, K_max, sigma_x, sigma_y, sigma_z, delayIm, POSITION);
  Generate_Potential(Potential, I_max, J_max, K_max, POTENTIAL_VALUE, Pot_form);

/////////////////////////////////////////////////////////////////////////////////////////
//
// starting time propagation
//
/////////////////////////////////////////////////////////////////////////////////////////
  FILE *yeeB1 = fopen("Schrodinger.txt", "wt");
  FILE *yeeB2 = fopen("Potential.txt", "wt");
  for (k=buffer; k<(K_max-buffer); k++){
    for (j=buffer; j<(J_max-buffer); j++){
      for (i=buffer; i<(I_max-buffer); i++){
        fprintf(yeeB2, "%.15f\n", Potential[I_max*J_max*k+I_max*j+i]);
      }
    }
  }
  for (t=0; t<=TIME; t++){
    double placementE, placementB;
  //printf("opened the files at time %d\n", t);
    int frames_every=TIME/100;
    if ((t%frames_every)==0){
      for (j=buffer; j<(J_max-buffer); j++){
        for (i=buffer; i<(I_max-buffer); i++){
          placementE = PsiRe[I_max*J_max*(int)(K_max/2-1)+I_max*j+i]+Potential[I_max*J_max*(int)(K_max/2-1)+I_max*j+i]*10;
          placementB = PsiIm[I_max*J_max*(int)(K_max/2-1)+I_max*j+i]+Potential[I_max*J_max*(int)(K_max/2-1)+I_max*j+i]*10;
          fprintf(yeeB1, "%.15f\n", (placementB*placementB+placementE*placementE));
        }
      }
      printf("wrote to file at time %d\n", t);
    }
/////////////////////////////////////////////////////////////////////////////////////////
//
// Propagation of Real part in 3D
//
/////////////////////////////////////////////////////////////////////////////////////////
    for (k=buffer; k<(K_max-buffer); k++){ //do +1 gre vse ker je vse skp večje za 1 na obeh straneh, torej je tale +1 zarad leve strani
      for (j=buffer; j<(J_max-buffer); j++){
        for (i=buffer; i<(I_max-buffer); i++){
          PsiRe[J_max*I_max*k+I_max*j+i]=PsiRe[J_max*I_max*k+I_max*j+i]
          -delta/(delta_x*delta_x*2)*(PsiIm[J_max*I_max*k+I_max*j+(i+1)]-2*PsiIm[J_max*I_max*k+I_max*j+(i+0)]+PsiIm[J_max*I_max*k+I_max*j+(i-1)])
          -delta/(delta_y*delta_y*2)*(PsiIm[J_max*I_max*k+I_max*(j+1)+i]-2*PsiIm[J_max*I_max*k+I_max*(j+0)+i]+PsiIm[J_max*I_max*k+I_max*(j-1)+i])
          -delta/(delta_z*delta_z*2)*(PsiIm[J_max*I_max*(k+1)+I_max*j+i]-2*PsiIm[J_max*I_max*(k+0)+I_max*j+i]+PsiIm[J_max*I_max*(k-1)+I_max*j+i])
          +Potential[J_max*I_max*k+I_max*j+i]*PsiIm[J_max*I_max*k+I_max*j+i]; // i is x direction
        }
      }
    }
/////////////////////////////////////////////////////////////////////////////////////////
//
// periodic boundaries of Real part
//
/////////////////////////////////////////////////////////////////////////////////////////
    exchange_x(PsiRe, I_max, J_max, K_max);
    exchange_y(PsiRe, I_max, J_max, K_max);
    exchange_z(PsiRe, I_max, J_max, K_max);
/////////////////////////////////////////////////////////////////////////////////////////
//
// Propagation of Imaginary part in 3D
//
/////////////////////////////////////////////////////////////////////////////////////////
    for (k=buffer; k<(K_max-buffer); k++){ //do +1 gre vse ker je vse skp vec�je za 1 na obeh straneh, torej je tale +1 zarad leve strani
      for (j=buffer; j<(J_max-buffer); j++){
        for (i=buffer; i<(I_max-buffer); i++){
          PsiIm[J_max*I_max*k+I_max*j+i]=PsiIm[J_max*I_max*k+I_max*j+i]
          +delta/(delta_x*delta_x*2)*(PsiRe[J_max*I_max*k+I_max*j+(i+1)]-2*PsiRe[J_max*I_max*k+I_max*j+(i+0)]+PsiRe[J_max*I_max*k+I_max*j+(i-1)])
          +delta/(delta_y*delta_y*2)*(PsiRe[J_max*I_max*k+I_max*(j+1)+i]-2*PsiRe[J_max*I_max*k+I_max*(j+0)+i]+PsiRe[J_max*I_max*k+I_max*(j-1)+i])
          +delta/(delta_z*delta_z*2)*(PsiRe[J_max*I_max*(k+1)+I_max*j+i]-2*PsiRe[J_max*I_max*(k+0)+I_max*j+i]+PsiRe[J_max*I_max*(k-1)+I_max*j+i])
          -Potential[J_max*I_max*k+I_max*j+i]*PsiRe[J_max*I_max*k+I_max*j+i]; // i is x direction
        }
      }
    }
/////////////////////////////////////////////////////////////////////////////////////////
//
// periodic boundaries of Imaginary part
//
/////////////////////////////////////////////////////////////////////////////////////////
    exchange_x(PsiIm, I_max, J_max, K_max);
    exchange_y(PsiIm, I_max, J_max, K_max);
    exchange_z(PsiIm, I_max, J_max, K_max);
/////////////////////////////////////////////////////////////////////////////////////////
    //printf("%d\n", t);
  }
  free(PsiRe);
  free(PsiIm);
  fclose(yeeB1);
  fclose(yeeB2);
  clock_t endClock = clock();
  printf("%ld\n", (endClock - startClock) / CLOCKS_PER_SEC);
  return 0;
}
