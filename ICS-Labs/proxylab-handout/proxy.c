/*
 * proxy.c - A Caching Web Proxy
 * 14307110190 Tianshu Zhou
 * ICS(II) - proxylab
 */

#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400
#define CACHE_HITS 32

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
// static const char *accept_hdr = "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n";
// static const char *accept_encoding_hdr = "Accept-Encoding: gzip, deflate\r\n";
static const char *conn_hdr = "Connection: close\r\n";
static const char *proxy_hdr = "Proxy-Connection: close\r\n";

typedef struct cache_line {
    char uri[MAXLINE];
    char *buf;
    int content_length;
    struct cache_line *prev;
    struct cache_line *next;
} cache_line;

typedef struct cache_set {
    int free_bytes;
    cache_line *head;
    cache_line *tail;
} cache_set;

typedef struct cache_block {
    int index;
    int size;
    cache_line **block;
} cache_block;

typedef struct web_buf {
    char buf[MAX_OBJECT_SIZE];
    char *index;
    int size;
    int flag;
} web_buf;

cache_set cache;
cache_block cache_hits;
int count;

/* mutexes */
sem_t w; /* modify list lock */
sem_t u; /* access to hits bag */
sem_t read_mutex; /* reader counter lock */

void init_buf(web_buf *w_buf);
void set_buf(web_buf *w_buf, char *buf, int size);
void init_cache();
void set_cache(char *uri, char *buf, int size);
int get_cache(char *uri, int fd);
cache_line *find_cache(char *uri);
cache_line *cache_hits_get();
void cache_hits_set(cache_line *node);
void delete_line(cache_line *node);
void insert_line(cache_line *node);
void evict_cache(int size);

int is_valid_http(char *buf);
void *thread(void *vargp);
void doit(int fd);
int read_request(rio_t *rp, char *request);
void forward_response(rio_t *rp, int client_fd, char *uri);
void parse_uri(char *uri, char *host, char *port, char *url);
void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);

