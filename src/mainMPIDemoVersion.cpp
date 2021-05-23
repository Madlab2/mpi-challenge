// ConsoleApplication3.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//Consoleeingabe: mpicc main.cpp
//

#include <iostream>
#include <string>
#include <algorithm>
#include <memory>
#include <vector>
#include <mpi.h>
#include <studio.h>
#include <time.h>


#include "MergeSort.hpp"
#include "InOutFuncs.hpp"



//TODO: use smart pointers instead of raw

int main() {

    auto mSort = std::make_unique<MergeSort>();

	//initialize MPI
	int word_rank;
	int world_size;

	//MPI starts
	MPI_Init(&argc, &wargv);
	//get current process id
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	//get number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	std::string input;
    
    std::cout << "[Menu] Enter a pah to a text file below or 'exit':" << std::endl;

    while(true) {

        std::getline(std::cin, input);

        if (input == "exit") {

            std::cout << "[Menu] exiting.." << std::endl;
            break;

        } else {

            std::cout << "[Menu] starting merge sort on file " << input << " ..." << std::endl;
            
            auto vec = std::make_shared<std::vector<std::string>>(IO::readStringsFromFile(input));
			
			//Divide the array in equal parts
			int size = n / world_size;

			// Send each subarray to each process
			int ∗sub_array = malloc(size ∗ sizeof(int));
			MPI_Scatter(vec, size, MPI_INT, sub_array, size, MPI_INT, 0, MPI_COMM_WORLD);
			
			 // Start Timer Sorting Parallel
			 parallelTime1 = MPI_Wtime();
			
			//Perform the mergesort on each process
			int ∗tmp_array = malloc(size ∗ sizeof(int));
			
			//mergeSort(sub_array, tmp_array, 0, (size − 1));

            mSort->mergeSort(sub_array, tmp_array, vec->size() - 1);

			std::cout << "[Menu] finished! Result will be printed below: \n\n\n";

			IO::printVector(vec);

			int ∗sorted = NULL;
			 if (world_rank == 0) {
				 sorted = malloc(n ∗ sizeof(int));
				
			}
			
				 // Start Timer Sorting Parallel
			 parallelTime2 = MPI_Wtime();
			 MPI_Gather(sub_array, size, MPI_INT, sorted, size, MPI_INT, 0, MPI_COMM_WORLD);

			 /********** Make the final mergeSort call **********/
			 if (world_rank == 0) {

				 int *other_array = malloc(n * sizeof(int));
				 mergeSort(sorted, other_array, 0, (n - 1));

				 /********** Display the sorted array **********/
				 printf("This is the sorted array: ");
				 for (c = 0; c < n; c++) {

					 printf("%d ", sorted[c]);

				 }

				 printf("\n");
				 printf("\n");

				 /********** Clean up root **********/
				 free(sorted);
				 free(other_array);

			 }

			 /********** Clean up rest **********/
			 free(original_array);
			 free(sub_array);
			 free(tmp_array);

			 /********** Finalize MPI **********/
			 MPI_Barrier(MPI_COMM_WORLD);
			 MPI_Finalize();

		}

            //will automatically delete vec a this point as going out of scope


    }

}


