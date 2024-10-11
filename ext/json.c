#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_QUIZ_COUNT 300

struct QuizQuestion {
    char* question;
    char* option1;
    char* option2;
    char* option3;
    char* option4;
    int answer;
};


struct JSONParseResult {
    int count;
    struct QuizQuestion* questions;
};




void parse_whitespace(const char** ptr) {
    while (**ptr == ' ' || **ptr == '\n' || **ptr == '\t') (*ptr)++;
}

char* parse_str(const char** ptr) {
    parse_whitespace(ptr);
    if (**ptr == '"') {
        (*ptr)++;
        const char* start = *ptr;
        while (**ptr != '"') (*ptr)++;
        int length = *ptr - start;
        char* result = (char*)malloc((length + 1) * sizeof(char));
        strncpy(result, start, length);
        result[length] = '\0';
        (*ptr)++;
        return result;
    }
    return NULL;
}

int parse_num(const char** ptr) {
    parse_whitespace(ptr);
    int number = 0;
    while (**ptr >= '0' && **ptr <= '9') {
        number = number * 10 + (**ptr - '0');
        (*ptr)++;
    }
    return number;
}

struct JSONParseResult parse(char* filename) {
    struct QuizQuestion* questions = malloc(MAX_QUIZ_COUNT * sizeof(struct QuizQuestion));
    int question_i = 0;
    struct JSONParseResult res;

    FILE* json_file = fopen(filename, "r");
    if (!json_file) {
        printf("Error opening file.\n");
	res.questions = NULL;
        return res;
    }

    fseek(json_file, 0, SEEK_END); 
    const int FILE_CHAR_COUNT = ftell(json_file); 
    fseek(json_file, 0, SEEK_SET);

    char* file_content = malloc((FILE_CHAR_COUNT + 1) * sizeof(char));
    fread(file_content, sizeof(char), FILE_CHAR_COUNT, json_file);
    file_content[FILE_CHAR_COUNT] = '\0'; 
    fclose(json_file);

    const char* ptr = file_content;
    parse_whitespace(&ptr);

    if (*ptr == '[') {
        ptr++;
        while (*ptr && *ptr != ']') {
            parse_whitespace(&ptr);

            if (*ptr == '{') {
                ptr++;

                struct QuizQuestion q;
                while (*ptr && *ptr != '}') {
                    parse_whitespace(&ptr);

                    if (strncmp(ptr, "\"question\"", 10) == 0) {
                        ptr += 10;
                        parse_whitespace(&ptr);
                        if (*ptr == ':') ptr++;
                        q.question = parse_str(&ptr);
                    } else if (strncmp(ptr, "\"option1\"", 9) == 0) {
                        ptr += 9;
                        parse_whitespace(&ptr);
                        if (*ptr == ':') ptr++;
                        q.option1 = parse_str(&ptr);
                    } else if (strncmp(ptr, "\"option2\"", 9) == 0) {
                        ptr += 9;
                        parse_whitespace(&ptr);
                        if (*ptr == ':') ptr++;
                        q.option2 = parse_str(&ptr);
                    } else if (strncmp(ptr, "\"option3\"", 9) == 0) {
                        ptr += 9;
                        parse_whitespace(&ptr);
                        if (*ptr == ':') ptr++;
                        q.option3 = parse_str(&ptr);
                    } else if (strncmp(ptr, "\"option4\"", 9) == 0) {
                        ptr += 9;
                        parse_whitespace(&ptr);
                        if (*ptr == ':') ptr++;
                        q.option4 = parse_str(&ptr);
                    } else if (strncmp(ptr, "\"answer\"", 8) == 0) {
                        ptr += 8;
                        parse_whitespace(&ptr);
                        if (*ptr == ':') ptr++;
                        q.answer = parse_num(&ptr);
                    }

                    parse_whitespace(&ptr);
                    if (*ptr == ',') ptr++;
                }

                questions[question_i++] = q;

                if (*ptr == '}') ptr++;
            }

            parse_whitespace(&ptr);
            if (*ptr == ',') ptr++;
        }
    }

    free(file_content);
    res.questions = questions;
    res.count = question_i;

    return res;
}
