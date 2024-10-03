#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>
#include "decode.h"

int main(int argc,char *argv[])
{   
    EncodeInfo E1;
    DecodeInfo D1;
    if(argc>=2){
    int res=check_operation_type(argv);
    if(res == e_encode){
        printf("selected Encoding\n");
        if(read_and_validate_encode_args(argv, &E1)== e_success){
            printf("read_and_validate_encode_arg are success\n");
            if(do_encoding(&E1)==e_success){
                printf("Encoding is success\n");
            }
            else{
                printf("Encoding is failed\n");
                return 1;
            }
        }
        else{
            printf("Failed to read_and_validate_encode_arg \n");
            return 1;
        }
    }
    else if(res== e_decode){
        printf("Selected Decoding\n");
        if(read_and_validate_decode_args(argv, &D1)== e_success){
            printf("read_and_validate_decode_arg are success\n");
            if(do_decoding(&D1)==e_success){
                printf("Decoding is success\n");
            }
            else{
                printf("Decoding is failed\n");
                return 1;
            }
        }
        else{
            printf("Failed to read_and_validate_encode_arg \n");
            return 1;
        }
    }
    else if(res == e_unsupported){
        printf("INVALID OPTION\n");
        printf("For Encode ./a.out -e beautiful.bmp ssecret.txt stegno.bmp \n");
        printf("Fo Decode ./a.out -d stegno.bmp data.txt\n");
    }
    }
    else{
        printf("INVALID OPTION\n");
        printf("For Encode ./a.out -e beautiful.bmp ssecret.txt stegno.bmp \n");
        printf("Fo Decode ./a.out -d stegno.bmp data.txt\n");
    }

    return 0;
}
OperationType check_operation_type(char *argv[]){
    if(strcmp(argv[1],"-e")== 0){
        return e_encode;
    }
    else if(strcmp(argv[1],"-d")== 0){
        return e_decode;
    }
    else{
        return e_unsupported;
    }
}
