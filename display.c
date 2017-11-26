#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>

void decode_command(DIR *, char *);
void display_dir_contents(DIR *);
char *trimwhitespace(char *);
void printfcomma2 (unsigned n);
void printfcomma (unsigned n);
void dir_call (DIR *);
void type_file (char *);
void type_file_ind (char *);
void copy_file (char *);
void clrscr();
void delete_file (char *);
char *make_regex(char *);

int main(void) {
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
            //printf("%s", input);
            if(input[0] != '\0') decode_command(d, input);
            printf("\n");
        } else {
            perror("Something went wrong. Please try again next time.");
            break;
        }
    }

    return(0);
}

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

    else if (strcmp(pch, "dir") == 0) { // && trimwhitespace(command + strlen(pch) + 1)[0] == '\n') {
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

    else {
        printf("'%s' is not recognized as an internal or external command, operable program or batch file.\n", pch);
    }
}

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

void printfcomma2 (unsigned n) {
    if (n < 1000) {
        printf ("%d", n);
        return;
    }
    printfcomma2 (n/1000);
    printf (",%03d", n%1000);
}

void printfcomma (unsigned n) {
    if (n < 0) {
        printf ("-");
        n = -n;
    }
    printfcomma2 (n);
}

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

void type_file (char *command) {
    char files[50][200];
    char *file_path = trimwhitespace(command + 5), contents; // temp_path_dest[250];
    int i = 0, j = 0;

    for (i = 0; i < 50; ++i) {
        files[i][0] = '\0';
    }

    i = 0;
    int state=0, index_start=0, file_ctr=0;
    while (file_path[i] != '\0') {
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

    // loop here to see all the file names
    for (j = 0; j < file_ctr; ++j) {
        type_file_ind(files[j]);
    }
}

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

void copy_file(char *command) {
    FILE *fp1, *fp2;
    char ch, *filename, *file_path = trimwhitespace(command + 5), contents;
    char *fn1 = strtok(file_path, " "), *fn2 = (char *) malloc(250);
    strcpy(fn2, file_path + strlen(fn1) + 1);
    fn2 = trimwhitespace(fn2);
    char temp_path_dest[250], temp_path_src[250];

    char *regex_fn1;
    int not_dir_flag = 0, i=0;
    DIR *d;

    DIR *dest_dir;
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

    int slash_index = 0;
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

                int pos;
                if (!not_dir_flag) {
                    fp2 = fopen(temp_path_dest, "w");
                    copied_counter ++;
                } else {
                    fp2 = fopen(temp_path_dest, "a");
                    copied_counter = 1;
                }

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
        if (copied_counter == 0) {
            printf("%s\nThe system cannot find the file specified.\n", fn1);
        }
        printf("        %d file(s) copied.\n\n", copied_counter);
    }
    else perror("getcwd() error");

    free(regex_fn1);
    free(file_path);
    free(fn2);
}

void clrscr() {
    system("@cls||clear");
}

void delete_file (char *command) {
    char *file_path = trimwhitespace(command + 4), temp_path_dest[250];
    int slash_index = 0, i = 0, flag_another_dir = 0;
    DIR *d;

    while (file_path[i] != '\0') {
        if(file_path[i] == '\\' || file_path[i] == '/')
            slash_index = i;
        i ++;
    }
    char *filename;

    if (slash_index == 0) {
        d = opendir(".");
        filename = file_path;
        file_path = (char *) malloc(250);
        strcpy(file_path, ".\\");
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
        // printf("besh? %s %s %d\n", temp_path_dest, regex_fn, S_ISDIR(attr.st_mode));
        if (!S_ISDIR(attr.st_mode) && match(regex_fn, dir->d_name)) {
            if (remove(temp_path_dest) == 0)
                delete_counter++;
        }

    }
    closedir(d);
    if (delete_counter == 0) {
        printf("Could Not Find %s\n", temp_path_dest);
    }
    free(regex_fn);
    free(file_path);
}

/* match: search for regexp anywhere in text */
int match(char *regexp, char *text) {
    if (regexp[0] == '^')
        return matchhere(regexp+1, text);
    do {    /* must look even if string is empty */
        if (matchhere(regexp, text))
            return 1;
    } while (*text++ != '\0');
    return 0;
}

/* matchhere: search for regexp at beginning of text */
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

/* matchstar: search for c*regexp at beginning of text */
int matchstar(int c, char *regexp, char *text) {
    do {    /* a * matches zero or more instances */
        if (matchhere(regexp, text))
            return 1;
    } while (*text != '\0' && (*text++ == c || c == '.'));
    return 0;
}
