#pragma once
#include <kamek.h>
#include <rvl/dvd/dvd.h>

typedef struct loaderFunctions loaderFunctions;

typedef void (*OSReport_t) (const char *str, ...);
typedef void (*OSFatal_t) (u32 *fg, u32 *bg, const char *str, ...);
typedef int (*DVDConvertPathToEntrynum_t) (const char *path);
typedef bool (*DVDFastOpen_t) (int entrynum, DVDFileInfo *fileInfo);
typedef int (*DVDReadPrio_t) (DVDFileInfo *fileInfo, void *buffer, int length, int offset, int prio);
typedef bool (*DVDClose_t) (DVDFileInfo *fileInfo);
typedef int (*sprintf_t) (char *str, const char *format, ...);
typedef void *(*KamekAlloc_t) (u32 size, bool isForCode, const loaderFunctions *funcs);
typedef void (*KamekFree_t) (void *buffer, bool isForCode, const loaderFunctions *funcs);

struct loaderFunctions {
    OSReport_t OSReport;
    OSFatal_t OSFatal;
    DVDConvertPathToEntrynum_t DVDConvertPathToEntrynum;
    DVDFastOpen_t DVDFastOpen;
    DVDReadPrio_t DVDReadPrio;
    DVDClose_t DVDClose;
    sprintf_t sprintf;
    KamekAlloc_t kamekAlloc;
    KamekFree_t kamekFree;
};

inline void cacheInvalidateAddress(void* address) {
    register void* addressRegister = address;
    asm {
        dcbst 0, addressRegister
        sync
        icbi 0, addressRegister
    }
}

void loadKamekBinaryFromDisc(const loaderFunctions *funcs, const char *path);
