#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define WAV_SIZE 11204480

int main(int argc, char *argv[])
{
    FILE *p = fopen("./mp3", "rb");
    if (!p)
        return -1;
    char *buf = (char *)malloc(sizeof(char) * WAV_SIZE);
    fread(buf, sizeof(char), WAV_SIZE, p);
    fclose(p);
    for (int i = 0; i < WAV_SIZE; ++i)
    {
        buf[i] = ~buf[i];
    }
    FILE *w = fopen("./3pm", "wb");
    if (!w)
    {
        return -1;
    }
    fwrite(buf, sizeof(char), WAV_SIZE, w);
    fclose(w);
    return 0;
}