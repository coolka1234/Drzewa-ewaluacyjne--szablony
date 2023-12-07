#pragma once
#include <vector>
#include "CNode.h"
#include <string>
#include <map>
#include <iostream>
#include <set>
#include <sstream>
#include <cctype>
#include <iomanip>

template <typename T> class CTree
{
	//domyslna pojemnosc
	int const iDeafultCapacity=2;
	//domsylna wartosc
	int const iDeafultValue=1;
	//korzen drzewa
	CNode<T>* cRoot;
	//wartosc ewaluacji
	float fCalculatedValue;
	//formula drzewa
	std::string sFormula;
public:
	//wyjasnienia funkcji w pliku ./cpp //
	void repairPrefix(std::vector<std::string>& vsFormula);
	void repairPrefixDouble(std::vector<std::string>& vsFormula) const;
	void repairPrefixString(std::vector<std::string>& vsFormula) const;
	static void deletQuotes(std::string& sToEdit);
	static std::vector<std::string> createFormula(const std::string& sFormula);
	static std::map<std::string, double> createVariableMap(const std::vector<std::string>& svFormula, std::vector<double> fvValues);
    static std::map<std::string, std::string> createStringMap(const std::vector<std::string>& svFormula, std::vector<std::string> fvValues);
	CTree<T>();
    CTree<T>(std::string sFormula, int iPosition, std::vector<std::string> vVariable);
	CTree<T>(std::string sFormula, int iPosition, std::vector<double> vVariables);
	void recursiveTree(std::vector<std::string> vsFormula, int iPosition, CNode<T>& cCheckedNode);
	double evaluatePrefixExpression(CNode<T>* root, std::vector<double>& fValues,std::map<std::string, double> mapOfVariables);
    std::string evaluateStringPrefix(CNode<T>* root, std::vector<std::string>& fValues, std::map<std::string, std::string> mapOfVariables);
	std::string retrunFormula();
	std::string returnVariables() const;
	CTree<T> operator+ (CTree<T> const& otherTree);
	CTree<T> operator= (CTree<T> const& otherTree);
	CNode<T>* operatorHelper(CNode<T>* root);
	CNode<T>* getRoot();
	std::string printPrefixNotation(CNode<T>* root);
	static std::string sGetKnownType();
};

