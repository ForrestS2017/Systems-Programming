#include "multiThreadSorter.h"

/**
* Gets the index of an entry in a row (used for target column index)
* @param source String array of cells (from row or header)
* @param target Target String
* @return target index, or -1 upon failure
*/
int GetIndex(char** source, char* target) {
    if (!source || !target) {
        fprintf(stderr, "ERROR: Missing source or target row");
        return -1;
    }

    int i = 0;
    while (source[i]) {
        if (strcmp(source[i], target) == 0)
        return i;
        i++;
    }

    return -1;
}

/**
* Calls read() and sets row to entries where the line parsed into a char** array where delimiter = ','
  @param row Destination of row entries
* @return Number of entries in the row
*/
int GetLine(char*** row, int fd, Header header, int columns, int NUMBER_OF_COLUMNS) {
    int length = 8;
    int count = 0;
    char * line = (char*)malloc(sizeof(char) * (length + 1));
    if (line == NULL) {
        fprintf(stderr, "ERROR: malloc failed when allocating memory for line in GetLine, terminating GetLine.\n");
        printf("ERROR: malloc failed when allocating memory for line in GetLine, terminating GetLine.\n");
        return -2;
    }
    int j;
    for (j = 0; j < length + 1; j++) {
        line[j] = '\0';
    }
    int b; // number of bytes read by read
    char c = '\0';
    while ((b = read(fd, &c, 1)) > 0) {
        if (c == '\n') {
            break;
        } else {
           if (count + 1 > length) {
                length *= 2;
                line = (char*)realloc(line, sizeof(char) * (length + 1));
                if (line == NULL) {
                    fprintf(stderr, "ERROR: realloc failed when reallocating memory for line in GetLine, terminating GetLine.\n");
                    printf("ERROR: realloc failed when reallocating memory for line in GetLine, terminating GetLine.\n");
                    return -2;
                }
                for (j = length / 2 + 1; j < length + 1; j++) {
                    line[j] = '\0';
                }
           }
           line[count] = c;
           count++;
        }
    }
    
    if (b == -1 || strcmp(line, "") == 0) {
        return -1;
    }

    // Create 2D array to hold each entry
    int quotes = 0;
    int arrsize = columns + 1;
    int position = 0;
    char** entries = (char**) malloc(arrsize * sizeof(char*));
    if (entries == NULL) {
        fprintf(stderr, "ERROR: malloc failed when allocating memory for row entries in GetLine, terminating GetLine.\n");
        printf("ERROR: malloc failed when allocating memory for row entries in GetLine, terminating GetLine.\n");
        return -2;
    }
    for (j = 0; j < arrsize; j++) {
        entries[j] = EMPTY;
    }

    // Parse for commas and quotes
    size_t i = 0;
    size_t linepos = 0;
    size_t linelength = strlen(line);

    // While still entering entries
    while (linepos < linelength) {

        // If we have more entries than array size, double array length
        /*if (position >= arrsize) {
            arrsize *= 2;
            entries = (char**)realloc(entries, sizeof(char *) * arrsize);
            if (entries == NULL) {
                fprintf(stderr, "ERROR: realloc failed when reallocating memory for row entries in GetLine, terminating GetLine.\n");
                printf("ERROR: realloc failed when reallocating memory for row entries in GetLine, terminating GetLine.\n");
                return -2;
            }
        }*/

        i = 0;
        size_t entrylength = 30;
        char* entry = (char*) malloc((entrylength + 1) * sizeof(char));
        if (entry == NULL) {
            fprintf(stderr, "ERROR: malloc failed when allocating memory for entry in GetLine, terminating GetLine.\n");
            printf("ERROR: malloc failed when allocating memory for entry in GetLine, terminating GetLine.\n");
            return -2;
        }

        int a = 0;
        for (a = 0; a < entrylength; a++) {
            entry[a] = '\0';
        }

        // While reading from the entries line
        while (linepos < linelength) {
            char c = line[linepos];

            // Check for quotes
            if (c == '"' && quotes == 0)
            quotes++;
            else if (c == '"' && quotes == 1)
            quotes--;

            // Check for commas sans quotes
            if ((c == ',' && quotes == 0) || c == '\n') {
                linepos++;
                break;
            }
            entry[i] = line[linepos];

            i++;
            linepos++;
            if (i >= entrylength) {
                entrylength *= 2;
                entry = (char*)realloc(entry, sizeof(char) * (entrylength + 1));
                if (entry == NULL) {
                    fprintf(stderr, "ERROR: realloc failed when reallocating memory for entry in GetLine, terminating GetLine.\n");
                    printf("ERROR: realloc failed when reallocating memory for entry in GetLine, terminating GetLine.\n");
                    return -2;
                }
                for (a = entrylength / 2 + 1; a < entrylength; a++) {
                    entry[a] = '\0';
                }
            }
        }
        if (position >= NUMBER_OF_COLUMNS) {
            return -4;
        }
        char * title = header.titles[position];
        int l = 0;
        int found = -1;
        for (l = 0; l < columns; l++) {
            if (strcmp(sortHeaders[l], title) == 0) {
                found = l;
                break;
            }
        }
        if (found == -1) {
            return -3;
        } else {
            entries[found] = entry;
            position++;
        }
    }

    /*if (line[linepos - 1] == ',') { // last column is null
        if (position >= arrsize) {
            arrsize++;
            entries = (char**)realloc(entries, sizeof(char*) * arrsize);
            if (entries == NULL) {
                fprintf(stderr, "ERROR: realloc failed when reallocating memory for last row in GetLine, terminating GetLine.\n");
                printf("ERROR: realloc failed when reallocating memory for last row in GetLine, terminating GetLine.\n");
                return -2;
            }
        }
        char* entry = (char*) malloc(sizeof(char));
        if (entry == NULL) {
            fprintf(stderr, "ERROR: malloc failed when allocating memory for last entry in GetLine, terminating GetLine.\n");
            printf("ERROR: malloc failed when allocating memory for last entry in GetLine, terminating GetLine.\n");
            return -2;
        }
        entry[0] = '\0';
        entries[position] = entry;
        position++;
    }*/
    
    /*int hhh;
    int notEmpty = 0;
    for (hhh = 0; hhh < arrsize; hhh++) {
        if (entries[hhh][0] != '\0') {
            notEmpty = 1;
        }
    }
    if (notEmpty == 0) {
        printf("EMPTY\n");
    }*/
    
    *row = entries;
    free(line);
    return columns;
}

