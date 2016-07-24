/*
 * Copyright (c) 2009-2010, Oracle and/or its affiliates. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Oracle nor the names of its contributors
 *   may be used to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "..\QRlib.h"

int code_size;
void go();
void QR_gen(char line);
void QR_data_encode(char *user_string);
int * QR_getbinary(unsigned value, unsigned bits);
int * QR_enc_alphanum(char *user_string);
int QR_binarytodecimal(int *binary_input, int length);
void QR_err_correct(int *bit_vector, int vector_length);




int Alphanum_table(char value){

    int index;
    switch(value){
        case '0':
            index = 0;
            break;
        case '1':
            index = 1;
            break;
        case '2':
            index = 2;
            break;
        case '3':
            index = 3;
            break;
        case '4':
            index = 4;
            break;
        case '5':
            index = 5;
            break;
        case '6':
            index = 6;
            break;
        case '7':
            index = 7;
            break;
        case '8':
            index = 8;
            break;
        case '9':
            index = 9;
            break;
        case 'A':
            index = 10;
            break;
        case 'B':
            index = 11;
            break;
        case 'C':
            index = 12;
            break;
        case 'D':
            index = 13;
            break;
        case 'E':
            index = 14;
            break;
        case 'F':
            index = 15;
            break;            
        case 'G':
            index = 16;
            break;
        case 'H':
            index = 17;
            break;
        case 'I':
            index = 18;
            break;
        case 'J':
            index = 19;
            break;
        case 'K':
            index = 20;
            break;
        case 'L':
            index = 21;
            break;
        case 'M':
            index = 22;
            break;
        case 'N':
            index = 23;
            break;
        case 'O':
            index = 24;
            break;
        case 'P':
            index = 25;
            break;
        case 'Q':
            index = 26;
            break;
        case 'R':
            index = 27;
            break;
        case 'S':
            index = 28;
            break;
        case 'T':
            index = 29;
            break;
        case 'U':
            index = 30;
            break;
        case 'V':
            index = 31;
            break;
        case 'W':
            index = 32;
            break;
        case 'X':
            index = 33;
            break;
        case 'Y':
            index = 34;
            break;
        case 'Z':
            index = 35;
            break;
        case ' ':
            index = 36;
            break;
        case '$':
            index = 37;
            break;
        case '%':
            index = 38;
            break;
        case '*':
            index = 39;
            break;
        case '+':
            index = 40;
            break;
        case '-':
            index = 41;
            break;
        case '.':
            index = 42;
            break;
        case '\/':
            index = 43;
            break;    
        case ':':
            index = 44;
            break;             
    }
    
    return index;
    
}

int main(int argc, char**argv) {
    go();
    return 0;
}

void go() {
    int i;
    
    char *prompt = "\nType a string (or 'q' when done) >>>\n";
    for (i = 0; i < 25; i++) {
	char line[16];
	printf(prompt);
	gets(line);
        printf("Read: %s\n", line);
        QR_data_encode(line);

        if (line[0] == 'q' || line[0] == 'Q')
	    break;
    }
}

int * QR_getbinary(unsigned value,unsigned bits){
    
    
    int i = 0;
    int *bin_value = malloc(bits*sizeof(int));


    
    for(int j = 0; j < bits; j++){ 
        bin_value[j] = 0;
    }

    i = bits - 1;
    
    while(value > 0){
        if(value % 2 == 0)
            bin_value[i] = 0;
        else
            bin_value[i] = 1;
        i--;
        value = value / 2;
    }
    
 return bin_value;
}

int QR_binarytodecimal(int *binary_input, int length)
{
    int decimal_number = 0;
    
    for(int i = 0; i < length; i++){
        decimal_number += binary_input[i]*pow(2,length-i-1);
    }
    
    
    return decimal_number;
}
/*
Step 1: Data Analysis
your first step should be to perform data analysis to determine whether your 
text can be encoded in numeric, alphanumeric, byte, or Kanji mode, then select 
the most optimal mode for your text.
*/
void QR_analyse_data(char *user_string) {

/*
Step 1.1 Choose most efficient mode for the QR code:
To select the most efficient mode for the QR code, examine the characters in the input string and check for the following conditions.
    1.If the input string only consists of decimal digits (0 through 9), use numeric mode.
    2.If numeric mode is not applicable, and if all of the characters in the input string can be found in the left column of the alphanumeric table, use alphanumeric mode. Lowercase letters CANNOT be encoded in alphanumeric mode; only uppercase.
    3.If there is a character that is not in the left column of the alphanumeric table but can be encoded in ISO 8859-1, use byte mode. As mentioned above, QR code readers may be able to recognize UTF-8 in byte mode.
    4.If all of the characters are in the Shift JIS character set, use Kanji mode  
Chose:  
Alphanumerical Mode allowing characters 0-9, A-Z, $ % * + - . / space and :
*/

}


