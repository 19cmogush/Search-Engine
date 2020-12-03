#include <iostream>
#include <fstream>
#include "searcheng.h"
#include "searchui.h"
#include "md_parser.h"
#include "txt_parser.h"
#include "cmdhandler.h"



int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "Must provide an index file" << std::endl;
        return 1;
    }

    // Declare the search engine
    SearchEng seng(new TXTParser);

    // ---------------------- [TO BE COMPLETED] -------------------------------
    // Create and register appropriate parsers for .md and .txt files
    PageParser* mdParse = new MDParser();
    PageParser* txtPar = new TXTParser();
    seng.register_parser("txt", txtPar);
    seng.register_parser("md", mdParse);

    // Create the SearchUI object
    SearchUI ui = SearchUI(&seng);

    // Create and add appropriate Command Handlers to the SearchUI
    Handler* quit = new QuitHandler();
    Handler* and1 = new AndHandler(quit);
    Handler* or1 = new OrHandler(and1);
    Handler* diff = new DiffHandler(or1);
    Handler* print = new PrintHandler(diff);
    Handler* incoming = new IncomingHandler(print);
    Handler* outgoing = new OutgoingHandler(incoming);
    Handler* prand = new PrandHandler(outgoing);
    Handler* pror = new ProrHandler(prand);
    ui.add_handler(pror);
    ui.add_handler(prand);
    ui.add_handler(outgoing);
    ui.add_handler(incoming);
    ui.add_handler(print);
    ui.add_handler(diff);
    ui.add_handler(or1);
    ui.add_handler(and1);
    ui.add_handler(quit);


    // ------------------------------------------------------------------------

    try {
        seng.read_pages_from_index(argv[1]);
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    bool result;

        
    if (argc == 2)
    {            
        result = ui.run(std::cin, std::cout);
    }
    else if (argc == 3) 
    {
        std::ifstream in_file(argv[2]);
        result = ui.run(in_file, std::cout);
    }
    else  {
        std::ifstream in_file(argv[2]);
        std::ofstream out_file(argv[3]);
        result = ui.run(in_file, out_file);
    }


    // You may add cleanup code here though you likely do not
    // need to.
    
    return result;
}
