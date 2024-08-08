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
#include <FastIfToken.h>
#include <FastElseToken.h>
#include <RandomToken.h>
#include <JumpToken.h>
#include <ShortJumpToken.h>

#include <MemoryStream.h>

#include <ReferenceItemSymbol.h>
#include <ArgumentPackToken.h>


#include <string>

void WriteArray(ArraySymbol *symbol, IStream *stream);
void WriteStructure(StructureSymbol *symbol, IStream *stream, bool packed = false);

extern "C" {
    extern int32_t g_last_script_keyword;
    extern int32_t g_last_script_keyword_write;
}
extern std::map<uint32_t, const char *> m_checksum_names;

template<typename T, typename ST> void WriteAsScriptToken(T *symbol, IStream *stream) {
    if(symbol->GetNameChecksum() != 0) {
        m_checksum_names[symbol->GetNameChecksum()] = NULL;
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
        m_checksum_names[symbol->GetNameChecksum()] = NULL;
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
        m_checksum_names[symbol->GetNameChecksum()] = NULL;
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

QScriptToken *token_at_offset(uint32_t offset, std::map<QScriptToken *, uint32_t> &offsets) {
    std::map<QScriptToken *, uint32_t>::iterator it = offsets.begin();
    while(it != offsets.end()) {
        if(it->second == offset) {
            return it->first;
        }
        it++;
    }
    return nullptr;
}
bool is_random_token(EScriptToken type) {
    switch(type) {
        case ESCRIPTTOKEN_KEYWORD_RANDOM:
        //case ESCRIPTTOKEN_KEYWORD_RANDOM2:
        case ESCRIPTTOKEN_KEYWORD_RANDOM_PERMUTE:
        case ESCRIPTTOKEN_KEYWORD_RANDOM_RANGE2:
        case ESCRIPTTOKEN_KEYWORD_RANDOM_RANGE:
            return true;
        break;
    }
    return false;
}
void rewrite_offsets(std::map<QScriptToken *, uint32_t> &original_offsets, IStream *stream, uint32_t prescript_offset) {
    std::map<QScriptToken *, uint32_t>::iterator it = original_offsets.begin();
    while(it != original_offsets.end()) {
        QScriptToken *token = it->first;

        if(token->GetType() == ESCRIPTTOKEN_KEYWORD_FASTIF) {
            
            FastIfToken *t = reinterpret_cast<FastIfToken*>(token);
            uint32_t offset = original_offsets[t] + sizeof(uint16_t) + t->GetOffset();
            QScriptToken *r = token_at_offset(offset, original_offsets);
            uint8_t type = 0;
            if(r) {
                type = r->GetType();
                size_t diff = r->GetFileOffset() - token->GetFileOffset() - 1;
                t->RewriteOffset(stream, diff);
            } else {
                assert(false);
            }
        } else if(token->GetType() == ESCRIPTTOKEN_KEYWORD_FASTELSE) {
            FastElseToken *t = reinterpret_cast<FastElseToken*>(token);
            uint32_t offset = original_offsets[t] + sizeof(uint16_t) + t->GetOffset();
            QScriptToken *r = token_at_offset(offset, original_offsets);
            uint8_t type = 0;
            if(r) {
                type = r->GetType();
                size_t diff = r->GetFileOffset() - token->GetFileOffset() - 1;
                t->RewriteOffset(stream, diff);
            } else {
                assert(false);
            }
        } else if(token->GetType() == ESCRIPTTOKEN_JUMP) {
            JumpToken *t = reinterpret_cast<JumpToken*>(token);
            uint32_t offset = original_offsets[t] + sizeof(uint32_t) + t->GetOffset();
            QScriptToken *r = token_at_offset(offset, original_offsets);
            uint8_t type = 0;
            if(r) {
                type = r->GetType();
                size_t diff = r->GetFileOffset() - token->GetFileOffset() - sizeof(uint32_t);
                t->RewriteOffset(stream, diff);
            } else {
                assert(false);
            }
        } else if(token->GetType() == ESCRIPTTOKEN_SHORTJUMP) {
            ShortJumpToken *t = reinterpret_cast<ShortJumpToken*>(token);
            uint32_t offset = original_offsets[t] + sizeof(uint32_t) + t->GetOffset();
            QScriptToken *r = token_at_offset(offset, original_offsets);
            uint8_t type = 0;
            if(r) {
                type = r->GetType();
                size_t diff = r->GetFileOffset() - token->GetFileOffset() - sizeof(uint16_t);
                t->RewriteOffset(stream, diff);
            } else {
                assert(false);
            }
        } else if(token->GetType() == ESCRIPTTOKEN_KEYWORD_ELSEIF) {
            assert(false);
        } else if(is_random_token(token->GetType())) {
            RandomToken *t = reinterpret_cast<RandomToken*>(token);
            for(int i=0;i<t->GetNumItems();i++) {
                uint32_t offset = original_offsets[t] + t->CalculateTokenOffset(i) +  t->GetRandomOffset(i);

                QScriptToken *r = token_at_offset(offset, original_offsets);
                assert(r);
                
                size_t diff = (r->GetFileOffset() - token->GetFileOffset() - prescript_offset) - t->CalculateTokenOffset(i);
                t->SetRandomOffset(i, diff);
            }

            //rewrite random data
            size_t cursor = stream->GetOffset();
            stream->SetCursor(t->GetFileOffset());
            t->Write(stream);
            stream->SetCursor(cursor);
        }

        it++;
    }
}

void WriteQScript(QScriptSymbol *qscript, IStream *stream) {
    std::map<QScriptToken *, uint32_t> original_offsets;

    stream->WriteByte(ESCRIPTTOKEN_KEYWORD_SCRIPT);

    NameToken nt(qscript->GetNameChecksum());
    nt.Write(stream);

    uint32_t prescript_offset = stream->GetOffset();

    g_last_script_keyword_write = stream->GetOffset();

    MemoryStream ms(qscript->GetDecompBuff(), qscript->GetDecompLen());

    printf("on qscript: %08x\n", qscript->GetNameChecksum());

    //iterate through all tokens... emit token
    while(qscript->GetDecompLen() > ms.GetOffset()) {
        uint32_t read_offset = ms.GetOffset(); //skip type(1) + keyword script byte(1), name type(1)+value(4)
        uint8_t type = ms.ReadByte();
        QScriptToken *token = QScriptToken::Resolve(type);
        if(!token) {
            assert(false);
            break;
        }
        
        token->SetFileOffset(ms.GetOffset()-1);
        token->LoadParams(&ms);
        token->Write(stream);

        original_offsets[token] = read_offset;

        if(token->GetType() == ESCRIPTTOKEN_INLINEPACKSTRUCT) {
            StructureSymbol sym = reinterpret_cast<InlinePackStructToken*>(token)->GetValue();
            WriteStructure(&sym, stream, true);
        } else if(token->GetType() == ESCRIPTTOKEN_NAME) {
            m_checksum_names[reinterpret_cast<NameToken *>(token)->GetChecksum()] = NULL;
        }
    }

    EndOfLineToken eolt;
    eolt.Write(stream);

    rewrite_offsets(original_offsets, stream, prescript_offset);
}

void WriteArgumentPack(ReferenceItemSymbol *symbol, IStream *stream) {
    ArgumentPackToken tok;
    tok.Write(stream);

    m_checksum_names[symbol->GetValue()] = NULL;

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
            m_checksum_names[reinterpret_cast<NameSymbol *>(symbol)->GetValue()] = NULL;
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
            m_checksum_names[symbol->GetNameChecksum()] = NULL;
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