#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>


#define INPUT_LENGTH 16


char* trim_input(char* s)
{
    char* begin = s;

    while(isspace(*begin)) ++begin;
    if( *begin == '\0' ) return begin;

    char* end=begin+strlen(begin)-1;
    while((end > begin) && (isspace(*end)) || iscntrl(*end)) --end;

    end[1] = '\0';
    return begin;
}


uint32_t decode_erc(uint32_t a, uint32_t b)
{
    // Reverse b
    uint32_t rb=0;
    for(int i=0; i<32; ++i)
    {
        rb |= ((b & 1<<i) >> i) << (31 - i);
    }

    return (a ^ rb) - 0xE010A11;
}


int main()
{
    char input_str[]  = "0000000000000000\0some extra space";
    assert(sizeof(input_str) > INPUT_LENGTH && input_str[INPUT_LENGTH] == '\0');

    printf("16 digits ERC : ");
    if( fgets(input_str, sizeof(input_str), stdin) == NULL )
    {
        int ec = ferror(stdin);
        fprintf(stderr, "error: %s\n", strerror(ec));
        return ec;
    }

    char* erc_str = trim_input(input_str);
    int len = strlen(erc_str);
    if( len != INPUT_LENGTH )
    {
        fprintf(stderr, "error: expecting %d digits (found %d)\n",
                INPUT_LENGTH, len);
        return 1;
    }

    //printf("converting \"%s\" \n", erc_str);
    uint32_t a=0;
    uint32_t b=0;

    char* startptr=erc_str+8;
    char* endptr;
    b = strtol(startptr, &endptr, 16);
    if(endptr<erc_str+16)
    {
        fprintf(stderr, "error: '%c' invalid character (%ld)\n",
                    *endptr, 8+endptr-startptr+1);
        return 2;
    }
    erc_str[8] = '\0';
    a = strtol(erc_str, &endptr, 16);
    if(endptr<erc_str+8)
    {
        fprintf(stderr, "error: '%c' invalid character (%ld)\n",
                    *endptr, endptr-startptr+1);
        return 2;
    }

    //printf("%u %u\n", a, b);

    printf("%X\n", decode_erc(a, b));

    return 0;
}
