#define MAX_BUF_SIZE 32

typedef struct serialize_buffer{

   char *b;  //buffer data
   int size; /*current size of buffer,it does not  
               mean that that the buffer will be fully occupied to the size indicated*/

   int next; //next available position to write data
}ser_buf_t;

/*initialize the serial buffer data*/
extern void init_ser_buf(ser_buf_t **buf);

/*serialize "data" of size "nbytes"*/
extern void ser_data(ser_buf_t **buf,void *data,int nbytes);

/*copy serialized data to the data structure used for serialization*/
extern void copy_ser_data(ser_buf_t **buf,char *data);

/*deserializing "data" of size "nbytes"*/
void deser_data(void *dest, ser_buf_t **buf, int nbytes);
