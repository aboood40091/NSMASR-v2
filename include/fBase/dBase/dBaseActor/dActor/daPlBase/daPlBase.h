#include <fBase/dBase/dBaseActor/dActor/dActor.h>

class dAcPyKey_c {
    public:
        u32 _0[0x164 / sizeof(u32)];

        bool buttonWalk(int* p_dir) const;
        bool buttonDush() const;
};
static_assert(sizeof(dAcPyKey_c) == 0x164);

struct PlayerSpeedDataInner {
    float _0;
    float _4;
    float _8;
    float _c;
    float _10;
    float _14;
    float _18;
    float _1c;
    float _20;
};
static_assert(sizeof(PlayerSpeedDataInner) == 0x24);

struct PlayerSpeedData {
    float _0;
    float _4;
    float _8;
    PlayerSpeedDataInner _c;
    PlayerSpeedDataInner _30;
    PlayerSpeedDataInner _54;
};
static_assert(sizeof(PlayerSpeedData) == 0x78);

class daPlBase_c : public dActor_c {
    public:
        u32 _394[(0xEA4 - 0x394) / sizeof(u32)];
        dAcPyKey_c key;
        u32 _1008[(0x1060 - 0x1008) / sizeof(u32)];
        PlayerSpeedData* speedDataNormal;
        PlayerSpeedData* speedDataStar;
        u32 _1068[(0x14D4 - 0x1068) / sizeof(u32)];

        static const float sc_DirSpeed[2];

        bool isSaka();
        float getSakaMoveAccele(unsigned char dir);
        bool isStatus(int status_bit);
        void icePowerChange(int);
        const PlayerSpeedData* getSpeedData();
        void getSpeedDataInner(PlayerSpeedDataInner*);
        float getSakaStopAccele(unsigned char dir);
        int getPowerChangeType(bool);

};
static_assert(sizeof(daPlBase_c) == 0x14D4);
