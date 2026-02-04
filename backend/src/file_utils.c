#include "file_utils.h"

void skip_line(FILE *fp){
    int ch;
    while((ch = fgetc(fp)) != '\n' && ch != EOF);
}
