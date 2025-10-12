#include <stdio.h>
#include "cfuncs.h"

#include <StringSymbol.h>
#include <IntegerSymbol.h>
namespace CFuncs {
    bool printf(StructureSymbol *args) {
        std::vector<QSymbol *> all_args = args->GetTokens();
        IntegerSymbol *s = reinterpret_cast<IntegerSymbol*>(all_args.front());
        ::printf("printf called: %d\n", s->GetValue());
        return true;
    }
}