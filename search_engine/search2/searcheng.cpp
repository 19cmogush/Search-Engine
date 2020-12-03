#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include "searcheng.h"
#include "util.h"





std::string toLower(std::string str)
{
    //helper function
    //changes each char to lowercase
    std::string s = str;
    for (size_t i = 0; i<str.size(); i++)
    {
        s[i] = tolower(s[i]);
    }
    return s;
}

void SearchEng::print_search_terms()
{
    //helper function for debuggin purposes
    for (std::map<std::string, WebPageSet>::iterator i = searchTerms.begin(); i!= searchTerms.end(); i++)
    {
        std::cout << "term: " << i->first << std::endl;
        WebPageSet set = i->second;
        for (WebPageSet::iterator j = set.begin(); j != set.end(); j++)
        {
            WebPage* page = *j;
            std::cout << page->filename() << std::endl;
        } 
    }
}

void SearchEng::register_parser(const std::string& extension, PageParser* parser)
{
    parsers.insert(make_pair(extension, parser));
}

WebPage* SearchEng::retrieve_page(const std::string& page_name) const
{
    if (allPages.find(page_name) == allPages.end())
    {
        return NULL;
    }
    WebPage* myPage = allPages.at(page_name);
    return myPage;

}

void SearchEng::display_page(std::ostream& ostr, const std::string& page_name) const
{
    if (allPages.find(page_name) == allPages.end())
    {
        throw std::invalid_argument("Page doesn't exist");
    }
    std::string extension = extract_extension(page_name);
    PageParser* myParse = parsers.at(extension);
    std::string result = myParse->display_text(page_name);
    ostr << result;
}

void SearchEng::read_page(const std::string& filename)
{
    //get extension and store it 
    std::string extension = extract_extension(filename);
    WebPage* myPage;
    if (allPages.find(filename) == allPages.end())
    {
        myPage = new WebPage(filename);
        allPages.insert(make_pair(filename, myPage));
    }
    else 
    {
       myPage = allPages[filename];
    }

    //deal with storing search terms
    std::set<std::string> terms;
    std::set<std::string> links;
    PageParser* myParser = parsers[extension];
    myParser->parse(filename, terms, links);

    //std:: cout << "Filename: " << filename << std::endl;
    //std::cout << "Outgoing Links: " << std::endl;
    //for (std::set<std::string>::const_iterator i = links.begin(); i != links.end(); ++i)
    //{
       // std::cout << *i << std::endl;
    //}

    myPage->all_terms(terms);
    for (std::set<std::string>::const_iterator i = terms.begin(); i != terms.end(); ++i)
    {
        std::string str = toLower(*i);
        if (searchTerms.find(str) != searchTerms.end())
        {
            WebPageSet mySet = searchTerms.at(str);
            mySet.insert(myPage);
            searchTerms[str] = mySet;
        }
        else
        {
            WebPageSet mySet;
            mySet.insert(myPage);
            searchTerms.insert(make_pair(str, mySet));
        }   
    }

    //store incoming and outgoing links
    for (std::set<std::string>::const_iterator i = links.begin(); i != links.end(); ++i)
    {
        if (allPages.find(*i) != allPages.end())
        {
            WebPage* tmp = allPages[*i];
            myPage->add_outgoing_link(tmp);
            tmp->add_incoming_link(myPage);
            allPages[*i] = tmp;
            WebPageSet set = tmp->incoming_links();
        }

        else
        {
            WebPage* tmp = new WebPage(*i);
            myPage->add_outgoing_link(tmp);
            tmp->add_incoming_link(myPage);
            allPages.insert(make_pair(*i, tmp));
        }
        
    }


}   


WebPageSet SearchEng::search(const std::vector<std::string>& terms, WebPageSetCombiner* combiner) const
{
    WebPageSet mySet;
    //search for first term
    if (searchTerms.find(terms[0]) != searchTerms.end())
    {
        mySet = searchTerms.at(terms[0]);
    }
    //for each next term combine based off certain strategy
    if (terms.size() > 1)
    {
        for (size_t i = 1; i<terms.size(); i++)
        {
            if (searchTerms.find(terms[i]) != searchTerms.end())
            {
                WebPageSet tmp = searchTerms.at(terms[i]);
                mySet = combiner->combine(mySet, tmp);
            }
        }
    }
    return mySet;
}

SearchEng::SearchEng(PageParser* noExtensionParser)
{
    if (NULL == noExtensionParser)
    {
        throw std::invalid_argument("default parser cannot be NULL");
    }
    this->noExtensionParser_ = noExtensionParser;

    // Add additional code if necessary


}


