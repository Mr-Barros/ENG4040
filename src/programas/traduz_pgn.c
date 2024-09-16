#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 8192

void parse_pgn(const char *file_data, size_t file_size, FILE *output) {
    char white_player[BUFFER_SIZE] = {0};
    char black_player[BUFFER_SIZE] = {0};
    char result[BUFFER_SIZE] = {0};

    const char *ptr = file_data;
    const char *end = file_data + file_size;

    // Write the CSV header
    fprintf(output, "White Player,Black Player,Result\n");

    while (ptr < end) {
        // Find the start of a line
        const char *line_start = ptr;
        while (ptr < end && *ptr != '\n') {
            ptr++;
        }
        // Process the line (now between line_start and ptr)
        size_t line_length = ptr - line_start;
        if (line_length > 0) {
            // Example: process [White "Player"]
            if (strncmp(line_start, "[White \"", 7) == 0) {
                sscanf(line_start, "[White \"%[^\"]", white_player);
            } else if (strncmp(line_start, "[Black \"", 7) == 0) {
                sscanf(line_start, "[Black \"%[^\"]", black_player);
            } else if (strncmp(line_start, "[Result \"", 8) == 0) {
                sscanf(line_start, "[Result \"%[^\"]", result);
            }

            // Once we have all the necessary fields, write them to the CSV
            if (white_player[0] && black_player[0] && result[0]) {
                fprintf(output, "%s,%s,%s\n", white_player, black_player, result);
                // Reset for the next game
                white_player[0] = '\0';
                black_player[0] = '\0';
                result[0] = '\0';
            }
        }

        // Move to the next line
        if (ptr < end && *ptr == '\n') {
            ptr++;
        }
    }
}

int main() {
    // Open the PGN file
    int fd = open("large_games.pgn", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // Get the size of the file
    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("Error getting file size");
        close(fd);
        return EXIT_FAILURE;
    }
    size_t file_size = st.st_size;

    // Memory-map the file
    char *file_data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file_data == MAP_FAILED) {
        perror("Error mapping file");
        close(fd);
        return EXIT_FAILURE;
    }

    // Open the CSV output file
    FILE *output_file = fopen("output_games.csv", "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        munmap(file_data, file_size);
        close(fd);
        return EXIT_FAILURE;
    }

    // Parse the PGN data and write to CSV
    parse_pgn(file_data, file_size, output_file);

    // Clean up
    fclose(output_file);
    munmap(file_data, file_size);
    close(fd);

    printf("Large PGN file parsed and saved to CSV successfully.\n");

    return 0;
}




// below here follows the mmap approach

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 8192

void parse_pgn(const char *file_data, size_t file_size, FILE *output) {
    char white_player[BUFFER_SIZE] = {0};
    char black_player[BUFFER_SIZE] = {0};
    char result[BUFFER_SIZE] = {0};

    const char *ptr = file_data;
    const char *end = file_data + file_size;

    // Write the CSV header
    fprintf(output, "White Player,Black Player,Result\n");

    while (ptr < end) {
        // Find the start of a line
        const char *line_start = ptr;
        while (ptr < end && *ptr != '\n') {
            ptr++;
        }
        // Process the line (now between line_start and ptr)
        size_t line_length = ptr - line_start;
        if (line_length > 0) {
            // Example: process [White "Player"]
            if (strncmp(line_start, "[White \"", 7) == 0) {
                sscanf(line_start, "[White \"%[^\"]", white_player);
            } else if (strncmp(line_start, "[Black \"", 7) == 0) {
                sscanf(line_start, "[Black \"%[^\"]", black_player);
            } else if (strncmp(line_start, "[Result \"", 8) == 0) {
                sscanf(line_start, "[Result \"%[^\"]", result);
            }

            // Once we have all the necessary fields, write them to the CSV
            if (white_player[0] && black_player[0] && result[0]) {
                fprintf(output, "%s,%s,%s\n", white_player, black_player, result);
                // Reset for the next game
                white_player[0] = '\0';
                black_player[0] = '\0';
                result[0] = '\0';
            }
        }

        // Move to the next line
        if (ptr < end && *ptr == '\n') {
            ptr++;
        }
    }
}

