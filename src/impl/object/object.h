#ifndef DEMON__IMPL_OBJECT_OBJECT_H
#define DEMON__IMPL_OBJECT_OBJECT_H

#include "../id.h"
#include "../3d.h"

typedef enum ObjectType {
    ObjectType_biped,
    ObjectType_vehicle,
    ObjectType_weapon,
    ObjectType_equipment,
    ObjectType_garbage,
    ObjectType_projectile,
    ObjectType_scenery,
    ObjectType_device_machine,
    ObjectType_device_control,
    ObjectType_device_light_fixture,
    ObjectType_device_placeholder,
    ObjectType_device_sound_scenery
} ObjectType;

typedef struct DynamicObjectBase {
    TableID tag;
    char field1_0x4;
    char field2_0x5;
    char field3_0x6;
    char field4_0x7;
    uint32_t flags_0;
    uint32_t existence_time;
    uint32_t flags_1;
    char field8_0x14;
    char field9_0x15;
    char field10_0x16;
    char field11_0x17;
    char field12_0x18;
    char field13_0x19;
    char field14_0x1a;
    char field15_0x1b;
    char field16_0x1c;
    char field17_0x1d;
    char field18_0x1e;
    char field19_0x1f;
    char field20_0x20;
    char field21_0x21;
    char field22_0x22;
    char field23_0x23;
    char field24_0x24;
    char field25_0x25;
    char field26_0x26;
    char field27_0x27;
    char field28_0x28;
    char field29_0x29;
    char field30_0x2a;
    char field31_0x2b;
    char field32_0x2c;
    char field33_0x2d;
    char field34_0x2e;
    char field35_0x2f;
    char field36_0x30;
    char field37_0x31;
    char field38_0x32;
    char field39_0x33;
    char field40_0x34;
    char field41_0x35;
    char field42_0x36;
    char field43_0x37;
    char field44_0x38;
    char field45_0x39;
    char field46_0x3a;
    char field47_0x3b;
    char field48_0x3c;
    char field49_0x3d;
    char field50_0x3e;
    char field51_0x3f;
    char field52_0x40;
    char field53_0x41;
    char field54_0x42;
    char field55_0x43;
    char field56_0x44;
    char field57_0x45;
    char field58_0x46;
    char field59_0x47;
    char field60_0x48;
    char field61_0x49;
    char field62_0x4a;
    char field63_0x4b;
    char field64_0x4c;
    char field65_0x4d;
    char field66_0x4e;
    char field67_0x4f;
    char field68_0x50;
    char field69_0x51;
    char field70_0x52;
    char field71_0x53;
    char field72_0x54;
    char field73_0x55;
    char field74_0x56;
    char field75_0x57;
    char field76_0x58;
    char field77_0x59;
    char field78_0x5a;
    char field79_0x5b;
    VectorXYZ position;
    VectorXYZ velocity;
    VectorIJK rotation[2];
    VectorPYR rotation_velocity;
    char field84_0x98;
    char field85_0x99;
    char field86_0x9a;
    char field87_0x9b;
    char field88_0x9c;
    char field89_0x9d;
    char field90_0x9e;
    char field91_0x9f;
    VectorXYZ center;
    char field93_0xac;
    char field94_0xad;
    char field95_0xae;
    char field96_0xaf;
    float scale;
    uint16_t object_type; // ObjectType
    char field99_0xb6;
    char field100_0xb7;
    char field101_0xb8;
    char field102_0xb9;
    char field103_0xba;
    char field104_0xbb;
    char field105_0xbc;
    char field106_0xbd;
    char field107_0xbe;
    char field108_0xbf;
    char field109_0xc0;
    char field110_0xc1;
    char field111_0xc2;
    char field112_0xc3;
    char field113_0xc4;
    char field114_0xc5;
    char field115_0xc6;
    char field116_0xc7;
    char field117_0xc8;
    char field118_0xc9;
    char field119_0xca;
    char field120_0xcb;
    TableID animation_tag_id;
    uint16_t animation;
    uint16_t animation_frame;
    char field124_0xd4;
    char field125_0xd5;
    char field126_0xd6;
    char field127_0xd7;
    float base_health;
    float base_shield;
    float health;
    float shield;
    float current_shield_damage;
    float current_health_damage;
    char field134_0xf0;
    char field135_0xf1;
    char field136_0xf2;
    char field137_0xf3;
    float recent_shield_damage;
    float recent_health_damage;
    uint32_t recent_shield_damage_time;
    uint32_t recent_health_damage_time;
    uint16_t shield_stun_time;
    uint16_t flags_2;
    char field144_0x108;
    char field145_0x109;
    char field146_0x10a;
    char field147_0x10b;
    char field148_0x10c;
    char field149_0x10d;
    char field150_0x10e;
    char field151_0x10f;
    char field152_0x110;
    char field153_0x111;
    char field154_0x112;
    char field155_0x113;
    char field156_0x114;
    char field157_0x115;
    char field158_0x116;
    char field159_0x117;
    TableID weapon;
    TableID parent;
    uint16_t parent_seat_index;
    char field163_0x122;
    char field164_0x123;
    char field165_0x124;
    char field166_0x125;
    char field167_0x126;
    char field168_0x127;
    char field169_0x128;
    char field170_0x129;
    char field171_0x12a;
    char field172_0x12b;
    char field173_0x12c;
    char field174_0x12d;
    char field175_0x12e;
    char field176_0x12f;
    char field177_0x130;
    char field178_0x131;
    char field179_0x132;
    char field180_0x133;
    char field181_0x134;
    char field182_0x135;
    char field183_0x136;
    char field184_0x137;
    char field185_0x138;
    char field186_0x139;
    char field187_0x13a;
    char field188_0x13b;
    char field189_0x13c;
    char field190_0x13d;
    char field191_0x13e;
    char field192_0x13f;
    char field193_0x140;
    char field194_0x141;
    char field195_0x142;
    char field196_0x143;
    char field197_0x144;
    char field198_0x145;
    char field199_0x146;
    char field200_0x147;
    char field201_0x148;
    char field202_0x149;
    char field203_0x14a;
    char field204_0x14b;
    char field205_0x14c;
    char field206_0x14d;
    char field207_0x14e;
    char field208_0x14f;
    char field209_0x150;
    char field210_0x151;
    char field211_0x152;
    char field212_0x153;
    char field213_0x154;
    char field214_0x155;
    char field215_0x156;
    char field216_0x157;
    char field217_0x158;
    char field218_0x159;
    char field219_0x15a;
    char field220_0x15b;
    char field221_0x15c;
    char field222_0x15d;
    char field223_0x15e;
    char field224_0x15f;
    char field225_0x160;
    char field226_0x161;
    char field227_0x162;
    char field228_0x163;
    char field229_0x164;
    char field230_0x165;
    char field231_0x166;
    char field232_0x167;
    char field233_0x168;
    char field234_0x169;
    char field235_0x16a;
    char field236_0x16b;
    char field237_0x16c;
    char field238_0x16d;
    char field239_0x16e;
    char field240_0x16f;
    char field241_0x170;
    char field242_0x171;
    char field243_0x172;
    char field244_0x173;
    char field245_0x174;
    char field246_0x175;
    char field247_0x176;
    char field248_0x177;
    char field249_0x178;
    char field250_0x179;
    char field251_0x17a;
    char field252_0x17b;
    char field253_0x17c;
    char field254_0x17d;
    char field255_0x17e;
    char field256_0x17f;
    char field257_0x180;
    char field258_0x181;
    char field259_0x182;
    char field260_0x183;
    char field261_0x184;
    char field262_0x185;
    char field263_0x186;
    char field264_0x187;
    ColorRGB color_change[4][2];
    char field266_0x1e8;
    char field267_0x1e9;
    char field268_0x1ea;
    char field269_0x1eb;
    char field270_0x1ec;
    char field271_0x1ed;
    char field272_0x1ee;
    char field273_0x1ef;
    char field274_0x1f0;
    char field275_0x1f1;
    char field276_0x1f2;
    char field277_0x1f3;
} DynamicObjectBase;
_Static_assert(sizeof(DynamicObjectBase) == 0x1F4);

/**
 * Resolve an object ID into a dynamic object.
 *
 * @param object_id  object ID to check (salt is checked unless salt is set to 0)
 * @param flag_check does a parity check in the entry (needs more investigation)
 *
 * @return pointer to object or NULL if object_id does not match anything
 */
DynamicObjectBase *resolve_object_id(uint32_t flag_check, TableID object_id);

/**
 * Resolve the parent ID of the object recursively (i.e. until it gets an object that has no parent).
 *
 * @param object_id
 *
 * @return parent object ID or a null object ID if object_id is null
 */
TableID resolve_parent_object_id(TableID object_id);

#endif
