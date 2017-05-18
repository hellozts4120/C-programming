/*
 * proxy.c - A Caching Web Proxy
 * 14307110190 Tianshu Zhou
 * ICS(II) - proxylab
 */

#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
static const char *accept_hdr = "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n";
static const char *accept_encoding_hdr = "Accept-Encoding: gzip, deflate\r\n";
static const char *conn_hdr = "Connection: close\r\n";
static const char *proxy_hdr = "Proxy-Connection: close\r\n";

int is_valid_http(char *buf);
void doit(int fd);
int read_request(rio_t *rp, char *request);
void forward_response(rio_t *rp, int client_fd);
void parse_uri(char *uri, char *host, char *port);
void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);

int main(int argc, char **argv) {
    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    struct sockaddr_in clientaddr;
    socklen_t clientlen = sizeof(clientaddr);

    /* check command line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    /* ignore SIGPIPE signal */
    signal(SIGPIPE, SIG_IGN);

    listenfd = Open_listenfd(argv[1]);
    while (1) {
        /* listen for incoming connections */
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *)&clientaddr, clientlen, hostname, MAXLINE, 
                     port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);
        /* service the request accordingly */
        doit(connfd);
        /* close the connection */
        Close(connfd);
    }
    return 0;
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
 * doit - serve as a proxy to handle one HTTP request/response transaction
 */
/* $begin doit */
void doit(int connfd) {
    char buf[MAXLINE], method[MAXLINE], host[MAXLINE], uri[MAXLINE], port[MAXLINE], version[MAXLINE], request[MAXBUF];
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
        if (strcasecmp(method, "GET")) {
            clienterror(connfd, method, "501", "Not Implemented", "Proxy does not implement this method");
            return;
        }
    }
    else {
        clienterror(connfd, "", "400", "Bad Requset", "Proxy does not understand this request");
        return;
    }
    
    parse_uri(uri, host, port);
    
    /* build the request line */
    sprintf(request, "GET %s HTTP/1.0\r\n", uri);
    
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

    forward_response(&rio_server, connfd);
    Close(serverfd);

    return 0;
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
void forward_response(rio_t *rp, int client_fd) {
    char buf[MAXBUF], hdr_name[MAXLINE], hdr_data[MAXLINE];
    char *buf_p;
    int bytes;
    int content_length = 0;
    char content_type[MAXLINE];

    /* read response line */
    bytes = rio_readlineb(rp, buf, MAXLINE);
    if (bytes <= 0) {
        printf("read response line failed or EOF encountered\n");
        return;
    }
    bytes = rio_writen(client_fd, buf, strlen(buf));
    if (bytes <= 0) {
        printf("forward response line failed\n");
        return;
    }

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
    if (content_length) {
        buf_p = malloc(content_length);
        bytes = rio_readnb(rp, buf_p, content_length);
        if (bytes <= 0) {
            printf("read response body failed or EOF encoutnered\n");
            free(buf_p);
            return;
        } 
        bytes = rio_writen(client_fd, buf_p, content_length);
        if (bytes <=0) {
            printf("write response body failed\n");
            free(buf_p);
            return;
        }
        printf("write %d bytes to client\n", bytes);
        free(buf_p);   
    }
    else
        printf("content length not found!");
}
/* $end forward_response */

/*
 * parse_uri - parse URI into host and port args
 *             return 0 if success, -1 if failed
 */
/* $begin parse_uri */
void parse_uri(char *uri, char *host, char *port) {
    char *start, *end;

    start = strstr(uri, "//");
    start += 2;
    end = start;
    /* port is specified */
    if ((end = strchr(end, ':'))) {
        *end = '\0';
        strcpy(host, start);
        start = end + 1;
        end = strchr(start, '/');
        *end = '\0';
        strcpy(port, start);
        *uri = '/';
        strcpy(uri + 1, end + 1);
    }
    /* use default port 80 */
    else {
        strcpy(port, "80");
        end = strchr(start, '/');
        *end = '\0';
        strcpy(host, start);
        *uri = '/';
        strcpy(uri + 1, end + 1);
    }
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
