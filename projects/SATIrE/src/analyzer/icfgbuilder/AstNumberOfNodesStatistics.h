// Copyright 2005,2006,2007 Markus Schordan, Gergo Barany
// $Id: AstNumberOfNodesStatistics.h,v 1.2 2007-03-08 15:36:48 markus Exp $

#ifndef H_ASTNUMBEROFNODESSTATISTICS
#define H_ASTNUMBEROFNODESSTATISTICS

#include "rose.h"

class AstNumberOfNodesStatistics : public SgSimpleProcessing
{
public:
    AstNumberOfNodesStatistics();
    unsigned long get_numberofnodes() const;
    unsigned long get_numberofstatements() const;

protected:
    virtual void visit(SgNode *node);
    unsigned long numnod, numst;
};

#endif
