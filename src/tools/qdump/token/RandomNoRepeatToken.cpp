#include "RandomNoRepeatToken.h"
#include "../IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

RandomNoRepeatToken::RandomNoRepeatToken() {

}
RandomNoRepeatToken::~RandomNoRepeatToken() {

}
EScriptToken RandomNoRepeatToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_RANDOM_NO_REPEAT;
}
void RandomNoRepeatToken::LoadParams(IQStream *stream) {
    uint32_t num_items = stream->ReadUInt32();
    //printf("num items: %d\n", num_items);
    for(int i=0;i<num_items;i++) {
        uint16_t weight = stream->ReadUInt16();
        //printf("item: %d - %d\n", i, weight);
    }
    for(int i=0;i<num_items;i++) {
        uint32_t o = stream->ReadUInt32();
        //printf("offset: %08x\n", o);
    }
}
std::string RandomNoRepeatToken::ToString() {
    return "RandomNoRepeat";
}