#include "GOST28147.h"
#include "RSAWizard.h"

using namespace boost::multiprecision;
using namespace boost::random;


int main()
{
    //std::vector<uint8_t> SBlock = makeSBlock();
  
    generator_type gen;

    /*Создаём рса классы клиента и сервера*/
    RSAWizard serverRSA;
    RSAWizard clientRSA;
    
    /*Сервер вычисляет и передаёт клиенту открытый ключ*/
    serverRSA.prepareKeys();
    clientRSA.setOpenKeys(serverRSA.getOpenKeys());

    /*Клиент генерирует сессионный ключ*/
    cpp_int sessionKeyClient = simpleStringToNumTranslate("this_is_a_pasw_for_GOST_28147_89");

    /*Клиент шифрует открытым ключём сессионный ключ. Сервер расшифровывает закрытым ключём*/
    cpp_int sessionKeyServer = serverRSA.decrypt(clientRSA.encrypt(sessionKeyClient));

    /*Оба участника имеют сессионный ключ, начинаем передачу сообщения*/
    GOSTWizard clientGOST;
    GOSTWizard serverGOST;
    
    /*Создаём сообщение для передачи*/
    cpp_int message = simpleStringToNumTranslate("THE MAGIC WORDS ARE SQUEAMISH OSSIFRAGE");

    /*Разделение сообщения на блоки по 64 бита*/
    std::vector<uint64_t> splitedMessage = splitter<uint64_t>(message);
    std::string decryptString;

    for(auto it : splitedMessage){
        /*Клиент шифрует часть сообщение по госту и передаёт серверу*/
        uint64_t res = clientGOST.encrypt(it, sessionKeyClient);

        /*сервер дешифрует сообщение*/
        res = serverGOST.decrypt(res, sessionKeyServer);

        decryptString.append(simpleNumToStringTranslate(res));
    }


    std::cout << decryptString << std::endl;

    return 0;
}