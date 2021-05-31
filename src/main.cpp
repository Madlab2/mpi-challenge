#include <iostream>
#include <chrono>
#include <string.h>

#include <memory>
#include <vector>
#include <deque>


#include <mpi.h>

#include "MergeSort.hpp"
#include "InOutFuncs.hpp"
#include "HelperFuncs.hpp"





//TODO: How to pass MPI context to other functions? 
int main(int argc, char **argv) {
    
    //basic mpi functionality to demostrate it is working
    int rank;
    int world_size;
    int length;
    char name[80];

    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Get_processor_name(name, &length);

    
    // TODO: slaves will turn off after one iteration!!! (there is no while loop)
    // implement using shutwodn flag sent from master
    if(rank != 0) { //slave
        //_________________________________________________________________________________________________________
        //                              S  L   A   V   E 
        //_________________________________________________________________________________________________________
        //slaveMerge(std::make_shared<MPI_Status> status, std::make_shared<MPI_Length> length, std::make_shared<MPI_Rank> rank, std::make_shared<int> world_size);

        //Some info output
        std::cout << "[Worker] Hello from slave " << rank << ". World-size: " << world_size << ". Running on " << name << std::endl;


        auto mSort = std::make_unique<MergeSort>();

        int length_word = 0;
        int num_words = 0;

        //reveive number of words to be received from master
        MPI_Recv(&num_words, 1, MPI_INT, 0, 666, MPI_COMM_WORLD, &status);

        //construct vector with given length
        std::vector<std::string> vec(num_words);

        std::cout << "[Worker] Slave " << rank << " running on " << name << std::endl;
        

        for (int word = 0; word < num_words; word++) {

            MPI_Recv(&length_word, 1, MPI_INT, 0, 666, MPI_COMM_WORLD, &status);
            
            char buf[length_word];
            MPI_Recv(&buf, 1, MPI_CHAR, 0, 777, MPI_COMM_WORLD, &status);

			std::cout << "[Worker] String: " << std::string(buf) << std::endl;
			vec.emplace(vec.begin() + word, std::move(std::string(buf)));
		}

		
        //reveive vector from master
        //vec_ptr points to first element
        //MPI_Recv(&vec, length, MPI_CHAR, 0, 777, MPI_COMM_WORLD, &status);
        //char test;
        
        //MPI_Recv(buf, 1, MPI_CHAR, 0, 777, MPI_COMM_WORLD, &status);

        std::cout << "[Worker] Slave " << rank << " running on " << name << ": Length and Data received." << std::endl;

        //std::cout << "[Worker] string test: " << test << std::endl;

        //ptr to vec (move constructor)
        auto vec_ptr = std::make_shared<std::vector<std::string>>(std::move(vec));

        std::cout << "[Worker] Slave " << rank << " running on " << name << ": ptr to vec constructed. Starting local merge..." << std::endl;
       
        //merge sort the vector received from master
        mSort->mergeSort(vec_ptr, 0, length - 1);

        std::cout << "[Worker] Slave " << rank << " running on " << name << ": merge sorted." << std::endl;


        MPI_Send(&length, 1, MPI_INT, 0, 666, MPI_COMM_WORLD);

        std::cout << "[Worker] Slave " << rank << " running on " << name << ": sent back length." << std::endl;
        //send back the sorted vector to master
        //dereference vec_ptr gives content of vec at element 0
        MPI_Send(&vec, length, MPI_INT, 0, 777, MPI_COMM_WORLD);

        std::cout << "[Worker] Slave " << rank << " running on " << name << ": sent back data." << std::endl;
    
    } 
    
    
    
    //_________________________________________________________________________________________________________
    //                              M   A   S   T   E   R    
    //_________________________________________________________________________________________________________
    
    else {    //master, menu shall run only on ONE of all nodes (=> node 0)

        std::string input;

        while(true) {

            std::cout << "[Master] Enter a path to a text file below or 'exit':" << std::endl;

            std::getline(std::cin, input);

            if (input == "exit") {

                std::cout << "[Master] exiting.." << std::endl;
                break;

            } else {

                //TODO: extract this whole scope to function void masterMerge(std::string input)

                std::cout << "[Master] Reading in file " << input << " ..." << std::endl;
                
                
                std::vector<std::string> string_to_sort;
                
                auto start = std::chrono::system_clock::now();
                IO::readStringsFromFile(input, string_to_sort);
                auto end = std::chrono::system_clock::now();
                

                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
                
                std::cout << "[Master] (time elapsed: " << elapsed.count() << "ms) Starting distributed merge sort on file " << input << " ..." << std::endl;
                


                //_________________________________________________________________________________________________________
                //                              M   A   S   T   E   R --- M   E   R   G   E 
                //_________________________________________________________________________________________________________
                //auto merged_vec = masterMerge(std::shared_ptr<std::vector<std::string>> vec_to_merge, 
                //    std::make_shared<MPI_Status> status, std::make_shared<MPI_Length> length, std::make_shared<MPI_Rank> rank, std::make_shared<int> world_size);


                start = std::chrono::system_clock::now();

                auto mSort = std::make_unique<MergeSort>();

                // One master, remaining nodes are slaves
                int num_slaves = world_size - 1;

                // split vector
                //boundaries.size() is guaranteed to be euqal to world size (or ==1 if world_size < 1)
                auto boundaries = split_even(string_to_sort.size(), num_slaves);

                std::cout << "[Master] boundaries calculated. " << std::endl;


                //Send size and contents of vectors to slave nodes
                for(int i = 0; i < num_slaves; i++) {
                    
                    int begin = boundaries.at(i).first;
                    int end = boundaries.at(i).second;

                    int length = end - begin + 1;

                    // ID 0 is Master, slaves begin from 1 (hence the offset)
                    int slave_id = i + 1;

                    int word_size = 0;
                    
                    std::cout << "[Master] sending " << length << " words to slave " << slave_id << std::endl;
                    for (int word = begin; word <= end; word++) {

                        //send length of word
                        word_size = string_to_sort.at(word).size();
                        MPI_Send(&word_size, 1, MPI_INT, slave_id, 666, MPI_COMM_WORLD);

                        //send chars of word in ONE char array
                        MPI_Send(string_to_sort.at(word).c_str(), word_size, MPI_CHAR, slave_id, 1, MPI_COMM_WORLD);
                        
                    }
                    
                    //char test = 'h';

                    //MPI_Send(&length, 1, MPI_INT, slave_id, 666, MPI_COMM_WORLD);
                    std::cout << "[Master] sent length " << length << " to slave " << slave_id << std::endl;
                    //MPI_Send(string_to_sort[begin].c_str(), length , MPI_CHAR, slave_id, 777, MPI_COMM_WORLD);
                    //MPI_Send(&test, 1 , MPI_CHAR, slave_id, 777, MPI_COMM_WORLD);
                    std::cout << "[Master] sent data to slave " << slave_id << std::endl;
                }

                //storage for result vectors
                std::deque<std::vector<std::string>> vecs_from_slaves(num_slaves);


                // receive sorted vectors from slaves (do we wait for each sender synchronously?)
                for(int i = 0; i < num_slaves; i++) {

                    int length = 0;

                    // ID 0 is Master, slaves begin from 1 (hence the offset)
                    int slave_id = i + 1;

                    //reveive length of vector from master
                    MPI_Recv(&length, 1, MPI_INT, slave_id , 666, MPI_COMM_WORLD, &status);

                    //create vector for slave result, to be stored in sub_vecs
                    std::vector<std::string> vec(length);
                    //auto temp_vec = std::make_shared<std::vector<std::string>> (vec);
                    
                    //receive sorted vector from slave
                    MPI_Recv(&vec, length, MPI_CHAR, slave_id, 777, MPI_COMM_WORLD, &status);
                    std::cout << "[Master] received lenght from slave " << slave_id << std::endl;

                    //TODO: revise, do not copy
                    vecs_from_slaves.emplace(vecs_from_slaves.begin() + i, std::move(vec));
                    std::cout << "[Master] received data from slave " << slave_id << std::endl;
                }


                // merge recursively backwards on master node using void merge() ( eg. from 8 sorted vecs to a single one)
                mSort->merge_back(vecs_from_slaves);

                std::cout << "[Master] merged slave results." << std::endl;

                // extract result. Is the first vector in a std::deque of vectors
                auto result = vecs_from_slaves.front();              

				end = std::chrono::system_clock::now();

                elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

                std::cout << "[Master] (time elapsed: " << elapsed.count() << "ms) Finished distributed merge!" << std::endl;

            }

        }
    }

    MPI_Finalize();

    return 0;    

}
