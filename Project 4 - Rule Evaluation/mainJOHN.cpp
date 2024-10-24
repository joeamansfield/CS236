#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"
#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
//comment out the while loop when submitting
//set the "i < 15" to however many test cases there are
for (unsigned int i = 0; i < 15; i++) {
if (argc < 2) {
return 1;
}
std::ifstream inFile("input" + std::to_string(i) + ".txt");
std::ifstream answerFile("answer" + std::to_string(i) + ".txt");
//std::ifstream inFile(argv[1]);
//std::ofstream outFile("output" + std::to_string(i) + ".txt");

if (!inFile) {
std::cerr << "Unable to open " << argv[1];
return 2;
}
/*std::ofstream outFile(argv[2]);
if (!outFile) {
inFile.close();
std::cerr << "Unable to open " << argv[2];
return 3;
}*/

//takes the entire input file and inserts it into a string
std::string answerInputBlock = "";
while (!answerFile.eof()) {
std::string inputLine;
getline(answerFile, inputLine);
answerInputBlock.append(inputLine + "\n");
}
answerInputBlock.pop_back();
std::string inputBlock = "";
while (!inFile.eof()) {
std::string inputLine;
getline(inFile, inputLine);
inputBlock.append(inputLine + "\n");
}
inputBlock.pop_back();
Lexer lexing;
std::vector<Token*> tokens = lexing.Lex(inputBlock);
Parser parsing(tokens);
tokens = parsing.clearComments();
DatalogProgram datalog;
std::string myAnswer = "";
if (parsing.isDatalogProgram()) {
datalog = parsing.parse();
Interpreter interpreter(&datalog);
interpreter.interpret();
//checks your output against the answer
myAnswer = interpreter.toaster();
//set this to the test case you want to see (for debugging)
//if (i == 2) std::cout << myAnswer;
if (myAnswer == answerInputBlock) {
std::cout << "True\n";
}
else {
std::cout << "False\n";
}
}
else {
std::cout << "False";
}
}
return 0;
}