#ifndef DECODE_H
#define DECODE_H

#include<stdio.h>
#include "types.h"

typedef struct _DecodeInfo
{
    char *stego_image_fname;
    FILE *fptr_stego_image;

    char *output_fname;
    FILE *fptr_output;

    char magic[20];

    int extn_size;
    char extn_secret_file[10];

    long secret_size;

} DecodeInfo;


/* validate arguments */
Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo);

/* do decoding */
Status do_decoding(DecodeInfo *decInfo);

/* open files */
Status open_decode_files(DecodeInfo *decInfo);

/* decode magic */
Status decode_magic_string(DecodeInfo *decInfo);

/* decode extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* decode extension */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* decode secret size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* decode secret data */
Status decode_secret_file_data(DecodeInfo *decInfo);

/* helpers */
Status decode_data_from_image(char *data,int size,DecodeInfo *decInfo);

Status decode_byte_from_lsb(char *ch,char *buffer);

Status decode_int_from_lsb(int *num,char *buffer);

#endif