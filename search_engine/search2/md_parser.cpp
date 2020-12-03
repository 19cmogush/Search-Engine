#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cctype>
#include "md_parser.h"
#include "util.h"
#include <stack>



typedef enum { NORMALTEXT, LINKTEXT, ISLINK, LINKURL } PARSE_STATE_T;


void MDParser::parse(std::string filename, std::set<std::string>& allSearchableTerms, std::set<std::string>& allOutgoingLinks)
{
    // Attempts to open the file.
    std::ifstream wfile(filename.c_str());
    if(!wfile) {
        throw std::invalid_argument("Bad webpage filename in MDParser::parse()");
    }

    // Remove any contents of the sets before starting to parse.
    allSearchableTerms.clear();
    allOutgoingLinks.clear();

    // The initial state is parsing a normal term.
    PARSE_STATE_T state = NORMALTEXT;

    // Initialize the current term and link as empty strings.
    std::string term = "";
    std::string link = "";

    // Get the first character from the file.
    char c = wfile.get();

    // Continue reading from the file until input fails.
    while(!wfile.fail())
    {
        // Logic for parsing a normal term.
        if(state == NORMALTEXT)
        {
            // ADD YOUR CODE HERE
            if (isalnum(c))
            {
                term += c;
            }
            else
            {
                allSearchableTerms.insert(term);
                term = "";
            }
            if (c == '[')
            {
                state = LINKTEXT;
            }
        }
        // Logic for parsing a link.
        else if (state == LINKTEXT)
        {
            // ADD YOUR CODE HERE
            if(isalnum(c))
            {
                term += c;
            }
            else
            {
                allSearchableTerms.insert(term);
                term = "";
            }
            if (c == ']')
            {
                state = ISLINK;
            }        
        }
        else if( state == ISLINK )
        {
            // ADD YOUR CODE HERE
            if (isalnum(c))
            {
                term += c;
            }
            else 
            {
                link = "";
            }
            if (c == '(')
            {
                state = LINKURL;
            }
            else
            {
                state = NORMALTEXT;
            }
            
        }
        // Else we are in the LINKURL state.
        else
        {
            // ADD YOUR CODE HERE
            if (c != ')')
            {
                link += c;
            }
            else 
            {
                allOutgoingLinks.insert(link);
                //std::cout << "Filename " << filename << std::endl;
                //std::cout << "link to be inserted: " << link << std::endl;
                state = NORMALTEXT;
            }


        }
        // Attempt to get another character from the file.
        c = wfile.get();
    }
    // ADD ANY REMAINING CODE HERE
    /*
    std::cout << "Searchable terms" << std::endl;
    for (std::set<std::string>::const_iterator cit = allSearchableTerms.begin(); cit != allSearchableTerms.end(); cit++)
    {
        std::cout << *cit << " " << std::endl;
    }

    std::cout << "Outgoing links" << std::endl;
    for (std::set<std::string>::const_iterator cit = allOutgoingLinks.begin(); cit != allOutgoingLinks.end(); cit++)
    {
        std::cout << *cit << " ";
    }
    */



    // Close the file.
    wfile.close();
}

std::string MDParser::display_text(std::string filename)
{
    // Attempts to open the file.
    std::ifstream wfile(filename.c_str());
    if (!wfile) {
        throw std::invalid_argument("Bad webpage filename in TXTParser::parse()");
    }
    std::string retval;

    // The initial state is parsing a normal term.
    PARSE_STATE_T state = NORMALTEXT;

    char c = wfile.get();

    // Continue reading from the file until input fails.
    while (!wfile.fail()) {
        // Logic for parsing a normal term.
        if (state == NORMALTEXT)
        {
            // The moment we hit a bracket, we input our current working term
            // into the allSearchableTerms set, reset the current term, and move into
            // parsing a link.
            if (c == '[')
            {
                state = LINKTEXT;
            }
            retval += c;
        }
        // Logic for parsing a link.
        else if (state == LINKTEXT)
        {
            // When we hit the closing bracket, then we must be finished getting the link.
            if (c == ']')
            {
                state = ISLINK;
            }
            retval += c;
        }
        else if (state == ISLINK) {
            if (c == '(') {
                state = LINKURL;
            }
            else
            {
                state = NORMALTEXT;
                retval += c;
            }
        }
        // Else we are in the LINKURL state.
        else
        {
            // When we hit a closing parenthese then we are done, and the link can be inserted.
            if (c == ')')
            {
                state = NORMALTEXT;
            }
        }
        c = wfile.get();
    }
    return retval;
}


void MDParser::crawl(
        const std::map<std::string, PageParser*>& parsers,
        std::string filename,
        std::set<std::string>& processed,
        std::ostream& ofile)
    {

        if (processed.find(filename) == processed.end())
        {
            std::stack<std::string> myStack;
            myStack.push(filename);
            processed.insert(filename);
            std::vector<std::string> links;
            while (myStack.empty() == false)
            {
                std::string curr = myStack.top();
                
                myStack.pop();
                std::ifstream wfile(curr);
                ofile << curr << std::endl;
                if (wfile.fail())
                {
                    throw std::invalid_argument("Bad webpage filename in MDParser::crawl()");
                }
                std::string term = "";
                std::string link = "";
                PARSE_STATE_T state = NORMALTEXT;
                char c = wfile.get();

                std::string ext = extract_extension(curr);
                /*
                if (ext == "txt")
                {
                    parsers.at("txt")->crawl(parsers, curr, processed, ofile);
                }
                */
                //else
                //{
                    while(!wfile.fail())
                    {
                        // Logic for parsing a normal term.
                        if(state == NORMALTEXT)
                        {
                            // ADD YOUR CODE HERE
                            if (isalnum(c))
                            {
                                term += c;
                            }
                            else
                            {
                                
                                term = "";
                            }
                            if (c == '[')
                            {
                                state = LINKTEXT;
                            }
                        }
                        // Logic for parsing a link.
                        else if (state == LINKTEXT)
                        {
                            // ADD YOUR CODE HERE
                            if(isalnum(c))
                            {
                                term += c;
                            }
                            else
                            {
                                
                                term = "";
                            }
                            if (c == ']')
                            {
                                state = ISLINK;
                            }        
                        }
                        else if( state == ISLINK )
                        {
                            // ADD YOUR CODE HERE
                            if (isalnum(c))
                            {
                                term += c;
                            }
                            else 
                            {
                                link = "";
                            }
                            if (c == '(')
                            {
                                state = LINKURL;
                            }
                            else
                            {
                                state = NORMALTEXT;
                            }
                            
                        }
                        // Else we are in the LINKURL state.
                        else
                        {
                            // ADD YOUR CODE HERE
                            if (c != ')')
                            {
                                link += c;
                            }
                            else 
                            {
                                if (processed.find(link) == processed.end())
                                {
                                    processed.insert(link);
                                    links.push_back(link);
                                    
                                }
                                state = NORMALTEXT;
                            }
                        }
                        // Attempt to get another character from the file.
                        c = wfile.get();
                    }
                if (links.size() > 1)
                {
                    for (int i=links.size()-1; i >= 0; i--)
                    {
                        //std::cout << links[i] << std::endl;
                        myStack.push(links[i]);
                    }
                    
                }
                else if (links.size() == 1)
                {
                    myStack.push(links[0]);
                }
                links.clear();
            //}
            }


    }
}