const std::string sDefaultOperator = "+";
inline bool isNumber(const std::string& str) {
    const char* c_str = str.c_str();
    char* endptr;

    std::strtol(c_str, &endptr, 10);
    if (*endptr == '\0') return true;

    std::strtof(c_str, &endptr);
    if (*endptr == '\0') return true;

    std::strtod(c_str, &endptr);
    if (*endptr == '\0') return true;

    return false;
}
static bool isValidVariable(const std::string& str) {
    //sprawdzanie czy jest chocby jedna litera i nie jest funckja
    if (str == "sin" || str == "cos" || str == "*" || str == "-" || str == "+" || str == "/") return false;
    bool hasLetter = false;
    for (char ch : str) {
        if (std::isalpha(ch)) {
            hasLetter = true;
            break;
        }
    }
    //sprawdzanie kazdego znaku - czy jest znakiem lub liczba
    bool isValid = !str.empty();
    for (char ch : str) {
        if (!std::isalnum(ch) && !std::isalpha(ch)&&ch!='/') {
            isValid = false;
            break;
        }
    }
    //zwroc jesli ma litere i nie ma smieci
    return isValid && hasLetter;
}
inline int numOfFunArg(const std::string& chOperand)
{
    if (chOperand == "va" || chOperand == "vr")
    {
        //zmienne i stale moga miec 0 argumentow
        return 0;
    }
    else if (chOperand == "cos" || chOperand == "sin")
    {
        //jednoargumentowe funckje
        return 1;
    }
    else
    {
        //dwuargumentowe funkcje
        return 2;
    }
}
inline void removeWhitespaces(std::string& str)
{
    str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
}
inline bool isStringValue(const std::string& sToCheck)
{
    if (sToCheck.empty()||sToCheck.size()<3) return false;
    if (sToCheck[0] == '"' && sToCheck[sToCheck.size() - 1] == '"') return true;
    return false;
}
inline void removeLastOccurrence(std::string& mainString, const std::string& toRemove) {
	const size_t found = mainString.rfind(toRemove);

    if (found != std::string::npos) {
        mainString.erase(found, toRemove.length());
    }
}
inline void insertSubstringAfterChar(std::string& mainString, const std::string& secondString) {
    if (secondString.length() < 2) {
        //std::cerr << "Error: Second string must have at least two characters." << std::endl;
        return;
    }

    const char targetChar = secondString[0];
    const std::string substringToInsert = secondString.substr(1);

    size_t found = mainString.find(targetChar);
    while (found != std::string::npos) {
        mainString.insert(found + 1, substringToInsert);
        found = mainString.find(targetChar, found + 1);
    }
}
static inline void removeSubstringExceptFirst(std::string& mainString, const std::string& secondString) {
    size_t found = mainString.find(secondString);

    while (found != std::string::npos) {
        // Keep the first character of the second string in the main string
        mainString.erase(found + 1, secondString.length() - 1);
        found = mainString.find(secondString, found + 1);
    }
}
inline static std::vector<double> stringToFloatVector(const std::string& input) {
    std::vector<double> result;
    std::istringstream iss(input);
    std::string token;

    while (iss >> token) {
        // Check if each character in the token is a number
        bool isNumber = true;
        for (const char ch : token) {
            if (!std::isdigit(ch) && ch != '.' && ch != '-') {
                isNumber = false;
                break;
            }
        }

        if (isNumber) {
            // Convert the token to a float and add to the vector
            float value;
            std::istringstream tokenStream(token);
            tokenStream >> value;
            result.push_back(value);
        }
        else {
            std::cerr << "Invalid number: " << token << ". Skipping." << std::endl;
        }
    }

    return result;
}
inline static std::vector<std::string> stringToStringVector(const std::string& input) {
    std::vector<std::string> tokens;
    std::istringstream iss(input);
    std::string token;

    while (iss >> std::quoted(token)) {
        tokens.push_back(token);
    }

    return tokens;
}
inline std::string determineType(const std::string& sToCheck)
{
    if (sToCheck.find('.') != -1) return "double";
    else if (sToCheck.find('"') != -1) return "string";
    else return "int";
}
inline bool isOperator(const std::string& sCharacter)
{
    //zasadniczo switch ale c++ nie pozwala na switcha z stringiem
    if (sCharacter == "+") return true;
    if (sCharacter == "-") return true;
    if (sCharacter == "*") return true;
    if (sCharacter == "/") return true;
    if (sCharacter == "sin") return true;
    if (sCharacter == "cos") return true;
    return false;
}
//funckja sprawdzajaca czy formula jest prefixowa
inline bool isPrefixFormula(const std::vector<std::string>& formula)
{
    if (formula.empty()) {
        return false;
        //pusta nie jest
    }

    int operatorCount = 0;
    int operandCount = 0;

    for (const std::string& ch : formula) {
        if (ch == " ") {
            continue; //spacje sa bez znaczenia
        }
        else if (!isOperator(ch)) {
            operandCount++;
        }
        else {
            //patrzymy ile kazdy operator "zjada" argumentow
            operatorCount += numOfFunArg(ch) - 1;
        }

        if (operandCount > operatorCount + 1) {
            // wiecej operatorow niz zmiennych
            return false;
        }
    }

    return (operatorCount == operandCount - 1);
}
template<typename T>
std::string CTree<T>::sGetKnownType()
{
	return "";
}
template <> inline
std::string CTree<int>::sGetKnownType()
{
	return "int";
}


