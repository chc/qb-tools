#ifndef _TYPEHINT_PLAYLIST_H
#define _TYPEHINT_PLAYLIST_H
#include "QScriptToken.h"

class TypeHint_Playlist : public QScriptToken {
    public:
        TypeHint_Playlist() {

        }
        ~TypeHint_Playlist() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_TYPEHINT_PLAYLIST;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - sizeof(uint8_t);
        }
        std::string ToString() {
            return AppendSpaceToString ? "TypeHint_Playlist " : "TypeHint_Playlist";
        }
    private:
};
#endif //_TYPEHINT_PLAYLIST_H