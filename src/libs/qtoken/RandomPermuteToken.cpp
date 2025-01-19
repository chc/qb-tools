#include "RandomPermuteToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>
#include <sstream>

RandomPermuteToken::RandomPermuteToken() : RandomToken() {

}
RandomPermuteToken::RandomPermuteToken(int num_items) : RandomToken(num_items) {

}
RandomPermuteToken::~RandomPermuteToken() {

}
EScriptToken RandomPermuteToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_RANDOM_PERMUTE;
}
std::string RandomPermuteToken::ToString() {
    return "RandomPermute(";
}