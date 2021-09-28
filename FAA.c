#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<float.h>
#include<math.h>

typedef struct{
	float real;
	float imag;
} complejo;

int main(int argv,char *args[]){

	int muestras=atoi(args[1]),segundos=atoi(args[2]);
	float ad[segundos][muestras], epochs[segundos*2-1][muestras], hamming_coef[muestras], d;
	FILE* file; 


	//*************************************************Input

	file= fopen (args[3], "r");
  	int i=0,j=0;
	while (!feof (file)){  
    	fscanf (file, "%f", &d); 

    	if(j==muestras){
    		j=0;
    		i++;
    	}
		ad[i][j++] = d;
    }
  	fclose (file);   

  	
	/*for(int i=0;i<segundos;i++){
		for(int j=0;j<muestras;j++){
			printf("%f\n",ad[i][j]);
		}
		//printf("\n");
	}
	*/
  	//*************************************************Hamming

	file = fopen (args[4], "r");
	int n=0;
	while (!feof (file)){  
    	fscanf (file, "%f", &d); 
		hamming_coef[n++] = d;
    }
  	fclose (file);   

  	/*for(int i=0;i<muestras;i++){
  		printf("%f\n",hamming_coef[i]);
  	}*/		

  	
	//*************************************************Epochs

	for(int i=0,k=0;i<segundos,k<(segundos*2);i++,k+=2){
		for(int j=0;j<muestras;j++){

			epochs[k][j]=ad[i][j];

			if(j<(muestras/2)) epochs[k+1][j]=	ad[i][j+muestras/2];
			else if(i<segundos-1)epochs[k+1][j] = ad[i+1][j-(muestras/2)];
				
		}
	}

	/*for(int i=0;i<segundos*2-1;i++){
		for(int j=0;j<muestras;j++){
			printf("%f\n",epochs[i][j]);
		}
	}*/

	
  	//************************************************Epochs + Hamming

	for(int i=0;i<segundos*2-1;i++){
		for(int j=0;j<muestras;j++){
			epochs[i][j] = epochs[i][j]*hamming_coef[j];
		}
	}

	/*for(int i=0;i<segundos*2-1;i++){
		for(int j=0;j<muestras;j++){
			printf("%f\n",epochs[i][j]);
		}
		break;
	}*/

	
	//************************************************DFT 

	complejo fft[segundos*2-1][muestras];

	for(int i=0;i<segundos*2-1;i++){
		for(int j=0;j<muestras;j++){
			fft[i][j].real=0.0;
			fft[i][j].imag=0.0;
		}
	}

	for(int i=0;i<segundos*2-1;i++){
		for(int k=0;k<muestras;k++){
			for(int n=0;n<muestras;n++){

				float real = cos(2*M_PI*k*n/muestras);
				float imag = sin(2*M_PI*k*n/muestras);

				fft[i][k].real+=(epochs[i][n]*(double)real);
				fft[i][k].imag+=(epochs[i][n]*(double)imag);				
			}
		}
	}
	
	//*************************************************FFT Magnitud

	float magnitud[segundos*2-1][muestras];

	for(int i=0;i<segundos*2-1;i++){
		for(int j=0;j<muestras;j++){

			float real = fft[i][j].real/muestras;
			float imag = fft[i][j].imag/muestras;

			magnitud[i][j]=sqrt(pow(real,2)+pow(imag,2));	
			//printf("%f\n",magnitud[i][j]);
		}
	}


	//*************************************************FFT Magnitud Promedio

	double magnitudFinal[muestras];
	for(int i=0;i<muestras;i++){
		magnitudFinal[i]=0.0;
	}

	for(int j=0;j<muestras;j++){
		for(int i=0;i<segundos*2-1;i++){
			magnitudFinal[j]+=magnitud[i][j];
		}		
	}

	for(int i=0;i<muestras;i++){
		magnitudFinal[i]/=(segundos*2-1);
		//printf("%f\n",magnitudFinal[i]);
	}

	printf("AV ALFA BAND [8-13Hz] %f", magnitudFinal[7] + magnitudFinal[8] + magnitudFinal[9] + magnitudFinal[10] + magnitudFinal[11]);
	
}

/*
C:\Users\marcos\Desktop\Trabajo Terminal\Trabajo terminal 2\capitulo 4\programa>gcc -o faa FAA.c

C:\Users\marcos\Desktop\Trabajo Terminal\Trabajo terminal 2\capitulo 4\programa>faa 256 60 EEG_FP1_F3_60S_FLOAT.txt HAMMING_FLOAT.txt
AV ALFA BAND [8-13Hz] faa= ln()-ln(4.826146)
*/