/**
* Copy the contents of input csv and parse into Row structs and validates each cell has correct type per its column
* @param Rows Row struct to fill
* @param header Header struct to fill
* @param columns Number of columns
* @return number of rows
*/
int FillRows(Row** Rows, Header header, int columns, int NUMBER_OF_COLUMNS, int fd) {
    int rows = 0;
    int capacity = 1;
    int w = 0;

    while (1) {
        char** entries = NULL;
        int c = GetLine(&entries, fd, header, columns, NUMBER_OF_COLUMNS);
        
        if (c < -1) {
            return c;
        }
        
        if (entries == NULL) {
            return rows;
        }

        (*Rows)[rows].entries = entries;
        /*for (w = 0; w < columns; w++) {
            format t = getType(entries[w]);
            if (t > header->types[w]) {
                header->types[w] = t;
            }
        }*/
        rows++;
        if (rows >= capacity) {
            capacity *= 2;
            *Rows = (Row*)realloc(*Rows, capacity * sizeof(Row));
            if (*Rows == NULL) {
                fprintf(stderr, "ERROR: realloc failed when reallocating memory for Rows in FillRows, terminating FillRows.\n");
                printf("ERROR: realloc failed when reallocating memory for Rows in FillRows, terminating FillRows.\n");
                return -2;
            }
            for (w = capacity / 2 + 2; w < capacity; w++) {
                (*Rows)[w].entries = NULL;
            }
        }
    }
    return rows;
}

/**
* Using GetLine(), fills a Header input, specifically
* @param h Header to be filled
* @return Number of columns, or -1 upon failure or no columns
*/

