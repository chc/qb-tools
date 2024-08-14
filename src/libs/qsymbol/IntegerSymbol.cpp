#include "IntegerSymbol.h"

#include <sstream>
#include <iomanip>

IntegerSymbol::IntegerSymbol() {

}
IntegerSymbol::IntegerSymbol(int32_t v) : m_value(v) {

}
IntegerSymbol::~IntegerSymbol() {

}
ESymbolType IntegerSymbol::GetType() {
    return ESYMBOLTYPE_INTEGER;
}
void IntegerSymbol::LoadParams(IStream *stream) {
    m_value = stream->ReadInt32();
    if(m_struct_item) {
        m_next_offset = stream->ReadUInt32();
    } else {
        stream->ReadUInt32();
    }
}
void IntegerSymbol::LoadParamsFromArray(IStream *stream) {
    m_value = stream->ReadInt32();
}
void IntegerSymbol::Write(IStream *stream) {
    stream->WriteInt32(m_value);
    stream->WriteUInt32(0);

}
void IntegerSymbol::WriteToArray(IStream *stream) {
    stream->WriteInt32(m_value);
}
std::string IntegerSymbol::ToString() {
    std::ostringstream ss;
    ss << m_value << " ";
    return ss.str();
}