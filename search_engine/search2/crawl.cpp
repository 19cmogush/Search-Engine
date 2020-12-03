#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include "md_parser.h"
#include "txt_parser.h"
#include "util.h"



int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "Must provide an index file and output file" << std::endl;
        return 1;
    }

    // ---------------------- [TO BE COMPLETED] -------------------------------
    std::set<std::string> processed;
    std::map<std::string, PageParser*> parsers;
    PageParser* m = new MDParser();
    PageParser* t = new TXTParser();
    parsers.insert(std::make_pair("md", m));
    parsers.insert(std::make_pair("txt", t));
    std::ifstream ifile(argv[1]);
    std::string page = "";
    std::ofstream ofile(argv[2]);
    while (ifile >> page)
    {
        std::string ext = extract_extension(page);
        if (ext == "md")
        {
            m->crawl(parsers, page, processed, ofile);
        }
        else
        {
            t->crawl(parsers, page, processed, ofile);
        }

    }
    
    // You may add cleanup code here if necessary
    ofile.close();
    ifile.close();
    delete t;
    delete m;


    return 0;
}
