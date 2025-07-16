#include <fstream>
#include <iostream>

#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"

using namespace std;

int main(int argc, char *argv[])
{
  ifstream sourceCodeFile;
  LexicalAnalyzer *la = nullptr;
  SyntaxAnalyzer *parser = nullptr;

  if (argc < 2)
  {
    cout << "Usage: " << argv[0] << " <source_file>" << endl;
    cout << "Example: " << argv[0] << " test1.c" << endl;
    return 1;
  }

  sourceCodeFile.open(argv[1], ifstream::in);
  if (!sourceCodeFile.is_open())
  {
    cout << "ERROR - cannot open input file: " << argv[1] << endl;
    cout << "Please check if the file exists and is readable." << endl;
    return 1;
  }

  try
  {
    la = new LexicalAnalyzer(&sourceCodeFile);
    parser = new SyntaxAnalyzer(la);
    
    cout << "Parsing file: " << argv[1] << endl;
    cout << "Tokens: ";
    
    parser->Program();
    cout << endl << "Parsing completed successfully!" << endl;
  }
  catch (const exception& e)
  {
    cout << endl << "Parsing failed with exception: " << e.what() << endl;
    delete la;
    delete parser;
    sourceCodeFile.close();
    return 1;
  }
  catch (...)
  {
    cout << endl << "Parsing failed with unknown error!" << endl;
    delete la;
    delete parser;
    sourceCodeFile.close();
    return 1;
  }
  
  delete la;
  delete parser;
  sourceCodeFile.close();
  return 0;
}
