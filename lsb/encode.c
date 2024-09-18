#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include <string.h>

/* Function Definitions */
Status read_and_validate_encode_args(char *argv[],EncodeInfo *encInfo)
{
    //STEP1: collect argv[2] and check the extensiom is ".bmp" or not
    // if yes -. store argv[2] into src_image_fname and go to step2
    if(!(strcmp(strstr(argv[2],".bmp"),".bmp")))
    {
	encInfo->src_image_fname=argv[2];
    }
	//else= return e_failure.
    else
    {
	printf("please pass the bmp file\n");
	return e_failure;
    }
	//step2: collect argv[3] and check the extension is ".txt" or not
	// : if yes->store argv[3] into secret_fname and store ".txt" into extn.sec ret_file[] then goto step 3
	// : ->return e_failure
    if (!(strcmp(strstr(argv[3],".txt"),".txt")))
    {
	encInfo->secret_fname=argv[3];
    }
    else
    {
	printf("please pass the secret file\n");
	return e_failure;
    }

	//step3 : check argv[4] passed or not
	//       : if yes-> check extension is bmp or not
	//                  : if yes->store argv[4] into stego_image_fname and return e_success
	//      : else->return e_failure
    if (argv[4]!= NULL)
     {
 	 if(!(strcmp(strstr(argv[4],".bmp"),".bmp")))
	 {
	     encInfo-> stego_image_fname=argv[4];
	     return e_success;
	 }
	 else
	 {
	     printf("please enter only .bmp file for o/p file");
	     return e_failure;
	 }
    }
    else
    {
	encInfo->stego_image_fname="stego.bmp";
	return e_success;
    }
}

Status do_encoding(EncodeInfo *encInfo)
{
    /*calling the function to open required file if files are not 
      opend printing an error and terminating the program*/
    if(open_files(encInfo)==e_success)
    {
	printf("INFO : files are opened successfuly\n");
	//goto step1
    }
    else
    {
	printf("INFO : files opening failed\n");
	return e_failure;
    }
//step1  : call the check_capacity()
   //     : check returning success or failure
   //     : success -> goto step2, failure->return e_failure. 
    if(check_capacity(encInfo) == e_success)
	printf("Info : capacity checked\n");
    else{
	printf("INFO : less capacity \n");
	return e_failure;}

    //step2 : copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_dest_image)
    //      : check returning success or failure
    //      : success -> goto step3, failur -> return e_failure
    if(copy_bmp_header(encInfo->fptr_src_image,encInfo-> fptr_stego_image) == e_success)
	printf("INFO : Copied Header file\n");
    else 
	return e_failure;
    //step3: call encode_magic_string(magic_string, encinfo);
    //	   : check returning success or failure
    //	   : success -> goto step4, failure -> return e_failure
    if(encode_magic_string(MAGIC_STRING,encInfo) == e_success)
	printf("INFO : magic string encoded\n");
    else
       return e_failure;    

    //step4 : call encode_secret_file_extn_size(size1,encInfo)
    //	   : check returning success or failure
    //	   : success -> goto step4, failure -> return e_failure
    long int size1 = strlen(strstr(encInfo->secret_fname,"."));
    strcpy(encInfo->extn_secret_file,strstr(encInfo->secret_fname,"."));

    if (encode_secret_file_extn_size(size1,encInfo) == e_success)
	printf("INFO : secret file size encoded\n");
    else
	return e_failure;
    //step 5 : call encode_secret_file_extn(encInfo->extn_secret_file,encInfo)
    //	   : check returning success or failure
    //	   : success -> goto step4, failure -> return e_failure
    if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo) == e_success)
	printf("INFO : extension copeid successfully \n");
    else
	return e_failure;
    //step 6 : call encode_secret_file_size(encInfo ->size_secret_file, encInfo)
    //	   : check returning success or failure
    //	   : success -> goto step4, failure -> return e_failure
    if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_success)
	printf("INFO : secret file size is copied\n");
    else 
	return e_failure;
    //step7 : call encode_secret_file_data(encInfo)
    //	   : check returning success or failure
    //	   : success -> goto step8, failure -> return e_failure
    if(encode_secret_file_data(encInfo) == e_success)
	printf("INFO : secrete Data copied\n");
    else
	return e_failure;
    //step8 : call copy_remaining_img_data(encInfo fptr_src_image, encInfo fptr_stego_image)
    //	   : check returning success or failure
    //	   : success -> goto step4, failure -> return e_failure
    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
	printf("INFO : remaining Data copied Successfully\n");
    else
	return e_failure;
    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    //step1 :find the size of source_image and store into image_capacity(struct member)
    encInfo -> image_capacity = get_image_size_for_bmp(encInfo ->fptr_src_image);
    //step2 : find the size of secret file.
    encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);
    /*step3 : compare image capacity > m.s(16)+size_of_extn(32)+exten(32)+file_size(32)+secret file size(8)*/
    if((encInfo->image_capacity) > (54 + strlen(MAGIC_STRING)*8+32+32+32+encInfo->size_secret_file*8))
	return e_success;
    else
	return e_failure;
