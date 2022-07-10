#ifndef DEBUGLOGGER
#define DEBUGLOGGER

#include <string>
#include <vector>
#include <queue>
#include <iostream>

    namespace debug_logger
    {
        #ifdef DEBUG_VAR
    
            // variable used for spawning a thread to read from logging queue
            const int debug_thread = 1;
            
            //NOTE:
            // using "inline" helped to get rid of "multiple definition of 'function'" error that happens when template functions are in a header file-
            // and that header file is called from multiple places. Inline helps the compiler to know that it should only be created once.

            // fifo used for storing strings to be printed and/or logged
            inline std::queue<std::string> logger_queue;

            // function used to replace "%v" variables in print string
            inline std::string replace_identifiers(std::vector<std::string> &print_vec)
            {
                size_t index;
                size_t vec_index = 1;

                std::string print_str = print_vec[0]; // first item stored in vector is print string

                while(((index = print_str.find("%v")) != std::string::npos)) // find %v in print string
                {
                    if(vec_index+1 > print_vec.size()) break;

                    else
                    {
                        print_str.replace(index, 2, print_vec[vec_index]); // replace "%v" (str len 2) with variable stored in print vector
                        vec_index++;
                    }
                }

                return print_str; // return final string to be printed and/or logged
            }

            // template for toString because "std::to_string()" isn't available for template inputs; need to clarify input
            template<typename T>
            inline std::string toString(T t) {
                return std::to_string(t);
            }

            // template for toString for "char*" inputs; no changed needed for these inputs
            inline std::string toString(const char* t) {
                return t;
            }

            // template for toString for "string" inputs; no changed needed for these inputs
            inline std::string toString(const std::string& t) {
                return t;
            }

            // variadic template that is called whne there is no variables left; all that is left is the vector
            inline void print_internal(std::vector<std::string> &vec)
            {
                std::string fullstring = replace_identifiers(vec); // replace "%v" with variables included in print() function sequentially
                logger_queue.push(fullstring); //push to queue
                //std::cout << fullstring << "\n";
            }

            // variadic template that is called whne there is one variable left (and the vector)
            template<typename T>
            inline void print_internal(std::vector<std::string> &vec, T var1)
            {
                vec.push_back(toString(var1));
                print_internal(vec);
            }

            // variadic template function called from "print()" and from itself. Takes variable inputs, converts them to string, then pushes converted variables to vector
            template <typename T, typename... Types>
            inline void print_internal(std::vector<std::string> &vec, T var1, Types... var2)
            {
                vec.push_back(toString(var1));
                print_internal(vec, var2...);
            }

            // empty print function for variadic template "print()" function when there are no more input variables to the variadic template
            // aka input = NULL
            inline void print()
            {

            }

            // variadic template function for printing/logging "debug_logger::print()" function.
            // Required format: debug_logger::print("String format with var1 = %v, and var2 = %v", var1, var2);
            // Variadic functions are recursive so a vector was created to hold: 
                // - first indext vector[0] holds the print string
                // - all variables converted to string
            template <typename T, typename... Types>
            inline void print(T var1, Types... var2)
            {
                std::vector<std::string> vec;

                print_internal(vec, var1, var2...);

                vec.clear();
            }
    

        #else

            const int debug_thread = 0;

            template <typename T, typename... Types>
            void print(T var1, Types... var2)
            {

            }

        #endif
    }

#endif