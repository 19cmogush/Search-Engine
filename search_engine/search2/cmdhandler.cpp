#include "cmdhandler.h"
#include "util.h"
#include "webpage.h"
#include <algorithm>


struct sort_pred {
    bool operator()(const std::pair<WebPage*, double> &left, const std::pair<WebPage*, double> &right) {
        return left.second > right.second;
    }
};

struct sortAlpha {
    bool operator()(const std::pair<WebPage*, double> &left, const std::pair<WebPage*, double> &right){
        WebPage* tmp1 = left.first;
        WebPage* tmp2 = right.first;
        return tmp2->filename() > tmp1->filename();
    }
};

std::string to_lower(std::string str)
{
    std::string s = str;
    for (size_t i = 0; i<str.size(); i++)
    {
        s[i] = tolower(s[i]);
    }
    return s;
}

QuitHandler::QuitHandler()
{

}

QuitHandler::QuitHandler(Handler* next)
    : Handler(next)
{

}

bool QuitHandler::canHandle(const std::string& cmd) const
{
    return cmd == "QUIT";

}

Handler::HANDLER_STATUS_T QuitHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    return HANDLER_QUIT;
}

PrintHandler::PrintHandler()
{

}

PrintHandler::PrintHandler(Handler* next)
    : Handler(next)
{

}

bool PrintHandler::canHandle(const std::string& cmd) const
{
    return cmd == "PRINT";

}

Handler::HANDLER_STATUS_T PrintHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    std::string name;

    if (!(instr >> name)) {
        return HANDLER_ERROR;
    }
    try {
        eng->display_page(ostr, name);
    }
    catch (std::exception& e) {
        return HANDLER_ERROR;
    }
    return HANDLER_OK;
}

AndHandler::AndHandler()
{}

AndHandler::AndHandler(Handler* next) 
    : Handler(next)
{}

bool AndHandler::canHandle(const std::string& cmd) const
{
    return cmd == "AND";
}

Handler::HANDLER_STATUS_T AndHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    //parses command and calls search function for AND command
    std::vector<std::string> filenames;
    std::string temp;
    while (instr >> temp)
    {
        std::string tmp = to_lower(temp);
        filenames.push_back(tmp);
        temp = "";
    }
    AndWebPageSetCombiner* comb = new AndWebPageSetCombiner();
    WebPageSet hits = eng->search(filenames, comb);
    display_hits(hits, ostr);
    delete comb;
    return Handler::HANDLER_OK;
}

PrandHandler::PrandHandler()
{}

PrandHandler::PrandHandler(Handler* next)
    : Handler(next)
{}

bool PrandHandler::canHandle(const std::string& cmd) const
{
    return cmd == "PRAND";
}

Handler::HANDLER_STATUS_T PrandHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    //parses command and calls search function for AND command
    std::vector<std::string> filenames;
    std::string temp;
    std::string comp;
    instr >> comp;
    while (instr >> temp)
    {
        std::string tmp = to_lower(temp);
        filenames.push_back(tmp);
        temp = "";
    }
    AndWebPageSetCombiner* comb = new AndWebPageSetCombiner();
    WebPageSet hits = eng->search(filenames, comb);
    std::vector<std::pair<WebPage*, double> > ranks = eng->pageRank(hits);
    if (to_lower(comp) == "r")
    {
        std::sort (ranks.begin(), ranks.end(), sort_pred());
    }
    else if (to_lower(comp) == "n")
    {
        std::sort (ranks.begin(), ranks.end(), sortAlpha());
    }
    else
    {
        throw std::invalid_argument("Invalid syntax for PRAND query must use r/n");
    }
    display_ranked_hits(ranks, ostr); 
    delete comb;
    return Handler::HANDLER_OK;
}

ProrHandler::ProrHandler()
{}

ProrHandler::ProrHandler(Handler* next)
    : Handler(next)
{}

bool ProrHandler::canHandle(const std::string& cmd) const
{
    return cmd == "PROR";
}

