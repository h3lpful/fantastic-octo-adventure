#include "wolfssh/ssh.h"
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <cstdint>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

int connect_to_ssh(char* addr, uint16_t port)
{
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = inet_addr(addr);
    int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(s < 0)
    {
        perror("socket");
        return s;
    }
    if(connect(s, (struct sockaddr*)&saddr, sizeof(saddr)) < 0)
    {
        perror("connect");
        close(s);
        return -1;
    }
    return s;
}

int ssh_accept_client(char* addr, uint16_t port)
{
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = inet_addr(addr);
    int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int opt = 1;
    if(s < 0)
    {
        perror("socket");
        return s;
    }
    if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt");
        close(s);
        return -1;
    }
    if(bind(s, (struct sockaddr*)&saddr, sizeof(saddr)) < 0)
    {
        perror("bind");
        close(s);
        return -1;
    }
    if(listen(s, 1) < 0)
    {
        perror("listen");
        close(s);
        return -1;
    }
    socklen_t slen = sizeof(saddr);
    int c = accept(s, (struct sockaddr*)&saddr, &slen);
    close(s);
    if(c < 0)
    {
        return -1;
    }
    return c;
}

void client(char* addr, uint16_t port)
{
    int sockfd = connect_to_ssh(addr, port);
    WOLFSSH_CTX *ctx;
    WOLFSSH *ssh;
    ctx = wolfSSH_CTX_new(WOLFSSH_ENDPOINT_CLIENT, NULL);
    
    ssh = wolfSSH_new(ctx);
    wolfSSH_CTX_SetSshProtoIdStr(ctx, "SSH-2.0-urmomssh\r\n");
    wolfSSH_CTX_SetAlgoListKex(ctx, "urmom,abcd");
    wolfSSH_CTX_SetAlgoListKey(ctx, "urmom,urmom,rsa");

    wolfSSH_SetUsername(ssh, "user");
    wolfSSH_set_fd(ssh, sockfd);
    wolfSSH_connect(ssh);

    wolfSSH_free(ssh);
    wolfSSH_CTX_free(ctx);
}

void server(char* addr, uint16_t port)
{
    int sockfd = ssh_accept_client(addr, port);
    WOLFSSH_CTX *ctx;
    WOLFSSH *ssh;
    ctx = wolfSSH_CTX_new(WOLFSSH_ENDPOINT_SERVER, NULL);
    ssh = wolfSSH_new(ctx);

    wolfSSH_set_fd(ssh, sockfd);
    wolfSSH_accept(ssh);

    wolfSSH_free(ssh);
    wolfSSH_CTX_free(ctx);
}

int main(int argc, char** argv)
{
    wolfSSH_Init();
    wolfSSH_Debugging_ON();
    printf("%d\n", atoi(argv[2]));
    std::thread t1(server, argv[1], atoi(argv[2]));
    usleep(10000);
    std::thread t2(client, argv[1], atoi(argv[2]));
    t1.join();
    t2.join();

    wolfSSH_Cleanup();
    return 0;
}