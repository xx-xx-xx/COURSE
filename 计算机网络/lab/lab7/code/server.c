#include <stdio.h>
#include <winsock2.h>
#include <process.h>

#define SEND_MSG_SOURCE_BIT 20
#define CONTENT_LENGTH_BIT 24
#define PACKAGE_TYPE_BIT 28
#define ACTION_TYPE_BIT 29
#define CONTENT_BIT 30
#define LIST_SIZE 3
#define PACKAGE_TYPE_GET 'G'
#define PACKAGE_TYPE_RESPONSE 'R'
#define PACKAGE_TYPE_ORDER 'O'
#define ACTION_TYPE_TIME 'T'
#define ACTION_TYPE_NAME 'N'
#define ACTION_TYPE_CLIENT_LIST 'L'
#define ACTION_TYPE_CONNECT_SUCCESS 'S'
#define ACTION_TYPE_CONNECT_FAIL 'F'
#define ACTION_TYPE_SEND_MESSAGE 'M'
#define ACTION_TYPE_CLOSE 'C'

enum state
{
    STOP,
    RUN,
    BLOCK
};
volatile enum state handleState[LIST_SIZE], mainState = RUN;
volatile int id;
char requestBuffer[MAXWORD] = {0};
char messageBuffer[MAXWORD] = {0};
char responseBuffer[MAXWORD] = {0};
SOCKET servSock;

struct data
{
    char time[20];
    int sourceID;
    int contentLength;
    char packageType;
    char actionType;
    char body[];
} model;

struct connection
{
    u_short port;
    struct in_addr ip;
    SOCKET clientSock;
    int state;
} clientList[LIST_SIZE];

