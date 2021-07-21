#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QFile>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <string>

class DBmanager
{
public:
    DBmanager();

    static void createUserDatabase();
    static bool isAccountExist(const QString &userName, const QString &userPass);
    static bool createAccount(const QString &userName, const QString &userPass);
    static void saveNewRiddle(const QMap<QString,QVariant> &riddleData);
    static void updateScore(const int &score, const unsigned short int &fAnswer);
    static void saveMarkAsAnswered(const unsigned int &id);

    static QVariantList getRiddleList();
    static QString getUserName();
    static int getUserScore();
    static unsigned short int getUserFAnswer();

private:

    static QString userRDBpath; /* user Riddle database pass:  every user have a special riddle
                                 * database with default riddles in "users Database" folder by
                                 * this format: "userName_Riddle.db"
                                 * it is a copy of appDB in .qrc file. appDB.db contains default riddles */

    // information of logned in user
    static QString m_userName;
    static unsigned int m_userID;
    static unsigned short int m_userFAnswer; // number of user false answer (3 false answer = score-1)
    static int m_userScore;
};

#endif // DBMANAGER_H