template<typename T>
inline CNode<T>* CTree<T>::getRoot()
{
    return cRoot;
}

template <typename T>
std::string CTree<T>::printPrefixNotation(CNode<T>* root)
{
    std::string result = "";
    if (root != NULL) {
        // wypisz dane
        result = root->chOperand + " ";
        std::cout << root->chOperand + " ";

        // rekurencyjnie idz glebiej
        for (CNode<T>* child : root->cChildren) {
            printPrefixNotation(child);
        }
    }
    return result;
}

template<> inline
std::string CTree<double>::sGetKnownType()
{
	return "double";
}
template<> inline
std::string CTree<std::string>::sGetKnownType()
{
	return "string";
}
template <typename T>
void CTree<T>::repairPrefixDouble(std::vector<std::string>& vsFormula) const
{
    int iOperatorCount = 0, iOperandCount = 0;
    for (int i = vsFormula.size() - 1; i >= 0; i--)
    {
        if (isNumber(vsFormula[i]) || isValidVariable(vsFormula[i]))
        {
            iOperandCount++; //licznik zmiennych stalych
        }
        else if (isOperator(vsFormula[i]))
        {
            iOperatorCount += numOfFunArg(vsFormula[i]) - 1; //licznik ilosci argumentow neutralizowanych przez operatory
        }
    }
    if (iOperatorCount + 1 == iOperandCount)
    {
        return; //jesli sie zgadza (+ 1 to wynik) to nic nie trzeba zmieniac
    }
    else if (iOperandCount > iOperatorCount + 1)
    {
        for (int i = iOperandCount - (iOperatorCount + 1); i > 0; i--)
        {
            //jesli jest wiecej zmiennych karmimy formule +, zjadajac jeden argument, az sie wynormuje
            vsFormula.insert(vsFormula.begin(), "+");
            //informacja dla uzytkownika
            std::cout << "Nieprawidlowa formula. " << std::endl;
        }
    }
    else if (iOperandCount < iOperatorCount + 1)
    {
        for (int i = iOperandCount - (iOperatorCount + 1); i > 0; i--)
        {
            //jesli jest wiecej operatorow karmimy formule 1, zjadajac jeden operator, az sie wynormuje
            vsFormula.insert(vsFormula.begin() + vsFormula.size() - 1, std::to_string(iDeafultValue));
            //informacja dla uzytkownika
            std::cout << "Nieprawidlowa formula. " << std::endl;
        }
    }
    //informacja dla uztkownika
    std::cout << "Formula ktora bedzie teraz przerabiana: ";
    for (std::string sS : vsFormula)
    {
        //wypisz naprawiona formule
        std::cout << sS << " ";
    }
    std::cout << std::endl;
}
template <typename T>
void CTree<T>::repairPrefixString(std::vector<std::string>& vsFormula) const
{
    int iOperatorCount = 0, iOperandCount = 0;
    for (int i = vsFormula.size() - 1; i >= 0; i--)
    {
        if (!isOperator(vsFormula[i]))
        {
            iOperandCount++; //licznik zmiennych stalych
        }
        else if (isOperator(vsFormula[i]))
        {
            if(vsFormula[i]=="cos"||vsFormula[i]=="sin")
            {
                vsFormula.erase(vsFormula.begin()+i);
                std::cout << "Usunieto nieprawidlowe elementy " << std::endl;
                continue;
            }
            iOperatorCount += numOfFunArg(vsFormula[i]) - 1; //licznik ilosci argumentow neutralizowanych przez operatory
        }
    }
    if (iOperatorCount + 1 == iOperandCount)
    {
        return; //jesli sie zgadza (+ 1 to wynik) to nic nie trzeba zmieniac
    }
    else if (iOperandCount > iOperatorCount + 1)
    {
        for (int i = iOperandCount - (iOperatorCount + 1); i > 0; i--)
        {
            //jesli jest wiecej zmiennych karmimy formule +, zjadajac jeden argument, az sie wynormuje
            vsFormula.insert(vsFormula.begin(), "+");
            //informacja dla uzytkownika
            //std::cout << "Nieprawidlowa formula. " << std::endl;
        }
    }
    else if (iOperandCount < iOperatorCount + 1)
    {
        for (int i = iOperatorCount - (iOperandCount - 1); i > 0; i--)
        {
            //jesli jest wiecej operatorow karmimy formule 1, zjadajac jeden operator, az sie wynormuje
            vsFormula.insert(vsFormula.begin()+vsFormula.size()-1, "a");
            //informacja dla uzytkownika
            //std::cout << "Nieprawidlowa formula. " << std::endl;
        }
    }
    //informacja dla uztkownika
    std::cout << "Formula ktora bedzie teraz przerabiana: ";
    for (const std::string& sS : vsFormula)
    {
        //wypisz naprawiona formule
        std::cout << sS << " ";
    }
    std::cout << std::endl;
}