void SearchEng::read_pages_from_index(const std::string& index_file)
{
    std::ifstream ifile(index_file.c_str());
    if(ifile.fail()) {
        std::cerr << "Unable to open index file: " << index_file << std::endl;
    }

    // Parse all the files
    std::string filename;
    while(ifile >> filename) {
#ifdef DEBUG
        cout << "Reading " << filename << endl;
#endif
        read_page(filename);
    }
    ifile.close();
}

std::vector<std::pair<WebPage*, double> > SearchEng::pageRank(const WebPageSet& in_pages)
{
    WebPageSet total_set;
    for(WebPageSet::const_iterator cit = in_pages.begin(); cit != in_pages.end(); cit++)
    {
        //get candidate set to run the page rank algorithm on
        WebPage* tmp = *cit;
        WebPageSet incoming = tmp->incoming_links();
        WebPageSet outgoing = tmp->outgoing_links();
        total_set.insert(*cit);
        for(WebPageSet::const_iterator it = incoming.begin(); it != incoming.end(); it++)
        {
            if (total_set.find(*it) == total_set.end())
            {
                total_set.insert(*it);
            }
        }
        for(WebPageSet::const_iterator i = outgoing.begin(); i != outgoing.end(); i++)
        {
            if (total_set.find(*i) == total_set.end())
            {
                total_set.insert(*i);
            }
        }
    }
    /*
    for(WebPageSet::const_iterator cit = total_set.begin(); cit != total_set.end(); cit++)
    {
        WebPage* tmp = *cit;
        std::cout << tmp->filename() << std::endl;
    }
    */

    int setSize = total_set.size();
    double* lastPR = new double[setSize];
    double* newPR = new double[setSize];
    std::vector<std::string> filenames;
    std::map<std::string, std::vector<std::string> > outgoing;
    for(WebPageSet::const_iterator i = total_set.begin(); i != total_set.end(); i++)
    {
        WebPage* tmp = *i;
        filenames.push_back(tmp->filename());
        WebPageSet out = tmp->outgoing_links();
        std::vector<std::string> myvec;
        myvec.push_back(tmp->filename());
        for(WebPageSet::const_iterator it = out.begin(); it != out.end(); it++)
        {
            WebPage* tmp2 = *it;
            if (total_set.find(*it) != total_set.end())
            {
                myvec.push_back(tmp2->filename());   
            } 
        }
        outgoing.insert(make_pair(tmp->filename(), myvec));
    }
    for(int i=0; i<setSize; i++)
    {
       lastPR[i] = 1.0/setSize;
    }
    double** matrix = new double*[setSize];
    for(int i=0; i<setSize; i++)
    {
       matrix[i] = new double[setSize];
    }

    //filling the matrix
    for(int i=0; i<setSize; i++)
    {
        for(int z=0; z<setSize; z++)
        {
            std::vector<std::string> tmp = outgoing[filenames[z]];

            bool link_exists = false;
            for(size_t k=0; k<tmp.size(); k++)
            {
                if (tmp[k] == filenames[i])
                {
                    link_exists = true;
                    matrix[i][z] = 1.0/tmp.size();
                    break;
                }
            }
            if (link_exists == false)
            {
                matrix[i][z] = 0.0;
            }
            
        }
    }

    //calculating pagerank
    for(int i=0; i<20; i++)
    {
        for (int j=0; j<setSize; j++)
        {
            double total = 0;
            for (int k=0; k<setSize; k++)
            {
                double tmp = matrix[j][k]*lastPR[k];
                total += tmp;
                
            }
            total = (total*.85) + (.15/setSize);
            newPR[j] = total;
        }
        for(int z=0; z<setSize; z++)
        {
            lastPR[z] = newPR[z];
        }
    }
    std::vector<std::pair<WebPage*, double> > ranks;
    for(WebPageSet::const_iterator cit = total_set.begin(); cit != total_set.end(); cit++)
    {
        WebPage* tmp = *cit;
        for(int i=0; i<setSize; i++)
        {
            if (tmp->filename() == filenames[i])
            {
                ranks.push_back(std::make_pair(tmp, newPR[i]));
            }
        }
    }

    //cleanup
    delete [] lastPR;
    delete [] newPR;
    for(int i=0; i<setSize; i++)
    {
        delete [] matrix[i];
    }
    delete [] matrix;
    //return
    return ranks;
    


}

SearchEng::~SearchEng()
{
    //cleanup webpages
    for(std::map<std::string, WebPage*>::iterator i = allPages.begin(); i != allPages.end(); i++)
    {
        delete i->second;
    }

    //cleanup parsers
    for(std::map<std::string, PageParser*>::iterator i = parsers.begin(); i != parsers.end(); i++)
    {
        delete i->second;
    }
    delete noExtensionParser_;
}




