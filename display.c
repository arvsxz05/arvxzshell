/**
 *  I, Arvin Arbuis, upon my honor, swear that we have made this mp all by myself
 **/

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <limits.h>
#include <stdlib.h>

// These are the function prototypes. I will describe them individually below.
void decode_command(DIR *, char *);
void display_dir_contents(DIR *);
char *trimwhitespace(char *);
void printfcomma (unsigned n);
void dir_call (DIR *);
void type_file (char *);
void type_file_ind (char *);
void copy_file (char *);
void clrscr();
void delete_file (char *);
char *make_regex(char *);
int file_name_extract(char (*)[200], char *, int size);
void delete_file_ind (char *);
void make_directory(char *);
void make_directory_ind(char *);
void rename_file(char *);
void remove_directory(char *);
int remove_directory_ind(const char *, int);
void move_file (char *);
void date_display(char *);
void time_display(char *);

/**
 * main() loops infinitely and passes commands to decoder decode_command()
 **/
int main() {
    DIR *d;
    struct dirent *dir;
    char cwd[1024];
    char input[1024];

    while (1) {
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            input[0] = '\0';
            printf("%s>", cwd);
            fflush(stdin);
            scanf("%[^\n]", input);
            if(input[0] != '\0') decode_command(d, input);
            printf("\n");
        } else {
            perror("Something went wrong. Please try again next time.");
            break;
        }
    }

    return(0);
}

/**
 * decode_command() determines what type of command are entered and passes control to 
 * functions that are responsible in carrying out the command
 *
 * @param d is the pointer to the original directory stream from the main
 * @param command is the user command string
 **/
void decode_command(DIR *d, char *command) {
    char * pch;
    command = trimwhitespace(command);
    pch = strtok (command, " ");


    if (strcmp(pch, "exit") == 0) {
        exit(0);
    }
    else if (strcmp(pch, "cd") == 0 || strcmp(pch, "chdir") == 0) {
        if (chdir(trimwhitespace(command + strlen(pch) + 1)) != 0) {
            printf("The system cannot find the path specified.\n");
        }
    }

    else if (strcmp(pch, "cd..") == 0) {
        chdir("..");
    }

    else if (strcmp(pch, "dir") == 0) {
        dir_call(d);
    }

    else if (strcmp(pch, "cmd") == 0) {
        printf("Microsoft Windows [Version 10.0.15063]\n(c) 2017 Microsoft Corporation. All rights reserved.\n\n");
    }

    else if (strcmp(pch, "type") == 0) {
        type_file (command);
    }

    else if (strcmp(pch, "copy") == 0) {
        copy_file (command);
    }

    else if (strcmp(pch, "del") == 0) {
        delete_file (command);
    }

    else if (strcmp(pch, "cls") == 0) {
        clrscr ();
    }

    else if (strcmp(pch, "mkdir") == 0) {
        make_directory (command);
    }

    else if (strcmp(pch, "rename") == 0) {
        rename_file (command);
    }

    else if (strcmp(pch, "rmdir") == 0) {
        remove_directory (command);
    }

    else if (strcmp(pch, "move") == 0) {
        move_file (command);
    }

    else if (strcmp(pch, "date") == 0) {
        date_display (command);
    }

    else if (strcmp(pch, "time") == 0) {
        time_display (command);
    }

    else {
        printf("'%s' is not recognized as an internal or external command, operable program or batch file.\n", pch);
    }
}

/**
 * trimwhitespace() is a helper function that omits trailing and leading spaces in the string.
 *
 * @param str is the original string to be trimmed
 **/
char *trimwhitespace(char *str) {
    char *end;

    // Trim leading space
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)  // All spaces?
    return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator
    *(end+1) = 0;

    return str;
}

/**
 * printfcomma() is a helper function that prints unsigned integer with commas.
 *
 * @param n is the number to be printed in the console
 **/
void printfcomma (unsigned n) {
    if (n < 1000) {
        printf ("%d", n);
        return;
    }
    printfcomma (n/1000);
    printf (",%03d", n%1000);
}


