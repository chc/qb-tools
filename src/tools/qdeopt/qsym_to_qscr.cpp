#include <QSymbolToken.h>
#include <QScriptToken.h>
#include <IntegerSymbol.h>
#include <IntegerToken.h>

#include <FloatSymbol.h>
#include <FloatToken.h>

#include <NameToken.h>
#include <EqualsToken.h>
#include <EndOfLineToken.h>
#include <NameSymbol.h>
#include <QScriptSymbol.h>
#include <ArraySymbol.h>
#include <StructureSymbol.h>

#include <cassert>

void WriteArray(ArraySymbol *symbol, IStream *stream);
void WriteStructure(StructureSymbol *symbol, IStream *stream);

template<typename T, typename ST> void WriteAsScriptToken(T *symbol, IStream *stream) {
    if(symbol->GetNameChecksum() != 0) {
        NameToken nt(symbol->GetNameChecksum());
        nt.Write(stream);

        EqualsToken et;
        et.Write(stream);
    }


    ST wt(symbol->GetValue());
    wt.Write(stream);

    EndOfLineToken eolt;
    eolt.Write(stream);
}
void WriteQScript(QScriptSymbol *qscript, IStream *stream) {
    stream->WriteByte(ESCRIPTTOKEN_KEYWORD_SCRIPT);

    NameToken nt(qscript->GetNameChecksum());
    nt.Write(stream);
    
    stream->WriteBuffer(qscript->GetDecompBuff(), qscript->GetDecompLen());

    EndOfLineToken eolt;
    eolt.Write(stream);
}

void WriteSymbolAsScriptToken(QSymbolToken *symbol, IStream *stream) {
    switch(symbol->GetType()) {
        case ESYMBOLTYPE_INTEGER:
            WriteAsScriptToken<IntegerSymbol, IntegerToken>(reinterpret_cast<IntegerSymbol *>(symbol), stream);
            break;
        case ESYMBOLTYPE_FLOAT:
            WriteAsScriptToken<FloatSymbol, FloatToken>(reinterpret_cast<FloatSymbol *>(symbol), stream);
            break;
        case ESYMBOLTYPE_STRING:
            assert(false);
            break;
        case ESYMBOLTYPE_LOCALSTRING:
            assert(false);
            break;
        case ESYMBOLTYPE_PAIR:
            assert(false);
            break;
        case ESYMBOLTYPE_VECTOR:
            assert(false);
            break;
        case ESYMBOLTYPE_QSCRIPT:
            WriteQScript(reinterpret_cast<QScriptSymbol *>(symbol), stream);
            break;
        case ESYMBOLTYPE_STRUCTURE:
            WriteStructure(reinterpret_cast<StructureSymbol *>(symbol), stream);
            break;
        case ESYMBOLTYPE_ARRAY:
            WriteArray(reinterpret_cast<ArraySymbol *>(symbol), stream);
            break;
        case ESYMBOLTYPE_NAME:
            WriteAsScriptToken<NameSymbol, NameToken>(reinterpret_cast<NameSymbol *>(symbol), stream);
            break;
    }
}

void WriteArray(ArraySymbol *symbol, IStream *stream) {
    if(symbol->GetNameChecksum() != 0) {
        NameToken nt(symbol->GetNameChecksum());
        nt.Write(stream);

        EqualsToken et;
        et.Write(stream);
    }


    stream->WriteByte(ESCRIPTTOKEN_STARTARRAY);
    for(int i=0;i<symbol->GetNumItems();i++) {
        QSymbolToken *s = symbol->GetToken(i);
        WriteSymbolAsScriptToken(s, stream);
        
    }

    stream->WriteByte(ESCRIPTTOKEN_ENDARRAY);

}
void WriteStructure(StructureSymbol *symbol, IStream *stream) {
    if(symbol->GetNameChecksum() != 0) {
        NameToken nt(symbol->GetNameChecksum());
        nt.Write(stream);

        EqualsToken et;
        et.Write(stream);
    }

    stream->WriteByte(ESCRIPTTOKEN_STARTSTRUCT);
    std::vector<QSymbolToken *> children = symbol->GetTokens();
    std::vector<QSymbolToken *>::iterator it = children.begin();
    while(it != children.end()) {
        WriteSymbolAsScriptToken(*it, stream);
        it++;
    }
    stream->WriteByte(ESCRIPTTOKEN_ENDSTRUCT);
}