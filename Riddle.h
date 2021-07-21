#ifndef RIDDLE_H
#define RIDDLE_H

#include <QString>
#include <string>
#include <QVariantList>
#include <QVariant>

#include "dbmanager.h"

using std::string;

class Riddle
{
public:
    Riddle();

    bool login();
    void createAC();
    void mainMenu();
    void addRiddle();
    void openRiddle(const unsigned int &index);
    void scoreCalculator(const bool &result); // calculate score after user input the result
    void markAsAnswered(const unsigned int &index); // put (answered) after the riddle in menu

private:

    QVariantList riddleList; /* list of map => "Question", "result", "answered"
                              * answered is a (int) flag for aswered riddles
                              * 1=true , 0=false cause sqlite had not bool type */
};

#endif // RIDDLE_H
