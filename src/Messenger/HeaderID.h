#ifndef HEADER_ID_H
#define HEADER_ID_H

#define __IP__ "127.0.0.1"
#define __PORT__ "8888"

#define ID_HANDSHAKE_OPEN_KEY_REQUEST 10 // Передача серверу запроса на получение открытого ключа
#define ID_HANDSHAKE_OPEN_KEY_RESPONSE 11  // Передача клиенту RSA открытого ключа
#define ID_HANDSHAKE_SESSION_KEY_REQUEST 12  // Запрос сервером сессионного ключа
#define ID_HANDSHAKE_SESSION_KEY_RESPONSE 13  // Передача серверу сессионного ключа ГОСТ

#define ID_SEND_MESSAGE_TO_SERVER 30 
#define ID_SEND_MESSAGE_TO_CLIENT 31

#define ID_ERROR 44


#define ID_SERVER 100
#define ID_CLIENT 101

#endif