#include <iostream>
#include <sstream>
#include "CNode.h"
#include "CTree.h"
#include "../../Lista4/Lista3/Constants.h"
using namespace std;
int main()
{
    string input;
    string command;
    string expression;
    CTree<int>* treeInt=new CTree<int>();
	CTree<double>* treeDouble=new CTree<double>();
    CTree<string>* treeString=new CTree<string>();
    string whatTree = "int";
    std::vector<double> fValues;
    vector<string> sValues;
    std::vector<std::string> vsFormula;
    float fResult = 0;
    string sResult = "";
    bool correctType = false;
    while(correctType!=true)
    {
        cout << inputCorrectType<<endl;
        cin >> command;
        if (command == "int" || command == "string" || command == "double") correctType = true;
        else cout << invalidType << endl;
        whatTree = command;
    }
    while (command != quitCommand) {
        cout << enterPrompt;
        cin >> command;

        if (command == enterCommand) {
            getline(cin, expression);
            if(whatTree=="int")
            {
                treeInt = new CTree<int>(expression, 0, fValues);
                whatTree = "int";
            }
            else if(whatTree=="double")
            {
                treeDouble = new CTree<double>(expression, 0, fValues);
                whatTree = "double";
            }
            else
            {
                treeString = new CTree<string>(expression, 0, sValues);
                whatTree = "string";
            }
        }
        else if (command == compCommand) {
            getline(cin, expression);
            if (whatTree != "string")
            {
                fValues = stringToFloatVector(expression);
            }
            else {//dziala
                sValues = stringToStringVector(expression);
            }

            vector<double> emptyVector;
            vector<string> sEmptyVector;
            if(whatTree=="int")   fResult = treeInt->evaluatePrefixExpression(treeInt->getRoot(), emptyVector, treeInt->createVariableMap(treeInt->createFormula(treeInt->retrunFormula()), fValues));
            if(whatTree=="double") fResult = treeDouble->evaluatePrefixExpression(treeDouble->getRoot(), emptyVector, treeDouble->createVariableMap(treeDouble->createFormula(treeDouble->retrunFormula()), fValues));
            if(whatTree=="string") sResult = treeString->evaluateStringPrefix(treeString->getRoot(), sEmptyVector, treeString->createStringMap(treeString->createFormula(treeString->retrunFormula()), sValues));
            if(whatTree=="string")
            {
	            CTree<std::basic_string<char, std::char_traits<char>, std::allocator<char>>>::deletQuotes(sResult);
                cout << sResult << endl;
            }
            else
            {
                
                cout << fResult << endl;
            }
            fResult = 0;
            fValues.clear();
        }
        else if (command == joinCommand) { //dziala
            getline(cin >> ws, expression);
            
        }
        else if (command == varsCommand) {
            if (whatTree == "string")  treeString->returnVariables();
            else if (whatTree == "int")  treeInt->returnVariables();
            else if (whatTree == "double") treeDouble->returnVariables();
            //cout << endl;
        }
        else if (command == printCommand) {
            if (whatTree == "string") { treeString->printPrefixNotation(treeString->getRoot()); cout << endl; }
            if (whatTree == "int") { treeInt->printPrefixNotation(treeInt->getRoot()); cout << endl; }
            if (whatTree == "double") { treeDouble->printPrefixNotation(treeDouble->getRoot()); cout << endl; }
        }
        else if (command == helpCommand)
        {
            cout << avaliableCommandsMessage << endl; //dziala
        }
        else if (command == quitCommand) {
            cout << exitMessage << endl; //dziala
        }
        else {
            cout << invalidCommandWarning << endl; //dziala
        }
    }
}