/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
}
uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    return ftell(fptr);
}
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char str[54];
    //step1 : rewind the file pointer
    //rewind(fptr_src_image);
    //rewind(fptr_stego_image);
    fseek(fptr_src_image,0,SEEK_SET);
    fseek(fptr_stego_image,0,SEEK_SET);
    //step2 : read 54 byte of data
    fread(str,54,1,fptr_src_image);
    //step3 : write 54 byte of data to stego image
    fwrite(str,54,1,fptr_stego_image);
    return e_success;
}
/*calling a function to encode magic string in destination file to source file*/
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
    encode_data_to_image(magic_string, strlen(magic_string), encInfo->fptr_src_image, encInfo-> fptr_stego_image);
    return e_success;
}
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char buffer[encInfo->size_secret_file];
    //step 1:rewind the fptr_src_image
    fseek(encInfo->fptr_secret,0,SEEK_SET);
    //step 2:read secret file_size data from fptr_secret store into buffer.
    fread(buffer,encInfo->size_secret_file,1,encInfo->fptr_secret);
    encode_data_to_image(buffer,encInfo->size_secret_file,encInfo -> fptr_src_image, encInfo ->fptr_stego_image);
    return e_success;
}
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
/*encoding size of extention that is integer so 32 bits are encoded in
  32 bites in lsb */
Status encode_secret_file_extn_size(long size,EncodeInfo *encInfo)
{
    char str[32];
    fread(str,32,1,encInfo->fptr_src_image);
    encode_size_to_lsb(size,str);
    fwrite(str,32,1,encInfo->fptr_stego_image);
    return e_success;
}
/*taking each single bit and replacing it in lsb of 
  source file each bite*/
Status encode_size_to_lsb(int data,char *image_buffer)
{
    for(int i=0;i<32;i++)
    {
	image_buffer[i] = (image_buffer[i] & 0xFE) | ((data & (1<<(31-i))) >> (31-i));
    }
    return e_success;
}
/*encoding file extension with the help of data_to_imagge function*/
Status encode_secret_file_extn( char *file_extn, EncodeInfo *encInfo)
{
    int size1 = strlen(strstr(encInfo->secret_fname,"."));
    encode_data_to_image(file_extn,size1,encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success;
}

/*since extension as caracter which content 8 bit each bit is encoded in each bite of source 
  file */
Status encode_data_to_image(char *data,int size,FILE *fptr_src_image,FILE *fptr_stego_image)
{
    char str[8];
    for(int i=0;i<size;i++)
    {
	fread(str,8,1,fptr_src_image);
	encode_byte_to_lsb(data[i],str);
	fwrite(str,8,1,fptr_stego_image);
    }
}

/*taking each single bit and replacing it in lsb of 
  source file each bite*/
Status encode_byte_to_lsb(char data,char *image_buffer)
{
    for (int i=0;i<8;i++)
    {
	image_buffer[i] = (image_buffer[i] & 0xFE) | ((data & (1<<(7-i))) >> (7-i));
    }
}

Status encode_secret_file_size(long file_size,EncodeInfo *encInfo)
{
    char str[32];
    fread(str,32,1,encInfo->fptr_src_image);
    encode_size_to_lsb(file_size,str);
    fwrite(str,32,1,encInfo->fptr_stego_image);
    return e_success;
}
/*coping remaning data from sorce file to destinationi to keep the
 image as it is*/
Status copy_remaining_img_data(FILE *src_image, FILE *stego_image)
{
    char ch;
    while(fread(&ch,1,1,src_image) != 0)
    {
	fwrite(&ch,1,1,stego_image);
    }
    return e_success;
}
