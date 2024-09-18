#include<stdio.h>
#include<string.h>
#include "types.h"
#include "common.h"
#include "decode.h"

/*cheaking for given input arguments are proper in .bmp and .txt file
  if not tarmenate the program and ask user to provide proper argument*/
Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo)
{
    if(strcmp(strstr(argv[2],"."),".bmp") == 0)
	decInfo -> src_image_fname = argv[2];
    else return e_failure;

    if(argv[3] == NULL)
    {
	printf("Default : Password_file is created\n");
	decInfo->secret_fname = "output.txt";
    }
    else 
    {
	    decInfo->secret_fname = argv[3];
    }

    return e_success;

}


Status do_decoding(DecodeInfo *decInfo)
{
    /*calling a function to open file returning e_success
      or failure*/
    if(open_decode_files(decInfo) == e_success)
	printf("INFO: Files opened successfully\n");
    else return e_failure;

    /*calling a function to decode magic string file returning e_success
      or failure*/
    if(decode_magic_string(MAGIC_STRING,decInfo) == e_success)
	printf("INFO: Magic string coordinate sucessfully\n");
    else return e_failure;

    /*calling a function to decode extension size file returning e_success
      or failure*/
    if(decode_secret_file_extn_size(decInfo) == e_success)
	printf("INFO: Size of file extension decoded\n");
    else return e_failure;

    /*calling a function to decode extension of file returning e_success
      or failure*/
    if(decode_secret_file_extn(decInfo) == e_success)
	printf("INFO: Decoded extension successfuly\n");
    else return e_failure;

    decInfo->fptr_secret = fopen(decInfo->secret_fname,"w");
    /*error handling*/
    if(decInfo->fptr_secret == NULL)
    {
	perror("fopen");
	fprintf(stderr,"ERROR: Unable to open file %s\n",decInfo->secret_fname);
	return e_failure;
    }
    /*calling a function to get secret  file size returning e_success
      or failure*/

    if(get_secret_msg_size(decInfo)==e_success)
	printf("INFO: Size of secret message fetched successfully\n");
    else return e_failure;

    /*calling a function to copy secret massege returning e_success
      or failure*/

    if(copy_secret_msg_to_file(decInfo) == e_success);
    printf("INFO: secret message fetched successfully\n");


    return e_success;

}

/*opening of files with error handling*/ 
Status open_decode_files(DecodeInfo *decInfo)
{
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname,"r");
    if(decInfo->fptr_src_image == NULL)
    {
	perror("fopen");
	fprintf(stderr,"ERROR: Unable to open file %s\n",decInfo->src_image_fname);
	return e_failure;
    }


    return e_success;

}
/*decoding the magic string from 54th position to length of magic string*/
Status decode_magic_string(char *magic_string, DecodeInfo *decInfo)
{
   
    fseek(decInfo->fptr_src_image,54,SEEK_SET);
    
    int len_magic_string = strlen(magic_string);

    char str[8];
    char ch;
    //compairing the decoded magic string with orignal
    //if does not match terminate the program
    for(int i=0;i<len_magic_string;)
    {

    	fread(str,8,1,decInfo->fptr_src_image);

    	ch = decode_lsb_to_byte(str);
   	 if( ch == magic_string[i])
   	 {
		i++;
    	 }
    	 else
	 {
	     printf("magic string does not match\n");
	     return e_failure;
	 }

    }

    return e_success;


}
//decoding the caracters by extracting lsb of avery 8 bites
char decode_lsb_to_byte(char *image_buffer)
{
    char ch = 0;
    for(int i=0;i<8;i++)
    {
	ch = ch<<1;
	ch = (image_buffer[i] & 1) | ch;
    }
    return ch;
}    
//decoding extension size of secret file
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char str[32];
    fread(str,32,1,decInfo->fptr_src_image);
    decInfo->size_extn = decode_lsb_to_size(str);
    return e_success;


}
//extracting an integer value that is every lsb from 32 bites
int decode_lsb_to_size(char *image_buffer)
{
    int num=0;
    for(int i=0;i<32;i++)
    {
	num = num<<1;
	num = (image_buffer[i] & 1) | num;
    }
    return num;

}
//decoding extention of secret file
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char arr[decInfo->size_extn+1];
    char str[8];
    int i;
    for( i=0;i<decInfo->size_extn;i++)
    {
	fread(str,8,1,decInfo->fptr_src_image);
	arr[i] = decode_lsb_to_byte(str);

    }
    arr[i] = '\0';
    decInfo->extn = arr;
    return e_success;

}

Status get_secret_msg_size(DecodeInfo *decInfo)
{
    char str[32];
    fread(str,32,1,decInfo->fptr_src_image);
    decInfo->size_password = decode_lsb_to_size(str);
	return e_success;
}
//decoding secret message from source file and storing it in
//destinantion file that is .txt
Status copy_secret_msg_to_file(DecodeInfo *decInfo)
{
    char arr[decInfo->size_password];
    char str[8];
    int i;
    fseek(decInfo->fptr_secret,0,SEEK_SET);
    for(i=0;i<decInfo->size_password;i++)
    {
	fread(str,8,1,decInfo->fptr_src_image);
	arr[i] = decode_lsb_to_byte(str);
	
    }

    fwrite(arr,decInfo->size_password,1,decInfo->fptr_secret);
    return e_success;
    
}
