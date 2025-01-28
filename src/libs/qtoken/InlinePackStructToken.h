#ifdef WITH_SYMBOL_SUPPORT
    #ifndef _INLINEPACKSTRUCTTOKEN_H
    #define _INLINEPACKSTRUCTTOKEN_H
    #include "QScriptToken.h"

    #include <StructureSymbol.h>
    #include <MemoryStream.h>
    class InlinePackStructToken : public QScriptToken {
        public:
            InlinePackStructToken() {
                m_inner_struct = NULL;
                m_padding = 0;
            }
            ~InlinePackStructToken() {

            }
            EScriptToken GetType() {
                return ESCRIPTTOKEN_INLINEPACKSTRUCT;
            }
            void LoadParams(IStream *stream) {
                assert(m_inner_struct == NULL);
                m_file_offset = stream->GetOffset() - 1;
                uint16_t len = stream->ReadUInt16();    

                //set required alignment
                int padding = m_padding;
                while(padding--) {
                    stream->ReadByte();
                }    
                //

                if(len > 0) {
                    uint8_t *buff = new uint8_t[len];
                    for(int i=0;i<len;i++) {
                        buff[i] = stream->ReadByte();
                    }

                    MemoryStream ms(buff, len);
                    ms.SetReadEndian(ISTREAM_SYMBOL_ENDIAN);

                    m_inner_struct = new StructureSymbol();
                    m_inner_struct->LoadParamsNoOffset(&ms);

                    delete[] buff;
                }
            }
            void Write(IStream *stream) {
                m_file_offset = stream->GetOffset();
                //don't write the actual pack struct data, the qdumper will dump the inner struct (this should be refactored)
                stream->WriteByte(ESCRIPTTOKEN_INLINEPACKSTRUCT);
                uint32_t len = 0;

                const int STRUCT_BUFF_SIZE = 1024;
                uint8_t struct_buff[STRUCT_BUFF_SIZE];

                if(m_inner_struct == NULL) {
                    len = 0;
                } else {

                    MemoryStream ms(&struct_buff, STRUCT_BUFF_SIZE);
                    ms.SetWriteEndian(ISTREAM_SYMBOL_ENDIAN);
                    m_inner_struct->WriteNoOffset(&ms);
                    len = ms.GetOffset();
                }
                stream->WriteUInt16(len);

                int padding = m_padding;
                while(padding--) {
                    stream->WriteByte(0);
                }
                if(len > 0) {
                    stream->WriteBuffer(struct_buff, len);
                }
            }
            void WriteStructure(IStream* stream, StructureSymbol *symbol) {
                assert(false);
            }
            std::string ToString() {
                return "$";
            }
            StructureSymbol *GetValue() { return m_inner_struct; }
            void SetValue(StructureSymbol *sym) {
                m_inner_struct = sym;
            }
            void SetPadding(int padding) { m_padding = padding; }
        private:
            StructureSymbol *m_inner_struct;
            int m_padding;
    };
    #endif //_INLINEPACKSTRUCTTOKEN_H
#endif //WITH_SYMBOL_SUPPORT