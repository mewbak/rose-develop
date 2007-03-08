// Copyright 2005,2006,2007 Markus Schordan, Gergo Barany
// $Id: ExprLabeler.h,v 1.2 2007-03-08 15:36:48 markus Exp $

#ifndef H_EXPRLABELER
#define H_EXPRLABELER

#include <config.h>
#include <rose.h>

#include "cfg_support.h"

// preorder -- but it doesn't really matter
class ExprLabeler : public AstSimpleProcessing
{
public:
    ExprLabeler(int);
    int get_expnum() const;

protected:
    void visit(SgNode *);

private:
    ExprLabeler();
    int expnum;
};

#endif