template <typename T>
void CTree<T>::deletQuotes(std::string& sToEdit)
{
    if (sToEdit.size() < 3) return;
    else
    {
		for(int i=0;i<sToEdit.size();i++)
		{
			if(sToEdit[i]=='"')
			{
                sToEdit.erase(sToEdit.begin() + i);
                i--;
			}
		}
    }
}

template<typename T>
std::vector<std::string> CTree<T>::createFormula(const std::string& sFormula)
{
    //kazde wyrazenie odzielone przynajmniej jedn¹ spacj¹
    std::vector<std::string> formulaElements;
    std::string wordToAdd = "";
    bool bFirstPut = false; //nie chcemy wczytac odrazu, pewnego rodzaju flaga
    for (int i = 0; i < sFormula.size(); i++)
    {
        if ((sFormula[i] == ' ') && bFirstPut)
        {
            //usun spacje
            removeWhitespaces(wordToAdd);
            //dodaj do vectoru
            formulaElements.push_back(wordToAdd);
            //zresteuj zmienne
            wordToAdd = "";
            bFirstPut = false;
        }
        else if (sFormula[i] == ' ')
        {
            //dodatkowe spacje pomijamy
            continue;
        }
        else if ((i == sFormula.size() - 1) && !std::isspace(sFormula[i]))
        {
            //ostatnie slowo moze nie miec na koncu spacji
            removeWhitespaces(wordToAdd);
            wordToAdd += sFormula[i];
            formulaElements.push_back(wordToAdd);
        }
        //tworzenie slowa do dodania
        wordToAdd += sFormula[i];
        bFirstPut = true;
    }
    //zwroc vectpr
    return formulaElements;
}

template<typename T>
std::map<std::string, double> CTree<T>::createVariableMap(const std::vector<std::string>& svFormula, std::vector<double> fvValues)
{
    std::map<std::string, double> mapOfVariables;
    int iWhatVariable = 0;
    for (std::string s : svFormula)
    {
        //jezeli istnieje zmienna i juz znajduje sie w mapie
        if (isValidVariable(s) && mapOfVariables.find(s) == mapOfVariables.end())
        {
            //powiaz s z ta wartoscia
            mapOfVariables.insert(std::make_pair(s, fvValues[iWhatVariable]));
            iWhatVariable++;
        }
    }
    //zwroc mape
    return mapOfVariables;
}

template <typename T>
std::map<std::string, std::string> CTree<T>::createStringMap(const std::vector<std::string>& svFormula,
	std::vector<std::string> fvValues)
{
    std::map<std::string, std::string> mapOfVariables;
    int iWhatVariable = 0;
    for (std::string s : svFormula)
    {
        //jezeli istnieje zmienna i juz znajduje sie w mapie
        if (isValidVariable(s) && mapOfVariables.find(s) == mapOfVariables.end())
        {
            //powiaz s z ta wartoscia
            if (iWhatVariable < fvValues.size())
            {
                mapOfVariables.insert(std::make_pair(s, fvValues[iWhatVariable]));
                iWhatVariable++;
            }
        }
    }
    //zwroc mape
    return mapOfVariables;
}

