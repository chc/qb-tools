#include "ReferenceItemSymbol.h"

#include <sstream>
#include <iomanip>
#include <cassert>
ReferenceItemSymbol::ReferenceItemSymbol() {

}
ReferenceItemSymbol::~ReferenceItemSymbol() {

}
ESymbolType ReferenceItemSymbol::GetType() {
    assert(false);
    return ESYMBOLTYPE_NONE;
}
void ReferenceItemSymbol::LoadParams(IStream *stream) {

}
void ReferenceItemSymbol::LoadParamsFromArray(IStream *stream) {

}
std::string ReferenceItemSymbol::ToString() {

}