Handler::HANDLER_STATUS_T ProrHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    //parses command and calls search function for OR command
    std::vector<std::string> filenames;
    std::string temp;
    std::string comp;
    instr >> comp;
    while (instr >> temp)
    {
        std::string tmp = to_lower(temp);
        filenames.push_back(tmp);
        temp = "";
    }
    OrWebPageSetCombiner* comb = new OrWebPageSetCombiner();
    WebPageSet hits = eng->search(filenames, comb);
    std::vector<std::pair<WebPage*, double> > ranks = eng->pageRank(hits);
    if (to_lower(comp) == "r")
    {
        //mergeSort(ranks, sortAlpha());
        std::sort (ranks.begin(), ranks.end(), sort_pred());
    }
    else if (to_lower(comp) == "n")
    {
        //mergeSort(ranks, sortAlpha());
        std::sort (ranks.begin(), ranks.end(), sortAlpha());

    }
    else
    {
        throw std::invalid_argument("Invalid syntax for PROR query must use r/n");
    }
    display_ranked_hits(ranks, ostr); 
    delete comb;
    return Handler::HANDLER_OK;
}

OrHandler::OrHandler()
{}

OrHandler::OrHandler(Handler* next) 
    : Handler(next)
{}

bool OrHandler::canHandle(const std::string& cmd) const
{
    return cmd == "OR";
}

Handler::HANDLER_STATUS_T OrHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    //parses command and calls search function for OR command
    std::vector<std::string> filenames;
    std::string temp;
    while (instr >> temp)
    {
        std::string tmp = to_lower(temp);
        filenames.push_back(tmp);
        temp = "";
    }
    OrWebPageSetCombiner* comb = new OrWebPageSetCombiner();
    WebPageSet hits = eng->search(filenames, comb);
    display_hits(hits, ostr);
    delete comb;
    return Handler::HANDLER_OK;
}

DiffHandler::DiffHandler()
{}

DiffHandler::DiffHandler(Handler* next)
    : Handler(next)
{}

bool DiffHandler::canHandle(const std::string& cmd) const
{
    return cmd == "DIFF";
}

Handler::HANDLER_STATUS_T DiffHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    //parses command and calls search function for DIFF command
    std::vector<std::string> filenames;
    std::string temp;
    while (instr >> temp)
    {
        std::string tmp = to_lower(temp);
        filenames.push_back(tmp);
        temp = "";
    }
    DiffWebPageSetCombiner* comb = new DiffWebPageSetCombiner();
    WebPageSet hits = eng->search(filenames, comb);
    display_hits(hits, ostr);
    delete comb;
    return Handler::HANDLER_OK;
}

IncomingHandler::IncomingHandler()
{}

IncomingHandler::IncomingHandler(Handler* next)
    : Handler(next)
{}

bool IncomingHandler::canHandle(const std::string& cmd) const
{
    return cmd == "INCOMING";
}

Handler::HANDLER_STATUS_T IncomingHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    //handles parsing and tracking of incoming links for each page
    std::string name;
    instr >> name;
    WebPage* page = eng->retrieve_page(name);
    if (page == NULL)
    {
        return Handler::HANDLER_ERROR;
    }
    std::set<WebPage*> links = page->incoming_links();
    ostr << links.size() << std::endl;
    for (std::set<WebPage*>::const_iterator i = links.begin(); i != links.end(); i++)
    {
        WebPage* page1;
        page1 = *i;
        ostr << page1->filename() << std::endl;
    }
    return Handler::HANDLER_OK;
}

OutgoingHandler::OutgoingHandler()
{}

OutgoingHandler::OutgoingHandler(Handler* next)
    : Handler(next)
{}

bool OutgoingHandler::canHandle(const std::string& cmd) const
{
    return cmd == "OUTGOING";
}

Handler::HANDLER_STATUS_T OutgoingHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    //handles parsing and tracking of outgoing links and given commands
    std::string name;
    instr >> name;
    WebPage* page = eng->retrieve_page(name);
    if (page == NULL)
    {
        return Handler::HANDLER_ERROR;
    }
    std::set<WebPage*> links = page->outgoing_links();
    ostr << links.size() << std::endl;
    for (std::set<WebPage*>::const_iterator i = links.begin(); i != links.end(); i++)
    {
        WebPage* page1 = *i;
        ostr << page1->filename() << std::endl;
    }
    return Handler::HANDLER_OK;  
}