int main() {
    // Open the PGN file
    int fd = open("large_games.pgn", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // Get the size of the file
    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("Error getting file size");
        close(fd);
        return EXIT_FAILURE;
    }
    size_t file_size = st.st_size;

    // Memory-map the file
    char *file_data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file_data == MAP_FAILED) {
        perror("Error mapping file");
        close(fd);
        return EXIT_FAILURE;
    }

    // Open the CSV output file
    FILE *output_file = fopen("output_games.csv", "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        munmap(file_data, file_size);
        close(fd);
        return EXIT_FAILURE;
    }

    // Parse the PGN data and write to CSV
    parse_pgn(file_data, file_size, output_file);

    // Clean up
    fclose(output_file);
    munmap(file_data, file_size);
    close(fd);

    printf("Large PGN file parsed and saved to CSV successfully.\n");

    return 0;
}




/***************************************************************
 * 
 * possible function to read moves and immediately write to csv
 * 
 * *************************************************************/

void parse_pgn(const char *file_data, size_t file_size, FILE *output) {
    char white_player[BUFFER_SIZE] = {0};
    char black_player[BUFFER_SIZE] = {0};
    char result[BUFFER_SIZE] = {0};

    const char *ptr = file_data;
    const char *end = file_data + file_size;

    // Write the CSV header
    fprintf(output, "White Player,Black Player,Result,Moves\n");

    int in_moves_section = 0;  // Flag to track if we are in the moves section

    while (ptr < end) {
        // Find the start of a line
        const char *line_start = ptr;
        while (ptr < end && *ptr != '\n') {
            ptr++;
        }
        size_t line_length = ptr - line_start;

        // Process the line (now between line_start and ptr)
        if (line_length > 0) {
            if (strncmp(line_start, "[White \"", 7) == 0) {
                sscanf(line_start, "[White \"%[^\"]", white_player);
                in_moves_section = 0;  // Reset the moves section flag when we hit metadata
            } else if (strncmp(line_start, "[Black \"", 7) == 0) {
                sscanf(line_start, "[Black \"%[^\"]", black_player);
                in_moves_section = 0;
            } else if (strncmp(line_start, "[Result \"", 8) == 0) {
                sscanf(line_start, "[Result \"%[^\"]", result);
                in_moves_section = 0;
            } else if (line_start[0] == '1' || in_moves_section) {
                // Start or continue collecting moves
                in_moves_section = 1;
                
                // If this is the first move in a game, write the players and prepare to write moves
                if (white_player[0] && black_player[0] && result[0]) {
                    fprintf(output, "%s,%s,%s,\"", white_player, black_player, result);
                    white_player[0] = black_player[0] = result[0] = '\0';  // Clear after writing
                }

                // Write the moves line by line to the CSV
                fwrite(line_start, 1, line_length, output);
                fputc(' ', output);  // Replace newline with space for the next line

                // Detect if the result is part of the moves, end the game entry here
                if (in_moves_section && strstr(line_start, result)) {
                fprintf(output, "\"\n");  // Close the moves field with a quote
                in_moves_section = 0;     // Reset for the next game
                }
            }
        }

        // Move to the next line
        if (ptr < end && *ptr == '\n') {
            ptr++;
        }
    }
}



