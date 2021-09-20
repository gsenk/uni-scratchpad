// we write a 3D PML solver on FDTD grid where in the center of a box, with a=MAX is no conduction but in it's shell,
// which is BARRIER think there is. Conduction is displayed by SIGMA_# in various directions.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#ifndef M_PI
  #define M_PI 3.14159265359
#endif

#define TIME 500
#define MAX 50

// sigma is size of initial gauss
#define SIGMA_X 10
#define SIGMA_Y 10
#define SIGMA_Z 10

#define DELTA_X 1
#define DELTA_Y 1
#define DELTA_Z 1
#define DELTA 0.4
// barrier je thickenss of cube which has ressistance, buffer is useful for border conditions
#define BUFFER 1
// Potential generation Box, Atom, wall & stuff
#define POTENTIAL_VALUE 0.1
// 1  Wall, 2 Box, 3 atom, 4 harmonical potential, 4 lattice
#define POT_FORM 1
#define MAX_LEVEL 0
// position of intial gauss from 0 to 1
#define DELAY (M_PI/2)
#define POSITION 0.3
#define PRINT_PAGES 100

/////////////////////////////////////////////////////////////////////////////////////////
//
// Check if our point is inside sphere
//
/////////////////////////////////////////////////////////////////////////////////////////
int Inside_Sphere(int x, int y, int z, int sx, int sy, int sz, double r){
  if (((x-sx)*(x-sx)+(y-sy)*(y-sy)+(z-sz)*(z-sz))<=r){
    return 1;
  }
  else 0;
}
/////////////////////////////////////////////////////////////////////////////////////////
//
// Factorial function
//
/////////////////////////////////////////////////////////////////////////////////////////
double Factorial(int a){
  double output=1;
  for (int i=2; i<=a; i++){
    output=output*i;
  }
  return output;
}
/////////////////////////////////////////////////////////////////////////////////////////
//
// 4th order of A, recursive function
//
/////////////////////////////////////////////////////////////////////////////////////////
/*double Operator4th(double *Func, double *Pot, int X, int Y, int x, int y, int z, double delta_x, double delta_y, double delta_z, double delta, double delta4, int level){
  if (level<=1){
    return ((-Func[X*Y*z+X*y+(x+2)]+16*Func[X*Y*z+X*y+(x+1)]-30*Func[X*Y*z+X*y+(x+0)]+16*Func[X*Y*z+X*y+(x-1)]-Func[X*Y*z+X*y+(x-2)])*delta_x
           +(-Func[X*Y*z+X*(y+2)+x]+16*Func[X*Y*z+X*(y+1)+x]-30*Func[X*Y*z+X*(y+0)+x]+16*Func[X*Y*z+X*(y-1)+x]-Func[X*Y*z+X*(y-2)+x])*delta_y
           +(-Func[X*Y*(z+2)+X*y+x]+16*Func[X*Y*(z+1)+X*y+x]-30*Func[X*Y*(z+0)+X*y+x]+16*Func[X*Y*(z-1)+X*y+x]-Func[X*Y*(z-2)+X*y+x])*delta_z
           -Pot[X*Y*z+X*y+(x+0)]*Func[X*Y*z+X*y+(x+0)]/delta)*delta4;
  }
  else{
    return ((-Operator4th(Func, Pot, X, Y, (x+2), y, z, delta_x, delta_y, delta_z, delta, delta4, level-1)+16*Operator4th(Func, Pot, X, Y, (x+1), y, z, delta_x, delta_y, delta_z, delta, delta4, level-1)-30*Operator4th(Func, Pot, X, Y, (x+0), y, z, delta_x, delta_y, delta_z, delta, delta4, level-1)+16*Operator4th(Func, Pot, X, Y, (x-1), y, z, delta_x, delta_y, delta_z, delta, delta4, level-1)-Operator4th(Func, Pot, X, Y, (x-2), y, z, delta_x, delta_y, delta_z, delta, delta4, level-1))*delta_x
           +(-Operator4th(Func, Pot, X, Y, x, (y+2), z, delta_x, delta_y, delta_z, delta, delta4, level-1)+16*Operator4th(Func, Pot, X, Y, x, (y+1), z, delta_x, delta_y, delta_z, delta, delta4, level-1)-30*Operator4th(Func, Pot, X, Y, x, (y+0), z, delta_x, delta_y, delta_z, delta, delta4, level-1)+16*Operator4th(Func, Pot, X, Y, x, (y-1), z, delta_x, delta_y, delta_z, delta, delta4, level-1)-Operator4th(Func, Pot, X, Y, x, (y-2), z, delta_x, delta_y, delta_z, delta, delta4, level-1))*delta_y
           +(-Operator4th(Func, Pot, X, Y, x, y, (z+2), delta_x, delta_y, delta_z, delta, delta4, level-1)+16*Operator4th(Func, Pot, X, Y, x, y, (z+1), delta_x, delta_y, delta_z, delta, delta4, level-1)-30*Operator4th(Func, Pot, X, Y, x, y, (z+0), delta_x, delta_y, delta_z, delta, delta4, level-1)+16*Operator4th(Func, Pot, X, Y, x, y, (z-1), delta_x, delta_y, delta_z, delta, delta4, level-1)-Operator4th(Func, Pot, X, Y, x, y, (z-2), delta_x, delta_y, delta_z, delta, delta4, level-1))*delta_z
           -Pot[X*Y*z+X*y+(x+0)]*Operator4th(Func, Pot, X, Y, (x+0), y, z, delta_x, delta_y, delta_z, delta, delta4, level-1)/delta)*delta4;
  }
}*/
/////////////////////////////////////////////////////////////////////////////////////////
//
// 2nd order of A, recursive function
//
/////////////////////////////////////////////////////////////////////////////////////////
/*double Operator2nd(double *Func, double *Pot, int X, int Y, int x, int y, int z, double delta_x, double delta_y, double delta_z, double delta, double delta4, int level){
  if (level<=1){
    return ((Func[X*Y*z+X*y+(x+1)]-2*Func[X*Y*z+X*y+(x+0)]+Func[X*Y*z+X*y+(x-1)])*delta_x
           +(Func[X*Y*z+X*(y+1)+x]-2*Func[X*Y*z+X*(y+0)+x]+Func[X*Y*z+X*(y-1)+x])*delta_y
           +(Func[X*Y*(z+1)+X*y+x]-2*Func[X*Y*(z+0)+X*y+x]+Func[X*Y*(z-1)+X*y+x])*delta_z
           -Pot[X*Y*z+X*y+(x+0)]*Func[X*Y*z+X*y+(x+0)]/delta)*delta4;
  }
  else{
    return ((Operator2nd(Func, Pot, X, Y, (x+1), y, z, delta_x, delta_y, delta_z, delta, delta4, level-1)-2*Operator2nd(Func, Pot, X, Y, (x+0), y, z, delta_x, delta_y, delta_z, delta, delta4, level-1)+Operator2nd(Func, Pot, X, Y, (x-1), y, z, delta_x, delta_y, delta_z, delta, delta4, level-1))*delta_x
           +(Operator2nd(Func, Pot, X, Y, x, (y+1), z, delta_x, delta_y, delta_z, delta, delta4, level-1)-2*Operator2nd(Func, Pot, X, Y, x, (y+0), z, delta_x, delta_y, delta_z, delta, delta4, level-1)+Operator2nd(Func, Pot, X, Y, x, (y-1), z, delta_x, delta_y, delta_z, delta, delta4, level-1))*delta_y
           +(Operator2nd(Func, Pot, X, Y, x, y, (z+1), delta_x, delta_y, delta_z, delta, delta4, level-1)-2*Operator2nd(Func, Pot, X, Y, x, y, (z+0), delta_x, delta_y, delta_z, delta, delta4, level-1)+Operator2nd(Func, Pot, X, Y, x, y, (z-1), delta_x, delta_y, delta_z, delta, delta4, level-1))*delta_z
           -Pot[X*Y*z+X*y+(x+0)]*Operator2nd(Func, Pot, X, Y, (x+0), y, z, delta_x, delta_y, delta_z, delta, delta4, level-1)/delta)*delta4;
  }
}*/
/////////////////////////////////////////////////////////////////////////////////////////
//
// generate the potential we wish for
//
/////////////////////////////////////////////////////////////////////////////////////////
void Generate_Potential(double *Val, int X, int Y, int Z, double value, int name){
  int z, y,x;
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
            Val[X*Y*z+X*y+x]=value;
          }
        }
      }
      break;
    }
    case 3:{ // center of the atom potential
      int C=3*X/4;
      double r0=10;
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
      double kx=50;
      double ky=50;
      double kz=50;
      for (z=0; z<Z; z++){
        for (y=0; y<Y; y++){
          for (x=0; x<X; x++){
            Val[X*Y*z+X*y+x]=0.1*value*((x-X/2)*(x-X/2)+(y-Y/2)*(y-Y/2)+(z-Z/2)*(z-Z/2));
          }
        }
      }
    break;
    }
    case 5:{
      double r=3;
      double sphere1[]={3*X/4, Y/2, Z/2};
      double sphere2[]={4*X/5, Y/3, Z/2};
      double sphere3[]={3*X/4, 3*Y/4, Z/2};
      for (z=0; z<Z; z++){
        for (y=0; y<Y; y++){
          for (x=0; x<X; x++){
            if (Inside_Sphere(x, y, z, sphere1[0], sphere1[1], sphere1[2], r)){
              Val[X*Y*z+X*y+x]=value;
            }
            else if (Inside_Sphere(x, y, z, sphere2[0], sphere2[1], sphere2[2], r)){
              Val[X*Y*z+X*y+x]=value;
            }
            else if (Inside_Sphere(x, y, z, sphere3[0], sphere3[1], sphere3[2], r)){
              Val[X*Y*z+X*y+x]=value;
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
        Val[X*Y*z+X*y+x]=10*exp(-(x_axis*x_axis+y_axis*y_axis+z_axis*z_axis))*sin(2*M_PI*x/10-delay); // 10 is for slowing down the fluctuations
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
void exchange_x(double *Val, int X, int Y, int Z, int buffer){
  int i, z, y;
  for (z=1; z<(Z-1); z++){
    for (y=1; y<(Y-1); y++){
      for (i=0; i<buffer; i++){
        Val[Y*X*z+X*y+i]=Val[Y*X*z+X*y+(X-2*buffer+i)];
        Val[Y*X*z+X*y+(X-buffer+i)]=Val[Y*X*z+X*y+(buffer+i)];
      }
    }
  }
}
// first in the back is the second last in front, and last in front is the same as second in the back.
void exchange_y(double *Val, int X, int Y, int Z, int buffer){
  int i, z, x;
  for (z=1; z<(Z-1); z++){
    for (x=1; x<(X-1); x++){
      for (i=0; i<buffer; i++){
        Val[Y*X*z+X*i+x]=Val[Y*X*z+X*(Y-2*buffer+i)+x];
        Val[Y*X*z+X*(Y-buffer+i)+x]=Val[Y*X*z+X*(buffer+i)+x];
      }
    }
  }
}
// first on bottom is the second last on top, and last on top is the same as second on bottom.
void exchange_z(double *Val, int X, int Y, int Z, int buffer){
  int i, y, x;
  for (y=1; y<(Y-1); y++){
    for (x=1; x<(X-1); x++){
      for (i=0; i<buffer; i++){
        Val[Y*X*i+X*y+x]=Val[Y*X*(Z-2*buffer+i)+X*y+x];
        Val[Y*X*(Z-buffer+i)+X*y+x]=Val[Y*X*(buffer+i)+X*y+x];
      }
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
  int K=MAX, J=MAX, I=2*MAX, k, j, i, buffer=BUFFER, t;
  int Pot_form=POT_FORM;
  int p=0, max_level=MAX_LEVEL;
  double delayRe=0, delayIm=DELAY;
  double predznak;

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
//printf("naredil bom zacetne pogoje\n");

// intital conditions
  initial_gauss3D(PsiRe, I_max, J_max, K_max, sigma_x, sigma_y, sigma_z, delayRe, POSITION);
  initial_gauss3D(PsiIm, I_max, J_max, K_max, sigma_x, sigma_y, sigma_z, delayIm, POSITION);
  Generate_Potential(Potential, I_max, J_max, K_max, POTENTIAL_VALUE, Pot_form);

//printf("sem naredil zacetne pogoje\n");
//printf("%d\n", 38%3);
/////////////////////////////////////////////////////////////////////////////////////////
//
// starting time propagation
//
/////////////////////////////////////////////////////////////////////////////////////////
  FILE *yeeB1 = fopen("Schrodinger.txt", "wt");
  FILE *yeeB2 = fopen("Potential.txt", "wt");
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
          fprintf(yeeB2, "%.15f\n", Potential[I_max*j+i]);
        }
      }
      printf("wrote to file at time %d\n", t);
    }
/////////////////////////////////////////////////////////////////////////////////////////
//
// Change deltas for propagation
//
/////////////////////////////////////////////////////////////////////////////////////////
    double delta4=delta*delta/4;

    double deltax=1/(2*delta_x*delta_x); // for Operator 2nd
    double deltay=1/(2*delta_y*delta_y);
    double deltaz=1/(2*delta_z*delta_z);

   /* double deltax=1/(12*delta_x*delta_x); // for operator 4th
    double deltay=1/(12*delta_y*delta_y);
    double deltaz=1/(12*delta_z*delta_z);*/
/////////////////////////////////////////////////////////////////////////////////////////
//
// Propagation of Real part in 3D
//
/////////////////////////////////////////////////////////////////////////////////////////
    /*for (k=buffer; k<(K_max-buffer); k++){ //do +1 gre vse ker je vse skp večje za 1 na obeh straneh, torej je tale +1 zarad leve strani
      for (j=buffer; j<(J_max-buffer); j++){
        for (i=buffer; i<(I_max-buffer); i++){
          for(p=0; p<=max_level; p=p+1){
            if (p%2==1) {predznak=-1;}
            else {predznak=1;}
            PsiRe[J_max*I_max*k+I_max*j+i]=PsiRe[J_max*I_max*k+I_max*j+i]-predznak*4/(delta*Factorial(2*p+1))
            *(Operator2nd(PsiIm, Potential, I_max, J_max, i, j, k, deltax, deltay, deltaz, delta, delta4, (2*p+1)));
          }
        }
      }
    }*/
    for (k=buffer; k<(K_max-buffer); k++){ //do +1 gre vse ker je vse skp večje za 1 na obeh straneh, torej je tale +1 zarad leve strani
      for (j=buffer; j<(J_max-buffer); j++){
        for (i=buffer; i<(I_max-buffer); i++){
          PsiRe[J_max*I_max*k+I_max*j+i]=PsiRe[J_max*I_max*k+I_max*j+i]
          -deltax*(PsiIm[J_max*I_max*k+I_max*j+(i+1)]-2*PsiIm[J_max*I_max*k+I_max*j+(i+0)]+PsiIm[J_max*I_max*k+I_max*j+(i-1)])
          -deltay*(PsiIm[J_max*I_max*k+I_max*(j+1)+i]-2*PsiIm[J_max*I_max*k+I_max*(j+0)+i]+PsiIm[J_max*I_max*k+I_max*(j-1)+i])
          -deltaz*(PsiIm[J_max*I_max*(k+1)+I_max*j+i]-2*PsiIm[J_max*I_max*(k+0)+I_max*j+i]+PsiIm[J_max*I_max*(k-1)+I_max*j+i])
          +Potential[J_max*I_max*k+I_max*j+i]*PsiIm[J_max*I_max*k+I_max*j+i]; // i is x direction
        }
      }
    }
/////////////////////////////////////////////////////////////////////////////////////////
//
// periodic boundaries of Real part
//
/////////////////////////////////////////////////////////////////////////////////////////
    exchange_x(PsiRe, I_max, J_max, K_max, buffer);
    exchange_y(PsiRe, I_max, J_max, K_max, buffer);
    exchange_z(PsiRe, I_max, J_max, K_max, buffer);
/////////////////////////////////////////////////////////////////////////////////////////
//
// Propagation of Imaginary part in 3D
//
/////////////////////////////////////////////////////////////////////////////////////////
    /*for (k=buffer; k<(K_max-buffer); k++){ //do +1 gre vse ker je vse skp vec�je za 1 na obeh straneh, torej je tale +1 zarad leve strani
      for (j=buffer; j<(J_max-buffer); j++){
        for (i=buffer; i<(I_max-buffer); i++){
          for(p=0; p<=max_level; p=p+1){
            if (p%2==1) {predznak=-1;}
            else {predznak=1;}
            PsiIm[J_max*I_max*k+I_max*j+i]=PsiIm[J_max*I_max*k+I_max*j+i]+predznak*4/(delta*Factorial(2*p+1))
            *(Operator2nd(PsiRe, Potential, I_max, J_max, i, j, k, deltax, deltay, deltaz, delta, delta4, (2*p+1)));
          }
        }
      }
    }*/
    for (k=buffer; k<(K_max-buffer); k++){ //do +1 gre vse ker je vse skp vec�je za 1 na obeh straneh, torej je tale +1 zarad leve strani
      for (j=buffer; j<(J_max-buffer); j++){
        for (i=buffer; i<(I_max-buffer); i++){
          PsiIm[J_max*I_max*k+I_max*j+i]=PsiIm[J_max*I_max*k+I_max*j+i]
          +deltax*(PsiRe[J_max*I_max*k+I_max*j+(i+1)]-2*PsiRe[J_max*I_max*k+I_max*j+(i+0)]+PsiRe[J_max*I_max*k+I_max*j+(i-1)])
          +deltay*(PsiRe[J_max*I_max*k+I_max*(j+1)+i]-2*PsiRe[J_max*I_max*k+I_max*(j+0)+i]+PsiRe[J_max*I_max*k+I_max*(j-1)+i])
          +deltaz*(PsiRe[J_max*I_max*(k+1)+I_max*j+i]-2*PsiRe[J_max*I_max*(k+0)+I_max*j+i]+PsiRe[J_max*I_max*(k-1)+I_max*j+i])
          -Potential[J_max*I_max*k+I_max*j+i]*PsiRe[J_max*I_max*k+I_max*j+i]; // i is x direction
        }
      }
    }
/////////////////////////////////////////////////////////////////////////////////////////
//
// periodic boundaries of Imaginary part
//
/////////////////////////////////////////////////////////////////////////////////////////
    exchange_x(PsiIm, I_max, J_max, K_max, buffer);
    exchange_y(PsiIm, I_max, J_max, K_max, buffer);
    exchange_z(PsiIm, I_max, J_max, K_max, buffer);
/////////////////////////////////////////////////////////////////////////////////////////
    printf("%d\n", t);
  }
  free(PsiRe);
  free(PsiIm);
  fclose(yeeB1);
  fclose(yeeB2);
  clock_t endClock = clock();
  printf("%ld\n", (endClock - startClock) / CLOCKS_PER_SEC);
  return 0;
}
