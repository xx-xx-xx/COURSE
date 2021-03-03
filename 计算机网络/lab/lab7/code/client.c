#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <process.h>

#define SEND_MSG_TARGET_BIT 20
#define CONTENT_LENGTH_BIT 24
#define PACKAGE_TYPE_BIT 28
#define ACTION_TYPE_BIT 29
#define CONTENT_BIT 30
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

enum bool{false, true} connectState=false;
enum state{STOP, RUN, BLOCK};
volatile enum state messageState,receiverState,mainState;
volatile int id;
struct data{
    char time[20];
    int targetID;
    int contentLength;
    char packageType;
    char actionType;
    char body[];
}model;

char targetIp[17];
u_short targetPort;
SOCKET serverSock;
struct sockaddr_in sockAddr;
char responseBuffer[MAXWORD] = {0};
char messageBuffer[MAXWORD] = {0};
char requestBuffer[MAXWORD] = {0};
const char* menu = "[1] ����\n[2] �Ͽ�����\n[3] ��ȡʱ��\n[4] ��ȡ����\n"
                   "[5] ��ȡ�����б�\n[6] ����Ϣ\n[7] �˳�\n\n";

void packData(char type,char action){
    memset(requestBuffer,0,sizeof(requestBuffer));
    SYSTEMTIME sys;
    GetLocalTime(&sys);
    sprintf(requestBuffer,"%4d/%02d/%02d %02d:%02d:%02d",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond);
    int length = (int)strlen(messageBuffer);
    strcpy(&requestBuffer[CONTENT_LENGTH_BIT], (char*)&length);
    strcpy(&requestBuffer[SEND_MSG_TARGET_BIT], (char*)&id);
    requestBuffer[PACKAGE_TYPE_BIT] = type;
    requestBuffer[ACTION_TYPE_BIT] = action;
    strcpy(&requestBuffer[CONTENT_BIT], messageBuffer);
    send(serverSock, requestBuffer, CONTENT_BIT + length + sizeof(char), 0);
    memset(requestBuffer,0,sizeof(requestBuffer));
}

struct data* unpackData(){
    int contentSize = *(int*)&responseBuffer[CONTENT_LENGTH_BIT];
    struct data* unpacked = (struct data*)malloc(sizeof(model)+contentSize+1);
    memset((void*)unpacked,0,sizeof(model)+contentSize+1);
    memcpy((void*)unpacked,(void*)responseBuffer,sizeof(model));
    memcpy(unpacked->body, &responseBuffer[CONTENT_BIT], contentSize);
    memset(responseBuffer,0,sizeof(responseBuffer));
    return unpacked;
}

void receiver(){
    receiverState = RUN;
    while(receiverState != STOP){
        recv(serverSock, responseBuffer, MAXWORD, 0);
        messageState = BLOCK;
        while(messageState == BLOCK);
    }
    _endthread();
}

void closeSocket(){
    if (connectState){
        messageBuffer[0] = '\0';
        packData(PACKAGE_TYPE_GET,ACTION_TYPE_CLOSE);
        connectState = false;
        receiverState = STOP;
        printf("��������ֹ\n");
        closesocket(serverSock);
    }
}

void message(){
    messageState = RUN;
    while (messageState != STOP){
        if (messageState == BLOCK){
            struct data* unpacked = unpackData();
            switch (unpacked->packageType) {
                case PACKAGE_TYPE_RESPONSE:
                    if (unpacked->actionType == ACTION_TYPE_TIME){
                        printf("��������ǰʱ��: %s",unpacked->time);
                    } else if ( unpacked->actionType == ACTION_TYPE_CONNECT_SUCCESS){
                        connectState = true;
                        printf("%s",unpacked->body);
                    } else {
                        printf("%s",unpacked->body);
                    }
                    break;
                case PACKAGE_TYPE_ORDER:
                    if (unpacked->actionType == ACTION_TYPE_CONNECT_FAIL){
                        closeSocket();
                        printf("\n[Disconnected] ������������\n");
                    } else if (unpacked->actionType == ACTION_TYPE_SEND_MESSAGE){
                        printf("[Receive]\n�Է��ͻ��˱�ţ�%d\n����ʱ�䣺%s",unpacked->targetID,unpacked->time);
                        printf("%s",unpacked->body);
                        printf("\n[Connected] ������������\n");
                    }
                    break;
            }
            free(unpacked);
            while (messageState == BLOCK) messageState = RUN;
            while (mainState == BLOCK) mainState = RUN;
        }
    }
    _endthread();
}

void connectServer(){
    if (connectState){
        closeSocket();
    }
    serverSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = PF_INET;
    printf("������Ŀ��IP(127.0.0.1)��˿�(5507)\n");
    scanf("%s%hu", targetIp, &targetPort);
    sockAddr.sin_addr.s_addr = inet_addr(targetIp);
    sockAddr.sin_port = htons(targetPort);
    if (connect(serverSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR)) == -1){
        printf("����ʧ��\n");
        while (mainState == BLOCK) mainState = RUN;
    } else {
        _beginthread(receiver,0,NULL);
    }
}

void sendMessage(){
    fflush(stdin);
    printf("���뷢�͸�[%d]�ŵ���Ϣ�������밴���λس�\n",id);
    char ch;
    int index = 0;
    enum bool over = false;
    while(true){
        ch = (char)getchar();
        if (ch == '\n'){
            if (over) break;
            else over = true;
        } else over = false;
        messageBuffer[index++] = ch;
    }
    messageBuffer[index] = '\0';
    packData(PACKAGE_TYPE_GET,ACTION_TYPE_SEND_MESSAGE);
}
int main(){
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    printf("%s", menu);
    _beginthread(message,0,NULL);
    mainState = RUN;
    while (mainState != STOP){
        int op;
        if (connectState){
            printf("\n[Connected] ������������\n");
        } else {
            printf("\n[Disconnected] ������������\n");
        }
        fflush(stdin);
        scanf("%d",&op);
        if (!connectState && op >= 3 && op <= 6){
            printf("�����ӷ�����\n");
            continue;
        }
        switch (op) {
            case 1:
                mainState = BLOCK;
                connectServer();
                break;
            case 2:
                closeSocket();
                break;
            case 3:
                mainState = BLOCK;
                    messageBuffer[0] = '\0';
                    packData(PACKAGE_TYPE_GET,ACTION_TYPE_TIME);
                break;
            case 4: 
                mainState = BLOCK;
                messageBuffer[0] = '\0';
                packData(PACKAGE_TYPE_GET,ACTION_TYPE_NAME);
                break;
            case 5: 
                mainState = BLOCK;
                messageBuffer[0] = '\0';
                packData(PACKAGE_TYPE_GET,ACTION_TYPE_CLIENT_LIST);
                break;
            case 6: 
                mainState = BLOCK;
                printf("������Ŀ���ID\n");
                scanf("%d",&id);
                sendMessage();
                break;
            case 7:
                mainState = STOP;
                closeSocket();
                break;
            default:
                printf("������Ч\n");
                break;
        }
        while(mainState == BLOCK);
    }
    while(messageState!=STOP || receiverState!=STOP){
        messageState = STOP;
        receiverState = STOP;
    }
    closesocket(serverSock);
    WSACleanup();

    system("pause");
    return 0;
}



