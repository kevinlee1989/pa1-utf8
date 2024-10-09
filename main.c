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

    if((start_byte & 0b11100000) == 0b11000000){
        return 2;
    }
    else if((start_byte & 0b11110000) == 0b11100000){
        return 3;
    }
    else if((start_byte & 0b11111000) == 0b11110000){
        return 4;
    }
    else if(start_byte>=0 && start_byte<= 255){
        return 1;
    }
    else{
        return -1;
    }
}

// Takes a UTF-8 encoded string and returns the number of UTF-8 codepoints it represents.
// Returns -1 if there are any errors encountered in processing the UTF-8 string.
int32_t utf8_strlen(char str[]){
    int length =0;

    for(int i =0; i < strlen(str); i++){
        if(width_from_start_byte(str[i]) == 1){
            length += 1;
        }
        else if(width_from_start_byte(str[i]) == 2){
            length += 1;
            i += 1;
        }
        else if(width_from_start_byte(str[i]) == 3){
            length += 1;
            i += 2;
        }
        else if(width_from_start_byte(str[i]) == 4){
            length += 1;
            i += 3;
        }
        else{
            return -1;
        }
    }
        
    return length;
}

//Given a UTF-8 encoded string, and a codepoint index, 
//return the byte index in the string where the Unicode character at the given codepoint index starts.
//Returns -1 if there are any errors encountered in processing the UTF-8 string.
int32_t codepoint_index_to_byte_index(char str[], int32_t cpi){

    int byte_index = 0;
    int codepoint_count = 0;
    while(str[byte_index] != '\0'){
        int width = width_from_start_byte(str[byte_index]);

        if(width == -1) return -1;

        if(codepoint_count == cpi) return byte_index;

        byte_index += width;
        codepoint_count += 1;
    }   
    return byte_index;
}

// Takes a UTF-8 encoded string and start(inclusive) and end(exclusive) codepoint indices, 
// and writes the substring between those indices to result, with a null terminator. 
// Assumes that result has sufficient bytes of space available. 
// (Hint: result will be created beforehand with a given size and passed as input here.
// Can any of the above functions be used to determine what the size of result should be?)
// If cpi_start is greater than cpi_end or either is negative,
// the function should have no effect.
void utf8_substring(char str[], int32_t cpi_start, int32_t cpi_end, char result[]){
    

    if(cpi_start < 0 || cpi_end < 0 || cpi_start >= cpi_end) {
        // No effect, leave result unchanged
        return;
    }

    int byte_start = codepoint_index_to_byte_index(str, cpi_start);
    int byte_end = codepoint_index_to_byte_index(str, cpi_end);
    printf("\n\n\n%d\n\n\n", byte_end);

    if(byte_start == -1 || byte_end == -1) {
        return;
    }
    
    int index_result =0;

    for(int i = byte_start; i < byte_end; i++){
        result[index_result] = str[i];
        index_result += 1;
    }


}
int main(){
    /* MILESTONE1
    printf("Is 🔥 ASCII? %d\n", is_ascii("🔥"));
    printf("Is abcd ASCII? %d\n", is_ascii("abcd"));

    
    int32_t ret = 0;
    char str[] = "abcd";
    ret = capitalize_ascii(str);
    printf("Capitalized String: %s\nCharacters updated: %d\n", str, ret);
    */

     //MILESTONE2
    
    //char s[] = "Héy"; // same as { 'H', 0xC3, 0xA9, 'y', 0 },   é is start byte + 1 cont. byte
    //printf("Width: %d bytes\n", width_from_start_byte(s[1])); // start byte 0xC3 indicates 2-byte sequence
    
    //char str[] = "Joséph";
    //printf("Length of string %s is %d\n", str, utf8_strlen(str));  // 6 codepoints, (even though 7 bytes)
    
    //char str[] = "🦀🦮🦮";
    //int32_t idx = 2;
    //printf("Codepoint index %d is byte index %d\n", idx, codepoint_index_to_byte_index(str, idx));
    char result[17];
    utf8_substring("🦀🦮🦮🦀🦀🦮🦮", 3, 5, result);
    printf("String: \nSubstring: %s\n", result); // these emoji are 4 bytes long

    //=== Output ===
    //String: 🦀🦮🦮🦀🦀🦮🦮
    //Substring: 🦀🦀🦮🦮
    //return 0;
}