/***************************************************************
 * 
 * possible function to read moves and store them in a buffer to later write
 * 
 * *************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 8192

void parse_pgn(const char *file_data, size_t file_size, FILE *output) {
    char white_player[BUFFER_SIZE] = {0};
    char black_player[BUFFER_SIZE] = {0};
    char result[BUFFER_SIZE] = {0};
    char game_moves[BUFFER_SIZE * 4] = {0};  // Buffer for game moves (increased size)

    const char *ptr = file_data;
    const char *end = file_data + file_size;

    // Write the CSV header
    fprintf(output, "White Player,Black Player,Result,Moves\n");

    int in_moves_section = 0;  // Flag to track if we are in the moves section

    while (ptr < end) {
        // Find the start of a line
        const char *line_start = ptr;
        while (ptr < end && *ptr != '\n') {
            ptr++;
        }
        size_t line_length = ptr - line_start;

        // Process the line (now between line_start and ptr)
        if (line_length > 0) {
            if (strncmp(line_start, "[White \"", 7) == 0) {
                sscanf(line_start, "[White \"%[^\"]", white_player);
                in_moves_section = 0;  // Reset the moves section flag when we hit metadata
            } else if (strncmp(line_start, "[Black \"", 7) == 0) {
                sscanf(line_start, "[Black \"%[^\"]", black_player);
                in_moves_section = 0;
            } else if (strncmp(line_start, "[Result \"", 8) == 0) {
                sscanf(line_start, "[Result \"%[^\"]", result);
                in_moves_section = 0;
            } else if (line_start[0] == '1' || in_moves_section) {
                // Start or continue collecting moves
                in_moves_section = 1;
                strncat(game_moves, line_start, line_length);
                strcat(game_moves, " ");  // Add a space between moves
            }

            // Once we have all the necessary fields, write them to the CSV
            if (white_player[0] && black_player[0] && result[0] && in_moves_section) {
                if (strstr(line_start, result)) {  // If the result line is in the moves
                    fprintf(output, "%s,%s,%s,\"%s\"\n", white_player, black_player, result, game_moves);

                    // Reset for the next game
                    white_player[0] = '\0';
                    black_player[0] = '\0';
                    result[0] = '\0';
                    game_moves[0] = '\0';
                    in_moves_section = 0;
                }
            }
        }

        // If we reach the end of the moves but haven't written the result yet, write it manually
        if (in_moves_section && ptr == end && !result_written) {
            fprintf(output, "\"\n");  // Close the moves section and finish the game
            in_moves_section = 0;
        }

        // Move to the next line
        if (ptr < end && *ptr == '\n') {
            ptr++;
        }
    }
}




#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 8192

void parse_pgn(const char *file_data, size_t file_size, FILE *output) {
    char white_player[BUFFER_SIZE] = {0};
    char black_player[BUFFER_SIZE] = {0};
    char result[BUFFER_SIZE] = {0};

    const char *ptr = file_data;
    const char *end = file_data + file_size;

    // Write the CSV header
    fprintf(output, "White Player,Black Player,Result,Moves\n");

    int in_moves_section = 0;  // Flag to track if we are in the moves section
    int result_written = 0;    // Flag to track if the result was written

    while (ptr < end) {
        // Find the start of a line
        const char *line_start = ptr;
        while (ptr < end && *ptr != '\n') {
            ptr++;
        }
        size_t line_length = ptr - line_start;

        // Process the line (now between line_start and ptr)
        if (line_length > 0) {
            if (strncmp(line_start, "[White \"", 7) == 0) {
                sscanf(line_start, "[White \"%[^\"]", white_player);
                in_moves_section = 0;  // Reset the moves section flag when we hit metadata
                result_written = 0;    // Reset the result flag for a new game
            } else if (strncmp(line_start, "[Black \"", 7) == 0) {
                sscanf(line_start, "[Black \"%[^\"]", black_player);
                in_moves_section = 0;
                result_written = 0;
            } else if (strncmp(line_start, "[Result \"", 8) == 0) {
                sscanf(line_start, "[Result \"%[^\"]", result);
                in_moves_section = 0;
            } else if (line_start[0] == '1' || in_moves_section) {
                // Start or continue collecting moves
                in_moves_section = 1;
                
                // If this is the first move in a game, write the players and prepare to write moves
                if (white_player[0] && black_player[0] && result[0] && !result_written) {
                    fprintf(output, "%s,%s,%s,\"", white_player, black_player, result);
                    white_player[0] = black_player[0] = result[0] = '\0';  // Clear after writing
                    result_written = 1;  // Mark result as written to avoid duplicate
                }

                // Write the moves line by line to the CSV
                fwrite(line_start, 1, line_length, output);
                fputc(' ', output);  // Replace newline with space for the next line

                // Detect if the result is part of the moves, end the game entry here
                if (strstr(line_start, result)) {
                    fprintf(output, "\"\n");  // Close the moves field with a quote and move to the next game
                    in_moves_section = 0;     // Reset for the next game
                    result_written = 1;       // Mark result as written
                }
            }
        }

        // Move to the next line
        if (ptr < end && *ptr == '\n') {
            ptr++;
        }

        // If we reach the end of the moves but haven't written the result yet, write it manually
        if (in_moves_section && !result_written) {
            fprintf(output, "\"\n");  // Close the moves section and finish the game
            in_moves_section = 0;
        }
    }
}


int main() {
    // Open the PGN file
    int fd = open("large_games.pgn", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // Get the size of the file
    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("Error getting file size");
        close(fd);
        return EXIT_FAILURE;
    }
    size_t file_size = st.st_size;

    // Memory-map the file
    char *file_data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file_data == MAP_FAILED) {
        perror("Error mapping file");
        close(fd);
        return EXIT_FAILURE;
    }

    // Open the CSV output file
    FILE *output_file = fopen("output_games.csv", "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        munmap(file_data, file_size);
        close(fd);
        return EXIT_FAILURE;
    }

    // Parse the PGN data and write to CSV
    parse_pgn(file_data, file_size, output_file);

    // Clean up
    fclose(output_file);
    munmap(file_data, file_size);
    close(fd);

    printf("Large PGN file parsed and saved to CSV successfully.\n");

    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 8192
#define MAX_HEADERS 100  // Maximum number of different headers to handle

typedef struct {
    char key[BUFFER_SIZE];
    char value[BUFFER_SIZE];
} Header;

void parse_pgn(const char *file_data, size_t file_size, FILE *output) {
    Header headers[MAX_HEADERS];
    int header_count = 0;
    char moves[BUFFER_SIZE] = {0};  // Buffer for the moves section
    int in_moves_section = 0;  // Flag to track if we are in the moves section
    int header_row_written = 0;  // Flag to track if the CSV header has been written

    const char *ptr = file_data;
    const char *end = file_data + file_size;

    while (ptr < end) {
        // Reset all header values for each game
        for (int i = 0; i < header_count; ++i) {
            headers[i].value[0] = '\0';
        }
        moves[0] = '\0';
        in_moves_section = 0;

        while (ptr < end) {
            const char *line_start = ptr;
            while (ptr < end && *ptr != '\n') {
                ptr++;
            }
            size_t line_length = ptr - line_start;

            if (line_length > 0) {
                if (line_start[0] == '[') {
                    // Parse the header key and value
                    char key[BUFFER_SIZE] = {0};
                    char value[BUFFER_SIZE] = {0};
                    sscanf(line_start, "[%[^ ] \"%[^\"]", key, value);

                    // Check if this key already exists in the header array
                    int header_index = -1;
                    for (int i = 0; i < header_count; ++i) {
                        if (strcmp(headers[i].key, key) == 0) {
                            header_index = i;
                            break;
                        }
                    }

                    // If it's a new key, store it
                    if (header_index == -1 && header_count < MAX_HEADERS) {
                        header_index = header_count;
                        strcpy(headers[header_count].key, key);
                        header_count++;
                    }

                    // Store the value in the corresponding header
                    if (header_index != -1) {
                        strcpy(headers[header_index].value, value);
                    }
                } else if (line_start[0] == '1' || in_moves_section) {
                    // Start or continue collecting moves
                    in_moves_section = 1;
                    strncat(moves, line_start, line_length);
                    strcat(moves, " ");
                }
            }

            // Move to the next line
            if (ptr < end && *ptr == '\n') {
                ptr++;
            }

            // If we reach an empty line, assume the game is over
            if (line_length == 0 && in_moves_section) {
                break;
            }
        }

        // If the header row hasn't been written, write it now
        if (!header_row_written) {
            for (int i = 0; i < header_count; ++i) {
                fprintf(output, "%s,", headers[i].key);
            }
            fprintf(output, "Moves\n");  // The last column will always be "Moves"
            header_row_written = 1;
        }

        // Write the collected header values to the CSV
        for (int i = 0; i < header_count; ++i) {
            fprintf(output, "\"%s\",", headers[i].value);
        }

        // Write the moves to the CSV
        fprintf(output, "\"%s\"\n", moves);
    }
}

int main() {
    // Open the PGN file
    int fd = open("large_games.pgn", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // Get the size of the file
    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("Error getting file size");
        close(fd);
        return EXIT_FAILURE;
    }
    size_t file_size = st.st_size;

    // Memory-map the file
    char *file_data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file_data == MAP_FAILED) {
        perror("Error mapping file");
        close(fd);
        return EXIT_FAILURE;
    }

    // Open the CSV output file
    FILE *output_file = fopen("output_games.csv", "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        munmap(file_data, file_size);
        close(fd);
        return EXIT_FAILURE;
    }

    // Parse the PGN data and write to CSV
    parse_pgn(file_data, file_size, output_file);

    // Clean up
    fclose(output_file);
    munmap(file_data, file_size);
    close(fd);

    printf("Large PGN file parsed and saved to CSV successfully.\n");

    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 8192
#define MAX_HEADERS 100  // Maximum number of different headers to handle

typedef struct {
    char key[BUFFER_SIZE];
    char value[BUFFER_SIZE];
} Header;

void parse_pgn(const char *file_data, size_t file_size, FILE *output) {
    Header headers[MAX_HEADERS];
    int header_count = 0;
    char moves[BUFFER_SIZE] = {0};  // Buffer for the moves section
    int in_moves_section = 0;  // Flag to track if we are in the moves section
    int header_row_written = 0;  // Flag to track if the CSV header has been written

    const char *ptr = file_data;
    const char *end = file_data + file_size;

    while (ptr < end) {
        // Reset all header values for each game
        for (int i = 0; i < header_count; ++i) {
            headers[i].value[0] = '\0';
        }
        moves[0] = '\0';
        in_moves_section = 0;

        while (ptr < end) {
            const char *line_start = ptr;
            while (ptr < end && *ptr != '\n') {
                ptr++;
            }
            size_t line_length = ptr - line_start;

            if (line_length > 0) {
                if (line_start[0] == '[') {
                    // Parse the header key and value
                    char key[BUFFER_SIZE] = {0};
                    char value[BUFFER_SIZE] = {0};
                    sscanf(line_start, "[%[^ ] \"%[^\"]", key, value);

                    // Check if this key already exists in the header array
                    int header_index = -1;
                    for (int i = 0; i < header_count; ++i) {
                        if (strcmp(headers[i].key, key) == 0) {
                            header_index = i;
                            break;
                        }
                    }

                    // If it's a new key, store it
                    if (header_index == -1 && header_count < MAX_HEADERS) {
                        header_index = header_count;
                        strcpy(headers[header_count].key, key);
                        header_count++;
                    }

                    // Store the value in the corresponding header
                    if (header_index != -1) {
                        strcpy(headers[header_index].value, value);
                    }
                } else if (line_start[0] == '1' || in_moves_section) {
                    // Start or continue collecting moves
                    in_moves_section = 1;
                    strncat(moves, line_start, line_length);
                    strcat(moves, " ");
                }
            }

            // Move to the next line
            if (ptr < end && *ptr == '\n') {
                ptr++;
            }

            // If we reach an empty line, assume the game is over
            if (line_length == 0 && in_moves_section) {
                break;
            }
        }

        // If the header row hasn't been written, write it now
        if (!header_row_written) {
            for (int i = 0; i < header_count; ++i) {
                fprintf(output, "%s,", headers[i].key);
            }
            fprintf(output, "Moves\n");  // The last column will always be "Moves"
            header_row_written = 1;
        }

        // Write the collected header values to the CSV
        for (int i = 0; i < header_count; ++i) {
            fprintf(output, "\"%s\",", headers[i].value);
        }

        // Write the moves to the CSV
        fprintf(output, "\"%s\"\n", moves);
    }
}

int main() {
    // Open the PGN file
    int fd = open("large_games.pgn", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // Get the size of the file
    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("Error getting file size");
        close(fd);
        return EXIT_FAILURE;
    }
    size_t file_size = st.st_size;

    // Memory-map the file
    char *file_data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file_data == MAP_FAILED) {
        perror("Error mapping file");
        close(fd);
        return EXIT_FAILURE;
    }

    // Open the CSV output file
    FILE *output_file = fopen("output_games.csv", "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        munmap(file_data, file_size);
        close(fd);
        return EXIT_FAILURE;
    }

    // Parse the PGN data and write to CSV
    parse_pgn(file_data, file_size, output_file);

    // Clean up
    fclose(output_file);
    munmap(file_data, file_size);
    close(fd);

    printf("Large PGN file parsed and saved to CSV successfully.\n");

    return 0;
}
