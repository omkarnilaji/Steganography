#include<stdio.h>
#include<string.h>
#include "types.h"
#include "decode.h"



Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo)
{
    if(strstr(argv[2],".bmp") != NULL)
    {
        printf(".bmp found\n");
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        printf(".bmp not found\n");
        return e_failure;
    }

    if(argv[3]!= NULL)
    {
        printf("file found\n");
        decInfo->output_fname = argv[3];
    }
    else
    {
        decInfo->output_fname = "output";
    }

    return e_success;
}




Status open_decode_files(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image =fopen(decInfo->stego_image_fname,"r");

    if(decInfo->fptr_stego_image == NULL)
    {
        printf("Stego file is not found\n");
        return e_failure;
    }

    decInfo->fptr_output =fopen(decInfo->output_fname,"w");

    if(decInfo->fptr_output == NULL)
    {
        printf("Output file is not found\n");
        return e_failure;
    }
    return e_success;
}




Status do_decoding(DecodeInfo *decInfo)
{
    printf("<-----Started Decoding----->\n");

    if(open_decode_files(decInfo) == e_success)
    {
        printf("All files opened successfully\n");
    }
    else
    {
        printf("files not opened successfully\n");
        return e_failure;
    }

    fseek(decInfo->fptr_stego_image,54,SEEK_SET);
    if(decode_magic_string(decInfo)==e_success)
    {
        printf("magic string is decoded successfully\n");
    }
    else
    {
        printf("magic string is not decoded successfully\n");
        return e_failure;
    }
   if(decode_secret_file_extn_size(decInfo)==e_success)
    {
        printf("secret file extn size decoded successfully\n");
    }
    else
    {
        printf("secret file extn size is not decoded successfully\n");
        return e_failure;
    }

    if(decode_secret_file_extn(decInfo)==e_success)
    {
        printf("secret file extension decoded successfully\n");
    }
    else
    {
        printf("secret file extension not decoded successfully\n");
        return e_failure;
    }

    if(decode_secret_file_size(decInfo)==e_success)
    {
        printf("Secret file size decoded successfully\n");
    }
    else
    {
        printf("Secret file size is not decoded successfully\n");
        return e_failure;
    }

    if(decode_secret_file_data(decInfo)==e_success)
    {
        printf("Secret file data decoded successfully\n");
    }
    else
    {
        printf("Secret file data is not decoded successfully\n");
        return e_failure;
    }

    return e_success;
}

Status decode_magic_string(DecodeInfo *decInfo)
{
    char buffer[32];
    int magic_len;

    /* read encoded magic length */
    fread(buffer,32,1,decInfo->fptr_stego_image);

    decode_int_from_lsb(&magic_len,buffer);

    printf("Magic length = %d\n",magic_len);

    if(magic_len <=0 || magic_len >=20)
    {
        printf("Invalid magic length\n");
        return e_failure;
    }

    /* read actual magic string */
    decode_data_from_image(decInfo->magic,magic_len,decInfo);

    decInfo->magic[magic_len]='\0';

    printf("Magic string = %s\n",decInfo->magic);

    return e_success;
}




Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char buffer[32];

    fread(buffer,32,1,decInfo->fptr_stego_image);

    decode_int_from_lsb(&decInfo->extn_size,buffer);

    printf("Extn size = %d\n",decInfo->extn_size);

    if(decInfo->extn_size <=0 || decInfo->extn_size >10)
    {
        printf("Corrupt extension size\n");
        return e_failure;
    }

    return e_success;
}




Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    decode_data_from_image(decInfo->extn_secret_file,decInfo->extn_size,decInfo);

    decInfo->extn_secret_file[decInfo->extn_size]='\0';

    printf("Extn = %s\n",decInfo->extn_secret_file);

    char full_name[50];

    sprintf(full_name,"%s%s",decInfo->output_fname,decInfo->extn_secret_file);

    fclose(decInfo->fptr_output);

    decInfo->fptr_output =fopen(full_name,"w");

    if(decInfo->fptr_output == NULL)
    {
        return e_failure;
    }

    return e_success;
}




Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char buffer[32];
    int size;

    fread(buffer,32,1,decInfo->fptr_stego_image);

    decode_int_from_lsb(&size,buffer);

    decInfo->secret_size = size;

    printf("Secret size=%ld\n",decInfo->secret_size);

    return e_success;
}




Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char ch;

    for(int i=0;i < decInfo->secret_size;i++)
    {
        decode_data_from_image(&ch,1,decInfo);

        fwrite(&ch,1,1,decInfo->fptr_output);
    }

    return e_success;
}




Status decode_data_from_image(char *data,int size,DecodeInfo *decInfo)
{
    char buffer[8];

    for(int i=0;i<size;i++)
    {
        fread(buffer,8,1,decInfo->fptr_stego_image);

        decode_byte_from_lsb(&data[i],buffer);
    }

    return e_success;
}




Status decode_byte_from_lsb(char *ch,char *buffer)
{
    *ch=0;

    for(int i=0;i<8;i++)
    {
        *ch = (*ch << 1)| (buffer[i] & 1);
    }

    return e_success;
}

Status decode_int_from_lsb(int *num,char *buffer)
{
    *num = 0;

    for(int i=0;i<32;i++)
    {
        *num = (*num << 1) | (buffer[i] & 1);
    }

    return e_success;
}