/**
 * dir_call() is a function for carrying out 'dir' command. This prints
 * all the sub-directories and files that can be found in the current working directory
 *
 * @param d is the pointer to the original directory stream from the main function
 **/
void dir_call(DIR *d) {
    struct dirent *dir;
    d = opendir(".");
    struct stat attr;
    char buff[20];
    struct tm * timeinfo;
    int file_cnt = 0, dir_cnt = 0;
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            stat(dir->d_name, &attr);
            timeinfo = localtime (&attr.st_mtime);
            strftime(buff, sizeof(buff), "%m/%d/%Y %I:%M %p", timeinfo);
            printf("%s", buff);
            if (S_ISDIR(attr.st_mode))
                printf("    <DIR>          ");
            else {
                int count = (int)log10(attr.st_size) + 1;
                count += (count-1)/3;

                int i=0;
                for (; i < 18-count; i++) {
                    printf(" ");
                }
                printfcomma ((unsigned)attr.st_size);
                printf(" ");
            }
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }
    else perror("getcwd() error");
}

/**
 * type_file() is the main function for carrying out 'type' command. This extracts the file names
 * in the command arguments and passes them to type_file_ind() for printing
 *
 * @param command holds the arguments of the command
 **/
void type_file (char *command) {
    char files[50][200];
    char *file_path = trimwhitespace(command + 5), contents;
    int i = 0, j = 0, file_ctr = 0;

    if(!(file_ctr = file_name_extract(files, file_path, 50))) {
        return;
    }

    // loop here to see all the file names
    for (j = 0; j < file_ctr; ++j) {
        type_file_ind(files[j]);
    }
}

/**
 * type_file_ind() is the helper function for carrying out 'type' command. This prints the contents 
 * of the file(s) that matches the filename and filepath specified
 *
 * @param filename is a string that holds the filename regex of the file to be printed, since 
 * the terminal supports wildcards
 **/
void type_file_ind (char *filename) {
    char *file_path = filename, temp_path_dest[250], contents;
    int slash_index = 0, i = 0;
    DIR *d;
    FILE *fptr;

    while (file_path[i] != '\0') {
        if(file_path[i] == '\\' || file_path[i] == '/')
            slash_index = i;
        i ++;
    }

    if (slash_index == 0) {
        d = opendir(".");
        filename = file_path;
        file_path = (char *) malloc(250);
        strcpy(file_path, ".");
    } else {
        filename = file_path + slash_index + 1;
        file_path[slash_index] = '\0';
        d = opendir(file_path);
    }

    char *regex_fn = make_regex(filename);

    if (d == NULL) {
        printf("The specified path is invalid.\n\n");
        return;
    }

    struct dirent *dir;
    struct stat attr;
    int print_counter = 0;

    while ((dir = readdir(d)) != NULL) {
        temp_path_dest[0] = '\0';
        strcat(temp_path_dest, file_path);
        strcat(temp_path_dest, "/");
        strcat(temp_path_dest, dir->d_name);
        stat(temp_path_dest, &attr);
        if (!S_ISDIR(attr.st_mode) && match(regex_fn, dir->d_name)) {
            fptr = fopen(temp_path_dest, "r");
            if (fptr == NULL) {
                printf("The system cannot find the file specified.\n");
                return;
            }
            printf("%s\n\n", temp_path_dest);
            contents = fgetc(fptr);
            print_counter++;
            while (contents != EOF) {
                printf ("%c", contents);
                contents = fgetc(fptr);
            }
            fclose(fptr);
            printf("\n");
        }

    }
    closedir(d);
    if (print_counter == 0) {
        temp_path_dest[0] = '\0';
        strcat(temp_path_dest, file_path);
        strcat(temp_path_dest, "\\");
        strcat(temp_path_dest, filename);
        printf("Could Not Find %s\n", temp_path_dest);
    }
    free(regex_fn);
    free(file_path);
}

/**
 * make_regex() is a function that converts filenames/wildcards to regex sequences that can be 
 * understood by this program. 
 *
 * @param source is a string that holds the filenames/wildcards in string format
 * @return char pointer of the new regex sequence
 **/
