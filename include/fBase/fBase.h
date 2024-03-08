#include <kamek.h>
#include <egg/core/eggFrmHeap.h>
#include <fBase/fManager.h>

#ifndef static_assert
    // https://stackoverflow.com/a/1597129
    #define TOKENPASTE(x, y) x ## y
    #define TOKENPASTE2(x, y) TOKENPASTE(x, y)

    #define static_assert(condition, ...) typedef int TOKENPASTE2(static_assert_, __LINE__)[(condition) ? 1 : -1]
#endif // static_assert

class fBase_c {
    public:
        enum MAIN_STATE_e {
            STATE_NOT_READY,
            STATE_DESTROY,
            STATE_READY,
        };

        u32 uniqueId; // unique for every actor
        u32 settings; // nybbles 5 to 12 of spritedata settings
        u16 profileId; // aka actor id

        // The following is for internal usage
        bool created;
        bool deleted;
        bool notDeferred;
        bool deferred;
        u8 groupType; // 0 = none, 1 = scene, 2 = stage
        u8 processType; // &1 = connect, &2 = create, &4 = execute, &8 = delete, &16 = draw
        fManager_c mgr;
        EGG::FrmHeap* heap;
        // vtable 0x60

        fBase_c();

        virtual int create();
        virtual int preCreate();
        virtual void postCreate(MAIN_STATE_e status);

        virtual int doDelete();
        virtual int preDelete();
        virtual void postDelete(MAIN_STATE_e status);

        virtual int execute();
        virtual int preExecute();
        virtual void postExecute(MAIN_STATE_e status);

        virtual int draw();
        virtual int preDraw();
        virtual void postDraw(MAIN_STATE_e status);

        virtual void deleteReady();

        virtual bool entryFrmHeap(size_t size, EGG::Heap* heap);
        virtual bool entryFrmHeapNonAdjust(size_t size, EGG::Heap* heap);
        virtual bool createHeap();

        virtual ~fBase_c();

        void deleteRequest();

        static void* operator new(size_t size);
        static void operator delete(void* ptr);

        static fBase_c* createChild(u16 profileId, fBase_c* parent, ulong settings, u8 groupType);
};
static_assert(sizeof(fBase_c) == 0x64);