/*
Step 2: Data Encoding
The result of this step is a string of bits that is split up into 
data codewords that are each 8 bits long.
*/
void QR_data_encode(char *user_string){
    int data_temp[13];
    int *encoded_data, *encoded_temp, codewords, encoded_data_size;
    int char_count = 0, padding = 0, padding_zeros = 0;
    int *char_count_ind, *QR_pad_bytes, QR_pad_count = 0;
    int QR_pad_236[8]={1,1,1,0,1,1,0,0}, QR_pad_17[8]={0,0,0,1,0,0,0,1};
/*
Step 2.1: Choose the Error Correction Level:   
Chose:  | Error Correction Level | Error Correction Capability | 
        |          Q             |      Recovers 25% of data   | 
*/

/*
Step 2.2: Determine the Smallest Version for the Data for QR code 1 (21x21)
Chose:  
| Version | Error Correction Level | Numeric Mode | Alphanumeric Mode | Byte Mode | Kanji Mode | 
|   1     |             Q          |         27   |        16         |    11     |      7     | 
*/

/*
Step 2.3: Add the Mode Indicator
Each encoding mode has a four-bit mode indicator that identifies it. The encoded 
data must start with the appropriate mode indicator that specifies the mode 
being used for the bits that come after it.
Chose:  
Mode Name         | Mode Indicator
Alphanumeric Mode | 0010
*/
    int mode_indicator[4] = {0,0,1,0};
/*
Step 2.4: Add the Character Count Indicator
The character count indicator is a string of bits that represents the number of 
characters that are being encoded. The character count indicator must be placed 
after the mode indicator. Furthermore, the character count indicator must be a 
certain number of bits long, depending on the QR version. Count the number of 
characters in the original input text, then convert that number into binary. 
The length of the character count indicator depends on the encoding mode and the 
QR code version that will be in use. To make the binary string the appropriate 
length, pad it on the left with 0s.
Chose:
For Versions 1 through 9 and Alphanumeric mode = 9 bits
*/
    char_count = strlen(user_string);
    printf("Character count = %d\n",char_count);
    char_count_ind = QR_getbinary(char_count,9);

    printf("Mode indicator: ");
    for(int j = 0; j < 4; j++){ 
        printf("%d",mode_indicator[j]);
    }    
    printf("\n");
    printf("Character count indicator: ");
    for(int j = 0; j < 9; j++){ 
        printf("%d",char_count_ind[j]);
    }
    
    for(int j = 0; j < 13; j++){ 
        if(j<3){
            data_temp[j] = mode_indicator[j];
        }
        else{
            data_temp[j] = char_count_ind[j-4];
        }   
    }
    printf("\n");
    printf("Concatenated arrays: ");
    for(int j = 0; j < 13; j++){
        printf("%d",data_temp[j]);
    }
    printf("\n");
/*
Step 2.5: Encode Using the Selected Mode    
chose: Encode with Alphanumeric
*/
    encoded_temp = QR_enc_alphanum(user_string);  
    
/*
Step 2.6: Break Up into 8-bit Codewords and Add Pad Bytes if Necessary
Determine the Required Number of Bits for this QR Code:    
From Error Correction table version 1-Q requires 13 codewords thus 13 * 8 = 104 bits
*/
    codewords=13;
    encoded_data_size = codewords*8;
    encoded_data = malloc(encoded_data_size*sizeof(int));
    
    for(int i = 0; i<encoded_data_size;i++){
        encoded_data[i]=0;
    }    
    
    while((code_size+4+9+padding_zeros)%8!=0){
        padding_zeros++;
    }
    
    QR_pad_count = (codewords*8 - 4 - 9 - code_size - padding_zeros)/8;
    QR_pad_bytes = malloc(8*QR_pad_count*sizeof(int));
    
    
    int index = 0, mode = 0;
    
    for(int i=0;i<QR_pad_count;i++){
        
        switch(mode){
            case 0:
                for (int j=0;j<8;j++){
                   QR_pad_bytes[index+j] = QR_pad_236[j];  
                }
                mode = 1;
                break;
            case 1:
                for (int j=0;j<8;j++){
                   QR_pad_bytes[index+j] = QR_pad_17[j];
                }
                mode = 0;
                break;
        }
        index = index+8;
    }
    
    printf("\nPadding zeros = %i",padding_zeros);
    printf("\nNr of padding bytes = %i",QR_pad_count);
    printf("\nPadding byte = ");
    for(int i=0;i < QR_pad_count*8;i++){
        printf("%d",QR_pad_bytes[i]);
    }
    
    printf("\nEncoded Value in 8-bit Codewords = ");
   
    
    
    mode = 0;
    index = 0;
    for(int i=0;i<5;i++){
        
        switch(mode){
            case 0:
                for (int j=0;j<4;j++){
                   encoded_data[index+j] = mode_indicator[j];  
                }
                mode = 1;
                index+=4;
                break;
            case 1:
                for (int j=0;j<9;j++){
                   encoded_data[index+j] = char_count_ind[j];
                }
                mode = 2;
                index+=9;
                break;
            case 2:
                for (int j=0;j<code_size;j++){
                   encoded_data[index+j] = encoded_temp[j];
                }
                mode = 3;
                index+=code_size;
                break;
            case 3:
                for (int j=0;j<padding_zeros;j++){
                   encoded_data[index+j] = 0;
                }
                mode = 4;
                index+=padding_zeros;
                break;
            case 4:
                for (int j=0;j<8*QR_pad_count;j++){
                   encoded_data[index+j] = QR_pad_bytes[j];
                }
                mode = 0;
                break;                
        }
    }    
    
    for(int i = 0; i<encoded_data_size;i++){
        
        printf("%d",encoded_data[i]);
    }
    
    QR_err_correct(encoded_data,encoded_data_size);
}

