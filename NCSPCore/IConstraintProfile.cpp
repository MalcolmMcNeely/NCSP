//
// Implementation of IConstraintProfile.h
//
#include "IConstraintProfile.h"

using namespace Gecode;

IConstraintProfile::IConstraintProfile()
{
}


IConstraintProfile::~IConstraintProfile()
{
}

void IConstraintProfile::ApplyDomainToRow(int& currentRow,
                                          Space* pSpace,
                                          Matrix<IntVarArray>* pMatrix,
                                          IntSet* pDomain)
{
   for (int i = 0; i < pMatrix->width(); i++)
   {
      (*pMatrix)(i, currentRow) = IntVar(*pSpace, *pDomain);;
   }

   currentRow++;
}