void packData(char type, char action, SOCKET targetSock)
{
    memset(responseBuffer, 0, sizeof(responseBuffer));
    SYSTEMTIME sys;
    GetLocalTime(&sys);
    sprintf(responseBuffer, "%4d/%02d/%02d %02d:%02d:%02d\n", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
    int length = (int)strlen(messageBuffer);
    strcpy(&responseBuffer[CONTENT_LENGTH_BIT], (char *)&length);
    strcpy(&responseBuffer[SEND_MSG_SOURCE_BIT], (char *)&id);
    responseBuffer[PACKAGE_TYPE_BIT] = type;
    responseBuffer[ACTION_TYPE_BIT] = action;
    strcpy(&responseBuffer[CONTENT_BIT], messageBuffer);
    send(targetSock, responseBuffer, CONTENT_BIT + length + sizeof(char), 0);
    memset(responseBuffer, 0, sizeof(responseBuffer));
}

struct data *unpackData()
{
    int contentSize = *(int *)&requestBuffer[CONTENT_LENGTH_BIT];
    struct data *unpacked = (struct data *)malloc(sizeof(model) + contentSize + 1);
    memcpy((void *)unpacked, (void *)requestBuffer, sizeof(model));
    memcpy(unpacked->body, &requestBuffer[CONTENT_BIT], contentSize);
    memset(requestBuffer, 0, sizeof(requestBuffer));
    return unpacked;
}
void clientListResponse(int index, SOCKET client)
{
    int count = 0;
    char tempBuffer[MAXBYTE] = {0};
    memset(messageBuffer, 0, MAXWORD);
    int check;
    for (check = 0; check < LIST_SIZE; check++)
    {
        if (clientList[check].state)
        {
            count++;
            sprintf(tempBuffer, "[%d] �ͻ��� -- ����%s:%d\n", check,
                    inet_ntoa(clientList[check].ip), clientList[check].port);
            strcat(messageBuffer, tempBuffer);
        }
    }
    sprintf(tempBuffer, "%d���ͻ������������������ǰ�ͻ��˱�ţ�%d \n", count, index);
    strcat(messageBuffer, tempBuffer);
    packData(PACKAGE_TYPE_RESPONSE, ACTION_TYPE_CLIENT_LIST, client);
    while (handleState[index] == BLOCK)
        handleState[index] = RUN;
}

void sendMessage(int index, SOCKET client, struct data *unpacked)
{
    int target = unpacked->sourceID;
    printf("[Request] %d����Ϣ��%d�ſͻ���\n", index, target);
    memset(messageBuffer, 0, MAXWORD);
    if (target < 0 || target >= LIST_SIZE)
    {
        sprintf(messageBuffer, "��Ч�Ŀͻ��˱��\n");
        printf("��Ч�Ŀͻ��˱��\n");
    }
    else if (clientList[target].state == STOP)
    {
        sprintf(messageBuffer, "�ͻ���δ������������\n");
        printf("�ͻ���δ������������\n");
    }
    else
    {
        strcpy(messageBuffer, unpacked->body);
        id = index;
        printf("[Point] ��%d�ſͻ��˷�����Ϣ��\n%s", target, unpacked->body);
        packData(PACKAGE_TYPE_ORDER, ACTION_TYPE_SEND_MESSAGE, clientList[target].clientSock);
        sprintf(messageBuffer, "���ͳɹ�\n");
        printf("[Hello] ���ͳɹ�\n");
    }
    packData(PACKAGE_TYPE_RESPONSE, ACTION_TYPE_SEND_MESSAGE, client);
    while (handleState[index] == BLOCK)
        handleState[index] = RUN;
}

void handleRequest(void *i)
{
    int index = *(int *)i;
    SOCKET client = clientList[index].clientSock;
    printf("[Hello] �ѽ��������ӣ���ţ� %d \n", index);

    handleState[index] = RUN;
    sprintf(messageBuffer, "[Hello] ��������������\n");
    packData(PACKAGE_TYPE_RESPONSE, ACTION_TYPE_CONNECT_SUCCESS, client);
    while (handleState[index])
    {
        int a = recv(client, requestBuffer, MAXWORD, 0);
        if (a < 0)
            break;
        if (handleState[index] == STOP)
            break;
        struct data *unpacked = unpackData();
        switch (unpacked->actionType)
        {
        case ACTION_TYPE_TIME:
            for (int i = 0; i < 100; i++)
            {
                messageBuffer[0] = '\0';
                printf("[Response] ��Ӧ%d�ſͻ��˵�ʱ������\n", index);
                packData(PACKAGE_TYPE_RESPONSE, ACTION_TYPE_TIME, client);
            }
            break;
        case ACTION_TYPE_NAME:
            sprintf(messageBuffer, "����������Ϊ: 3180105507\n");
            printf("[Response] ��Ӧ%d�ſͻ��˵���������\n", index);
            packData(PACKAGE_TYPE_RESPONSE, ACTION_TYPE_NAME, client);
            break;
        case ACTION_TYPE_CLOSE:
            handleState[index] = STOP;
            clientList[index].state = STOP;
            printf("[Request] �ر�%d�ſͻ��˵�����\n", index);
            break;
        case ACTION_TYPE_CLIENT_LIST:
            handleState[index] = BLOCK;
            printf("[Response] ��Ӧ%d�ſͻ��˵��б�����\n", index);
            clientListResponse(index, client);
            break;
        case ACTION_TYPE_SEND_MESSAGE:
            handleState[index] = BLOCK;
            sendMessage(index, client, unpacked);
            break;
        }
        while (handleState[index] == BLOCK)
            ;
        free(unpacked);
    }
    _endthread();
}

int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    int i;
    for (i = 0; i < LIST_SIZE; i++)
    {
        clientList[i].state = STOP;
    }
    servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(5507);
    if (SOCKET_ERROR == bind(servSock, (SOCKADDR *)&sockAddr, sizeof(SOCKADDR)))
    {
        printf("�˿�bindʧ��\n");
        closesocket(servSock);
        WSACleanup();
        return 0;
    }
    SOCKADDR clientAddr;
    SOCKET clientSock;
    int nSize = sizeof(SOCKADDR);
    listen(servSock, LIST_SIZE);
    printf("[Hello] ������������(127.0.0.1:5507)����������� %d\n", LIST_SIZE);
    while (mainState)
    {
        clientSock = accept(servSock, (SOCKADDR *)&clientAddr, &nSize);
        char tempPort[2];
        tempPort[1] = *((char *)&((*(SOCKADDR_IN *)&clientAddr).sin_port));
        tempPort[0] = *((char *)&((*(SOCKADDR_IN *)&clientAddr).sin_port) + 1);
        u_short port = *(u_short *)&tempPort;
        printf("[Request] �յ�%s:%d���������� \n",
               inet_ntoa(*(struct in_addr *)&clientAddr.sa_data[2]), port);
        int i;
        for (i = 0; i < LIST_SIZE; i++)
        {
            if (clientList[i].state == STOP)
            {
                clientList[i].ip = (*(SOCKADDR_IN *)&clientAddr).sin_addr;
                clientList[i].port = port;
                clientList[i].state = RUN;
                clientList[i].clientSock = clientSock;
                break;
            }
        }
        if (i == LIST_SIZE)
        {
            int randomKill = rand() % LIST_SIZE;
            sprintf(messageBuffer, "��������ֹ\n");
            printf("[Ponit] ����رղ�ʹ�� %d ����Ϊ�µ�����\n", randomKill);
            packData(PACKAGE_TYPE_ORDER, ACTION_TYPE_CONNECT_FAIL, clientList[randomKill].clientSock);
            handleState[randomKill] = STOP;
            clientList[randomKill].ip = (*(SOCKADDR_IN *)&clientAddr).sin_addr;
            clientList[randomKill].port = port;
            clientList[randomKill].clientSock = clientSock;
            i = randomKill;
        }
        _beginthread(handleRequest, 0, (void *)&i);
    }
    closesocket(servSock);
    WSACleanup();
    return 0;
}
