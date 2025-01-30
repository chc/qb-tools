#include <stdio.h>
#include <string.h>
#include <crc32.h>
#include <ctype.h>
#include <stdlib.h>
#include <dbginfo.h>
#include <pre.h>
#include <cassert>

#include <QStream.h>
#include <QScriptToken.h>
#include <MemoryStream.h>
#include <ChecksumNameToken.h>

const char* dbginfo_path = NULL;


bool unpre_file_info_callback(PreItem *item) {
    if(strstr(item->filename,".qb") == NULL) {
        return true;
    }
    uint8_t *buff = (uint8_t*)malloc(item->original_size);
    unpre_read_file(item, buff);

    MemoryStream ms(buff, item->original_size);

    QStream qs = QStream(&ms);

    QScriptToken *token;
    while(true) {
        token = qs.NextToken();
        if(token == NULL || token->GetType() == ESCRIPTTOKEN_ENDOFFILE) {
            break;
        } else if(token->GetType() == ESCRIPTTOKEN_CHECKSUM_NAME) {
            ChecksumNameToken* c = reinterpret_cast<ChecksumNameToken *>(token);
            DbgChecksumInfo line_info;
            line_info.checksum = c->GetChecksum();
            line_info.name = c->GetName();
            dbginfo_append_cache(dbginfo_path, &line_info);
        }

    }
    free(buff);
    return true;
}

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s [pre_path]\n", argv[0]);
        return -1;
    }

    dbginfo_path = getenv("QBTOOLS_DBGINFO_PATH");
    if (dbginfo_path != NULL) {
        dbginfo_load_cache(dbginfo_path);
    }
    else {
        printf("** no dbg data specified\n");
        return -1;
    }

    printf("importing dbginfo pre at: %s\n", argv[1]);

    unpre_iterate_files(argv[1], unpre_file_info_callback);
    return 0;
}