int main(int argc, char **argv) {
    int listenfd;
    int *connfd;
    char host[MAXLINE], port[MAXLINE];
    struct sockaddr_in clientaddr;
    socklen_t clientaddr_len = sizeof(clientaddr);
    pthread_t tid;

    /* check command line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    /* ignore SIGPIPE signal */
    signal(SIGPIPE, SIG_IGN);

    /* init cache */
    init_cache();

    listenfd = Open_listenfd(argv[1]);
    while (1) {
        /* listen for incoming connections */
        connfd = malloc(sizeof(int));
        *connfd = Accept(listenfd, (SA *)&clientaddr, &clientaddr_len);
        Getnameinfo((SA *)&clientaddr, clientaddr_len, host, MAXLINE, 
                     port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", host, port);
        /* service the request accordingly */
        // doit(connfd);
        /* close the connection */
        // Close(connfd);
        Pthread_create(&tid, NULL, thread, connfd);
    }
    return 0;
}

/*
 * init web buf
 */
void init_buf(web_buf *w_buf) {
    w_buf->index = w_buf->buf;
    w_buf->size = 0;
    w_buf->flag = 1;
}

/*
 * set web buf
 */
void set_buf(web_buf *w_buf, char *buf, int size) {
    if (w_buf->flag) {
        if (w_buf->size + size < MAX_OBJECT_SIZE) {
            memcpy(w_buf->index, buf, size);
            w_buf->index += size;
            w_buf->size += size;
        }
        else {
            w_buf->flag = 0;
        }
    }
    else {
        return;
    }
}

/*
 * init cache
 */
void init_cache() {
    /* init global variables */
    cache.free_bytes = MAX_CACHE_SIZE;
    cache.head = NULL;
    cache.tail = NULL;
    cache_hits.block = malloc(CACHE_HITS * sizeof(cache_line *));
    cache_hits.index = 0;
    cache_hits.size = CACHE_HITS;
    count = 0;

    /* init semaphores */
    Sem_init(&w, 0, 1);
    Sem_init(&u, 0, 1);
    Sem_init(&read_mutex, 0, 1);
}

/*
+ * put a cache_line pointer in cache_hits
+ */
void cache_hits_set(cache_line *node) {
    cache_hits.block[cache_hits.index] = node;
    cache_hits.index++;
    if (cache_hits.index >= cache_hits.size) {
        cache_hits.block = realloc(cache_hits.block, 2 * cache_hits.size * (sizeof(cache_line *)));
        cache_hits.size *= 2;
    }
}

/*
+ * pop a cache_line pointer in cache_hits
+ */
cache_line *cache_hits_get() {
    if (cache_hits.index) {
        int i = --cache_hits.index;
        return cache_hits.block[i];
    }
    return NULL;
}

/*
 * delete a node from the cache list
 */
void delete_line(cache_line *node) {
    if (node->prev) {
        node->prev->next = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    }
    if (node == cache.tail) {
        cache.tail = node->prev;
    }
}

/*
 * insert a node in front of the cache list
 */
void insert_line(cache_line *node) {
    if (cache.head) {
        node->prev = NULL;
        node->next = cache.head;
        cache.head->prev = node;
        cache.head = node;
    }
    else {
        node->prev = NULL;
        node->next = NULL;
        cache.head = node;
        cache.tail = node;
    }
}

/*
 * LRU
 */
void evict_cache(int size) {
    cache_line *cur = cache.tail;
    while (cache.free_bytes < size) {
        delete_line(cur);
        cache.free_bytes += cur->content_length;
        cache.tail = cur->prev;
        free(cur->buf);
        free(cur);
        cur = cache.tail;
    }

}

/*
 * cache in the web object passed by proxy buffer
 * 1. if not enough space, evict least recently used nodes
 * 2. update the cached node's position according to LRU rule
 */
void set_cache(char *uri, char *buf, int size) {
    cache_line *cur, *new;

    P(&w);

    while ((cur = cache_hits_get())) {
        if (cur != cache.head) {
            delete_line(cur);
            insert_line(cur);
        }
    }

    // evict lines until space is enough
    evict_cache(size);

    new = malloc(sizeof(cache_line));
    new->buf = malloc(size);
    strcpy(new->uri, uri);
    memcpy(new->buf, buf, size);
    new->content_length = size;
    insert_line(new);
    cache.free_bytes -= size;

    V(&w);
}

/*
 * get the cache of the web object to client request
 * if url matched in cache, write cached content to client and update hit bag.
 * return 1, if request has been cached
 * return 0, otherwise
 */
int get_cache(char *uri, int fd) {
    cache_line *line;
    int flag = 0;

    P(&read_mutex);
    count++;
    // first in
    if (count == 1) {
        P(&w);
    }
    V(&read_mutex);

    /* get line from cache_set */
    line = find_cache(uri);
    if (line) {
        P(&u);
        Rio_writen(fd, line->buf, line->content_length);
        cache_hits_set(line);
        V(&u);
        flag = 1;
    }

    P(&read_mutex);
    count--;
    // last out
    if (!count) {
        V(&w);
    }
    V(&read_mutex);

    return flag;
}

/*
 * traverse the list to find cached request
 */
cache_line *find_cache(char *uri) {
    cache_line *cur = cache.head;
    while (cur) {
        if (!strcmp(uri, cur->uri)) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

/*
 * determines whether if is a valid http request
 * a form other than <method> <uri> <version> or
 * a method not implemented is considered as invalid.
 * 1 for valid, 0 for not valid
 */
int is_valid_http(char *buf) {
    char *c;

    if (*buf != ' ') {
        if ((c = strchr(buf, ' '))) {
            if (*(c + 1) != ' ' && strchr(c + 1, ' ')) {
                return 1;
            }
        }
    }
    return 0;
}

/*
 * thread - new a thread for one specific request
 */
void *thread(void *vargp) {
    int connfd;

    /* detach the thread for automatic reap */
    Pthread_detach(Pthread_self());
    connfd = *((int *)vargp);
    free(vargp);
    doit(connfd);
    Close(connfd);
    
    return NULL;
}

/*
 * doit - serve as a proxy to handle one HTTP request/response transaction
 */
/* $begin doit */
void doit(int connfd) {
    char buf[MAXLINE], method[MAXLINE], host[MAXLINE], uri[MAXLINE], url[MAXLINE], port[MAXLINE], version[MAXLINE], request[MAXBUF];
    int serverfd;
    rio_t rio_server, rio_client;

    memset(buf, 0, sizeof(buf));
    memset(method, 0, sizeof(method));
    memset(host, 0, sizeof(host));
    memset(uri, 0, sizeof(uri));
    memset(port, 0, sizeof(port));
    memset(version, 0, sizeof(version));

    // read request line and headers
    Rio_readinitb(&rio_client, connfd);
    if (!Rio_readlineb(&rio_client, buf, MAXLINE)) {
        return;
    }
    
    if (is_valid_http(buf)) {
        sscanf(buf, "%s %s %s", method, uri, version);
        if (strcmp(method, "GET")) {
            clienterror(connfd, method, "501", "Not Implemented", "Proxy does not implement this method");
            return;
        }
    }
    else {
        clienterror(connfd, "", "400", "Bad Requset", "Proxy does not understand this request");
        return;
    }

    /* if request has already cached */
    if (get_cache(uri, connfd)) {
        return;
    }
    
    parse_uri(uri, host, port, url);
    
    /* build the request line */
    sprintf(request, "GET %s HTTP/1.0\r\n", url);
    
    if (!read_request(&rio_client, request)) {
        sprintf(request, "%sHost: %s\r\n", request, host);
    }
    sprintf(request, "%s%s", request, user_agent_hdr);
    sprintf(request, "%s%s", request, conn_hdr);
    sprintf(request, "%s%s", request, proxy_hdr);
    sprintf(request, "%s\r\n", request); /* empty line to end headers */

    serverfd = open_clientfd(host, port);
    if (serverfd < 0) {
        fprintf(stderr, "can't open serverfd\n");
        return;
    }

    Rio_readinitb(&rio_server, serverfd);
    if (rio_writen(serverfd, request, sizeof(request)) < 0) {
        fprintf(stderr, "error happens when Rio_writen sends request\n");
        // send back error message to client
        clienterror(connfd, "request error", "404", "Not found", "Send request to server error");
        return;
    }

    forward_response(&rio_server, connfd, uri);
    Close(serverfd);

    return;
}
/* $end doit */

/*
 * read_request - read HTTP request headers and parse it
 * add the Host header if client provides and return 1
 * otherwise return 0
 */
/* $begin read_request */
int read_request(rio_t *rp, char *request) {
    char buf[MAXLINE], header[MAXLINE], body[MAXLINE];
    int ret = 0;

    memset(buf, 0, sizeof(buf));
    memset(header, 0, sizeof(header));
    memset(body, 0, sizeof(body));

    while (1) {
        Rio_readlineb(rp, buf, MAXLINE);
        if (!strcmp(buf, "\r\n")) {
            break;
        }
        sscanf(buf, "%s %s", header, body);
        if (!strcmp(header, "Host:")) {   
            sprintf(request, "%s%s", request, buf);
            ret = 1;
        }
        else if (strcmp(header, "User-Agent:") && 
                 strcmp(header, "Connection:") &&
                 strcmp(header, "Proxy-Connection:")) {
            sprintf(request, "%s%s", request, buf);
        }
    }

    return ret;
}
/* $end read_request */

/*
 * forwards server's response to client
 */
/* $begin forward_response */
void forward_response(rio_t *rp, int client_fd, char *uri) {
    char buf[MAXBUF], hdr_name[MAXLINE], hdr_data[MAXLINE];
    int bytes;
    int content_length = 0;
    char content_type[MAXLINE];
    web_buf w_buf;

    /* init web buf */
    init_buf(&w_buf);

    /* read response line */
    bytes = rio_readlineb(rp, buf, MAXLINE);
    if (bytes <= 0) {
        printf("read response line failed or EOF encountered\n");
        return;
    }
    bytes = rio_writen(client_fd, buf, bytes);
    if (bytes <= 0) {
        printf("forward response line failed\n");
        return;
    }

    /* buffer response line */
    set_buf(&w_buf, buf, bytes);

    /* forward response headers */
    while (1) {
        bytes = rio_readlineb(rp, buf, MAXLINE);
        if (bytes <= 0) {
            printf("read response header failed or EOF encountered\n");
            return;
        }
        printf("read response header: %s", buf);
        bytes = rio_writen(client_fd, buf, strlen(buf));
        if (bytes <= 0) {
            printf("forward response header failed\n");
            return;
        }
        /* buffer response headers */
        set_buf(&w_buf, buf, bytes);

        /* empty line, response hdrs finish */
        if (!strcmp(buf, "\r\n")) 
            break;
        /* not empty line, extract content type and content length */
        sscanf(buf, "%s %s", hdr_name, hdr_data);
        // printf("hdr_name: %s\n", hdr_name);
        // printf("hdr_data: %s\n", hdr_data);
        if (!strcmp(hdr_name, "Content-Type:")
            || !strcmp(hdr_name, "Content-type:"))
            strcpy(content_type, hdr_data);
        if (!strcmp(hdr_name, "Content-Length:")
            || !strcmp(hdr_name, "Content-length:"))
            content_length = atoi(hdr_data);
    }
    printf("content type: %s\n", content_type);
    printf("content length: %d\n", content_length);

    /* forward response body */
    printf("forward response body begin...\n");
    while ((bytes = rio_readnb(rp, buf, MAXBUF))) {
        if (bytes < 0) {
            printf("read response body failed\n");
            return;
        } 
        bytes = rio_writen(client_fd, buf, bytes);
        if (bytes < 0) {
            printf("write response body failed\n");
            return;
        }
        printf("write %d bytes to client\n", bytes);
        /* buffer response body */
        set_buf(&w_buf, buf, bytes);
    }  

    if (w_buf.flag) {
        set_cache(uri, w_buf.buf, w_buf.size);
    }
}
/* $end forward_response */

/*
 * parse_uri - parse URI into host and port args
 *             return 0 if success, -1 if failed
 */
/* $begin parse_uri */
void parse_uri(char *uri, char *host, char *port, char *url) {
    char *start, *end;
    char temp[MAXLINE];

    start = strstr(uri, "http://");
    start = start + strlen("http://");
    end = strchr(start, '/');
    *end = '\0';
    memcpy(temp, start, strlen(start)+1);
    /* port number provided */
    if (strchr(temp, ':')) {
        end = strchr(temp, ':');
        *end = '\0';
        memcpy(host, temp, strlen(temp) + 1);
        memcpy(port, end + 1, strlen(end + 1) + 1);
    }
    /* port numbe not provided */
    else {
        memcpy(host, temp, strlen(temp) + 1);
        memcpy(port, "80", strlen("80") + 1);
    }
    start = strchr(start, '\0');
    *start = '/';
    memcpy(url, start, strlen(start) + 1);   
}
/* $end parse_uri */

/*
 * clienterror - returns an error message to the client
 */
/* $begin clienterror */
void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg) {
    char buf[MAXLINE], body[MAXBUF];

    memset(buf, 0, sizeof(buf));
    memset(body, 0, sizeof(body));

    /* Build the HTTP response body */
    sprintf(body, "<html><title>Tiny Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);

    /* Print the HTTP response */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    Rio_writen(fd, buf, strlen(buf));
    Rio_writen(fd, body, strlen(body));
}