template <typename T>
void CTree<T>::repairPrefix(std::vector<std::string>& vsFormula)
{
	if (sGetKnownType() == "int")
	{
        int iOperatorCount = 0, iOperandCount = 0;
        for (int i = vsFormula.size() - 1; i >= 0; i--)
        {
            if (isNumber(vsFormula[i]) || isValidVariable(vsFormula[i]))
            {
                iOperandCount++; //licznik zmiennych stalych
            }
            else if (isOperator(vsFormula[i]))
            {
                iOperatorCount += numOfFunArg(vsFormula[i]) - 1; //licznik ilosci argumentow neutralizowanych przez operatory
            }
        }
        if (iOperatorCount + 1 == iOperandCount)
        {
            return; //jesli sie zgadza (+ 1 to wynik) to nic nie trzeba zmieniac
        }
        else if (iOperandCount > iOperatorCount + 1)
        {
            for (int i = iOperandCount - (iOperatorCount + 1); i > 0; i--)
            {
                //jesli jest wiecej zmiennych karmimy formule +, zjadajac jeden argument, az sie wynormuje
                vsFormula.insert(vsFormula.begin(), "+");
                //informacja dla uzytkownika
                std::cout << "Nieprawidlowa formula. " << std::endl;
            }
        }
        else if (iOperandCount < iOperatorCount + 1)
        {
            for (int i = iOperatorCount+1 - (iOperandCount); i > 0; i--)
            {
                //jesli jest wiecej operatorow karmimy formule 1, zjadajac jeden operator, az sie wynormuje
                vsFormula.insert(vsFormula.begin() + vsFormula.size() - 1, std::to_string(iDeafultValue));
                //informacja dla uzytkownika
                std::cout << "Nieprawidlowa formula. " << std::endl;
            }
        }
        //informacja dla uztkownika
        std::cout << "Formula ktora bedzie teraz przerabiana: ";
        for (std::string sS : vsFormula)
        {
            //wypisz naprawiona formule
            std::cout << sS << " ";
        }
        std::cout << std::endl;
	}
	else if (sGetKnownType() == "double") repairPrefixDouble(vsFormula);
	else if (sGetKnownType() == "string") repairPrefixString(vsFormula);
}

template<typename T>
CTree<T>::CTree()
{
    cRoot = NULL;
    sFormula = "";
    fCalculatedValue = 0;
}