char *make_regex(char *source) {
    char *ch = source;
    char *result = (char *) malloc(strlen(source) * 2 + 2);
    int i = 1;
    result[0] = '^';
    while (*ch != '\0') {
        if (*ch == '*') {
            result[i] = '.';
            result[i+1] = '*';
            i += 2;
        }
        else {
            result[i] = *ch;
            i ++;
        }
        ch ++;
    }
    result[i] = '$';
    result[i+1] = '\0';
    return result;
}

/**
 * file_name_extract() is a helper function that tokenizes the command arguments to extract
 * the filenames/wildcards individually. The function stores them in 'files'
 *
 * @param files is a 2d array that is used as a buffer for storing tokenized arguments
 * @param command is the original string command of the user
 * @param size is the size of the buffer 'files' (how many tokens can be stored)
 * @return int number of tokens
 **/
int file_name_extract(char files[][200], char *command, int size) {
    char *file_path = command, *filename;

    int i = 0;

    for (i = 0; i < size; ++i) {
        files[i][0] = '\0';
    }

    i = 0;
    int state=0, index_start=0, file_ctr=0;
    while (file_path[i] != '\0') {
        if (file_ctr > size-1) {
            printf("The syntax of the command is incorrect.\n\n");
            return 0;
        }
        if(state == 0) {
            if(file_path[i] == '"') {
                state = 1;
                index_start = i+1;
            } else if (file_path[i] != ' ') {
                state = 3;
                index_start = i;
            }
        } else if (state == 1) {
            if(file_path[i] == '"') {
                state = 2;
                file_path[i] = '\0';
                // printf("%s + %s\n", files[file_ctr], file_path+index_start);
                strcat(files[file_ctr], file_path+index_start);
            }
        } else if (state == 2) {
            if(file_path[i] == '"') {
                state = 1;
                index_start = i+1;
            } else if (file_path[i] == ' ' || file_path[i] == '\n') {
                state = 0;
                file_ctr++;
            } else {
                state = 3;
                index_start = i;
            }
        } else if (state == 3) {
            if (file_path[i] == ' ' || file_path[i] == '\n') {
                state = 0;
                file_path[i] = '\0';
                // printf("%s + %s\n", files[file_ctr], file_path+index_start);
                strcat(files[file_ctr], file_path+index_start);
                file_ctr++;
            } else if(file_path[i] == '"') {
                state = 1;
                file_path[i] = '\0';
                // printf("%s + %s\n", files[file_ctr], file_path+index_start);
                strcat(files[file_ctr], file_path+index_start);
                index_start = i+1;
            }
        }
        i++;
    }

    if (state == 1 || state == 3) {
        strcat(files[file_ctr], file_path+index_start);
        file_ctr++;
    } else if (state == 2) {
        file_ctr++;
    }

    if (file_ctr > size-1) {
        printf("The syntax of the command is incorrect.\n\n");
        return 0;
    }

    if (file_ctr <= 0) {
        printf("No arguments provided.\n\n");
        return 0;
    }

    return file_ctr;
}

/**
 * copy_file() is a function for carrying out 'copy' command. This copies the file(s) specified in the 
 * command through filename/wildcards and copies them to file(s)/directory specified in the command
 *
 * @param command holds the arguments of the command
 **/
