#include "ReferenceItemSymbol.h"

#include <sstream>
#include <iomanip>
#include <cassert>
ReferenceItemSymbol::ReferenceItemSymbol(uint8_t type) : m_type(type) {

}
ReferenceItemSymbol::~ReferenceItemSymbol() {

}
ESymbolType ReferenceItemSymbol::GetType() {
    return ESYMBOLTYPE_INTERNAL_REFERENCE;
}
void ReferenceItemSymbol::LoadParams(IStream *stream) {
    SetValue(stream->ReadUInt32());
    SetNextOffset(stream->ReadUInt32());
}
void ReferenceItemSymbol::LoadParamsFromArray(IStream *stream) {
    SetValue(stream->ReadUInt32());
}
#include <cassert>
void ReferenceItemSymbol::Write(IStream *stream) {
    assert(0);
}
void ReferenceItemSymbol::WriteToArray(IStream *stream) {
    assert(0);
}
std::string ReferenceItemSymbol::ToString() {
    return "";
}