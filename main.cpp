#include <QCoreApplication>
#include <iostream>
#include <string>
//#include "windows.h"

#include "Riddle.h"

using std::cout;
using std::endl;
using std::cin;
using std::string;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    Riddle riddle;

    for(;;)
    {
        string choice{}; // use string istead char to Avoid exceptions
        cout<<"do you have account? (y/n): ";
        getline(cin,choice); // use getline instead cin to Avoid exceptions

        if(choice == "y")
        {
            if(riddle.login()) // if login was Successful break for(;;) and go to main menu
                break;
            else
                cout<<endl<<"account not founded!"<<endl;
        }
        else if(choice == "n")
            riddle.createAC(); // need to login after creating account
    }

    riddle.mainMenu();


    return a.exec();
}