void copy_file(char *command) {
    char files[3][200];
    char *file_path = trimwhitespace(command + 5), *filename;

    if(!file_name_extract(files, file_path, 3)) {
        return;
    }

    int i = 0;

    FILE *fp1, *fp2;
    char *fn1, *fn2 = files[1];
    char temp_path_dest[250], temp_path_src[250];


    char *regex_fn1, ch;
    int not_dir_flag = 0;
    DIR *d;

    // predict if the source file is not in the current working directory
    // by finding some slash in path.
    file_path = files[0];
    int slash_index = 0; i = 0;
    while (file_path[i] != '\0') {
        if(file_path[i] == '\\' || file_path[i] == '/')
            slash_index = i;
        i ++;
    }

    // if the file is copied to itself
    if(slash_index == 0 && files[1][0] == '\0') {
        printf("The file cannot be copied onto itself.\n");
        printf("        0 file(s) copied.\n\n");
        return;
    }

    // open directory of source file
    if (slash_index == 0) {
        d = opendir(".");
        filename = file_path;
        file_path = (char *) malloc(250);
        strcpy(file_path, ".");
    } else {
        filename = file_path + slash_index + 1;
        file_path[slash_index] = '\0';
        d = opendir(file_path);
    }

    // open directory of destination file
    DIR *dest_dir;
    if (!strcmp(fn2, "\0")) {
        strcpy(fn2, ".");
    }
    dest_dir = opendir(fn2);
    if (dest_dir == NULL) {         // if the destination is a specific file
        fp2 = fopen(fn2, "a");
        if (fp2 == NULL) {
            printf("The syntax of the command is incorrect.\n\n");
            return;
        }
        not_dir_flag = 1;
        fclose(fp2);
    }

    fn1 = filename;
    regex_fn1 = make_regex(fn1);

    //TODO loop sa tanan files and copy them
    struct dirent *dir;
    struct stat attr;
    int copied_counter = 0;

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            temp_path_src[0] = '\0';
            strcat(temp_path_src, file_path);
            strcat(temp_path_src, "\\");
            strcat(temp_path_src, dir->d_name);

            stat(temp_path_src, &attr);

            // printf("%s %s %d\n", dir->d_name, regex_fn1, match(regex_fn1, dir->d_name));
            if (!S_ISDIR(attr.st_mode) && match(regex_fn1, dir->d_name)) {

                if ((fp1 = fopen(temp_path_src, "r")) == NULL) {
                    printf("\nFile cannot be opened");
                    return;
                }

                if (dest_dir != NULL) {
                    strcat(temp_path_dest, fn2);
                    strcat(temp_path_dest, "/");
                    strcat(temp_path_dest, dir->d_name);
                } else {
                    strcat(temp_path_dest, fn2);
                }

                if (!not_dir_flag) {
                    fp2 = fopen(temp_path_dest, "w");
                    copied_counter ++;
                } else {
                    fp2 = fopen(temp_path_dest, "a");
                    copied_counter = 1;
                }

                int pos;
                fseek(fp1, 0L, SEEK_END); // file pointer at end of file
                pos = ftell(fp1);
                fseek(fp1, 0L, SEEK_SET); // file pointer set at start
                while (pos--) {
                    ch = fgetc(fp1);  // copying file character by character
                    if (ch == EOF) break;
                    fputc(ch, fp2);
                }
                fputc('\n', fp2);

                fclose(fp1);
                fclose(fp2);
            }
            temp_path_dest[0] = '\0';
        }
        closedir(d);
        closedir(dest_dir);
        if (copied_counter == 0) {
            printf("%s\nThe system cannot find the file specified.\n", fn1);
        }
        printf("        %d file(s) copied.\n\n", copied_counter);
    }
    else perror("getcwd() error");

    free(regex_fn1);
    free(file_path);
}

/**
 * clrscr() is a function for carrying out 'cls' command. This clears the console.
 **/
void clrscr() {
    HANDLE hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count, cellCount;
    COORD homeCoords = { 0, 0 };

    hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
    if (hStdOut == INVALID_HANDLE_VALUE) return;

    /* Get the number of cells in the current buffer */
    if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
    cellCount = csbi.dwSize.X *csbi.dwSize.Y;

    /* Fill the entire buffer with spaces */
    if (!FillConsoleOutputCharacter(
        hStdOut,
        (TCHAR) ' ',
        cellCount,
        homeCoords,
        &count
    )) return;

    /* Fill the entire buffer with the current colors and attributes */
    if (!FillConsoleOutputAttribute(
        hStdOut,
        csbi.wAttributes,
        cellCount,
        homeCoords,
        &count
    )) return;

    /* Move the cursor home */
    SetConsoleCursorPosition( hStdOut, homeCoords );
}

