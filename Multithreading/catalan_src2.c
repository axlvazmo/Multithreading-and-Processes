/******* Assignment 3 *******/
/*** Axel Vazquez Montano ***/

/* importing libraries */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/* function definitions */
void *thread_function(void *arg);
long double catalan_number_opperation(int c);

long double *catalan_output_array;

/* data structure for load balancing */
typedef struct thread{
  int thread_number;
  int computation_number;
  int *thread_catalan_numbers;
}thread_parameters;

/* main function */
int main(int argc, char **argv){

  int i = 0;
  int j = 0;

  /* user input error checking */
  if(argc < 2 || argc > 3){
    printf("Please enter the catalan number and the number of threads\n"); // if user input is less or more than required echo feedback
    return -1;  //return error
  }

  int thread_quantity = atoi(argv[2]);    //passing and converting the number of threads to integers
  int catalan_number = atoi(argv[1]);   //passing and converting the catalan numbers to integers

  pthread_t thread_id[thread_quantity];   //declaring thread array with length of thread_number
  thread_parameters thread_array[thread_quantity];     //declaring an array for thread parameters structure

  catalan_output_array = malloc(catalan_number * sizeof(long double));

  for(i = 1; i <= thread_quantity; i++){
    thread_array[i].thread_catalan_numbers = malloc((catalan_number/thread_quantity)* sizeof(int));
    thread_array[i].thread_number = i;
    for(j = 0; j < (catalan_number/thread_quantity); j++){
      thread_array[i].thread_catalan_numbers[j] = (thread_quantity * j) + i;
    }
  }

  for(i = 1; i <= thread_quantity; i++){
    printf("%d\n", thread_array[i].thread_number = i);
    for(j = 0; j < (catalan_number/thread_quantity); j++){
      thread_array[i].thread_catalan_numbers[j] = (thread_quantity * j) + i;
    }
  }

  /* creating threads */
  for(i = 0; i < thread_quantity; i++){
    pthread_create(&thread_id[i], NULL, thread_function, &thread_array[i]);
  }

  /* setting up file */
  FILE *catalan_output_file;    //declaring pointer to file for final output
  FILE *thread_output_file;     //declaring pointer file for thread output
  char catalan_file_name[12];
  char file_holder;   //variable holds the content of files
  catalan_output_file = fopen("catalan.dat", "w");

  /* waiting for threads to finish */
  for(i = 0; i < thread_quantity; i++){
    pthread_join(thread_id[i], NULL);   //same as wait()
    sprintf(catalan_file_name, "thread%d.dat",i);   //passing the file name to specified buffer
    thread_output_file = fopen(catalan_file_name, "r");   //opening respective thread files
    if(thread_output_file == NULL){
      printf("Failed to open thread output file %s\n", catalan_file_name);  //echo error if there are no files
      return -1;
    }
    else{
      while((file_holder = fgetc(thread_output_file)) != EOF){
        fputc(file_holder, catalan_output_file);  //while is not the end of the file, put the contents of the thread file into the final output file
      }
    }
    fclose(thread_output_file);
  }
  fclose(catalan_output_file);

  for(i = 0; i < catalan_number; i++){
    printf("******* %Lf *********\n", catalan_output_array[i]);
  }

  free(catalan_output_array);
  for(i == 0; i < thread_quantity; i++) free(thread_array[i].thread_catalan_numbers);
  return 0;
}

/* funcrion for writing each thread catalan number sequence */
void *thread_function(void *arg){

  /* getting member values from structure */
  int number_thread = ((thread_parameters *)arg) -> thread_number;  //thread number identifier
  int number_computations = ((thread_parameters *)arg) -> computation_number;  //number of catalan numbers to compute
  int *thread_cat_num = ((thread_parameters *)arg) -> thread_catalan_numbers; //which catalan numbers each thread must compute

  char file_name[12];   //buffer for thread output file name
  FILE *thread_output_file;   //setting pointer to the thread output file
  sprintf(file_name, "thread%d.dat", number_thread);   //passing the respective file name to the buffer
  thread_output_file = fopen(file_name, "w");

  long double thread_output_value;
  for(int i = 0; i < number_computations; i++){
    thread_output_value = catalan_number_opperation(i);   //calling the opperation to compute catalan sequence
    catalan_output_array[thread_cat_num[i]] = thread_output_value;
    fprintf(thread_output_file, "%Lf\n", thread_output_value);    //writing catalan sequence to file
  }
  fclose(thread_output_file);
}

/* function for computing the catalan number sequence */
long double catalan_number_opperation(int c){

  if(c <= 1) return 1;
  long double catalan_value = 0;
  for(int i = 0; i < c; i++){
    catalan_value += catalan_number_opperation(i)*catalan_number_opperation(c-i-1);   //using recursive method to compute the catalan sequence
  }
  return catalan_value;
}
