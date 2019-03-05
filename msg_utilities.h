#include "ser_des_api.h"
#define PFIFO 13

typedef enum _Tlvtype{
   TCA_KIND = 123, //TLV type (2 bytes), Length 2 bytes
   TCA_OPTIONS     //TLV type (2 bytes), Length 2 bytes
}Tlvtype;


/*  */
typedef enum _Msgtype{
   RTM_NEWQDISC = 2, //start from value "2"
   RTM_DELQDISC,
   RTM_GETQDISC,
   RTM_NEWTCLASS,
   RTM_DELTCLASS,
   RTM_GETTCLASS,
   RTM_NEWTFILTER,
   RTM_DELTFILTER,
   RTM_GETTFILTER
}MsgType;


typedef enum _Flags{
   NLM_F_EXCL=5,
   NLM_F_CREATE,
   NLM_F_REQUEST
}Flags;

//encode a specific message by using serialization functions
extern void encode(ser_buf_t **buf);

//decode a specific message by using deserialization functions
extern void decode(char *data);

//decode TLV
void tlvdecode(void *tlvtype, ser_buf_t **buf, int *length);
