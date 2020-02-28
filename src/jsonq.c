#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*Parsing and Querying JSON*/

/**
 * Tokens
 */
#define JSONT_OSTART '{'
#define JSONT_OEND '}'
#define JSONT_ASTART '['
#define JSONT_AEND ']'
#define JSONT_STR '"'
#define JSONT_COMMA ','
#define JSONT_COLON ':'
#define JSONT_TSTART 't'
#define JSONT_FSTART 'f'
#define JSONT_NSTART 'n'
#define JSONT_D0 '0'
#define JSONT_D1 '1'
#define JSONT_D2 '2'
#define JSONT_D3 '3'
#define JSONT_D4 '4'
#define JSONT_D5 '5'
#define JSONT_D6 '6'
#define JSONT_D7 '7'
#define JSONT_D8 '8'
#define JSONT_D9 '9'
#define JSONT_DOT '.'

struct json_t;

typedef struct {
    char* key;
    struct json_t* value;
} json_pair_t;

typedef struct {
    json_pair_t* items;
    size_t len;
    size_t cap;
} json_pair_buf_t;

typedef enum {
    JSON_TYPE_NULL,
    JSON_TYPE_BOOL,
    JSON_TYPE_NUMBER,
    JSON_TYPE_STRING,
    JSON_TYPE_ARRAY,
    JSON_TYPE_OBJECT
} json_type_t;

typedef union {
    int _bool;
    double _num;
    char* _str;
    json_pair_buf_t* _items;
} json_val_t;

struct json_t {
    json_type_t type;
    json_val_t val;
};

typedef struct json_t json_t;

#define JSON_PAIR_BUF_DEFAULT_CAP 10

void json_pair_buf_init(json_pair_buf_t* buf)
{
    buf->len = 0;
    buf->cap = JSON_PAIR_BUF_DEFAULT_CAP;
    buf->items = malloc(sizeof(json_pair_t) * JSON_PAIR_BUF_DEFAULT_CAP);
    assert(buf->items != NULL);
}


int main(void) {
	printf("running: %s\n", __FILE__);
	return 0;
}