/**
 * delete_file() is a function for carrying out 'del' command. This extacts all the specified files
 * from the command using file_name_extract() and passes them to delete_file_ind() for individual deletion
 *
 * @param command holds the arguments of the command
 **/
void delete_file (char *command) {
    char files[50][200];
    char *file_path = trimwhitespace(command + 4), contents;
    int i = 0, j = 0, file_ctr = 0;

    if(!(file_ctr = file_name_extract(files, file_path, 50))) {
        return;
    }

    // loop here to see all the file names
    for (j = 0; j < file_ctr; ++j) {
        delete_file_ind(files[j]);
    }
}

/**
 * delete_file_ind() is the helper function for carrying out 'del' command. This deletes the
 * file(s) that matches the filename and filepath specified
 *
 * @param filename is a string that holds the filename regex of the file to be deleted, since 
 * the terminal supports wildcards
 **/
void delete_file_ind (char *filename) {
    // printf("%s\n", filename);
    char *file_path = trimwhitespace(filename), temp_path_dest[250];
    int slash_index = 0, i = 0, flag_another_dir = 0;
    DIR *d;

    while (file_path[i] != '\0') {
        if(file_path[i] == '\\' || file_path[i] == '/')
            slash_index = i;
        i ++;
    }

    if (slash_index == 0) {
        d = opendir(".");
        filename = file_path;
        file_path = (char *) malloc(250);
        strcpy(file_path, ".");
    } else {
        filename = file_path + slash_index + 1;
        file_path[slash_index] = '\0';
        d = opendir(file_path);
        flag_another_dir = 1;
    }

    char *regex_fn = make_regex(filename);
    // printf("%s %s %d\n", file_path, filename, flag_another_dir);

    if (d == NULL) {
        printf("The specified path is invalid.\n\n");
        return;
    }

    struct dirent *dir;
    struct stat attr;
    int delete_counter = 0;

    while ((dir = readdir(d)) != NULL) {
        temp_path_dest[0] = '\0';
        strcat(temp_path_dest, file_path);
        strcat(temp_path_dest, "/");
        strcat(temp_path_dest, dir->d_name);

        stat(temp_path_dest, &attr);
        if (!S_ISDIR(attr.st_mode) && match(regex_fn, dir->d_name)) {
            if (remove(temp_path_dest) == 0)
                delete_counter++;
        }

    }
    closedir(d);
    if (delete_counter == 0) {
        printf("The system cannot find the file specified - %s/%s\n", file_path, filename);
    }
    free(regex_fn);
    free(file_path);
}

/**
 * match() is a function that determines whether the filename matches with a regex 
 * sequence. Search for regexp anywhere in text
 *
 * @param regexp is the regex sequence
 * @param text is the string to be tested
 * @return 1 if it matches, 0 if not
 **/
int match(char *regexp, char *text) {
    if (regexp[0] == '^')
        return matchhere(regexp+1, text);
    do {    /* must look even if string is empty */
        if (matchhere(regexp, text))
            return 1;
    } while (*text++ != '\0');
    return 0;
}

/**
 * match() is a helper function that searches for regexp at beginning of text
 **/
int matchhere(char *regexp, char *text) {
    if (regexp[0] == '\0')
        return 1;
    if (regexp[1] == '*')
        return matchstar(regexp[0], regexp+2, text);
    if (regexp[0] == '$' && regexp[1] == '\0')
        return *text == '\0';
    if (*text!='\0' && (regexp[0]=='.' || regexp[0]==*text))
        return matchhere(regexp+1, text+1);
    return 0;
}

/**
 * match() is a helper function that searches for c*regexp at beginning of text
 **/
int matchstar(int c, char *regexp, char *text) {
    do {    /* a * matches zero or more instances */
        if (matchhere(regexp, text))
            return 1;
    } while (*text != '\0' && (*text++ == c || c == '.'));
    return 0;
}

/**
 * make_directory() is a function for carrying out 'mkdir' command. This extacts all the specified 
 * directories to be created from the command using file_name_extract() and passes them to 
 * make_directory_ind() for individual creation
 *
 * @param command holds the arguments of the command
 **/
