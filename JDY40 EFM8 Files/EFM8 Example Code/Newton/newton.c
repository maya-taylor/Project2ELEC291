#include <stdio.h>
#include <math.h>

#define MAXITER 100
#define TOL 1.0e-6
#define ITOL (1.0/TOL)

typedef float (*_equation)(void);

#define NEQS 4  // The number of equations to solve

// The augmented matrix and vectors used to solve the non-linear system of equations.
xdata float J[NEQS][NEQS+1], X[NEQS];

// Some definitions to access the vector of solutions.
#define ID1 X[0]
#define ID2 X[1]
#define V1  X[2]
#define V2  X[3]

// Constants used by the equations below.
xdata float n=2.0; // Different for each version of the exam
#define VS1 -8.0
#define VS2 8.0
#define R1 1000
#define R2 2000
#define IS 8.32e-9
#define VT 0.025

// The equations to solve.  Remember: replace the '=' with '-' as required by the Newton-Rampson algorithm.
float equ1 (void) { return ( ID1+ID2-((V1-VS1)/R1) ); }
float equ2 (void) { return ( ID2-((VS2-V2)/R2) ); }
float equ3 (void) { return ( ID1-IS*expf((0.0-V1)/(n*VT)) ); }
float equ4 (void) { return ( ID2-IS*expf((V2-V1)/(n*VT)) ); }

const _equation equation[NEQS]={equ1, equ2, equ3, equ4}; // Array of pointers to the equations to solve

void Jacobian (void)
{
	char i, j;
	xdata float saved_X;
	
	for(i=0; i<NEQS; i++)
	{
		for(j=0; j<NEQS; j++)
		{
			saved_X=X[j];
			X[j]+=TOL;
			J[i][j]=(equation[i]()+J[i][NEQS])*ITOL;
			X[j]=saved_X;
		}
	}
}

void Gauss_Elimination (void)
{
	char i, j, k;
	xdata float temp;
	
	// Apply Gauss Elimination to the augmented Jacobian matrix
	for(i=0; i<NEQS; i++)
	{
		if(J[i][i] == 0.0) // Pivot element can not be zero.
		{
			// Try to swap rows.
			for(j=i; j<NEQS; j++)
			{
				if(J[j][i]!=0.0)
				{ 
				    // Swap rows
					for(k=0; k<NEQS+1; k++)
					{
						temp=J[j][k];
						J[j][k]=J[i][k];
						J[i][k]=temp;
					}
					break;	
				}
			}
			if(j==NEQS) // Didn't find a non-zero pivot row to swap with
			{
				printf("ERROR: could not find solution.\n");
				return;
			}
		}
		
		for(j=i+1; j<NEQS; j++)
		{
			temp = J[j][i]/J[i][i];
			for(k=i; k<NEQS+1; k++)
			{
			 	J[j][k] = J[j][k] - temp*J[i][k];
			}
		}
	}
	
	// Perform back substitution
	for (j = NEQS - 1; j >= 0; j--)
	{
		for (i = NEQS - 1; i > j; i--)
		{
			J[j][NEQS] -= J[i][NEQS] * J[j][i];
			J[j][i] = 0.0;
		}
		J[j][NEQS] /= J[j][j];
		J[j][j] = 1.0;
	}
}

int Newton_Raphson (void)
{
	char j, k;
	bit notdone;

	for(j=0; j<MAXITER; j++)
	{
		for(k=0; k<NEQS; k++) J[k][NEQS]=-equation[k](); // Augment the Jacobian matrix with vector Y
		Jacobian();     // Complete the augmented Jacobian using an approximate to the partial derivatives.
		Gauss_Elimination(); // Solve the resulting system of equations (augmented J matrix)
		
		// Compute new approximation to the solution and check if it is good enough
		for(k=0, notdone=0; k<NEQS; k++)
		{
			X[k]+=J[k][NEQS];
			if (fabsf(J[k][NEQS])>TOL) notdone=1;
		}
		
		if (notdone==0) break;
	}

	return j;
}

void main(void)
{
	char j;
	for(n=2.0; n>1.5; n-=0.1) // Five exams that differ only on the diode's intrinsic constant
	{
		// Initial estimates
		if(n>1.9)
		{
			ID1= 1e-3;
			ID2= 1e-3;
			V1= -0.7;
			V2= 0.0;
		}
		j=Newton_Raphson(); // Solve
		// Display results
		printf("----------------------------------------------------------------\n");
		printf("n=%3.1f\tIterations=%d\nVD1=%9.6fV\tVD2=%9.6fV\tID1=%9.6fmA\tID2=%9.6fmA\n", n, j, 0.0-V1, V2-V1, ID1*1.0e3, ID2*1.0e3);
	}
}
