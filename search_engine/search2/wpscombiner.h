#ifndef WPSCOMBINER_H
#define WPSCOMBINER_H

#include "webpage.h"

class WebPageSetCombiner
{
public:
    /**
     * Destructor
     */
    virtual ~WebPageSetCombiner() {}

    /**
     * Combines two sets of webpages into a resulting set based on some strategy
     *
     * @param[in] setA
     *   first set of webpages
     * @param[in] setB
     *   second set of webpages
     * @return set of webpages that results from the combination strategy
     */
    virtual WebPageSet combine(const WebPageSet& setA, const WebPageSet& setB) = 0;
};

    /*** Define Derived WebPageSetCombiners (for AND, OR, DIFF) classes below ***/
    class AndWebPageSetCombiner : public WebPageSetCombiner {
        public:
            ~AndWebPageSetCombiner() {}
            WebPageSet combine(const WebPageSet& setA, const WebPageSet& setB);
    };

    class OrWebPageSetCombiner : public WebPageSetCombiner {
        public:
            ~OrWebPageSetCombiner() {}
            WebPageSet combine(const WebPageSet& setA, const WebPageSet& setB);
    };

    class DiffWebPageSetCombiner : public WebPageSetCombiner {
        public:
            ~DiffWebPageSetCombiner() {}
            WebPageSet combine(const WebPageSet& setA, const WebPageSet& setB);
    };


#endif