void make_directory(char *command) {
    char *file_path = trimwhitespace(command + 6);
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));

    char dirs[50][200];
    int i = 0, j = 0, file_ctr = 0;
    if(!(file_ctr = file_name_extract(dirs, file_path, 50))) {
        return;
    }

    for (i = 0; i < file_ctr; ++i) {
        if(dirs[i][0] == '\\' || dirs[i][0] == '/') {
            if(chdir("\\") != 0) {
                printf("Something went wrong. Please try again later\n");
                return;
            }
            make_directory_ind(dirs[i]+1);
        } else {
            make_directory_ind(dirs[i]);
        }
        chdir(cwd);
    }
}

/**
 * make_directory_ind() is the helper function for carrying out 'mkdir' command. This creates the
 * directory in the path and with a name specified in the command
 *
 * @param command is a string that holds the directory name and path of directory to be created
 **/
void make_directory_ind(char *command) {
    char * token;
    token = strtok (command, "/\\");

    while( token != NULL ) {
        mkdir(token);
        chdir(token);
        token = strtok(NULL, "/\\");
    }
}


void rename_file(char *command) {
    char files[3][200];
    char *file_path = trimwhitespace(command + 7), *filename;
    int file_ctr = 0, ret;

    if(!(file_ctr = file_name_extract(files, file_path, 3))) {
        return;
    }

    if(file_ctr != 2) {
        printf("The syntax of the command is incorrect.\n\n");
    }

    char *oldname = files[0];
    char *newname = files[1];

    ret = rename(oldname, newname);
    printf("%d\n", ret);
    if(ret == 0) {
        printf("File renamed successfully");
    } else {
        printf("The syntax of the command is incorrect.\n\n");
    }
}

void remove_directory(char *command) {
    char *file_path = trimwhitespace(command + 6);
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    int hard = 0;

    char dirs[50][200];
    int i = 0, j = 0, file_ctr = 0;
    if(!(file_ctr = file_name_extract(dirs, file_path, 50))) {
        return;
    }

    if (strcmp(dirs[file_ctr-1], "\\q") == 0) {
        hard = 1;
        file_ctr --;
    }

    for (i = 0; i < file_ctr; ++i) {
        if(dirs[i][0] == '\\' || dirs[i][0] == '/') {
            if(chdir("\\") != 0) {
                printf("Something went wrong. Please try again later\n");
                return;
            }
            remove_directory_ind(dirs[i]+1, hard);
        } else {
            remove_directory_ind(dirs[i], hard);
        }
        chdir(cwd);
    }
}

int remove_directory_ind(const char *path, int hard) {
    DIR *d = opendir(path);

    size_t path_len = strlen(path);
    int r = -1;

    if (d) {
        if (hard) {
            struct dirent *p;

            r = 0;

            while (!r && (p=readdir(d))) {
                int r2 = -1;
                char *buf;
                size_t len;

                /* Skip the names "." and ".." as we don't want to recurse on them. */
                if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")) {
                    continue;
                }

                len = path_len + strlen(p->d_name) + 2;
                buf = malloc(len);

                if (buf) {
                    struct stat statbuf;

                    snprintf(buf, len, "%s/%s", path, p->d_name);

                    if (!stat(buf, &statbuf)) {
                        if (S_ISDIR(statbuf.st_mode)) {
                            r2 = remove_directory_ind(buf, hard);
                        } else {
                            r2 = unlink(buf);
                        }
                    }
                    free(buf);
                }
                r = r2;
            }
        }

        closedir(d);
        r = rmdir(path);
        if (r) {
            printf("The directory is not empty.\n\n");
        }
        return r;
    } else {
        printf("The system cannot find the file specified.\n\n");
        return -1;
    }
}