template<>
inline CTree<int>::CTree(std::string sFormula, int iPosition, std::vector<double> vVariables)
{
    //domyslne wartosci dla zmiennych
    cRoot = new CNode<int>();
    cRoot->chOperand = "";
    CNode<int>* cChild = new CNode<int>();
    cChild->parent = cRoot;
    this->sFormula = sFormula;
    cRoot = cChild;
    //tworzymy vector z formuly stringowej
    std::vector<std::string> vsFormula = createFormula(sFormula);
    //sprawdzanie poprawnosci formul
    if (!isPrefixFormula(vsFormula)) {
        std::cout << "Nieprawidlowa formula" << std::endl;
        repairPrefix(vsFormula);
    }
    //tworzenie z vector drzewa
    recursiveTree(vsFormula, iPosition, *cChild);
}
template<>
inline CTree<std::string>::CTree(std::string sFormula, int iPosition, std::vector<std::string> vVariables)
{
    //domyslne wartosci dla zmiennych
    cRoot = new CNode<std::string>();
    cRoot->chOperand = "";
    CNode<std::string>* cChild = new CNode<std::string>();
    cChild->parent = cRoot;
    this->sFormula = sFormula;
    cRoot = cChild;
    //tworzymy vector z formuly stringowej
    std::vector<std::string> vsFormula = createFormula(sFormula);
    //sprawdzanie poprawnosci formul
    if (!isPrefixFormula(vsFormula)) 
    {
        std::cout << "Nieprawidlowa formula" << std::endl;
        repairPrefixString(vsFormula);
    }
    //tworzenie z vector drzewa
    recursiveTree(vsFormula, iPosition, *cChild);
}
template<>
inline CTree<double>::CTree(std::string sFormula, int iPosition, std::vector<double> vVariables)
{
    //domyslne wartosci dla zmiennych
    cRoot = new CNode<double>();
    cRoot->chOperand = "";
    CNode<double>* cChild = new CNode<double>();
    cChild->parent = cRoot;
    this->sFormula = sFormula;
    cRoot = cChild;
    //tworzymy vector z formuly stringowej
    std::vector<std::string> vsFormula = createFormula(sFormula);
    //sprawdzanie poprawnosci formul
    if (!isPrefixFormula(vsFormula))
    {
        std::cout << "Nieprawidlowa formula" << std::endl;
        repairPrefixDouble(vsFormula);
    }
    //tworzenie z vector drzewa
    recursiveTree(vsFormula, iPosition, *cChild);
}
template<typename T>
inline CTree<T>::CTree(std::string sFormula, int iPosition, std::vector<double> vVariables)
{

}
template <typename T>
void CTree<T>::recursiveTree(std::vector<std::string> vsFormula, int iPosition, CNode<T>& cCheckedNode)
{
    if (iPosition == vsFormula.size())
    {
        return; //skonczylismy parsowanie stringu- zakoncz prace
    }
    //w formule jest operator a lisc jest pusty
    if (isOperator(vsFormula[iPosition]) && cCheckedNode.chOperand.empty())
    {
        //nadajemy liscowi tozsamosc
        cCheckedNode.chOperand = vsFormula[iPosition];
        cCheckedNode.iNumOfMaxChildren = numOfFunArg(vsFormula[iPosition]);
        CNode<T>* cChildren = new CNode<T>();
        //tworzymy jego dziecko - puste
        cCheckedNode.addChildren(cChildren);
        //idziemy glebiej
        recursiveTree(vsFormula, iPosition + 1, *cChildren);
    } //w formule jest zmienna/stala i lisc jest pusty
    else if (!isOperator(vsFormula[iPosition]) && cCheckedNode.chOperand.empty())
    {
        //nadanie tozsamosci
        cCheckedNode.chOperand = vsFormula[iPosition];
        cCheckedNode.iNumOfMaxChildren = numOfFunArg(vsFormula[iPosition]);
        //wracamy sie do rodzica- zmienne nie maja dzieci
        recursiveTree(vsFormula, iPosition + 1, *cCheckedNode.parent);
    } //w formule jest zmienna/stala, lisc nie jest pusty i ma miejsce na dziecko
    else if (!isOperator(vsFormula[iPosition]) && cCheckedNode.chOperand != "" && cCheckedNode.iNumOfChildren < cCheckedNode.iNumOfMaxChildren)
    {
        //tworzymy dla liscia nowe dziecko
        CNode<T>* cChildren = new CNode<T>(vsFormula[iPosition], 0, "", &cCheckedNode);
        cCheckedNode.addChildren(cChildren);
        //wywolujemy jeszcze raz, moze bedzie mial dalej miejsce na dziecko
        recursiveTree(vsFormula, iPosition + 1, cCheckedNode);
    }
    else if (isOperator(vsFormula[iPosition]) && !cCheckedNode.chOperand.empty() && cCheckedNode.iNumOfChildren < cCheckedNode.iNumOfMaxChildren)
    {
        //w formule jest operator, a badany lisc jest operatorem
        //tworzymy dziecko
        CNode<T>* cChildren = new CNode<T>(vsFormula[iPosition], 0, "", &cCheckedNode);
        cChildren->iNumOfMaxChildren = numOfFunArg(vsFormula[iPosition]);
        cCheckedNode.addChildren(cChildren);
        //idziemy glebiej
        recursiveTree(vsFormula, iPosition + 1, *cChildren);
    }
    else
    {
        //w reszcie przypadkow wracamy sie
        recursiveTree(vsFormula, iPosition, *cCheckedNode.parent);
    }
}

