#include <cassert>
#include <map>

#include <QSymbol.h>
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

#if QTOKEN_SUPPORT_LEVEL > 4
    #include <ElseIfToken.h>
#endif

#include <string>

void WriteArray(ArraySymbol *symbol, IStream *stream);
void WriteStructure(StructureSymbol *symbol, IStream *stream, bool packed = false);

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
        case ESCRIPTTOKEN_KEYWORD_RANDOM_PERMUTE:
        case ESCRIPTTOKEN_KEYWORD_RANDOM_NO_REPEAT:
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
            uint32_t offset = original_offsets[t] + sizeof(uint16_t) + t->GetOffset() - sizeof(uint8_t);
            QScriptToken *r = token_at_offset(offset, original_offsets);
            if(r) {
                size_t diff = r->GetFileOffset() - token->GetFileOffset() - sizeof(uint8_t);
                t->RewriteOffset(stream, diff);
            } else {
                assert(false);
            }
        } else if(token->GetType() == ESCRIPTTOKEN_KEYWORD_FASTELSE) {
            FastElseToken *t = reinterpret_cast<FastElseToken*>(token);
            uint32_t offset = original_offsets[t] + t->GetOffset() - sizeof(uint8_t);
            QScriptToken *r = token_at_offset(offset, original_offsets);
            if(r) {
                size_t diff = r->GetFileOffset() - token->GetFileOffset() + sizeof(uint8_t);
                t->RewriteOffset(stream, diff);
            } else {
                assert(false);
            }
        } 
        #if QTOKEN_SUPPORT_LEVEL > 4
        else if(token->GetType() == ESCRIPTTOKEN_KEYWORD_ELSEIF) {
            ElseIfToken *t = reinterpret_cast<ElseIfToken*>(token);
            uint32_t offset = original_offsets[t] + t->GetNextOffset() + sizeof(uint8_t);
            QScriptToken *r = token_at_offset(offset, original_offsets);
            if(r) {
                size_t diff = r->GetFileOffset() - token->GetFileOffset() - sizeof(uint16_t) + sizeof(uint8_t);
                t->SetNextOffset(stream, diff);
            } else {
                assert(false);
            }
            
            offset = original_offsets[t] + sizeof(uint16_t) + t->GetEndIfOffset() + sizeof(uint8_t);
            r = token_at_offset(offset, original_offsets);
            if(r) {
                size_t diff = r->GetFileOffset() - token->GetFileOffset() - sizeof(uint32_t) + sizeof(uint8_t);
                t->SetEndIfOffset(stream, diff);
            } else {
                assert(false);
            }
        } 
        #endif //QTOKEN_SUPPORT_LEVEL > 4
        else if(token->GetType() == ESCRIPTTOKEN_JUMP) {
            JumpToken *t = reinterpret_cast<JumpToken*>(token);
            uint32_t offset = original_offsets[t] + sizeof(uint32_t) + sizeof(uint8_t) + t->GetOffset();
            QScriptToken *r = token_at_offset(offset, original_offsets);
            if(r) {
                size_t diff = r->GetFileOffset() - token->GetFileOffset() - sizeof(uint32_t) - sizeof(uint8_t);
                t->RewriteOffset(stream, diff);
            } else {
                assert(false);
            }
        } else if(token->GetType() == ESCRIPTTOKEN_SHORTJUMP) {
            ShortJumpToken *t = reinterpret_cast<ShortJumpToken*>(token);
            uint32_t offset = original_offsets[t] + t->GetOffset() + sizeof(uint8_t);
            QScriptToken *r = token_at_offset(offset, original_offsets);
            if(r) {
                size_t diff = r->GetFileOffset() - token->GetFileOffset() - sizeof(uint8_t);
                t->RewriteOffset(stream, diff);
            } else {
                assert(false);
            }
        } else if(is_random_token(token->GetType())) {
            RandomToken *t = reinterpret_cast<RandomToken*>(token);
            for(int i=0;i<t->GetNumItems();i++) {
                uint32_t offset = original_offsets[t] + t->CalculateTokenOffset(i) +  t->GetRandomOffset(i);
                QScriptToken *r = token_at_offset(offset, original_offsets);
                
                assert(r);

                uint32_t updated_offset = r->GetFileOffset() - t->GetFileOffset() -  t->CalculateTokenOffset(i);
                t->SetRandomOffset(i, updated_offset);
            }

            //rewrite random data
            t->Rewrite(stream);
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

    MemoryStream ms(qscript->GetDecompBuff(), qscript->GetDecompLen());

    //iterate through all tokens... emit token
    while(qscript->GetDecompLen() > ms.GetOffset()) {
        uint32_t read_offset = ms.GetOffset(); //skip type(1) + keyword script byte(1), name type(1)+value(4)
        uint8_t type = ms.ReadByte();
        QScriptToken *token = QScriptToken::Resolve(type);
        if(!token) {
            assert(false);
            break;
        }
        if(token->GetType() == ESCRIPTTOKEN_INLINEPACKSTRUCT) {
            size_t inline_offset = ms.GetOffset() + 2;
            int padding = (sizeof(uint32_t)) - (((inline_offset - 1) % sizeof(uint32_t)) + 1);
            assert(padding <= 4);
            InlinePackStructToken* ip = reinterpret_cast<InlinePackStructToken*>(token);
            ip->SetPadding(padding);            
        }
        
        token->SetFileOffset(ms.GetOffset()-1);
        token->LoadParams(&ms);
        if(token->GetType() == ESCRIPTTOKEN_INLINEPACKSTRUCT) {
            InlinePackStructToken* ip = reinterpret_cast<InlinePackStructToken*>(token);
            StructureSymbol *sym = ip->GetValue();
            ip->SetValue(nullptr);
            ip->SetPadding(0);
            token->Write(stream);

            WriteStructure(sym, stream, true);
            delete sym;
        } else if(token->GetType() == ESCRIPTTOKEN_ARGUMENTPACK) {
            ArgumentPackToken* ap = reinterpret_cast<ArgumentPackToken*>(token);
            ap->WriteExtendedParams(stream);
        } else {
            token->Write(stream);
        }
        

        original_offsets[token] = read_offset;

        if(token->GetType() == ESCRIPTTOKEN_NAME) {
            m_checksum_names[reinterpret_cast<NameToken *>(token)->GetChecksum()] = NULL;
        }
    }

    EndOfLineToken eolt;
    eolt.Write(stream);

    rewrite_offsets(original_offsets, stream, prescript_offset);
}

void WriteArgumentPack(ReferenceItemSymbol *symbol, IStream *stream) {
    if(symbol->GetNameChecksum() != 0) {
        m_checksum_names[symbol->GetNameChecksum()] = NULL;
        NameToken nt(symbol->GetNameChecksum());
        nt.Write(stream);

        EqualsToken et;
        et.Write(stream);
    }
    ArgumentPackToken tok;
    tok.SetRefType(symbol->GetRefType());
    tok.SetIsRequiredParams(symbol->GetIsRequiredParams());
    tok.WriteExtendedParams(stream);

    m_checksum_names[symbol->GetValue()] = NULL;

    NameToken nt(symbol->GetValue());
    nt.Write(stream);

    EndOfLineToken eolt;
    eolt.Write(stream);
}
void WriteSymbolAsScriptToken(QSymbol *symbol, IStream *stream) {
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
        QSymbol *s = symbol->GetToken(i);
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

    std::vector<QSymbol *> children = symbol->GetTokens();
    std::vector<QSymbol *>::iterator it = children.begin();
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