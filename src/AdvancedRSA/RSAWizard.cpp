#include "RSAWizard.h"

//g++ RSAWizard.cpp -lboost_system -lboost_filesystem

using namespace boost::multiprecision;
using namespace boost::random;

    void RSAWizard::calculateClosedExp()
    {
        cpp_int eulerFunctionResult = eulerFunc(prime_1, prime_2);
        
        /*mod_fi(n) (d*e) == 1*/
        closedExp = modInverse(openExp, eulerFunctionResult);

        return;

    };

    void RSAWizard::printWizardParams()
    {
        std::cout << "Prime 1 = " << prime_1 << std::endl 
        << "Prime 2 = " << prime_2 << std::endl
        << "Prime multiply = " << primeMlpProduct << std::endl
        << "Open exp = " << openExp << std::endl
        << "Closed exp = " << closedExp << std::endl;

        return;
    }

    void RSAWizard::generatePrimeNumbers()
    {
        generator_type gen;

        cpp_int p1 = gen();

        std::cout << "Start generating prime numbers\n";

        while (!millerRabinPrimeTest(p1, MRPTROUNDS))
        {
            p1 = gen();
        }

        cpp_int p2 = gen();

        while (p1 == p2 || !millerRabinPrimeTest(p2, MRPTROUNDS))
        {
            p2 = gen();    
        }

        std::cout << "Prime number was generated. P1 = " << p1 << std::endl << "p2 = " << p2 << std::endl << std::endl;

        this->prime_1 = p1;
        this->prime_2 = p2;

        primeMlpProduct = prime_1 * prime_2;

        calculateClosedExp();

        return;
    };

    void RSAWizard::prepareKeys()
    {
        prime_1 = cpp_int("5559299515056720706822139232458725704753458821948444722038172091960828867993966228699995320894190564248340098221134406957131985736784428345326790557782689178614091019012019828429592539086646033984251832131922518397738545351235069114505994023209050092424554402219498412181835377308782584603653194424017720300887017304173408755787902320377802097239541275710488428384615973428059791986658172640686256639726118370633072409551022535113283364628641357609085492547150961212792466564034420829860942631683889859984562532017438849314869528481579138567630565755272968327070119986852868999481269378618044637092107145229942456656156830990253287152715279043263655554473295734120861451731589153098207021116501741822602099670806845546622164479388600419697766933951890180737323991017277826727261710072230456702205745568535367992056834097480729556622418260245244328131773311763221653157601222134117600688936971360916307232928720533530686983747");
        prime_2 = cpp_int("4931380667561426600991861575165413172031687197542593895617382666832058754727594326221076596615973675456088337538823190376089854646699794950421587795704089741779451466162619787276377118744082141761504909877579108786085637784694722525525833713919778885700419167958978082579125012942182524425527398831351071473681670154953920228487616163113943172716704540378737456343536300846911365433331338447473120121882639583739778530224654815435917316679796436591977236871849084451168478171812133286063808428335660792956958393610899276433963894977947035418745617019911592200119765024737655638468445877650489684973410027873448613478057109191796924961070065262393870915531646676875825055882622204976320799754885252132021610779007705123176668138887125901075828537144865065358823797310676386278760322165539239688715399033171613705774125020356432305237908643425629610797639608458521461708051013521540826769139431062634570462296834659017677598427");

        primeMlpProduct = prime_1 * prime_2;

        calculateClosedExp();

        return;
    }

    void RSAWizard::setOpenKeys(boost::multiprecision::cpp_int e, boost::multiprecision::cpp_int n)
    {
        primeMlpProduct = n;
        openExp = e;

        return;
    }

    void RSAWizard::setOpenKeys(std::pair<boost::multiprecision::cpp_int, boost::multiprecision::cpp_int> keyPair)
    {
        openExp = keyPair.first;
        primeMlpProduct = keyPair.second;

        return;
    }

    std::pair<boost::multiprecision::cpp_int, boost::multiprecision::cpp_int> RSAWizard::getOpenKeys()    
    {
        std::pair<cpp_int, cpp_int> res (openExp, primeMlpProduct);
        return res;
    }

    boost::multiprecision::cpp_int RSAWizard::encrypt(boost::multiprecision::cpp_int message)
    {
        cpp_int res = powMod(message, openExp, primeMlpProduct);
        return res;
    }

    /*m_x^d %n*/
    boost::multiprecision::cpp_int RSAWizard::decrypt(boost::multiprecision::cpp_int message)
    {
        cpp_int res = powMod(message, closedExp, primeMlpProduct);
        return res;
    }

