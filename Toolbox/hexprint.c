// a little program like hexdump, but outputs hex initializer lists instead of hex space concatenated

#include <errno.h>
#include <stdio.h>
#include <string.h>

unsigned char buffer[BUFSIZ];

int main(int argc, char* argv[])
{
    FILE* fin = NULL;
    for (int in_file_index = 1; in_file_index < argc; in_file_index++)
    {
        fin = fopen(argv[in_file_index], "rb");
        if (!fin)
        {
            fprintf(stderr, "Error opening file '%s': %s; skipping\n", argv[in_file_index], strerror(errno));
            continue;
        }

        size_t in_read;
        if (argc == 2)
            fprintf(stdout, "{");
        else
            fprintf(stdout, "//%s\n{", argv[in_file_index]);
        while ((in_read = fread(buffer, 1, sizeof buffer, fin)))
        {
            for (size_t i = 0; i < in_read; i++)
            {
                fprintf(stdout, "0x%02X", buffer[i]);
                if (i != in_read - 1 || !feof(fin))
                {
                    fprintf(stdout, ",");
                }
            }
        }
        fprintf(stdout, "}\n");

        fclose(fin);
    }
}