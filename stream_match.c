#include "stream_match.h"


Matcher* matcher_init(const char *match_target) {
    Matcher *matcher = (Matcher *)malloc(sizeof(Matcher));
    if (matcher == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    matcher->match_target = strdup(match_target);
    matcher->target_length = strlen(match_target);

    if (matcher->match_target == NULL) {
        perror("strdup failed");
        free(matcher);
        exit(EXIT_FAILURE);
    }

    matcher->buffer = NULL;

    return matcher;
}

void matcher_resize_buffer(Matcher *matcher, size_t n) {
    size_t old_len = strlen(matcher->buffer);
    
    if (old_len <= n) {
        // 不需要缩小
        return;
    }
    
    char *new_buffer = (char *)malloc((n + 1) * sizeof(char));
    if (new_buffer == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    // 将原始数据的后n个字节复制到新缓冲区
    memcpy(new_buffer, matcher->buffer + (old_len - n), n);
    new_buffer[n] = '\0'; // 添加字符串结尾的空字符

    // 释放原始缓冲区，并更新matcher->buffer指向新缓冲区
    free(matcher->buffer);
    matcher->buffer = new_buffer;
}

int matcher_receive(Matcher *matcher, const char *new_content) {
    if (matcher->buffer == NULL) {
        matcher->buffer = strdup(new_content);
    } else {
        size_t old_len = strlen(matcher->buffer);
        size_t new_len = strlen(new_content);
        matcher->buffer = realloc(matcher->buffer, old_len + new_len + 1);
        if (matcher->buffer == NULL) {
            perror("realloc failed");
            exit(EXIT_FAILURE);
        }
        strcat(matcher->buffer, new_content);
    }

    if (strstr(matcher->buffer, matcher->match_target) != NULL) {
        return MATCH;
    } else {
        // not match. we shrink the buffer to the size of target.
        matcher_resize_buffer(matcher, matcher->target_length - 1);

        return NOT_MATCH;
    }
}

void matcher_destroy(Matcher *matcher) {
    free(matcher->match_target);
    free(matcher->buffer);
    free(matcher);
}
