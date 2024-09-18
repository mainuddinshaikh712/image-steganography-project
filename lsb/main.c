/*DOC
NAME: Mainuddin m shaikh
DATE: 03-01-2024
DESCRIPTION: iSteganography is the art of hiding the fact that communication is taking place,
by hiding information in other information. Many different carrier file formats can be used,
but digital images are the most popular because of their frequency on the internet.
DOC*/
#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include <string.h>

int main(int argc,char *argv[])
{
    /*check_operation_type checks which operation is passed by user through
      command line arguments*/
    int res = check_operation_type(argv);
    /*step1 : check the res is e_encode or not
            : if yes->enter the encoding part*/
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    if(res==e_encode)
    {
	/*calling the read and validate function to validate the given arguments
	  are suitable extension or not*/
	printf("you have entered encoding\n");
	if(read_and_validate_encode_args(argv,&encInfo)== e_success)
	{
	    printf("validation of given input is sucessful\n");
	    printf("***ENCODING STARTED***\n");
	    /*if read and validate returns e_success then call the do_encoding function
	      it exicute the encoding and return e_success if encoding is done properly*/
	    if ( do_encoding(&encInfo)==e_success)
	    {
		printf("*-*-*-ENCODING SUCCESSFULL-*-*-*\n");
	    }
	    /*else function return e_failure and program is terminated with the error 
	      massege*/
	    else
	    {
		printf("^^^^^ENCODING FAILDED^^^^^\n");
	        printf("Programme tarminated\n");
		return 0;
	    }
	    /*if read_and_validate fails it returns e_failure and programm is terminated
	      with error massege*/
	}
	else
	{
	    printf("encoding validation failure\n");
	    printf("Programme tarminated\n");
	    return 0;
	}
    }
    /*if cheack_operation funtion returns e_decode then it enters the decode part*/
    else if (res==e_decode)
    {
	/*calling the read and validate function to validate the given arguments
	  are suitable extension or not*/
	printf("you have enterd decoding\n");
	if (read_and_validate_decode_args(argv,&decInfo)== e_success)
	{
	    printf("validation of given input is sucessful\n");
	    printf("***DECODING STARTED***\n");
	    /*if read and validate returns e_success then call the do_encoding function
	      it exicute the encoding and return e_success if encoding is done properly*/
	    if ( do_decoding(&decInfo)==e_success)
	    {
		printf("*-*-*-DECODING SUCCESSFULL-*-*-*\n");
	    }
	    /*else function return e_failure and program is terminated with the error 
	      massege*/
	    else
	    {
		printf("^^^^^DECODING FAILDED^^^^^\n");
	        printf("Programme tarminated\n");
		return 0;
	    }
	    /*if read_and_validate fails it returns e_failure and programm is terminated
	      with error massege*/
	}
	else
	{
	    printf("Decoding validation failure\n");
	    printf("Programme tarminated\n");
	    return 0;
	}
    }
    /*if check_operation returns e_unsupported then the argument passed by
      user id invalide (apart from encode->-e and decode->-d)*/
    else if (res==e_unsupported)
    {
	printf("the given input is unsuported");
    }
}
/*it checks the giving argument is -e or -d in command line
  argument in index number 1*/
OperationType check_operation_type(char *argv[])
{
    if (strcmp(argv[1],"-e")==0)
	return e_encode;
    else if (strcmp(argv[1],"-d")==0)
	return e_decode;
    else
	return e_unsupported;
}