int * QR_enc_alphanum(char *input_string){
    
    int pairs,odd_num,temp_val,*bin_val,encoded_index = 0, pair_index = 0;
    pairs=strlen(input_string)/2;
    
    printf("\nPairs = %d\n",pairs);
    
    if(strlen(input_string)%2==0){
        code_size=11*pairs;
        odd_num = 0;
    }else{
        code_size=11*pairs+6;
        odd_num = 1;
    }
    printf("Code size = %d\n",code_size);
    int * encoded_string = malloc(code_size*sizeof(int));
    
    for(int j = 0; j < code_size; j++){ 
        encoded_string[j] = 0;
        
    }
    
    for(int j = 0; j < pairs + odd_num; j++){ 
        if(j < pairs){
          
            temp_val = 45*Alphanum_table(input_string[j+pair_index])+Alphanum_table(input_string[j+1+pair_index]);
            bin_val = QR_getbinary(temp_val,11);
            printf("IND %i Pair %c%c Val %i Bin Val ",pair_index,input_string[j+pair_index],input_string[j+1+pair_index],temp_val);
            for(int i = 0;i<11;i++){
                printf("%d",bin_val[i]);
                encoded_string[encoded_index]=bin_val[i];
                encoded_index++;
            } 
            printf("\n");
        }
        
        if(j == pairs){
            
            temp_val = Alphanum_table(input_string[j+pair_index]);
            bin_val = QR_getbinary(temp_val,6);
            printf("IND %i Last Character %c Val %i Bin Val ",pair_index,input_string[j+pair_index],temp_val);
            for(int i = 0;i<6;i++){
                printf("%d",bin_val[i]);
                encoded_string[encoded_index]=bin_val[i];
                encoded_index++;                
            } 
            printf("\n");            
        }
        pair_index++;
    }
    
    printf("Alphanumerically encoded value = ");
    for(int j = 0; j < code_size; j++){ 
        printf("%d",encoded_string[j]);
    }
        
    
 return encoded_string;   
}

/*
Step 3: Error Correction Coding
after you create the string of data bits that represent your text, 
you must then use those bits to generate error correction codewords 
using a process called Reed-Solomon error correction.
*/

void QR_err_correct(int *bit_vector, int vector_length){
    int temp_array[8]={0}, temp=0,index=0;
    int * message_polynomial = malloc((vector_length/8)*sizeof(int));
    printf("\nMessage polynomial = ");
    for(int i = 0; i<vector_length/8; i++){
        for(int j=0; j < 8; j++){
          temp_array[j]=bit_vector[index+j];
        }
        message_polynomial[i] = QR_binarytodecimal(temp_array,8);
        printf("%d ",message_polynomial[i]);
        index+=8;
    }
    
}

/*
Step 4: Structure Final Message
The data and error correction codewords generated in the previous steps must
now be arranged in the proper order. For large QR codes, the data and error 
correction codewords are generated in blocks, and these blocks must be 
interleaved according to the QR code specification.
*/

void QR_struct_message(void){
    
}

/*
Step 5: Module Placement in Matrix
After generating the data codewords and error correction codewords and arranging 
them in the correct order, you must place the bits in the QR code matrix.
*/

void QR_gen_matrix(void){
    
}

/*
Step 6: Data Masking
Certain patterns in the QR code matrix can make it difficult for QR code 
scanners to correctly read the code. To counteract this, the QR code 
specification defines eight mask patterns, each of which alters the QR code 
according to a particular pattern. You must determine which of these mask 
patterns results in the QR code with the fewest undesirable traits. This is 
done by evaluating each masked matrix based on four penalty rules. 
Your final QR code must use the mask pattern that resulted in the lowest 
penalty score.
*/

void QR_data_mask(void){
    
}

/*
Step 7: Format and Version Information
The final step is to add format and (if necessary) version information to the 
QR code by adding pixels in particular areas of the code that were left blank 
in previous steps. The format pixels identify the error correction level and 
mask pattern being used in this QR code. The version pixels encode the size of 
the QR matrix and are only used in larger QR codes.
*/

void QR_add_info(void){
    
}