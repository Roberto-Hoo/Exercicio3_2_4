/*
 * Faça um código MPI para computação do produto interno entre dois vetores
 * Para a comunicação entre os processos, utilize apenas as rotinas MPI_Send e MPI_Recv.
 * O processo 0 deve receber os resultados parciais dos demais processos e escrever na tela
 * o valor computado do produto interno. */

 //C?digo: SendRecvArray.cpp
#include <stdio.h>
#include <cstdlib> // contains function prototype for rand
#include <ctime>
// API MPI
#include <mpi.h>

int world_size;
int world_rank;
int errorcode;
double soma, soma0, soma1; //Privado, cada processo tem a variável soma
double media = 0;
int const n = 11; //tamanho do vetor
int mod; // mod (resto da divisão)
double v[n], w[n];
int k; // tamanho da computação em cada processo

char const s[4] = "6.1";
bool debug = true; // debug = 1;

/*
 * Retorna um numero aleatorio inteiro entre minimo e o maximo(inclusive os extremos)
 */
int numeroAleatorio(int minimo, int maximo) {
	return minimo + rand() % (maximo - minimo + 1);
}

int main() {
	srand((unsigned)time(0)); //para gerar numeros aleatórios reais.

	// Inicializa o MPI
	MPI_Init(NULL, NULL);

	// numero total de processos
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// ID (rank) do processo
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	k = int(n / world_size);
	mod = n % world_size;
	if ((debug) and (world_rank == 0))
		printf("Valor de k = %d e o valor de mod eh %d\n", k, mod);


	// verifica o num. de processos
	if (world_size < 2) {
		printf("ERRO! Numero de processos deve ser maior que 1.\n");
		errorcode = -137;
		MPI_Abort(MPI_COMM_WORLD, errorcode);
	}


	if (world_rank == 0) {
		for (int i = 0; i < n; i++) {
			v[i] = (double)numeroAleatorio(1, 20) + \
				(double)numeroAleatorio(0, 0) / 1;

			w[i] = (double)numeroAleatorio(1, 20) + \
				(double)numeroAleatorio(0, 0) / 1;
		}

		printf("Vetor v feito pelo processo 0, antes de enviar para o processo 1: v = ( ");
		for (int i = 0; i < n; i++) {
			printf("%6.2f ", v[i]);
		}
		printf(")");

		printf("\nVetor w do processo 0, antes de enviar para os outros processos: w = ( ");
		for (int i = 0; i < n; i++) {
			printf("%6.2f ", w[i]);
		}
		printf(")");

		//soma0 = 0;
		//for (int i = 0; i < (int) n / 2; i++) {
		//    soma0 += v[i];
		//}

		//printf("\nA soma dos primeiros %d termos de v eh %6.2f\n", n / 2, soma0);
		int ultimo;
		for (int j = 1; j < mod; j++) {
			MPI_Send(&v[(k + 1) * j], k + 1, MPI_DOUBLE, j, 0, MPI_COMM_WORLD);
			MPI_Send(&w[(k + 1) * j], k + 1, MPI_DOUBLE, j, 0, MPI_COMM_WORLD);
		}
		
		ultimo = (k + 1) * mod;
		
		for (int j = 0; j < world_size - mod; j++) {
			MPI_Send(&v[ultimo + k * j], k, MPI_DOUBLE, j + mod, 0, MPI_COMM_WORLD);
			MPI_Send(&w[ultimo + k * j], k, MPI_DOUBLE, j + mod, 0, MPI_COMM_WORLD);
		}


	}
	else {

		MPI_Recv(&w[0], n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		printf("\nApos receber do processo 0 o vetor w no processo 1 eh: w = ( ");
		soma1 = 0;  // nem precisa zerar, j? inicia zerado
		for (int i = 0; i < n; i++) {
			printf("%6.2f ", w[i]);
		}
		for (int i = (int)n / 2; i < n; i++) {
			soma1 += w[i];
		}
		printf(")");
		printf("\nA soma dos ultimos %d termos de w eh %6.2f\n", n - n / 2, soma1);

		MPI_Send(&soma1, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}

	if (world_rank == 0) {
		MPI_Recv(&soma, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		soma = soma + soma0;
		media = (double)(soma / (double)n);
		printf("\nA soma total eh %6.2f e a media eh %6.2f\n", soma, media);
	}

	// Finaliza o MPI
	MPI_Finalize();

	return 0;
}
/*
 * Rode com mpic++ E311.cpp -o E311
 * depois mpirun -np 4 E311
 * mpirun -np 8 -use-hwthread-cpus E311
 * mpirun -np 9 -oversubscribe E311
*/