int SetHeader(Header* h, int fd) {

    // Malloc default space to be re-allocated

    int length = 8;
    int count = 0;
    char * line = (char*)malloc(sizeof(char) * (length + 1));
    if (line == NULL) {
        fprintf(stderr, "ERROR: malloc failed when allocating memory for line in SetHeader, terminating SetHeader.\n");
        printf("ERROR: malloc failed when allocating memory for line in SetHeader, terminating SetHeader.\n");
        return -2;
    }
    int j;
    for (j = 0; j < length + 1; j++) {
        line[j] = '\0';
    }

    // Read the entire line byte by byte
    int b; // number of bytes read by read
    char c = '\0';
    while ((b = read(fd, &c, 1)) > 0) {
        if (c == '\n') {
            break;
        } else {
           if (count + 1 > length) {
                length *= 2;
                line = (char*)realloc(line, sizeof(char) * (length + 1));
                if (line == NULL) {
                    fprintf(stderr, "ERROR: realloc failed when reallocating memory for line in SetHeader, terminating SetHeader.\n");
                    printf("ERROR: realloc failed when reallocating memory for line in SetHeader, terminating SetHeader.\n");
                    return -2;
                }
                for (j = length / 2 + 1; j < length + 1; j++) {
                    line[j] = '\0';
                }
           }
           line[count] = c;
           count++;
        }
    }
    
    // Reached the end inappropriately
    if (b == -1 || strcmp(line, "") == 0) {
        return -1;
    }

    int quotes = 0;
    int arrsize = 1;
    int position = 0;

    size_t i = 0;
    size_t linepos = 0;
    size_t linelength = strlen(line);
    
    // Prepping array size and string size for header array
    while (linepos < linelength) {

        if (position >= arrsize) {
            arrsize *= 2;
            h->titles = (char**)realloc(h->titles, sizeof(char*) * (arrsize + 1));
            if (h->titles == NULL) {
                fprintf(stderr, "ERROR: realloc failed when reallocating memory for titles in SetHeader, terminating SetHeader.\n");
                printf("ERROR: realloc failed when reallocating memory for titles in SetHeader, terminating SetHeader.\n");
                return -1;
            }
        }

        i = 0;
        size_t entrylength = 30;
        char* entry = (char*) malloc((entrylength + 1) * sizeof(char));
        if (entry == NULL) {
            fprintf(stderr, "ERROR: malloc failed when allocating memory for entry in SetHeader, terminating SetHeader.\n");
            printf("ERROR: malloc failed when allocating memory for entry in SetHeader, terminating SetHeader.\n");
            return -1;
        }

        int a = 0;
        for (a = 0; a < entrylength; a++) {
            entry[a] = '\0';
        }
        
        int hCount = 27;    // Ensure we are 
        while (linepos < linelength) {
            char c = line[linepos];

            if (c == '"' && quotes == 0)
            quotes++;
            else if (c == '"' && quotes == 1)
            quotes--;

            if ((c == ',' && quotes == 0) || c == '\n') {
                linepos++;
                break;
            }

            if (c == '\r') {
                linepos++;
                continue;
            }

            entry[i] = line[linepos];

            i++;
            linepos++;
            if (i >= entrylength) {
                entrylength *= 2;
                entry = (char*)realloc(entry, sizeof(char) * (entrylength + 1));
                if (entry == NULL) {
                    fprintf(stderr, "ERROR: realloc failed when reallocating memory for entry in SetHeader, terminating SetHeader.\n");
                    printf("ERROR: realloc failed when reallocating memory for entry in SetHeader, terminating SetHeader.\n");
                    return -1;
                }
                for (a = entrylength / 2 + 1; a < entrylength; a++) {
                    entry[a] = '\0';
                }
            }
        }
        h->titles[position] = entry;
        position++;
    }

    if (line[linepos - 1] == ',') { // last column is null
        if (position >= arrsize) {
            arrsize++;
            h->titles = (char**)realloc(h->titles, sizeof(char*) * arrsize);
            if (h->titles == NULL) {
                fprintf(stderr, "ERROR: realloc failed when reallocating memory for last title column in SetHeader, terminating SetHeader.\n");
                printf("ERROR: realloc failed when reallocating memory for last title column in SetHeader, terminating SetHeader.\n");
                return -2;
            }
        }
        char* entry = (char*) malloc(sizeof(char));
        if (entry == NULL) {
            fprintf(stderr, "ERROR: malloc failed when allocating memory for last entry in SetHeader, terminating SetHeader.\n");
            printf("ERROR: malloc failed when allocating memory for last entry in SetHeader, terminating SetHeader.\n");
            return -2;
        }
        entry[0] = '\0';
        h->titles[position] = trim(entry);
        position++;
    }

    return position;
}

/**
 * Trims white space and removes quotation marks from a string
 * @param str String to parse
 * @return Formatted string
 */
char* trim(char* str) {
    int length = strlen(str);
    char * ret = (char*)malloc(sizeof(char) * (length + 1));
    if (ret == NULL) {
        fprintf(stderr, "ERROR: malloc failed when allocating memory in trim, terminating trim.\n");
        printf("ERROR: malloc failed when allocating memory in trim, terminating trim.\n");
        return str;
    }
    int j = 0;
    int i = 0;
    char prev = '\0';
    char firstNonSpaceChar = '\0';
    short started = 0; // for determining whether string started or not... quotes and spaces don't count as start
    for (i = 0; i < length; i++) {
        if (started == 0 && (str[i] == '"' || str[i] == ' ')) {
            if (str[i] == '"' && firstNonSpaceChar == '\0') {
                firstNonSpaceChar = '"';
            }
            continue;
        }
        if (str[i] == '"' && i == length - 1 && firstNonSpaceChar == '"') {
            break;
        }
        if (str[i] == ' ') {
            prev = ' ';
            continue;
        }
        if (isspace(str[i])) {
            continue;
        }

        if (prev == ' ') {
            ret[j] = ' ';
            j++;
        }

        if (started == 0) {
            started = 1;
        }

        ret[j] = str[i];
        j++;
        prev = str[i];
    }
    ret[j] = '\0';
    return ret;
}

/**
* Returns format enum of the input string/cell
* @param str String to be determined the format type
* @return format of input str
*/
format getType(char* str) {
    int i = 0;
    char * s = trim(str);
    int n = strlen(s);
    format ret = NUMBER;
    int periods = 0;
    for (i = 0; i < n; i++) {
        if (i == 0 && s[i] == '-') {
            continue;
        } else if (!isdigit(s[i])) {
            if (s[i] == '.' && periods == 0) {
                periods++;
            } else {
                ret = STRING;
                break;
            }
        }
    }
    free(s);
    return ret;
}
