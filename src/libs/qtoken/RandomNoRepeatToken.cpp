#include "RandomNoRepeatToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>
#include <sstream>

RandomNoRepeatToken::RandomNoRepeatToken() : RandomToken() {

}
RandomNoRepeatToken::RandomNoRepeatToken(int num_items) : RandomToken(num_items) {

}
RandomNoRepeatToken::~RandomNoRepeatToken() {

}
EScriptToken RandomNoRepeatToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_RANDOM_NO_REPEAT;
}
std::string RandomNoRepeatToken::ToString() {
    return "RandomNoRepeat(";
}