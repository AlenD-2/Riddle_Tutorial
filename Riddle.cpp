#include "Riddle.h"

#include <iostream>

using std::cout;
using std::endl;
using std::cin;


Riddle::Riddle()
{
    /* if user.db file not exist in current directory then created it
     * this function use for the first time app opened */
    QString dbPath = QDir::currentPath()+"/user.db";
    if(!QFile::exists(dbPath))
        DBmanager::createUserDatabase();
}




bool Riddle::login()
{
    cout<<"\n<=====LOGIN=====>"<<endl
        <<"user name: ";

    string userName;
    getline(cin,userName);

    cout<<"password: ";
    string userPass;
    getline(cin,userPass);

    // send information to check is exist if yes then go to menu (look main.cpp file)
    return DBmanager::isAccountExist(QString::fromStdString(userName),
                                     QString::fromStdString(userPass));
}






void Riddle::createAC()
{
    cout<<"\n<===CREATE ACCOUNT===>\n";
    cout<<"user name: ";
    string userName;
    getline(cin,userName);

    cout<<"password: ";
    string userPass;
    getline(cin,userPass);

    if(DBmanager::createAccount(QString::fromStdString(userName),
                                QString::fromStdString(userPass)))
        cout<<"acount created! please enter y and login"<<endl; /* after creating account
                                                                 * need to login (see main.cpp file) */
    else
        cout<<"this user name is already exist!"<<endl;
}







void Riddle::mainMenu()
{
    riddleList = DBmanager::getRiddleList();
    string userName = DBmanager::getUserName().toStdString();

    for(;;)
    {
        int userScore = DBmanager::getUserScore();

        cout<<"\n<===========Menu============>"<<endl
            <<"Hello <"<<userName<<"> your score is: "<<userScore<<endl<<endl
            <<"enter '+' to add a riddle\n\n";

        for(int i=1 ; i<riddleList.size()+1 ; i++)
        {
            cout<<i<<"_"<<riddleList.at(i-1).toMap().take("Question").toString().toStdString();

            if(riddleList.at(i-1).toMap().take("answered") == 1) // a flag for answered riddle
                cout<<" (Answered)";

            cout<<endl;
        }

        char choice{};
        for(;;)
        {
            cout<<"\nChoose a Riddle:";
            cin>>choice;

            if(choice == '+')
            {
                addRiddle();
                break;
            }
            else
            {
                int choiceToInt = choice-'0'; // convert char to int

                if(choiceToInt > 0 && choiceToInt <= riddleList.size())
                {
                    openRiddle(choiceToInt-1); // -1 because this is the index of riddleList
                    break;
                }
                else
                    cout<<"invalid input!"<<endl;
            }
        }
    }
}






void Riddle::addRiddle()
{
    cout<<"\n<====Adding Riddle====>"<<endl<<
          "ENTER RIDDLE: ";

    string newQuestion;
    cin.ignore();
    getline(cin,newQuestion);

    cout<<"\nENTER RESULT: ";
    string newResult;
    getline(cin,newResult);

    for(auto &i : newResult) // converting to lowercase to match with user result
        if(!std::islower(i))
            i = std::tolower(i);

    QMap<QString,QVariant> newRiddle;
    newRiddle["Question"] = QString::fromStdString(newQuestion);
    newRiddle["result"] = QString::fromStdString(newResult);

    riddleList.append(newRiddle);

    DBmanager::saveNewRiddle(newRiddle); // save to database
}







void Riddle::openRiddle(const unsigned int &index) // show the riddle to answer
{
    string question{riddleList.at(index).toMap().take("Question").toString().toStdString()};
    string result{riddleList.at(index).toMap().take("result").toString().toStdString()};

    cout<<"\n<======Riddle======>"<<endl
       <<"RIDDLE: "<<question<<endl;

    cin.ignore(); // for cin>>char that use before this function
    string userResult{};
    for(;;)
    {
        cout<<"\nEnter # to back\n"
            <<"\nENTER RESULT: ";
        getline(cin,userResult);

        for(auto &i : userResult) // converting to lowercase to match with result
            if(!std::islower(i))
                i = std::tolower(i);

        if(userResult == "#")
        {
            return; // return to menu
        }
        else if(userResult == result)
        {
            cout<<"RIGHT :D"<<endl;
            scoreCalculator(true);
            markAsAnswered(index);
            return; // return to menu
        }
        else
        {
            cout<<"WRONG :("<<endl;
            scoreCalculator(false);
        }
    }
}




void Riddle::scoreCalculator(const bool &isRightAnswer)
{
    int score{DBmanager::getUserScore()};
    unsigned short int fAnswer{DBmanager::getUserFAnswer()};

    if(isRightAnswer)
    {
        score++;
        DBmanager::updateScore(score, fAnswer);
    }
    else
    {
        fAnswer++;
        if(fAnswer < 3)
        {
            DBmanager::updateScore(score, fAnswer);
        }
        else
        {
            fAnswer = 0;
            score--;
            DBmanager::updateScore(score, fAnswer);
            cout<<"\nYou got a negative score!"<<endl;
        }
    }
}





void Riddle::markAsAnswered(const unsigned int &index)
{
    auto itr = riddleList[index].toMap();
    itr["answered"] = 1;
    riddleList[index] = itr;

    DBmanager::saveMarkAsAnswered(index+1); // ID = index+1
}
