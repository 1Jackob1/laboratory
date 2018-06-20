#include <stdio.h>
#include <stdlib.h>

#define BUFF_SIZE 10

typedef unsigned char byte;

typedef struct Bit_Stream{
    FILE* file;
    byte buff[BUFF_SIZE];
    int pos, len;
    byte bit;
    int type;
}t_bit_stream;

t_bit_stream* bit_open(FILE *f, int type){
    if(f == NULL)
        return NULL;
    t_bit_stream *tmp = malloc(sizeof(t_bit_stream));
    tmp->bit = 1;
    tmp->len = 0;
    tmp->pos = 0;
    tmp->file = f;
    tmp->type = type;
    return tmp;
}

void bit_flush(t_bit_stream *a){
    if(!(a->type)) fwrite(a->buff, 1, a->pos+1, a->file);
    a->len = 0;
    a->pos = 0;
    return;
}

void bit_close(t_bit_stream *a){
    bit_flush(a);
    free(a);
    a=NULL;
}

int bit_feof(t_bit_stream *a){
    return (a->pos == a->len) && feof(a->file);
}

int bit_getc(t_bit_stream *a){
    if(a->bit==0){
        a->pos++;
        a->bit = 1;
    }
    if(a->pos == a->len){
        a->len = fread(a->buff, 1, BUFF_SIZE, a->file);
        a->pos = 0;
        if(a->len == 0)
            return EOF;

    }
    int c=((a->buff[a->pos] & a->bit) != 0);
    a->bit <<= 1;
    return c;
}

int bit_putc(t_bit_stream *a, int src){
    if(a->bit == 0){
        (a->pos)++;
        a->bit = 1;
    }
    if(a->pos == BUFF_SIZE){
        fwrite(a->buff, 1, BUFF_SIZE, a->file);
        a->pos=0;
        }
    if(src)
        a->buff[a->pos] |= a->bit;
    else
        a->buff[a->pos] &= ~(a->bit);
    a->bit <<= 1;
    return src;
}

int main()
{
    FILE *fin=fopen("input.txt", "rb");
    FILE *fout=fopen("output.txt", "wb");
    t_bit_stream *in = bit_open(fin, 1);
    t_bit_stream *out = bit_open(fout, 0);
    while(!(bit_feof(in))){
        int a = bit_getc(in);
        if(a != (-1))
            bit_putc(out, a);
        printf("%i\n", a);
    }
    bit_close(in);
    bit_close(out);
    fclose(fin);
    fclose(fout);
    return 0;
}