void move_file (char *command) {
    // for construction
    char command_copy[250], *file_path = trimwhitespace(command + 5), *filename;
    FILE *fp1, *fp2;
    char *fn1, *fn2;
    char buf[PATH_MAX];

    char files[3][200];
    if(!file_name_extract(files, file_path, 3)) {
        return;
    }

    strcpy(command_copy, files[0]);

    if(files[1][0] == '\0')
        strcpy(files[1], ".");

    DIR *dest_dir, *d;
    dest_dir = opendir(files[1]);
    if (!dest_dir) {
        printf("The filename, directory name, or volume label syntax is incorrect.\n\n");
        return;
    }

    fn2 = files[1];

    char temp_path_dest[250], temp_path_src[250];
    char *regex_fn1, ch;

    // predict if the source file is not in the current working directory
    // by finding some slash in path.
    file_path = files[0];
    int slash_index = 0, i = 0;
    while (file_path[i] != '\0') {
        if(file_path[i] == '\\' || file_path[i] == '/')
            slash_index = i;
        i ++;
    }

    // if the file is moved to itself
    if(slash_index == 0 && files[1][0] == '\0') {
        printf("The file cannot be moved onto its current directory.\n");
        return;
    }

    // open directory of source file
    if (slash_index == 0) {
        d = opendir(".");
        filename = file_path;
        file_path = (char *) malloc(250);
        strcpy(file_path, ".");
    } else {
        filename = file_path + slash_index + 1;
        file_path[slash_index] = '\0';
        d = opendir(file_path);
    }

    fn1 = filename;
    regex_fn1 = make_regex(fn1);

    //TODO loop sa tanan files and copy them
    struct dirent *dir;
    struct stat attr;
    int copied_counter = 0;

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            temp_path_src[0] = '\0';
            strcat(temp_path_src, file_path);
            strcat(temp_path_src, "\\");
            strcat(temp_path_src, dir->d_name);

            stat(temp_path_src, &attr);

            if (!S_ISDIR(attr.st_mode) && match(regex_fn1, dir->d_name)) {

                if ((fp1 = fopen(temp_path_src, "r")) == NULL) {
                    printf("A duplicate file name exists, or the file cannot be found.\n");
                    return;
                }

                if (dest_dir != NULL) {
                    strcat(temp_path_dest, fn2);
                    strcat(temp_path_dest, "/");
                    strcat(temp_path_dest, dir->d_name);
                } else {
                    strcat(temp_path_dest, fn2);
                }

                fp2 = fopen(temp_path_dest, "w");
                _fullpath(buf, temp_path_src, PATH_MAX);
                printf("%s\n", buf);

                copied_counter ++;

                int pos;
                fseek(fp1, 0L, SEEK_END); // file pointer at end of file
                pos = ftell(fp1);
                fseek(fp1, 0L, SEEK_SET); // file pointer set at start
                while (pos--) {
                    ch = fgetc(fp1);  // copying file character by character
                    if (ch == EOF) break;
                    fputc(ch, fp2);
                }
                fputc('\n', fp2);

                fclose(fp1);
                fclose(fp2);
            }
            temp_path_dest[0] = '\0';
        }
        free(regex_fn1);
        free(file_path);
        closedir(dest_dir);
        closedir(d);
        if (copied_counter == 0) {
            printf("A duplicate file name exists, or the file cannot be found.\n");
            return;
        }

        char temp[250];
        strcpy(temp, "del ");
        strcat(temp, command_copy);
        delete_file(temp);

    }
    else perror("getcwd() error");
}

void date_display(char *command) {
    command = strtok(NULL, " ");
    if(command != NULL) {
        printf("The system cannot accept the date entered.\n");
        return;
    }

    time_t t = time(NULL);
    struct tm * tm = localtime(&t);

    char buff[20];
    strftime(buff, sizeof(buff), "%a %m/%d/%Y", tm);
    printf("The current date is: %s\n", buff);
}

void time_display(char *command) {
    command = strtok(NULL, " ");
    if(command != NULL) {
        printf("The system cannot accept the time entered.\n");
        return;
    }

    time_t t = time(NULL);
    struct tm * tm = localtime(&t);

    char buff[20];
    strftime(buff, sizeof(buff), "%X", tm);
    printf("The current time is: %s\n", buff);
}