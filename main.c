#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024

void convert_markdown_to_html(FILE *input, FILE *output) {
    char line[MAX_LINE_LENGTH];

    while (fgets(line, MAX_LINE_LENGTH, input) != NULL) {
        // Trim newline
        line[strcspn(line, "\n")] = 0;

        // Handle headers
        if (line[0] == '#') {
            int header_level = 0;
            while (line[header_level] == '#') {
                header_level++;
            }
            fprintf(output, "<h%d>%s</h%d>\n", header_level, line + header_level + 1, header_level);
        }
        // Handle bold and italic (very basic, doesn't handle nested styles)
        else if (strstr(line, "**")) {
            char *bold_start = strstr(line, "**") + 2;
            char *bold_end = strstr(bold_start, "**");
            *bold_end = 0;
            fprintf(output, "<p>%.*s<b>%s</b>%s</p>\n", (int)(bold_start - line - 2), line, bold_start, bold_end + 2);
        }
        // Paragraphs
        else {
            fprintf(output, "<p>%s</p>\n", line);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s README.md output.html\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (!input) {
        perror("Error opening input file");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (!output) {
        perror("Error opening output file");
        fclose(input);
        return 1;
    }

    convert_markdown_to_html(input, output);

    fclose(input);
    fclose(output);

    printf("Conversion complete. Output written to %s\n", argv[2]);
    return 0;
}
