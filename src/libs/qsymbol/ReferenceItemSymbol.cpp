#include "ReferenceItemSymbol.h"

#include <sstream>
#include <iomanip>
#include <cassert>
ReferenceItemSymbol::ReferenceItemSymbol() {

}
ReferenceItemSymbol::~ReferenceItemSymbol() {

}
ESymbolType ReferenceItemSymbol::GetType() {
    return ESYMBOLTYPE_INTERNAL_REFERENCE;
}
void ReferenceItemSymbol::LoadParams(IStream *stream) {
    assert(false);
}
void ReferenceItemSymbol::LoadParamsFromArray(IStream *stream) {
    assert(false);
}
std::string ReferenceItemSymbol::ToString() {

}