template <typename T>
double CTree<T>::evaluatePrefixExpression(CNode<T>* root, std::vector<double>& fValues,
                                          std::map<std::string, double> mapOfVariables)
{
    if (root == NULL) { //doszlismy do korzenia, wracamy z 0
        return 0;
    }
    if (isNumber(root->chOperand)) { //jesli lisc ma liczbe, to zapisujemy ja po prostu

        fValues.push_back(std::stod(root->chOperand.c_str()));
    }
    else if (isValidVariable(root->chOperand)) //jest zmienna- sprawdzamy w mapie
    {
        fValues.push_back(mapOfVariables[root->chOperand]); //uzwyamy mapy by zwrocic warosc zmiennej
    }
    for (CNode<T>* child : root->cChildren)
    {
        evaluatePrefixExpression(child, fValues, mapOfVariables); //teraz robimy to rekursyjnie
    }
    float fV1, fV2;
    std::string sToCheck = root->chOperand; //zajmujemy sie operatorami
    //Kazdy z operatorow dziala podobnie. Pobiera wartosc rowna jego liczby argumentow
    //a nastepnie oddaje jej wynik, takze niszzcac wartosci ktore pobral.
    if (sToCheck == "+") {
        fV1 = fValues[fValues.size() - 1]; fV2 = fValues[fValues.size() - 2]; //pobierz wartosci
        for (int i = 0; i < numOfFunArg(root->chOperand); i++)
        {
            fValues.pop_back(); //niszcz wartosc
        }
        fValues.push_back(fV1 + fV2); //zwroci wynik - nie ma znaczenia kolejnosc
    }
    else if (sToCheck == "-") {
        fV1 = fValues[fValues.size() - 1]; fV2 = fValues[fValues.size() - 2]; //analogicznie jak +
        for (int i = 0; i < numOfFunArg(root->chOperand); i++)
        {
            fValues.pop_back();
        }
        fValues.push_back(fV2 - fV1); //kolejnosc ma znaczenie
    }
    else if (sToCheck == "*") {
        fV1 = fValues[fValues.size() - 1]; fV2 = fValues[fValues.size() - 2]; //analogicznie jak +, nie ma znaczenia kolejnosc
        for (int i = 0; i < numOfFunArg(root->chOperand); i++)
        {
            fValues.pop_back();
        }
        fValues.push_back(fV1 * fV2); //nie ma znaczenia kolejnosc
    }
    else if (sToCheck == "/") {
        fV1 = fValues[fValues.size() - 1]; fV2 = fValues[fValues.size() - 2];
        for (int i = 0; i < numOfFunArg(root->chOperand); i++)
        {
            fValues.pop_back();
        }
        fValues.push_back(fV2 / fV1); //nie ma znaczenia kolejnosc
    }
    else if (sToCheck == "sin") //analogicznie, ale 1 argument
    {
        fV1 = fValues[fValues.size() - 1];
        for (int i = 0; i < numOfFunArg(root->chOperand); i++)
        {
            fValues.pop_back();
        }
        fValues.push_back(std::sin(fV1)); //zwroc wartosc
    }
    else if (sToCheck == "cos") //jak sinus
    {
        fV1 = fValues[fValues.size() - 1];
        for (int i = 0; i < numOfFunArg(root->chOperand); i++)
        {
            fValues.pop_back();
        }
        fValues.push_back(std::cos(fV1));
    }
    return fValues[0];
}

