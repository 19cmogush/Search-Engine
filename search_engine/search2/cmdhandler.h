#ifndef CMDHANDLER_H
#define CMDHANDLER_H
#include <string>
#include "handler.h"
#include "searcheng.h"

/**
 * Handles the QUIT command - COMPLETED in cmdhandler.cpp
 */
class QuitHandler : public Handler
{
public:
    QuitHandler();
    QuitHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};

/*********** OTHER CLASSES WILL NEED TO BE COMPLETED IN cmdhandler.cpp *******/
/***********  You will also need to declare the classes for handling   *******/
/***********          the AND, OR, and DIFF command handlers           *******/

/**
 * Handles the PRINT command
 */
class PrintHandler : public Handler
{
public:
    PrintHandler();
    PrintHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};

/**
 * Handles the INCOMING command
 */
class IncomingHandler : public Handler
{
public:
    IncomingHandler();
    IncomingHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};

/**
 * Handles the OUTGOING command
 */
class OutgoingHandler : public Handler
{
public:
    OutgoingHandler();
    OutgoingHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};

/****************************************************************************/
/* Declare the handler classes for AND, OR, and DIFF here                   */
/****************************************************************************/

//handles the AND comman
class AndHandler : public Handler
{
    public:
        AndHandler();
        AndHandler(Handler* next);
    
    protected:

        virtual bool canHandle(const std::string& cmd) const;
        virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};

class PrandHandler : public Handler
{
    public:
        PrandHandler();
        PrandHandler(Handler* next);
    
    protected:

        virtual bool canHandle(const std::string& cmd) const;
        virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};

class ProrHandler : public Handler
{
    public:
        ProrHandler();
        ProrHandler(Handler* next);
    
    protected:

        virtual bool canHandle(const std::string& cmd) const;
        virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};

//handles the OR command
class OrHandler : public Handler
{
    public:
        OrHandler();
        OrHandler(Handler* next);      

    protected:
        virtual bool canHandle(const std::string& cmd) const;
        virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};

//handles the DIFF command
class DiffHandler : public Handler
{
        public:
        DiffHandler();
        DiffHandler(Handler* next);      

    protected:
        virtual bool canHandle(const std::string& cmd) const;
        virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};

#endif
