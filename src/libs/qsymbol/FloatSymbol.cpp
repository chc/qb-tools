#include "FloatSymbol.h"

#include <sstream>
#include <iomanip>

FloatSymbol::FloatSymbol() {

}
FloatSymbol::FloatSymbol(float v) : m_value(v) {

}
FloatSymbol::~FloatSymbol() {

}
ESymbolType FloatSymbol::GetType() {
    return ESYMBOLTYPE_FLOAT;
}
void FloatSymbol::LoadParams(IStream *stream) {
    m_value = stream->ReadFloat();

    if(m_struct_item) {
        m_next_offset = stream->ReadUInt32();
    } else {
        stream->ReadUInt32();
    }
    
}
void FloatSymbol::LoadParamsFromArray(IStream *stream) {
    m_value = stream->ReadFloat();
}
void FloatSymbol::Write(IStream *stream) {
    stream->WriteFloat(m_value);
    stream->WriteUInt32(0);

}
void FloatSymbol::WriteToArray(IStream *stream) {
    stream->WriteFloat(m_value);
}
std::string FloatSymbol::ToString() {
    std::ostringstream ss;
    ss << m_value << " ";
    return ss.str();
}