#include <kamek.h>
//#include <rvl/os/OS.h>

#include <fBase/dBase/dBaseActor/dActor/daPlBase/daPlBase.h>

// Disable powerup freeze
kmWrite8(0x800BB481, 0xED);

template <typename T>
inline T ABS(T x)
{
    if (x < 0)
        x *= -1;
    return x;
}

extern "C" {

const float luigi__x_speed_inc = 0.07f;
const float luigi__swim_y_speed_add = 1.8f;

extern const float FLOAT_ARRAY_802eedc0[2];

void daPlBase_c__slipPowerSet_1(daPlBase_c* _this)
{
  if (_this->isSaka()) {
    _this->accelX = _this->getSakaMoveAccele((_this->speedF < 0.0) ? 1 : 0);
    int walk_dir;
    if (_this->key.buttonWalk(&walk_dir)) {
      _this->accelX *= FLOAT_ARRAY_802eedc0[_this->bgCheck.getSakaUpDown(walk_dir)];
    }
  }
  else {
    if (_this->isStatus(0x31)) {
      _this->accelX = 0.05;
    }
    else {
      _this->accelX = 0.09;
    }
  }
  _this->icePowerChange(1);
}

void daPlBase_c__slipPowerSet_0(daPlBase_c* _this)
{
  if (ABS(_this->speedF) > _this->getSpeedData()->_8) {
    _this->accelX = 0.75;
    return;
  }
  {
    // HACK: Patching addresses 0x802ef118 & 0x802ef208 using Kamek is not enough
    // as Kamek is not able to perform patches before the ctors of the original game run
    _this->speedDataNormal->_c._c = 0.04f;
  }
  PlayerSpeedDataInner local_4c;
  _this->getSpeedDataInner(&local_4c);
  int walk_dir;
  if (_this->key.buttonWalk(&walk_dir)) {
    if (_this->speedF * daPlBase_c::sc_DirSpeed[_this->direction] < 0.0) {
    //OSReport("powerChangeType(false): %d, powerChangeType(true): %d, local_4c._c: %f\n", _this->getPowerChangeType(false), _this->getPowerChangeType(true), local_4c._c);
      _this->accelX = local_4c._c;
      if (_this->isStatus(0x89)) {
        _this->accelX *= 3;
      }
      return;
    }
    if (_this->isSaka()) {
      _this->accelX = _this->getSakaMoveAccele((_this->speedF < 0.0) ? 1 : 0) * FLOAT_ARRAY_802eedc0[_this->bgCheck.getSakaUpDown(walk_dir)];
      _this->icePowerChange(0);
      return;
    }
    const float x_speed_1_mag = ABS(_this->speedF);
    const float x_speed_2_mag = ABS(_this->accelF);
    if (x_speed_1_mag < 0.5) {
      _this->accelX = local_4c._10;
    }
    else if (x_speed_1_mag < _this->getSpeedData()->_0) {
      if (_this->key.buttonDush()) {
        _this->accelX = local_4c._18;
      }
      else {
        _this->accelX = local_4c._14;
      }
    }
    else if (x_speed_1_mag < _this->getSpeedData()->_4) {
      if (x_speed_2_mag >= _this->getSpeedData()->_4) {
        _this->accelX = local_4c._1c;
      }
      else {
        _this->accelX = local_4c._0;
      }
    }
    else {
      if (x_speed_2_mag >= _this->getSpeedData()->_4) {
        _this->accelX = local_4c._20;
      }
      else {
        _this->accelX = local_4c._0;
      }
    }
  }
  else {
    if (_this->isSaka()) {
      _this->accelX = _this->getSakaStopAccele((_this->speedF < 0.0) ? 1 : 0);
    }
    else if (_this->speedF * daPlBase_c::sc_DirSpeed[_this->direction] < 0.0) {
      _this->accelX = local_4c._8;
    }
    else if (ABS(_this->speedF) < _this->getSpeedData()->_0) {
      _this->accelX = local_4c._4;
    }
    else {
      _this->accelX = local_4c._0;
    }
    if (_this->isStatus(0x89)) {
      _this->accelX *= 3;
    }
    _this->icePowerChange(0);
  }
}

}

kmBranchDefAsm(0x8004c394, 0x8004c398) {

    // No stack saving needed
    nofralloc

    lis       r3, luigi__x_speed_inc@ha
    lfs       f0, luigi__x_speed_inc@l(r3)
    blr
}

kmBranchDefAsm(0x801307fc, 0x80130800) { // TODO: Patch check at 8013080c

    // No stack saving needed
    nofralloc

    lis       r3, luigi__swim_y_speed_add@ha
    lfs       f1, luigi__swim_y_speed_add@l(r3)
    blr
}

kmCall(0x80135290, daPlBase_c__slipPowerSet_0); // Originally daPlBase_c::normalPowerSet()

kmCall(0x8004ba98, daPlBase_c__slipPowerSet_1); // Originally daPlBase_c::slipPowerSet(1)
kmCall(0x8004baa8, daPlBase_c__slipPowerSet_0); // Originally daPlBase_c::slipPowerSet(0)

kmWrite32(0x802ef118, 0x3D23D70A);  // 0.04f
kmWrite32(0x802ef208, 0x3D23D70A);  // ^^^

kmWrite32(0x802f591c, 0x3FCCCCCD); // 1.6f
kmWrite32(0x802f5920, 0x3F800000); // 1.0f
kmWrite32(0x802f5924, 0x3E800000); // 0.25f
kmWrite32(0x802f5928, 0xBE3851EC); // -0.18f
kmWrite32(0x802f5930, 0xBE3851EC); // ^^^
kmWrite32(0x802f5934, 0xBE800000); // -0.25f
kmWrite32(0x802f5950, 0xBE99999A); // -0.3f
kmWrite32(0x802f5954, 0xBCF5C28F); // -0.03f
kmWrite32(0x802f5958, 0xBE800000); // -0.25f
kmWrite32(0x802f5964, 0x40000000); // 2.0f
kmWrite32(0x802f5968, 0x3F99999A); // 1.2f
kmWrite32(0x802f5990, 0xBD0F5C29); // -0.035f
kmWrite32(0x802f5994, 0xBD4CCCCD); // -0.05f
kmWrite32(0x802f5998, 0xBD99999A); // -0.075f
// I don't think the following are relevant
kmWrite32(0x802f59c0, 0xBE3851EC); // -0.18f
kmWrite32(0x802f59c4, 0xBE800000); // -0.25f
kmWrite32(0x802f59dc, 0xBDCCCCCD); // -0.1f
kmWrite32(0x802f59e0, 0xBDCCCCCD); // ^^^
kmWrite32(0x802f59e4, 0xBDA3D70A); // -0.08f
kmWrite32(0x802f59ec, 0xBEAE147B); // -0.34f

kmWrite32(0x802f58b8, 0x3D4CCCCD); // 0.05f
kmWrite32(0x802f58bc, 0x3D4CCCCD); // ^^^
kmWrite32(0x802f58c0, 0x3D23D70A); // 0.04f

kmWrite32(0x8042db68, 0xBD4CCCCD); // -0.05f
kmWrite32(0x8042db70, 0xBD0F5C29); // -0.035f
kmWrite32(0x8042db6c, 0xBDA7EF9E); // -0.082f
kmWrite32(0x8042bb3c, 0x40000000); // 2.0f
