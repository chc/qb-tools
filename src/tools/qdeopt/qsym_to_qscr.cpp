#include <cassert>
#include <map>

#include <QSymbolToken.h>
#include <QScriptToken.h>
#include <IntegerSymbol.h>
#include <IntegerToken.h>

#include <FloatSymbol.h>
#include <FloatToken.h>

#include <StringSymbol.h>
#include <LocalStringSymbol.h>
#include <StringToken.h>
#include <LocalStringToken.h>

#include <NameToken.h>
#include <EqualsToken.h>
#include <EndOfLineToken.h>
#include <NameSymbol.h>
#include <QScriptSymbol.h>
#include <ArraySymbol.h>
#include <StructureSymbol.h>
#include <InlinePackStructToken.h>
#include <PairSymbol.h>
#include <VectorSymbol.h>
#include <PairToken.h>
#include <VectorToken.h>

#include <MemoryStream.h>

#include <ReferenceItemSymbol.h>
#include <ArgumentPackToken.h>

void WriteArray(ArraySymbol *symbol, IStream *stream);
void WriteStructure(StructureSymbol *symbol, IStream *stream, bool packed = false);

extern uint32_t g_last_script_keyword;
extern uint32_t g_last_script_keyword_write;
extern std::map<uint32_t, const char *> m_checksum_names;

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

void WritePair(PairSymbol *symbol, IStream *stream) {
    if(symbol->GetNameChecksum() != 0) {
        NameToken nt(symbol->GetNameChecksum());
        nt.Write(stream);

        EqualsToken et;
        et.Write(stream);
    }

    PairToken pt(symbol->GetX(), symbol->GetY());
    pt.Write(stream);

    EndOfLineToken eolt;
    eolt.Write(stream);
}
void WriteVector(VectorSymbol *symbol, IStream *stream) {
    if(symbol->GetNameChecksum() != 0) {
        NameToken nt(symbol->GetNameChecksum());
        nt.Write(stream);

        EqualsToken et;
        et.Write(stream);
    }

    VectorToken pt(symbol->GetX(), symbol->GetY(), symbol->GetZ());
    pt.Write(stream);

    EndOfLineToken eolt;
    eolt.Write(stream);
}

void WriteQScript(QScriptSymbol *qscript, IStream *stream) {
    stream->WriteByte(ESCRIPTTOKEN_KEYWORD_SCRIPT);

    NameToken nt(qscript->GetNameChecksum());
    nt.Write(stream);

    g_last_script_keyword_write = stream->GetOffset();

    MemoryStream ms(qscript->GetDecompBuff(), qscript->GetDecompLen());

    //iterate through all tokens... emit token
    while(qscript->GetDecompLen() > ms.GetOffset()) {
        uint8_t type = ms.ReadByte();
        QScriptToken *token = QScriptToken::Resolve(type);
        if(token) {
            token->LoadParams(&ms);
        }
        token->Write(stream);
        if(token->GetType() == ESCRIPTTOKEN_INLINEPACKSTRUCT) {
            StructureSymbol sym = reinterpret_cast<InlinePackStructToken*>(token)->GetValue();
            WriteStructure(&sym, stream, true);
        }
    }

    EndOfLineToken eolt;
    eolt.Write(stream);
}

void WriteArgumentPack(ReferenceItemSymbol *symbol, IStream *stream) {
    ArgumentPackToken tok;
    tok.Write(stream);

    NameToken nt(symbol->GetValue());
    nt.Write(stream);
}
void WriteSymbolAsScriptToken(QSymbolToken *symbol, IStream *stream) {
    NameToken nt(symbol->GetNameChecksum());
    m_checksum_names[symbol->GetNameChecksum()] = NULL;
    switch(symbol->GetType()) {
        case ESYMBOLTYPE_INTEGER:
            WriteAsScriptToken<IntegerSymbol, IntegerToken>(reinterpret_cast<IntegerSymbol *>(symbol), stream);
            break;
        case ESYMBOLTYPE_FLOAT:
            WriteAsScriptToken<FloatSymbol, FloatToken>(reinterpret_cast<FloatSymbol *>(symbol), stream);
            break;
        case ESYMBOLTYPE_STRING:
            WriteAsScriptToken<StringSymbol, StringToken>(reinterpret_cast<StringSymbol *>(symbol), stream);
            break;
        case ESYMBOLTYPE_LOCALSTRING:
            WriteAsScriptToken<LocalStringSymbol, LocalStringToken>(reinterpret_cast<LocalStringSymbol *>(symbol), stream);
            break;
        case ESYMBOLTYPE_PAIR:
            WritePair(reinterpret_cast<PairSymbol *>(symbol), stream);
            break;
        case ESYMBOLTYPE_VECTOR:
            WriteVector(reinterpret_cast<VectorSymbol *>(symbol), stream);
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
        case ESYMBOLTYPE_INTERNAL_REFERENCE:
            WriteArgumentPack(reinterpret_cast<ReferenceItemSymbol *>(symbol), stream);
            break;
    }
}

void WriteArray(ArraySymbol *symbol, IStream *stream) {
    if(symbol->GetNameChecksum() != 0) {
        NameToken nt(symbol->GetNameChecksum());
        m_checksum_names[symbol->GetNameChecksum()] = NULL;
        nt.Write(stream);

        EqualsToken et;
        et.Write(stream);
    }

    EndOfLineToken eolt;


    stream->WriteByte(ESCRIPTTOKEN_STARTARRAY);
    eolt.Write(stream);
    for(int i=0;i<symbol->GetNumItems();i++) {
        QSymbolToken *s = symbol->GetToken(i);
        WriteSymbolAsScriptToken(s, stream);
    }

    stream->WriteByte(ESCRIPTTOKEN_ENDARRAY);
    eolt.Write(stream);

}
void WriteStructure(StructureSymbol *symbol, IStream *stream, bool packed) {
    if(!packed) {
        if(symbol->GetNameChecksum() != 0) {
            NameToken nt(symbol->GetNameChecksum());
            nt.Write(stream);

            EqualsToken et;
            et.Write(stream);
        }
    }


    stream->WriteByte(ESCRIPTTOKEN_STARTSTRUCT);
    if(!packed) {
        EndOfLineToken eolt;
        eolt.Write(stream);
    }

    std::vector<QSymbolToken *> children = symbol->GetTokens();
    std::vector<QSymbolToken *>::iterator it = children.begin();
    while(it != children.end()) {
        WriteSymbolAsScriptToken(*it, stream);
        it++;
    }
    stream->WriteByte(ESCRIPTTOKEN_ENDSTRUCT);

    if(!packed) {
        EndOfLineToken eolt;
        eolt.Write(stream);
    }
    
}