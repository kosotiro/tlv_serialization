#include "ser_des_api.h"
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

void init_ser_buf(ser_buf_t **buf){
   
   //allocate memory for serialize data structure
   *buf = (ser_buf_t *)calloc(1, sizeof(ser_buf_t));

   if(*buf == NULL){
      perror("Mem alloc error for \"buf\"");
      exit(EXIT_FAILURE);
   }

   (*buf)->b = (char *)calloc(1, MAX_BUF_SIZE);
 
   if((*buf)->b == NULL){
       perror("Mem alloc error for \"b\" field");
       exit(EXIT_FAILURE);
   }

   //set it to the size that is currently occupies on heap
   (*buf)->size = MAX_BUF_SIZE;

   //it should point to the start of the buffer for writing
   (*buf)->next = 0;
}


void ser_data(ser_buf_t **buf, void *data, int nbytes){

   //calculate the available space for writing data
   int available = (*buf)->size - (*buf)->next;

   //indication if buffer has been resized  
   bool resized = false;

   /***********************************************************/
   /****buffer size is doubled until the available space*******/
   /****is enough to host the data requesting serialization****/
   /***********************************************************/
   while(available < nbytes){
       
       (*buf)->size = (*buf)->size * 2;
       available = (*buf)->size - (*buf)->next;
       resized =  true;
   }

   if(!resized){
       memcpy((*buf)->b + (*buf)->next, data, nbytes);
       (*buf)->next += nbytes;
       
   }
   else{
       //resize should be performed
       (*buf)->b = (char *)realloc((*buf)->b, (*buf)->size);

       if((*buf)->b == NULL){
           perror("Mem alloc error inside \"ser_data\" function");
           exit(EXIT_FAILURE);
       }
       

       memcpy((*buf)->b + (*buf)->next, data, nbytes);
       (*buf)->next += nbytes;
   }

}

void copy_ser_data(ser_buf_t **buf, char *data){
    
    //the actual length of data
    int size = (int) data[0];

    printf("Mphka sthn copy_ser_data %d\n",size);

    //allocate space for the current serialization structure
    *buf = (ser_buf_t *)calloc(1, sizeof(ser_buf_t));

    if(*buf == NULL){
        perror("Mem alloc error inside \"copy_ser_data\"");
        exit(EXIT_FAILURE);
    }

    /***************************************************/
    /***Allocate a buffer equal to the size indicated***/
    /***by the first element of serialized data*********/
    /***************************************************/
    (*buf)->b = (char *)calloc(1, size);
    (*buf)->size = size;
    (*buf)->next = 0;

    //fill in the buffer with data
    memcpy((*buf)->b, data, size);
}


void deser_data(void *dest, ser_buf_t **buf, int nbytes){

     memcpy(dest,(*buf)->b + (*buf)->next, nbytes);
     (*buf)->next += nbytes;
}


