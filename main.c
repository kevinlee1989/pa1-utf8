#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

    // Keep looping while str's byte index reaches to null-terminator
    while(str[byte_index] != '\0'){
        // Getting how many bytes they have for a codepoint.
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
    
    // if starting and ending codepoints are less than 0 or starting codepoint is larger than ending codepoint then return with no changes.
    if(cpi_start < 0 || cpi_end < 0 || cpi_start >= cpi_end) {
        return;
    }

    // Changing the codepoint index to byte index to get all the bytes for 4 bytes long emoji
    int byte_start = codepoint_index_to_byte_index(str, cpi_start);
    int byte_end = codepoint_index_to_byte_index(str, cpi_end);

    // if codepoint_index_to_byte_index return -1 then return with no changes.
    if(byte_start == -1 || byte_end == -1) {
        return;
    }
    
    // New index for putting the bytes in to new str[]
    int index_result =0;

    for(int i = byte_start; i < byte_end; i++){
        result[index_result] = str[i];
        index_result += 1;
    }

    // ending with null-terminator at the last byte index.
    result[index_result] = '\0';

}

// Milestone3

// Takes a UTF-8 encoded string and a codepoint index, and returns a decimal representing the codepoint at that index.

int32_t codepoint_at(char str[], int32_t cpi){

    int byte_index = codepoint_index_to_byte_index(str, cpi);
    if(byte_index == -1) return -1;
    int width = width_from_start_byte(str[byte_index]);
    if(width == -1) return -1;

    int code_point =0;

    if(width == 1){
        code_point = str[byte_index];
    }
    else if(width == 2){
        // 2byte sequence : (110xxxxx 10xxxxxx)
        code_point = (((str[byte_index] & 0b00011111) * (int)pow(2,6)) | (str[byte_index + 1] & 0b00111111));
    }
    else if (width == 3) {
        // 3-byte : (1110xxxx 10xxxxxx 10xxxxxx)
        code_point = ((str[byte_index] & 0b00001111) * (int)pow(2, 12)) | 
                    ((str[byte_index + 1] & 0b00111111) * (int)pow(2, 6)) |
                    (str[byte_index + 2] & 0b00111111);
    } else if (width == 4) {
        // 4-byte : (11110xxx 10xxxxxx 10xxxxxx 10xxxxxx)
        code_point = ((str[byte_index] & 0b00000111) * (int)pow(2, 18)) | 
                    ((str[byte_index + 1] & 0b00111111) * (int)pow(2, 12)) |
                    ((str[byte_index + 2] & 0b00111111) * (int)pow(2, 6)) |
                    (str[byte_index + 3] & 0b00111111);
    }
    return code_point;

}

// Takes a UTF-8 encoded string and an codepoint index, and returns if the code point at that index is an animal emoji.
char is_animal_emoji_at(char str[], int32_t cpi){

    int codepoint = codepoint_at(str, cpi);

    // Check if the codepoint falls within the two specified ranges for animal emojis
    if ((codepoint >= 0x1F400 && codepoint <= 0x1F43F) ||  // 🐀 to 🐿️
        (codepoint >= 0x1F980 && codepoint <= 0x1F9AE)) {  // 🦀 to 🦮
        return 1;  // True: it is an animal emoji
    }
    
    return 0;
}


int main(){
    /* Mile Stone
    printf("Is 🔥 ASCII? %d\n", is_ascii("🔥"));
    printf("Is abcd ASCII? %d\n", is_ascii("abcd"));
    
    
    int32_t ret = 0;
    char str[] = "abcd";
    ret = capitalize_ascii(str);
    printf("Capitalized String: %s\nCharacters updated: %d\n", str, ret);
    */

     //MILESTONE2
    
    //char s[] = "Ω"; // same as { 'H', 0xC3, 0xA9, 'y', 0 },   é is start byte + 1 cont. byte
    //printf("Width: %d bytes\n", width_from_start_byte(s[0])); // start byte 0xC3 indicates 2-byte sequence
    
    //char str[] = "Joséph.";
    //printf("Length of string %s is %d\n", str, utf8_strlen(str));  // 6 codepoints, (even though 7 bytes)
    
    //char str[] = "🦀🦮🦮";
    //int32_t idx = 2;
    //printf("Codepoint index %d is byte index %d\n", idx, codepoint_index_to_byte_index(str, idx));
    //char result[17];
    //utf8_substring("🦀🦮🦮🦀🦀🦮🦮", 3, 5, result);
    //printf("String: \nSubstring: %s\n", result); // these emoji are 4 bytes long

    // MileStone3
    //char str[] = "Jos🦀ph";
    //int32_t idx = 3;
    //printf("Codepoint at %d in %s is %d\n", idx, str, codepoint_at(str, idx)); // 'p' is the 4th codepoint
 
    //UFTanalyzer
    //"My 🐩’s name is Erdős."
    char input[100];
    printf("Enter a UFT-8 encoded string: ");
    fgets(input, 100, stdin);

    // Getting rid of new line character, and if it is then replace to null character.
    int len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    //Is the string valid ASCII
    printf("Valid ASCII: %s\n", is_ascii(input) ? "true" : "false");
   
    // Uppercase the ASCII characters
    char uppercased[100];
    strcpy(uppercased, input);
    capitalize_ascii(uppercased);
    printf("Uppercased ASCII: %s\n", uppercased);

    // Length in bytes 
    int byte_length = strlen(input);
    printf("Length in bytes: %d\n", byte_length);

    // Number of codepoints
    int codepoints = utf8_strlen(input);
    printf("Number of code points: %d\n", codepoints);

    // Bytes per codepoint
    printf("bytes per code point: ");
    for(int i =0; i< byte_length;){
        int width = width_from_start_byte(input[i]);
        printf("%d ", width);
        i += (width);
    }
    printf("\n");
    
    // Substring of the first 6 code ponints
    char substring[30];
    utf8_substring(input, 0, 6, substring);
    printf("Substring of the first 6 code points: \"%s\"\n", substring);

    // Code points as decimal numbers
    int length_codepoint = utf8_strlen(input);
    printf("Code points as decimal numbers: ");
    for (int i = 0; i < length_codepoint; i++) {
        printf("%d ", codepoint_at(input, i));
    }
    printf("\n");

    // Animal emojis
    printf("Animal emojis: ");
    for (int i = 0; i < length_codepoint; i++) {
        if (is_animal_emoji_at(input, i)) {
            char emoji[5]; // emojis are 4 bytes max + null terminator
            utf8_substring(input, i, i + 1, emoji);
            printf("%s ", emoji);
        }
    }
    printf("\n");
}