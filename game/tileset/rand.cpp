#include <kamek.h>
#include <dBgGlobal.h>
#include <dBgUnit.h>
#include <dRes.h>
#include "tileset/rand.h"

// Define for common.cpp
#define RANDCPP

// Extern for ASM call
extern "C" {
bool DoRandTile(dBgUnit_c* unit, BGRender* render);
}

// Static instance
dRandTileMng_c* dRandTileMng_c::instance;

// Build function
dRandTileMng_c* dRandTileMng_c::build(int area) {

    // If an instance is already initialized, return it
    if (dRandTileMng_c::instance != NULL)
        return dRandTileMng_c::instance;

    // Create class and return it
    return new dRandTileMng_c(area);
}

// Constructor
dRandTileMng_c::dRandTileMng_c(int area) {

    // Process every tileset
    for (int slot = 0; slot < 4; slot++) {

        // Get tileset name
        char* tilesetName = dBgGlobal_c::instance->getEnvironment(area, slot);

        // If tileset name is valid, get data
        if (tilesetName[0] != '\0') {

            // Get the data
            RandTileBin* data = (RandTileBin*)dResMng_c::instance->res.getRes(tilesetName, RANDDATA);

            // If the data exists and the version matches, set it
            if (data != NULL && data->version == RANDSPECVERSION)
                this->randData[slot] = data;
        }
    }

    // Set static instance
    dRandTileMng_c::instance = this;
}

// Destructor
dRandTileMng_c::~dRandTileMng_c() {

    // Reset static instance
    dRandTileMng_c::instance = NULL;
}

// Get entry from data given a tile
RandTileBinEntry* GetTileFromData(RandTileBin* data, u8 tile) {

    // Process every entry
    for (int i = 0; i < data->numEntries; i++) {
        RandTileBinEntry* currEntry = &data->entries[i];

        // If tile matches, return the entry
        if (currEntry->tile == tile)
            return currEntry;
    }

    // No entry found, return none
    return NULL;
}

// Main random function
bool DoRandTile(dBgUnit_c* unit, BGRender* render) {

    // Get tile and slot
    u16 tile = render->currTile & 0x3FF;
    int slot = tile >> 8;

    // Get random data for slot
    RandTileBin* data = dRandTileMng_c::instance->randData[slot];

    // If no data is found, fall back to Nintendo's code
    if (data == NULL)
        return false;

    // Find the entry for this tile
    RandTileBinEntry* entry = GetTileFromData(data, tile & 0xFF);

    // If no tile is found, still skip Nintendo's code and render original tile
    if (entry == NULL)
        return true;

    // Initialize pointer
    u16* tilePtr;

    // Handle special tiles first
    switch(entry->specialType) {
        case VDOUBLE_BOTTOM:
            // Get pointer to top tile
            tilePtr = unit->getTileP(render->currX*16, (render->currY-1) * 16, NULL, false);

            // If pointer is not null, set the new tile
            if (tilePtr != NULL)
                render->currTile = (*tilePtr + 16) & 0xFF;
            return true;

        case HDOUBLE_RIGHT:
            // Get pointer to left tile
            tilePtr = unit->getTileP((render->currX-1) * 16, render->currY*16, NULL, false);

            // If pointer is not null, set the new tile
            if (tilePtr != NULL)
                render->currTile = (*tilePtr + 1) & 0xFF;
            return true;
        default:
            break;
    }

    // Get a pointer to the random choices list
    u16* tileArray = (u16*)((u32)data + entry->tileArrayOffs);

    // Handle regular tiles with the game's original functions (setting slot to 0 in order to insert it manually later)
    switch(entry->normalType) {
        case HORZ:
            unit->generateHorizontalTile(render, tileArray, entry->arrayLen, 0);
            break;
        case VERT:
            unit->generateVerticalTile(render, tileArray, entry->arrayLen, 0);
            break;
        case BOTH:
            unit->generateTile(render, tileArray, entry->arrayLen, 0);
            break;
        default:
            return true;
    }

    // Reload tile and add slot to it
    render->currTile |= slot * 0x100;
    return true;
}

kmCallDefAsm(0x80086B60) {
    // Let me free
    nofralloc

    // Push stack manually
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)

    // Modified original instruction
    mr r3, r29
    mr r4, r30
    bl DoRandTile

    // Restore r0 and reload LR
    lhz r0, 0xC10(r30)
    lwz r12, 0x14(r1)

    // If return is false, fall back to original call
    cmpwi r3, 0
    beq+ end

    // Else skip PTMF call
    addi r12, r12, 0x108

    // Pop stack manually and return
    end:
    mtlr r12
    addi r1, r1, 0x10
    blr
}
