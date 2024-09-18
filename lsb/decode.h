#ifndef DECODE_H
#define DECODE_H




typedef struct _DecodeInfo
{
    //Source Image Info/
    char *src_image_fname;
    FILE *fptr_src_image;
    int size_password;  


    char *secret_fname;
    FILE *fptr_secret;

    int size_extn;
    char *extn;


} DecodeInfo;

//to check which operation is selected
OperationType check_operation_type(char *argv[]);

//to validate the arguments
Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo);

//to extract magic string
Status decode_magic_string(char *magic_string,DecodeInfo *decInfo);

//to start decoding process
Status do_decoding(DecodeInfo *decInfo);

//to open require files
Status open_decode_files(DecodeInfo *decInfo);

//to decode caracter
char decode_lsb_to_byte(char *image_buffer);

//to extract size of extension
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

//to extract extension of secret file
Status decode_secret_file_extn(DecodeInfo *decInfo);

//to extract integer
int decode_lsb_to_size(char *image_buffer);

//extracting secret msg size
Status get_secret_msg_size(DecodeInfo *decInfo);

Status validate_secret_file(DecodeInfo *decInfo);

//extracting secret msg
Status copy_secret_msg_to_file(DecodeInfo *decInfo);

#endif
