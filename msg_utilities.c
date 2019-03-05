#include "msg_utilities.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

/******************************************************************
 ***********Below message
0                   1                   2                   3
   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                          Length (52)                        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | Type (RTM_NEWQDISC)           | Flags (NLM_F_EXCL |         |
   |                               |NLM_F_CREATE | NLM_F_REQUEST)|
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                      Sequence Number(arbitrary number)      |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                        Process ID (0)                       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |Family(AF_INET)|  Reserved1    |         Reserved1           |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                     Interface Index  (4)                    |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                      Qdisc handle  (0x1000001)              |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                     Parent Qdisc   (0x1000000)              |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                        TCM Info  (0)                        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |            Type (TCA_KIND)   |           Length(4)          |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                        Value ("pfifo")                      |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |            Type (TCA_OPTIONS) |          Length(4)          |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                        Value (limit=100)                    |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

********************************************************************/
void encode(ser_buf_t **buf){

   //initialize the buffer
   init_ser_buf(buf);

   //serialize "length" field of message
   unsigned int length = 52;
   ser_data(buf, &length, 4);


   //serialize "type" of message
   MsgType type = RTM_NEWQDISC;
   ser_data(buf, &type, 2);

   //serialize "Flags"
   Flags flags = NLM_F_EXCL | NLM_F_CREATE | NLM_F_REQUEST;
   ser_data(buf, &flags, 2);

   //serialize "sequence number"
   unsigned int seqn = 1000;
   ser_data(buf, &seqn, 4);

   //serialize process ID
   int pid = (int) getpid();
   ser_data(buf, &pid, 4);


   //serialize Family (AF_INET)
   unsigned int family = AF_INET;
   ser_data(buf, &family, 1);

   

   //Reserved1 field, fill in with zeroes
   unsigned int reserved = 0;
   ser_data(buf, &reserved, 3);

   
   //Interface Index
   unsigned int ifIndex = 5;
   ser_data(buf, &ifIndex, 4);

   
   //Qdisc handle  (0x1000001)
   unsigned int Qdisc = 0x1000001;
   ser_data(buf, &Qdisc, 4);

   
   //Parent Qdisc   (0x1000000)
   unsigned int PQdisc = 0x1000000;
   ser_data(buf, &PQdisc, 4);


   //TCM Info  (0)
   unsigned int TCMinfo = 0;
   ser_data(buf, &TCMinfo, 4);


   //Start serialization of TLVs
   //serialize its type
   Tlvtype tlv_type = TCA_KIND;
   ser_data(buf, &tlv_type, 2);

   //serialize its length
   length = 4;
   ser_data(buf, &length, 2);


   //serialize its value
   unsigned int value = PFIFO;
   ser_data(buf, &value, 4);

   //serialize its type
   tlv_type = TCA_OPTIONS;
   ser_data(buf, &tlv_type, 2);

   //serialize its length
   length = 4;
   ser_data(buf, &length, 2);

   //serialize its value
   value = 100;
   ser_data(buf, &value, 4);

   
}





