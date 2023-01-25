/******* Assignment 3 *******/
/*** Axel Vazquez Montano ***/

/* importing libraries */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/* function definitions */
void *thread_function(void *arg);
long double catalan_calculator(int n);
long double factorial(int n);

/* defining global variables */
long double *catalan_output_array;  // array contains the computed catalan numbers
unsigned int catalan_number;      // contains the target catalan number
unsigned int thread_quantity;     // contains the number of threads

/* data structure for load balancing */
typedef struct thread{
  unsigned int thread_number;  //thread identifier
}thread_parameters;

/* main function */
int main(int argc, char **argv){

  /* user input error checking */
  if(argc < 2 || argc > 3){
    printf("Please enter the catalan number and the number of threads\n"); // if user input is less or more than required echo feedback
    return -1;  //return error
  }

  /* variable initialization */
  catalan_number = atoi(argv[1]);   //passing and converting the catalan numbers to integers
  thread_quantity = atoi(argv[2]);    //passing and converting the number of threads to integers
  int i = 0;

  /* initializing threads */
  pthread_t thread_id[thread_quantity];   //declaring thread array with length of thread_number

  thread_parameters thread_array[thread_quantity];
  /* dynamically allocating the data structure */
  //thread_parameters *thread_array;     //declaring an array for thread parameters structure
  //thread_array = malloc(thread_quantity * sizeof(thread_parameters));

  catalan_output_array = malloc(catalan_number * sizeof(long double));  //allocating memory in the heap for array of computed catalan numbers

  for(i = 0; i < thread_quantity; i++){
    thread_array[i].thread_number = i+1;  // setting the thread numbers
  }

  /* creating threads */
  for(i = 0; i < thread_quantity; i++){
    pthread_create(&thread_id[i], NULL, thread_function, &thread_array[i]);
  }

  /* waiting for threads to finish */
  for(i = 0; i < thread_quantity; i++){
    pthread_join(thread_id[i], NULL); //same as wait()
  }

  /* initializing output file */
  FILE *catalan_output_file;
  catalan_output_file = fopen("catalan.dat","w");

  /* writing the results in the file */
  for(i = 1; i <= catalan_number; i++){
    fprintf(catalan_output_file, "%d = %Lf\n",i, catalan_output_array[i]);
  }

  fclose(catalan_output_file);  //closing file
  free(catalan_output_array);   //freeing space allocated for results

  return 0;
}

/* critical section for threads */
void *thread_function(void *arg){

  unsigned int number_thread = ((thread_parameters *)arg) -> thread_number;  //thread number identifier
  for(int i = number_thread; i <= catalan_number; i += thread_quantity){
    catalan_output_array[i] = catalan_calculator(i);    // calling the calculator function and storing the result in the heap
  }
  pthread_exit(NULL); // terminating thread execution
}
/* calculating the catalan numbers */
long double catalan_calculator(int n){

  long double catalan_value = 0;
  catalan_value = (factorial(2 * n))/(factorial(n + 1) * factorial(n)); // using factorial apporach
  return catalan_value;
}
/* factorial number calculator */
long double factorial(int n){

  long double f = 1;
  if(n < 0){
    printf("Error in factorial calculation: Value passed < 0 \n");
    return -1;
  }
  else{
    for(int i = 1; i <= n; i++){
      f = f * i;
    }
  }
  return f;
}
