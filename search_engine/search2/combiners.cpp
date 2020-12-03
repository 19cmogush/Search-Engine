#include "wpscombiner.h"

// Place your function implementations for your derived
// WebPageSetCombiner classes here
WebPageSet AndWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB)
{
    //return set with all webpages that contain a term out of sets A and B
    WebPageSet mySet = setA;
    for (WebPageSet::const_iterator cit = setA.begin(); cit != setA.end(); cit++)
    {
        if (setB.find(*cit) == setB.end())
        {
            mySet.erase(*cit);
        }
    }
    return mySet;
}

WebPageSet OrWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB)
{
    //return a set of pages that have the term
    WebPageSet mySet = setA;
    for (WebPageSet::const_iterator cit = setB.begin(); cit != setB.end(); cit++)
    {
        if (setA.find(*cit) == setA.end())
        {
            mySet.insert(*cit);
        }
    }
    return mySet;
}

WebPageSet DiffWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB)
{
    //return web page sets that have different terms
    WebPageSet mySet = setA;
    for (WebPageSet::const_iterator cit = setA.begin(); cit != setA.end(); cit++)
    {
        if (setB.find(*cit) != setB.end())
        {
            mySet.erase(*cit);
        }
    }
    return mySet;
}