template<typename T>
inline std::string CTree<T>::evaluateStringPrefix(CNode<T>* root, std::vector<std::string>& fValues, std::map<std::string, std::string> mapOfVariables)
{
    if (root == NULL) { //doszlismy do korzenia, wracamy z 0
        return 0;
    }
    if (isStringValue(root->chOperand)) { //jesli lisc ma liczbe, to zapisujemy ja po prostu

        fValues.push_back((root->chOperand));
    }
    else if (isValidVariable(root->chOperand)) //jest zmienna- sprawdzamy w mapie
    {
        fValues.push_back(mapOfVariables[root->chOperand]); //uzwyamy mapy by zwrocic warosc zmiennej
    }
    for (CNode<T>* child : root->cChildren)
    {
        evaluateStringPrefix(child, fValues,mapOfVariables); //teraz robimy to rekursyjnie
    }
    std::string fV1, fV2;
    const std::string sToCheck = root->chOperand; //zajmujemy sie operatorami
    //Kazdy z operatorow dziala podobnie. Pobiera wartosc rowna jego liczby argumentow
    //a nastepnie oddaje jej wynik, takze niszzcac wartosci ktore pobral.
    if (sToCheck == "+") {
        fV1 = fValues[fValues.size() - 1]; fV2 = fValues[fValues.size() - 2]; //pobierz wartosci
        for (int i = 0; i < numOfFunArg(root->chOperand); i++)
        {
            fValues.pop_back(); //niszcz wartosc
        }
        fValues.push_back(fV2 + fV1); //zwroci wynik - nie ma znaczenia kolejnosc
    }
    else if (sToCheck == "-") {
        fV1 = fValues[fValues.size() - 1]; fV2 = fValues[fValues.size() - 2]; //analogicznie jak +
        for (int i = 0; i < numOfFunArg(root->chOperand); i++)
        {
            fValues.pop_back();
        }
        removeLastOccurrence(fV2, fV2);
        fValues.push_back(fV2); //kolejnosc ma znaczenie
    }
    else if (sToCheck == "*") {
        fV1 = fValues[fValues.size() - 1]; fV2 = fValues[fValues.size() - 2]; //analogicznie jak +, nie ma znaczenia kolejnosc
        for (int i = 0; i < numOfFunArg(root->chOperand); i++)
        {
            fValues.pop_back();
        }
        insertSubstringAfterChar(fV2, fV1);
        fValues.push_back(fV2); //nie ma znaczenia kolejnosc
    }
    else if (sToCheck == "/") {
        fV1 = fValues[fValues.size() - 1]; fV2 = fValues[fValues.size() - 2];
        for (int i = 0; i < numOfFunArg(root->chOperand); i++)
        {
            fValues.pop_back();
        }
        removeSubstringExceptFirst(fV2,fV1);
        fValues.push_back(fV2); //nie ma znaczenia kolejnosc
    }
    return fValues[0];
}

template <typename T>
std::string CTree<T>::retrunFormula()
{
    return sFormula;
}


template <typename T>
std::string CTree<T>::returnVariables() const
{
    std::string result = "";
    const std::vector<std::string> svFormula = createFormula(this->sFormula);
    std::set<std::string> setVariables;
    for (std::string sS : svFormula) //dla wszystkich stringow w wektorze
    {
        if (isValidVariable(sS)) //jesli jest zmienna
        {
            setVariables.insert(sS); //dodaj do stringa
        }
    }
    for (std::string sS : setVariables)
    {
        std::cout << sS << " "; //wypisz wszystkie zmienne
        result += sS + " ";
    }
    std::cout << std::endl;
    return result; //zwroc string
}

template <typename T>
CTree<T> CTree<T>::operator+(CTree<T> const& otherTree)
{
    return CTree<T>();
}

template <typename T>
CTree<T> CTree<T>::operator=(CTree<T> const& otherTree)
{
    return CTree<T>();
}

template<typename T>
inline CNode<T>* CTree<T>::operatorHelper(CNode<T>* root)
{
    return nullptr;
}
