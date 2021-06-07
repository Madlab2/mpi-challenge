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
	if (rank != 0) { //slave
		//_________________________________________________________________________________________________________
		//                              S  L   A   V   E 
		//_________________________________________________________________________________________________________
		//slaveMerge(std::make_shared<MPI_Status> status, std::make_shared<MPI_Length> length, std::make_shared<MPI_Rank> rank, std::make_shared<int> world_size);

		//Some info output
		std::cout << "[Worker] Hello from slave " << rank << ". World-size: " << world_size << ". Running on " << name << std::endl;


		auto mSort = std::make_unique<MergeSort>();

		int message_size;
		int num_packages = 1;

		//reveive number of packages to be received from master
		//MPI_Recv(&num_packages, 1, MPI_INT, 0, 555, MPI_COMM_WORLD, &status);

		//construct vector with given length
		//std::vector<std::string> vec(num_words);
		std::vector<std::string> words_to_sort;
		std::cout << "[Worker] Slave " << rank << " running on " << name << " is going to receive " << num_packages << " packages." << std::endl;


		MPI_Recv(&message_size, 1, MPI_INT, 0, 666, MPI_COMM_WORLD, &status);


		std::cout << "[Worker] Length of word received: " << message_size << std::endl;
		char *buf = new char[message_size];		

		MPI_Recv(buf, message_size, MPI_CHAR, 0, 777, MPI_COMM_WORLD, &status);

		int begin_word = 0;

		for (int character = 0; character < message_size; character++) {
			
			//";" is seperator marking begin of a word (";abc;...;wtf;")
			if (buf[character] == ';') {
				
				if(character != 0) {

					int index_new_word = 0;
					int size_new_word = character - begin_word;
					int end_word = character - 1;

					char *new_word = new char[size_new_word];
					for (int word_character = begin_word; word_character <= end_word; word_character++) {
						
						new_word[index_new_word] = buf[word_character];
						index_new_word++;
					}
					
					if(new_word[size_new_word] != '\0')
									      {
									      	std::cout << "hier -> Worker" << rank << "  " << new_word[size_new_word] << " == " << size_new_word << std::endl;
									      }
						
						
					words_to_sort.push_back(std::string(new_word));
					delete[] new_word;
				}
				//needs to be set in first iteration in order to "skip" the first ";"
				begin_word = character + 1;
			}

		}
		
		delete[] buf;
		
		//std::cout << "[Worker] Received word: " << std::string(buf) << std::endl;

		
		std::cout << "[Worker] Slave " << rank << " running on " << name << ": Length and Data received. Starting local merge..." << std::endl;
		std::cout << words_to_sort[28] << std::endl;
		//ptr to vec (move constructor)
		auto words_to_sort_ptr = std::make_shared<std::vector<std::string>>(std::move(words_to_sort));
		std::cout << words_to_sort_ptr->at(28) << std::endl;
		//merge sort the vector received from master
		mSort->mergeSort(words_to_sort_ptr, 0, words_to_sort_ptr->size() - 1);

		std::cout << words_to_sort_ptr->at(28) << std::endl;
		std::cout << "[Worker] Slave " << rank << " running on " << name << ": merge sorted." << std::endl;

		//sending back result to Master

		//MPI_Send(&num_packages, 1, MPI_INT, 0, 555, MPI_COMM_WORLD);

		std::cout << "[Worker] Slave " << rank << " running on " << name << ": sent back length to Master." << std::endl;		

		//std::cout << "[Master] sent length: " << word_size << " to slave " << slave_id << std::endl;

		std::stringstream buffer;
		buffer << ";";

		for (std::string word : *words_to_sort_ptr) {
			
			//";" is seperator marking begin of a word (";abc;...;wtf;")
			buffer << word << ";";

		}

		std::string temp = buffer.str();
		message_size = temp.size() + 1;

		char * to_send = new char[message_size];
		strcpy(to_send, temp.c_str());

		MPI_Send(&message_size, 1, MPI_INT, 0, 666, MPI_COMM_WORLD);
		
		//send chars of word in ONE char array
		MPI_Send(to_send, message_size, MPI_CHAR, 0, 777, MPI_COMM_WORLD);

		//std::cout << "[Worker] sent word " << to_send << " to master " << slave_id << std::endl;

		delete[] to_send;

		std::cout << "[Worker] Slave " << rank << " running on " << name << ": sent back data to Master." << std::endl;
	}



	//_________________________________________________________________________________________________________
	//                              M   A   S   T   E   R    
	//_________________________________________________________________________________________________________

	else {    //master, menu shall run only on ONE of all nodes (=> node 0)

		std::string input;

		while (true) {

			std::cout << "[Master] Enter a path to a text file below or 'exit':" << std::endl;

			std::getline(std::cin, input);

			if (input == "exit") {

				std::cout << "[Master] exiting.." << std::endl;
				break;

			}
			else {

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
				for (int i = 0; i < num_slaves; i++) {

					int begin = boundaries.at(i).first;
					int end = boundaries.at(i).second;

					int length = end - begin +1;

					// ID 0 is Master, slaves begin from 1 (hence the offset)
					int slave_id = i + 1;
					int to_send_counter = 0;
					// int word_size = 0;

					std::cout << "[Master] sending " << length << " chars to slave " << slave_id << std::endl;

					// MPI_Send(&length, 1, MPI_INT, slave_id, 555, MPI_COMM_WORLD);

					//std::cout << "[Master] sent length: " << word_size << " to slave " << slave_id << std::endl;

                    			std::stringstream buffer;
					
                    			buffer << ";";

                    			for (auto word = begin; word <= end; word++) {
                      
                        		//";" is seperator marking begin of a word (";abc;...;wtf;")
                        		buffer << string_to_sort[word] << ";";

                    			}

                    			std::string temp = buffer.str();

                    			int message_size = temp.size() + 1;
					std::cout << "[Master] MPI " <<  message_size << std::endl;
                    			char * to_send = new char[message_size];
					
                    			strcpy(to_send, temp.c_str());
		    			std::cout << "[Master] MPI " <<  message_size  << " to slave " << slave_id << std::endl;
                    			// send length of whole array
					message_size = message_size +1;
                    			MPI_Send(&message_size, 1, MPI_INT, slave_id, 666, MPI_COMM_WORLD);

		    			//send word in ONE char array
		    			MPI_Send(to_send, message_size, MPI_CHAR, slave_id, 777, MPI_COMM_WORLD);

		   			//std::cout << "[Master] sent word " << to_send << " to slave " << slave_id << std::endl;

		  			delete[] to_send;
		    			std::cout << "[Master] sent " << length << "words to slave " << slave_id << std::endl;

				}

				//storage for result vectors
				//std::deque<std::vector<std::string>> vecs_from_slaves(num_slaves);
				std::deque<std::vector<std::string>> vecs_from_slaves;


				// receive sorted vectors from slaves (do we wait for each sender synchronously?)
				for (int i = 0; i < num_slaves; i++) {

					int message_size = 0;

					// ID 0 is Master, slaves begin from 1 (hence the offset)
					int slave_id = i + 1;

					//receive number of words from slaves
					// MPI_Recv(&num_words, 1, MPI_INT, slave_id, 555, MPI_COMM_WORLD, &status);


					//create vector for slave result, to be stored in sub_vecs
					std::vector<std::string> words_to_sort;
					// std::cout << "[Master] Going to receive " << num_words << " words." << std::endl;
					MPI_Recv(&message_size, 1, MPI_INT, slave_id, 666, MPI_COMM_WORLD, &status);


					char * buf = new char[message_size];

					//std::cout << "[Worker] Length of word received: " << word_size << std::endl;

					MPI_Recv(buf, message_size, MPI_CHAR, slave_id, 777, MPI_COMM_WORLD, &status);

                    			int begin_word = 0;     

					for (int character = 0; character < message_size; character++) {
			
                        		//";" is seperator marking begin of a word (";abc;...;wtf;")
                        		if (buf[character] == ';') {
                            
                            			if(character != 0) {

                                		int index_new_word = 0;
                                		int size_new_word = character - begin_word;
                                		int end_word = character - 1;

                                		char *new_word = new char[size_new_word];

                                		for (int word_character = begin_word; word_character <= end_word; word_character++) {
                                    
                                    			new_word[index_new_word] = buf[word_character];
                                    			index_new_word++;
                                		}

                                	words_to_sort.push_back(std::string(new_word));
					delete [] new_word;
                            	}
                            //needs to be set in first iteration in order to "skip" the first ";"
                            begin_word = character + 1;
                        }

                    }
					//std::cout << "[Worker] Received word: " << std::string(buf) << std::endl;

					//vec.emplace(vec.begin() + word, std::move(std::string(buf)));
					//vec.assign()

					
					delete[] buf;



					//TODO: revise, do not copy
					//vecs_from_slaves.emplace(vecs_from_slaves.begin() + i, std::move(vec));
					vecs_from_slaves.push_back(words_to_sort);
					std::cout << "[Master] received data from slave " << slave_id << std::endl;
				}


				// merge recursively backwards on master node using void merge() (eg. from 8 sorted vecs to a single one)
				mSort->merge_back(vecs_from_slaves);

				std::cout << "[Master] merged slave results." << std::endl;

				// extract result. Is the first vector in a std::deque of vectors
				auto result = vecs_from_slaves.front();

				end = std::chrono::system_clock::now();

				elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

				std::cout << "[Master] (time elapsed: " << elapsed.count() << "ms) Finished distributed merge!" << std::endl;

				std::cout << "[Master] Press 'p' to print the result, 'f' to write it to file, 's' to skip" << std::endl;

				std::getline(std::cin, input);

				if (input == "p") {

					std::cout << "[Master] result.size() = " << result.size() << std::endl;
					for (int i = 0; i < result.size(); i++) {

						std::cout << result.at(i) << std::endl;

					}
					//IO::printVector(std::make_shared<std::vector<std::string>>(result));

				}
				else if (input == "f") {

					IO::printVectorToFile(std::make_shared<std::vector<std::string>>(result));

				}

			}

		}
	}

	MPI_Finalize();

	return 0;

}