//decode function on server side
void decode(char *data){

   //buffer for serialization
   ser_buf_t *buf =(ser_buf_t *)malloc(sizeof(ser_buf_t));

   //initialize the fields of this buffer
   buf->b = (char *)malloc(*((int *)data));
   buf->next = 0;
   buf->size = *((int *)data);

   printf("length %d\n",*((int *)data));

   if( buf != NULL ){

      memcpy(buf->b,data,*((int *)data));

      unsigned int length;

      deser_data(&length, &buf, 4);

      printf("Length field of message is %d\n",length);

      //you should count length field too
      length -= 4;

      //loop over all elements of serialized message
      while(length > 0){

         

         //deserialize "type" of message
         MsgType type;
         deser_data(&type, &buf, 2);

         printf("Message type field is:%d %s\n",type, (type == RTM_NEWQDISC) ? "OK" : "NOK");

         length -= 2;

         //deserialize "Flags"
         Flags flags;
         deser_data(&flags, &buf, 2);

         printf("Flags field is:%d %s\n",flags, (flags == NLM_F_EXCL | NLM_F_CREATE | NLM_F_REQUEST) ? "OK" : "NOK" );

         length -= 2;

         //deserialize "sequence number"
         unsigned int seqn;
         deser_data(&seqn, &buf, 4);

         printf("Sequence number field is:%d %s\n",seqn, (seqn == 1000) ? "OK" : "NOK");

         length -= 4;

         //deserialize process ID
         unsigned int pid;
         deser_data(&pid, &buf, 4);

         printf("Process id field is:%d\n",pid);

         length -= 4;

         //deserialize Family (AF_INET)
         unsigned int family;
         deser_data(&family, &buf, 1);

         printf("Family field is:%d %s\n", family, (family == AF_INET) ? "OK" : "NOK");

         length -= 1;

         //deserialize Reserved1 field
         unsigned int reserved;
         deser_data(&reserved, &buf, 3);

         printf("Reserved1 field is:%d %s\n", reserved, (reserved == 0) ? "OK" : "NOK");

         length -= 3;

         //deserialize Interface Index
         unsigned int ifIndex;
         deser_data(&ifIndex, &buf, 4);

         printf("Interface Index field is:%d %s\n", ifIndex, (ifIndex == 5) ? "OK" : "NOK");

         length -= 4;


         //deserialize Qdisc handle  (0x1000001)
         unsigned int Qdisc;
         deser_data(&Qdisc, &buf, 4);

         printf("Qdisc field is:%x %s\n", Qdisc, (Qdisc == 0x1000001) ? "OK" : "NOK");

         length -= 4;

         //deserialize Parent Qdisc   (0x1000000)
         unsigned int PQdisc;
         deser_data(&PQdisc, &buf, 4);

         printf("Parent Qdisk field is:%x %s\n", PQdisc, (PQdisc == 0x1000000) ? "OK" : "NOK");

         length -= 4;

         //deserialize TCM Info  (0)
         unsigned int TCMinfo;
         deser_data(&TCMinfo, &buf, 4);

         printf("TCM info is:%d %s\n", TCMinfo, (TCMinfo == 0) ? "OK" : "NOK");

         length -= 4;

         
         //Start deserialization of TLVs

         /*********************************/
         /****first TLV deserialization****/
         /*********************************/
         //deserialize its type
         Tlvtype tlv_type;
         deser_data(&tlv_type, &buf, 2);

         //update length since tlv type has been read
         length -= 2;

         printf("length0:%d\n",length);

         //decode the TLV
         tlvdecode(&tlv_type, &buf, &length);

         printf("length1:%d\n",length);


         /*********************************/
         /****second TLV deserialization****/
         /*********************************/
         //deserialize its type
         deser_data(&tlv_type, &buf, 2);

         //update length since tlv type has been read
         length -= 2;

         //decode the TLV
         tlvdecode(&tlv_type, &buf, &length);

         printf("length2:%d\n",length);

         //break;

      }

      free(buf);
      buf = NULL;      
   }

   
    
}


void tlvdecode(void *tlvtype, ser_buf_t **buf, int *length){

  
   //retrieve the type of TLV
   Tlvtype type = *((Tlvtype *) tlvtype);


   int len,value;

   switch(type){
      case TCA_KIND:

         //deserialize length field of TLV (occupies 2 bytes)
         deser_data(&len, buf, 2);

         //deserialize value field of TLV (occupies "len" bytes)
         deser_data(&value, buf, len); //we know the bytes that occupied for the value field

         //update the length because length field has been read
         *length -= 2;

         //update the length because value field has been read
         *length -= len;

         printf("TLV type \"TCA_KIND\" with length %d and value %d is %s\n", len, value, ( value == PFIFO ) ? "OK" : "NOK");
         break;

       case TCA_OPTIONS:

          printf("mphka 2");
          //deserialize length field of TLV (occupies 2 bytes)
          deser_data(&len, buf, 2);

          //deserialize value field of TLV (occupies "len" bytes)
          deser_data(&value, buf, len); //we know the bytes that occupied for the value field

          //update the length because length field has been read
          *length -= 2;

          //update the length because value field has been read
          *length -= len;

          printf("TLV type \"TCA_OPTIONS\" with length %d and value %d is %s\n", len, value, ( value == 100 ) ? "OK" : "NOK");
          break;
                
        //a TLV that is not known for the specific network device, so bypass it
        default:

          printf("mphka 3");
          //deserialize length field of TLV (occupies 2 bytes)
          deser_data(&len, buf, 2);

          //deserialize value field of TLV (occupies "len" bytes)
          deser_data(&value, buf, len); //we know the bytes that occupied for the value field

          //update the length because length field has been read
          *length -= 2;

          //update the length because value field has been read
          *length -= len;

          break;
   
   }      
    
}


int main(int argc, char *argv[]){
   
   ser_buf_t *buf;

   int size;

   printf("unsigned int:%d\n",sizeof(unsigned int));

   encode(&buf);

   printf("mes = %d\n",*((int *)buf->b));
   printf("size = %d\n", buf->next);

   decode(buf->b);

   free(buf->b);
   buf->b = NULL;

   free(buf);
   buf = NULL;
}


