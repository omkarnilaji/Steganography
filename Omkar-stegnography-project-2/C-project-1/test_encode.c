#include<stdio.h>
#include<string.h>
#include "types.h"
#include "encode.h"
#include "decode.h"
int main(int argc,char **argv)
{
    /* Check operation type */
if(check_operation_type(argv) == e_encode)
{
    printf("You choosed encoding\n");
    EncodeInfo encInfo;
    if(read_and_validate_encode_args(argv,&encInfo) == e_success)
    {
        printf("Read and validate is successful\n");
        if(do_encoding(&encInfo)==e_success)
        {
            printf("Encoding successful\n");
            return e_success;
        }
        else
        {
            printf("Encoding not successful\n");
            return e_failure;
        }
    }
    else
    {
        printf("Read and validate is un-successful\n");
        return e_failure;
    }
    return e_success;
}
else if(check_operation_type(argv) == e_decode)
{
   printf("You choosed decoding\n");
   DecodeInfo decInfo;

    if(read_and_validate_decode_args(argv,&decInfo)== e_success)
    {
        if(do_decoding(&decInfo)==e_success)
        {
            printf("Decoding successful\n");
            return e_success;
        }
        else
        {
            printf("Decoding not successful\n");
            return e_failure;
        }
    }

    return e_failure;
}
else
{
    printf("Invalid expression\n");
    printf("for encoding--> ./a.out -e beautiful.bmp secret.txt (stego.bmp)\n");
    printf("for decoding--> ./a.out -d stego.bmp (output.txt)\n");
    return e_failure;
}
}
OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"-e")==0)
        return e_encode;
    else if(strcmp(argv[1],"-d")==0)
        return e_decode;
    else
        return e_unsupported;
}