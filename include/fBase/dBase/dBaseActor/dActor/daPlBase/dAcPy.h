#include <fBase/dBase/dBaseActor/dActor/daPlBase/daPlBase.h>
#include <m/m3d/anmchr.h>

class dPyMdlBase_c {
    public:
        dPyMdlBase_c(unsigned char);
        virtual ~dPyMdlBase_c();

        virtual int getFaceJointIdx() const;
        virtual void createModel();
        virtual void initialize();
        virtual void play();
        virtual void _calc();
        virtual void calc2();
        virtual void draw();
        virtual void* getBodyMdl();
        virtual const void* getAnmResFile() const;
        virtual void setPlayerMode(int);
        virtual void setColorType(unsigned char);
        virtual void setDark(int);
        virtual void vf3C();
        virtual void onStarAnm();
        virtual void offStarAnm();
        virtual void onStarEffect();
        virtual void offStarEffect();
        virtual void getJointMtx(mMtx_c*, int);
        virtual bool getHeadPropelJointMtx(mMtx_c*);
        virtual bool vf58(int, char*, int);
        virtual void setAnm(int anmID, float rate, float blendDuration, float frame);
        // ...

        bool isAnmStop() const
        {
            return anm.isStop();
        }

        float getAnmFrameMax() const
        {
            return anm.numFrames;
        }

        float checkAnmFrame(float frame) const
        {
            return anm.checkFrame(frame);
        }

        u32 _4[(0x28 - 4) / sizeof(u32)];
        m3d::anmChr_c anm;
        u32 _60[(0x20C - 0x60) / sizeof(u32)];
};
static_assert(sizeof(dPyMdlBase_c) == 0x20c);

class dPyAnm_HIO_c {
    public:
        unsigned char id;
        float rate;
        float blendDuration;
};
static_assert(sizeof(dPyAnm_HIO_c) == 0xC);

class dPyAnmMain_HIO_c {
    public:
        dPyAnm_HIO_c pyAnm_HIO[177];
};
static_assert(sizeof(dPyAnmMain_HIO_c) == 0x84C);

class dPyMdlBase_HIO_c {
    public:
        u32 _0[0x28 / sizeof(u32)];
        dPyAnmMain_HIO_c pyAnmMain_HIO;
        u32 _874[(0x950 - 0x874) / sizeof(u32)];
};
static_assert(sizeof(dPyMdlBase_HIO_c) == 0x950);

class dPyMdlMng_c {
    public:
        enum ModelType_e {
        };

        dPyMdlMng_c(ModelType_e);
        virtual ~dPyMdlMng_c();

        static dPyMdlBase_HIO_c m_hio;

        static float getAnmRate(int anmID) {
            return m_hio.pyAnmMain_HIO.pyAnm_HIO[anmID].rate;
        }
        
        static float getAnmBlendDuration(int anmID) {
            return m_hio.pyAnmMain_HIO.pyAnm_HIO[anmID].blendDuration;
        }

        void setAnm(int anmID, float rate, float blendDuration, float frame) {
            pMdlBase->setAnm(
                anmID,
                rate,
                blendDuration,
                frame
            );
        }
        
        void setAnm(int anmID, float blendDuration, float frame) {
            pMdlBase->setAnm(
                anmID,
                getAnmRate(anmID),
                blendDuration,
                frame
            );
        }
        
        void setAnm(int anmID, float frame) {
            pMdlBase->setAnm(
                anmID,
                getAnmRate(anmID),
                getAnmBlendDuration(anmID),
                frame
            );
        }

        dPyMdlBase_c* pMdlBase;
        u32 _8;
};
static_assert(sizeof(dPyMdlMng_c) == 0xC);

class dAcPy_c : public daPlBase_c {
    public:
        u32 _14d4[(0x1564 - 0x14D4) / sizeof(u32)];
        int jumpSoundRelated;
        u32 _1568[(0x27CC - 0x1568) / sizeof(u32)];
        u8 _27cc; // Some sort of direction
        u32 _27d0[(0x2A60 - 0x27D0) / sizeof(u32)];
        dPyMdlMng_c mdlMng;
        u32 _2a6c[(0x2BA8 - 0x2A6C) / sizeof(u32)];

        void setWaterWalkFlag();
        void jumpExecAir();

        // Actually a virtual function of dPlBase_c, but we can guarantee that
        // instances of dAcPy_c will definitely call this one specifically
        int isStar() const;
};
static_assert(sizeof(dAcPy_c) == 0x2BA8);
