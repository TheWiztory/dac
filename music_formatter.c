#include <stdio.h>

int main() {
    int rate = 22050;
    int seconds = 5;
    int c;

    printf("#ifndef MUSIC_H\n");
    printf("#define MUSIC_H\n\n");
    printf("#define DEFAULT_POSITION_DELTA 1\n\n");
    printf("#define REWIND_POSITION_DELTA -2\n");
    printf("#define FAST_FORWARD_POSITION_DELTA 2\n\n");
    printf("int samples_size = %d;\n\n", rate * seconds);
    printf("int position = 0;\n\n");
    printf("int position_delta = DEFAULT_POSITION_DELTA;\n\n");
    printf("uint8_t samples[] = {");
    for (int i = 0; i < rate * seconds; i++) {
        c = getchar();
        printf("%d", c);
        if (i != rate * seconds - 1)
            printf(", ");
    }
    printf("};\n\n");
    printf("#endif\n");
}