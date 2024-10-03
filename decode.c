#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "decode.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if (strcmp(strrchr(argv[2], '.'), ".bmp") == 0)
    {
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        return e_failure;  
    }
    if (argv[3] != NULL)
    {
        decInfo->secret_fname = argv[3];
    }
    else
    {
        decInfo->secret_fname = "output.txt";
    }

    return e_success;
}

Status open_files_d(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
        return e_failure;
    }
    decInfo->fptr_secret = fopen(decInfo->secret_fname, "wb");
    if (decInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->secret_fname);
        return e_failure;
    }

    return e_success; 
}

Status decode_image_to_data(DecodeInfo *decInfo)
{
    for (int j = 0; j < 8; j++)
    {
        decInfo->decode_data[0] |= (decInfo->str[j] & 1) << j;
    }
    return e_success;
}

Status decode_magic_string(DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
    for (int i = 0; i < strlen(MAGIC_STRING); i++)
    {
        fread(decInfo->str, 8, 1, decInfo->fptr_stego_image);
        decInfo->decode_data[0] = 0;
        decode_image_to_data(decInfo);
        if (decInfo->decode_data[0] == MAGIC_STRING[i])
        {
            continue;
        }
        else
        {
            return e_failure;
        }
    }
    //printf("\n");
    return e_success;
}

Status decode_size_to_data(DecodeInfo *decInfo)
{
    fread(decInfo->secret_file_size, 32, 1, decInfo->fptr_stego_image);
    decInfo->image_data_size = 0;
    for (int i = 0; i < 32; i++)
    {
        decInfo->image_data_size |= (decInfo->secret_file_size[i] & 1) << i;
    }
    return e_success;
}

Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    decode_size_to_data(decInfo);
    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    for (int i = 0; i < decInfo->image_data_size; i++)
    {
        fread(decInfo->str, 8, 1, decInfo->fptr_stego_image);
        decInfo->decode_data[0] = 0;

        decode_image_to_data(decInfo);
    }
    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    decode_size_to_data(decInfo);
    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    for (int i = 0; i < decInfo->image_data_size; i++)
    {
        fread(decInfo->str, 8, 1, decInfo->fptr_stego_image);
        decInfo->decode_data[0] = 0;
        decode_image_to_data(decInfo);
        fwrite(&decInfo->decode_data[0], 1, 1, decInfo->fptr_secret);  
    }
    return e_success;
}


Status do_decoding(DecodeInfo *decInfo)
{
    if (open_files_d(decInfo) == e_success)
    {
        printf("Opening file successfully\n");
        if (decode_magic_string(decInfo) == e_success)
        {
            printf("Magic string decoded successfully\n");
            if (decode_secret_file_extn_size(decInfo) == e_success)
            {
                printf("Secret file extension size decoded successfully\n");
                if (decode_secret_file_extn(decInfo) == e_success)
                {
                    printf("Secret file extension decoded successfully\n");
                    if (decode_secret_file_size(decInfo) == e_success)
                    {
                        printf("Secret file size decoded successfully\n");
                        if (decode_secret_file_data(decInfo) == e_success)
                        {
                            printf("Secret file data decoded successfully\n");
                        }
                        else
                        {
                            printf("Failed to decode secret file data\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("Failed to decode secret file size\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("Failed to decode file extension\n");
                    return e_failure;
                }
            }
            else
            {
                printf("Failed to decode secret file extension size\n");
                return e_failure;
            }
        }
        else
        {
            printf("Failed to decode magic string\n");
            return e_failure;
        }
    }
    else
    {
        printf("Failed to open files\n");
        return e_failure;
    }

    return e_success;
}