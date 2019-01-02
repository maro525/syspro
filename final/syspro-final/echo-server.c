#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFSIZE 1024
#define HIRAGANA_LEN 68
#define WORD_BUFSIZE 20


char* getWord(char * _hiragana)
{   

    printf("hiragana = %s", _hiragana);

    char begin[256];
    char *sp = _hiragana, *bp = begin;
    char *ssp = _hiragana, *bbp = begin;
    char c1 = 0, c2 = 0;
    int i = 0;

    while (*sp != '\0'){
        if ((*sp & 0xf0) == 0xe0){
            c1 = *sp;
        }
        else if ((*sp & 0xc0) == 0x80){
            if (c2 == 0){
                c2 = *sp;
            }
            else {
                *bp = *sp;
                bp++;
                c1 = c2 = 0;
            }
        }
        sp++;
        i += 1;
    }

    for(int a =0; a<i-4; a++)
        ssp++;

    c1 = *ssp;
    ssp++;
    c2 = *ssp;
    ssp++;
    *bbp = c1;
    bbp++;
    *bbp = c2;
    bbp++;
    *bbp = *ssp;
    bbp++;

    printf("begin = %s\n", begin);

    static char hiragana[HIRAGANA_LEN][4] = 
    {
        "あ", "い", "う", "え", "お", "か", "き", "く", "け", "こ",
        "が", "ぎ", "ぐ", "げ", "ご", "さ", "し", "す", "せ", "そ",
        "ざ", "じ", "ず", "ぜ", "ぞ", "た", "ち", "つ", "て", "と",
        "だ", "ぢ", "で", "ど", "な", "に", "ぬ", "ね", "の",
        "は", "ひ", "ふ", "へ", "ほ", "ば", "び", "ぶ", "べ", "ぼ",
        "ぱ", "ぴ", "ぷ", "ぺ", "ぽ", "ま", "み", "む", "め", "も",
        "や", "ゆ", "よ", "ら", "り", "る", "れ", "ろ", "わ"
    };

    static char word[HIRAGANA_LEN][WORD_BUFSIZE] = 
    {
        "あいなめ\n", "いさき\n", "うみへび\n", "えい\n", "おいかわ\n",
        "かさご\n", "きじはた\n", "くろさぎ\n", "けしょうふぐ\n", "こい\n",
        "がしら\n", "ぎぎ\n", "ぐるくま\n", "げんろくだい\n", "ごんずい\n",
        "さわら\n", "しまあじ\n", "すずき\n", "せぜら\n", "そうだがつお\n",
        "ざんだー\n", "じんべいざめ\n", "ずな\n", "ぜにごち\n", "ぞう\n",
        "たら\n", "ちんあなご\n", "つばめうお\n", "てらふ\n", "とらふぐ\n",
        "だんじゅうろ\n", "ぢごい\n", "でぶがつお\n", "どちざめ\n",
        "ながいわし\n", "にじます\n", "ぬたうなぎ\n", "ねこざめ\n", "のどぐろ\n",
        "はす\n", "ひらめ\n", "ふさぎんぽ\n", "へいて\n", "ほうぼう\n",
        "ばたばた\n", "びんきり\n", "ぶり\n", "べら\n", "ぼら\n",
        "ぱんがれい\n", "ぴりか\n", "ぷれこ\n", "ぺへれい\n", "ぽんだら\n",
        "まんぼう\n", "みずいか\n", "むつごろう\n", "めばる\n", "もどろ\n",
        "やがら\n", "ゆめかさご\n", "よこえそ\n",
        "らくだざめ\n", "りよおごち\n", "るりはた\n", "れんこだい\n", "ろうにんあじ\n",
        "わかさぎ\n"
    };

    int ind  = -1;
    for(int i=0; i < HIRAGANA_LEN; i++)
    {
        char *n = strstr(begin, hiragana[i]);
        if(n != NULL)
        {
            ind = i; 
            break;
        }
    }
    char *w = malloc(sizeof(char) * 1024);
    if(ind != -1)
        w = word[ind];
    else
        w = "No Word\n";

    return w;
}

void echo_execute(int fd)
{
    char buffer[BUFSIZE];
    int t;
    while(1)
    {   
        bzero(buffer, BUFSIZE);
        t = read(fd, buffer, sizeof(buffer));

        if(t < 0)
            perror("read");

        // t = write(fd, buffer, strlen(buffer));
        char *word;
        word = getWord(buffer);
        t = write(fd, word, strlen(word));
    }
}


int main(int argc, char **argv)
{
    pid_t child_pid;
    int listenfd, connfd, n;
    int port;
    socklen_t chilen;
    struct sockaddr_in cliaddr, servaddr;

    if (argc != 2)
        perror("argc");

    port = atoi(argv[1]);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    if(listenfd < 0)
        perror("socket");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons((unsigned short)port);
    
    n = bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    if (n < 0)
        perror("bind");

    n = listen(listenfd, 5);

    if (n < 0)
        perror("listen");

    while(1)
    {   
        chilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &chilen);
        
        if ( (child_pid = fork()) == 0)
        {    
            echo_execute(connfd);
        } 
        close(connfd);
    }

    close(listenfd);
    exit(0);
}