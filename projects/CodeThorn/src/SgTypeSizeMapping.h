#ifndef SGTYPE_SIZE_MAPPING_H
#define SGTYPE_SIZE_MAPPING_H

#include "TypeSizeMapping.h"

namespace SPRAY {
  class SgTypeSizeMapping : public TypeSizeMapping {
  public:
    /* determine size of type in bytes from SgType and stored mapping
       of builtin types. The computation of the type size uses only
       type sizes provided by the type size mapping (it is independent
       of the system the analyzer is running on).
    */
    SPRAY::TypeSize determineTypeSize(SgType* sgType);

    // returns the element type size of an array
    SPRAY::TypeSize determineElementTypeSize(SgArrayType* sgType);

    // returns the size of the type pointed to.
    SPRAY::TypeSize determineTypeSizePointedTo(SgPointerType* sgType);
  };
} // end of namespace SPRAY

#endif
