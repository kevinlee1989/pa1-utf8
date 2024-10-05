#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Takes a UTF-8 encoded string and returns if it is valid ASCII (e.g. all bytes are 127 or less).
int32_t is_ascii(char str[]){

    for(int i=0; i < strlen(str); i++){
        if(str[i] > 127 || str[i] <= 0){
            return 0;
        }
    }
    return 1;
}

// Takes a UTF-8 encoded string and changes it in-place so that any ASCII lowercase characters a-z 
// are changed to their uppercase versions. Leaves all other characters unchanged. 
// It returns the number of characters updated from lowercase to uppercase.
int32_t capitalize_ascii(char str[]){

    int Update =0;
    for( int i =0; i < strlen(str); i++){

        if(str[i] >= 97 && str[i] <= 122 ){
            Update += 1;
            str[i] = str[i] - 32;
        }

    }
    return Update;
}

// Given the start byte of a UTF-8 sequence,
// return how many bytes it indicates the sequence will take (start byte + continuation bytes).
// Returns 1 for ASCII characters, and -1 if byte is not a valid start byte.
int32_t width_from_start_byte(char start_byte){

    if(start_byte == 0xC3){
        return 2;
    }
    else if(start_byte>=0 && start_byte<= 255){
        return 1;
    }
    else if(start_byte == 0xEF){
        return 3;
    }
    else{
        return -1;
    }
}


int main(){
    /*
    printf("Is 🔥 ASCII? %d\n", is_ascii("🔥"));
    printf("Is abcd ASCII? %d\n", is_ascii("abcd"));


    int32_t ret = 0;
    char str[] = "abcd";
    ret = capitalize_ascii(str);
    printf("Capitalized String: %s\nCharacters updated: %d\n", str, ret);
    */

    char s[] = "Héy"; // same as { 'H', 0xC3, 0xA9, 'y', 0 },   é is start byte + 1 cont. byte
    printf("Width: %d bytes\n", width_from_start_byte(s[2])); // start byte 0xC3 indicates 2-byte sequence

    return 0;
}