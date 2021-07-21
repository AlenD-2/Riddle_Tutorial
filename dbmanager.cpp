#include "dbmanager.h"

#include <QDebug>


// initialization static variables
QString DBmanager::userRDBpath{};
QString DBmanager::m_userName{};
unsigned int DBmanager::m_userID{};
unsigned short int DBmanager::m_userFAnswer{};
int DBmanager::m_userScore{};
// END


DBmanager::DBmanager()
{

}

void DBmanager::createUserDatabase()
{
    // creating a user.db file in current directory

    QString dbPath = QDir::currentPath();

    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath + "/user.db");

    db.open();

    QSqlQuery query(db);

    /* user.db file have a users table with:
     * userID - userName - userPass - userScore - fAnswer
     * fAnswer is the number of user false answers */
    if(!query.exec("CREATE TABLE IF NOT EXISTS users("
               "userID INTEGER PRIMARY KEY,"
               "userName TEXT,"
               "userPass TEXT,"
               "userScore INTEGER NUT NULL,"
               "fAnswer INTEGER NOT NULL)"))
        qDebug()<<"Error in:"<<__FUNCTION__<<query.lastError();

    db.close();
}

bool DBmanager::isAccountExist(const QString &userName, const QString &userPass)
{
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::currentPath()+"/user.db");
    db.open();

    QSqlQuery query(db);
    if(!query.exec("SELECT userName, userPass, userID, fAnswer, userScore FROM users"))
        qDebug()<<"Error in:"<<__FUNCTION__<<query.lastError();

    while (query.next())
    {
        if(userName==query.value(0) && userPass==query.value(1))
        {
            m_userName    = query.value(0).toString();
            m_userID      = query.value(2).toInt();
            m_userFAnswer = query.value(3).toInt();
            m_userScore   = query.value(4).toInt();
            userRDBpath   = QDir::currentPath()+"/users Database/"+userName+"_Riddle.db";
            //The userRDBpath variable is described in dbmanager.h

            db.close();
            return true;
        }
    }
    db.close();
    return false;
}

bool DBmanager::createAccount(const QString &userName, const QString &userPass)
{
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::currentPath()+"/user.db");
    db.open();

    QSqlQuery query(db);
    query.exec("SELECT userName FROM users");
    // check is exist this user name? if exist then return false
    while (query.next())
    {
        if(userName == query.value(0))
            return false;
    }

    query.prepare("INSERT INTO users("
                  "userName,"
                  "userPass,"
                  "userScore,"
                  "fAnswer) "
                  "VALUES("
                  ":userName,"
                  ":userPass,"
                  "0,"
                  "0);");
    query.bindValue(":userName",userName);
    query.bindValue(":userPass",userPass);

    if(!query.exec())
        qDebug()<<"Error in:"<<__FUNCTION__<<query.lastError();
    db.close();

    // The codes below are described in dbmanager.h (userRDBpath description)

    // if "user Database" folder is not exist then create it
    QDir dir(QDir::currentPath()+"/users Database");
    if(!dir.exists())
        dir.mkpath(".");

    // creating special riddle database for this user (copy default database for this user)
    QString dbPath = QDir::currentPath()+"/users Database/"+userName+"_Riddle.db";
    QFile::copy(":/appDB.db",dbPath);
    QFile::setPermissions(dbPath, QFile::ReadOwner | QFile::WriteOwner); // cause can edit database

    return true;
}

void DBmanager::saveNewRiddle(const QMap<QString, QVariant> &riddleData)
{
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(userRDBpath);
    db.open();

    QSqlQuery query(db);
    query.prepare("INSERT INTO questions("
                  "question,"
                  "result,"
                  "answered) "
                        "VALUES("
                        ":question,"
                        ":result,"
                        "0);");
    query.bindValue(":question",riddleData["Question"]);
    query.bindValue(":question",riddleData["result"]);

    if(!query.exec())
        qDebug()<<"Error in:"<<__FUNCTION__<<query.lastError();

    db.close();
}

void DBmanager::updateScore(const int &score, const unsigned short &fAnswer)
{
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::currentPath()+"/user.db");
    db.open();

    QSqlQuery query(db);
    query.prepare("UPDATE users "
                  "SET userScore = :score,"
                  "fAnswer = :fAnswer "
                  "WHERE userID = :ID");
    query.bindValue(":score",score);
    query.bindValue(":fAnswer",fAnswer);
    query.bindValue(":ID",m_userID); // current logned in user id

    if(!query.exec())
        qDebug()<<"Error in:"<<__FUNCTION__<<query.lastError();

    db.close();

    m_userScore = score;
    m_userFAnswer = fAnswer;
}

void DBmanager::saveMarkAsAnswered(const unsigned int &id)
{
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(userRDBpath);
    db.open();

    QSqlQuery query;
    query.prepare("UPDATE questions "
                  "SET answered = 1 "
                  "WHERE qID = :ID");
    query.bindValue(":ID",id);

    if(!query.exec())
        qDebug()<<"Error in:"<<__FUNCTION__<<query.lastError();

    db.close();
}

QVariantList DBmanager::getRiddleList()
{
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(userRDBpath);
    db.open();

    QSqlQuery query(db);
    if(!query.exec("SELECT question, result, answered FROM questions"))
        qDebug()<<"Error in:"<<__FUNCTION__<<query.lastError();

    QMap<QString,QVariant> riddleData;
    QVariantList riddleList;
    while (query.next())
    {
        riddleData["Question"] = query.value(0);
        riddleData["result"] = query.value(1);
        riddleData["answered"] = query.value(2);

        riddleList.append(riddleData);
    }
    db.close();

    return riddleList;
}

QString DBmanager::getUserName()
{
    return m_userName;
}

int DBmanager::getUserScore()
{
    return m_userScore;
}

unsigned short DBmanager::getUserFAnswer()
{
    return m_userFAnswer;
}
