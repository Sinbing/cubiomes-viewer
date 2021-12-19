#ifndef SEARCH_H
#define SEARCH_H

#include "cubiomes/finders.h"

#include <QVector>
#include <atomic>

#define PRECOMPUTE48_BUFSIZ ((int64_t)1 << 30)

enum
{
    CAT_NONE,
    CAT_HELPER,
    CAT_QUAD,
    CAT_STRUCT,
    CAT_BIOMES,
    CAT_NETHER,
    CAT_END,
    CAT_OTHER,
    CATEGORY_MAX,
};

struct FilterInfo
{
    int cat;    // seed source category
    bool dep64; // depends on 64-bit seed
    bool coord; // requires coordinate entry
    bool area;  // requires area entry
    int layer;  // associated generator layer
    int stype;  // structure type
    int step;   // coordinate multiplier
    int count;  //
    int mcmin;  // minimum version
    int mcmax;  // maximum version
    int dim;    // dimension
    int hasy;   // has vertical height
    int disp;   // display order
    const char *icon;
    const char *name;
    const char *description;
};

enum
{
    F_SELECT,
    F_QH_IDEAL,
    F_QH_CLASSIC,
    F_QH_NORMAL,
    F_QH_BARELY,
    F_QM_95,
    F_QM_90,
    F_BIOME,
    F_BIOME_4_RIVER,
    F_BIOME_16,
    F_BIOME_64,
    F_BIOME_256,
    F_BIOME_256_OTEMP,
    F_TEMPS,
    F_SLIME,
    F_SPAWN,
    F_STRONGHOLD,
    F_DESERT,
    F_JUNGLE,
    F_HUT,
    F_IGLOO,
    F_MONUMENT,
    F_VILLAGE,
    F_OUTPOST,
    F_MANSION,
    F_TREASURE,
    F_RUINS,
    F_SHIPWRECK,
    F_PORTAL,
    F_FORTRESS,
    F_BASTION,
    F_ENDCITY,
    F_BIOME_NETHER_1,
    F_BIOME_NETHER_4,
    F_BIOME_NETHER_16,
    F_BIOME_NETHER_64,
    F_BIOME_NETHER_256,
    F_BIOME_END_1,
    F_BIOME_END_4,
    F_BIOME_END_16,
    F_BIOME_END_64,
    F_PORTALN,
    F_GATEWAY,
    F_MINESHAFT,
    // possibly add scales: 4, 240, 256, 320, 384, 400, 432, 512, 1280
    F_REFERENCE_1,
    F_REFERENCE_16,
    F_REFERENCE_64,
    F_REFERENCE_256,
    F_REFERENCE_512,
    F_REFERENCE_1024,
    F_BIOME_4, // differs from F_BIOME_4_RIVER, since this may include oceans
    F_SCALE_TO_NETHER,
    F_SCALE_TO_OVERWORLD,
    // new filters should be added here at the end to keep some downwards compatibility
    FILTER_MAX,
};

// global table of filter data (as constants with enum indexing)
static const struct FilterList
{
    FilterInfo list[FILTER_MAX];

    FilterList() : list{}
    {
        int disp = 0; // display order

        list[F_SELECT] = FilterInfo{
            CAT_NONE, 0, 0, 0, 0, 0, 0, 0, MC_1_0, MC_NEWEST, 0, 0, disp++,
            NULL,
            "",
            ""
        };

        const char *ref_desc =
            "<html><head/><body><p>"
            "(朕的专用翻译官罢工了)"
            "</p><p>"
            "A basic search will test each condition once, where relative "
            "locations refer to the center of their parent. "
            "This can be undesirable if the parent can have multiple triggering "
            "instances. To address this, a <b>reference point</b> traverses an "
            "origin-aligned grid that can be referred to by other conditions."
            "</p><p>"
            "Example - \"Double Swamp Hut\":"
            "</p><table>"
            "<tr><td>[1]</td><td width=\"10\"/><td>"
            "Reference point with scale 1:512 for a square of side 3."
            "</td></tr><tr><td/><td/><td>"
            "(traverses the structure regions for the range -1024 to 1023)"
            "</td></tr><tr><td>[2]</td><td/><td>"
            "Swamp Hut x1 in blocks (0,0) to (511,511) relative to [1]."
            "</td></tr><tr><td/><td/><td>"
            "(gets the position of the single swamp hut inside the region)"
            "</td></tr><tr><td>[3]</td><td/><td>"
            "Swamp Hut x2 in a 128 block square relative to [2]."
            "</td></tr><tr><td/><td/><td>"
            "(checks there is a second swamp hut in range of the first)"
            "</td></tr></table></body></html>";

        list[F_REFERENCE_1] = FilterInfo{
            CAT_HELPER, 0, 1, 1, 0, 0, 1, 0, MC_1_0, MC_NEWEST, 0, 0, disp++,
            ":icons/reference.png",
            "参照点 1:1",
            ref_desc
        };
        list[F_REFERENCE_16] = FilterInfo{
            CAT_HELPER, 0, 1, 1, 0, 0, 16, 0, MC_1_0, MC_NEWEST, 0, 0, disp++,
            ":icons/reference.png",
            "参照点 1:16",
            ref_desc
        };
        list[F_REFERENCE_64] = FilterInfo{
            CAT_HELPER, 0, 1, 1, 0, 0, 64, 0, MC_1_0, MC_NEWEST, 0, 0, disp++,
            ":icons/reference.png",
            "参照点 1:64",
            ref_desc
        };
        list[F_REFERENCE_256] = FilterInfo{
            CAT_HELPER, 0, 1, 1, 0, 0, 256, 0, MC_1_0, MC_NEWEST, 0, 0, disp++,
            ":icons/reference.png",
            "参照点 1:256",
            ref_desc
        };
        list[F_REFERENCE_512] = FilterInfo{
            CAT_HELPER, 0, 1, 1, 0, 0, 512, 0, MC_1_0, MC_NEWEST, 0, 0, disp++,
            ":icons/reference.png",
            "参照点 1:512",
            ref_desc
        };
        list[F_REFERENCE_1024] = FilterInfo{
            CAT_HELPER, 0, 1, 1, 0, 0, 1024, 0, MC_1_0, MC_NEWEST, 0, 0, disp++,
            ":icons/reference.png",
            "参照点 1:1024",
            ref_desc
        };
        list[F_SCALE_TO_NETHER] = FilterInfo{
            CAT_HELPER, 0, 0, 0, 0, 0, 1, 0, MC_1_0, MC_NEWEST, 0, 0, disp++,
            ":icons/portal.png",
            "坐标系数 x/8",
            "将坐标除以8，用作主世界到地狱的坐标转换。"
        };
        list[F_SCALE_TO_OVERWORLD] = FilterInfo{
            CAT_HELPER, 0, 0, 0, 0, 0, 1, 0, MC_1_0, MC_NEWEST, 0, 0, disp++,
            ":icons/portal.png",
            "坐标系数 x*8",
            "将坐标乘以8，用作地狱到主世界的坐标转换."
        };

        list[F_QH_IDEAL] = FilterInfo{
            CAT_QUAD, 0, 1, 1, 0, Swamp_Hut, 512, 0, MC_1_4, MC_NEWEST, 0, 0, disp++,
            ":icons/quad.png",
            "四联女巫小屋(理想型)",
            "种子的低48位(二进制)使得该种子具有成为最佳配置的四联女巫小屋的可能性"
        };

        list[F_QH_CLASSIC] = FilterInfo{
            CAT_QUAD, 0, 1, 1, 0, Swamp_Hut, 512, 0, MC_1_4, MC_NEWEST, 0, 0, disp++,
            ":icons/quad.png",
            "四联女巫小屋(经典型)",
            "种子的低48位(二进制)使得该种子具有成为经典配置的四联女巫小屋的可能性"
        };

        list[F_QH_NORMAL] = FilterInfo{
            CAT_QUAD, 0, 1, 1, 0, Swamp_Hut, 512, 0, MC_1_4, MC_NEWEST, 0, 0, disp++,
            ":icons/quad.png",
            "四联女巫小屋(普通型)",
            "种子的低48位(二进制)使得该种子具有成为普通配置的四联女巫小屋的可能性"
            "(保证4个小屋都在单人挂机距离内并且都有足够的垂直空间来摔死女巫)"
        };

        list[F_QH_BARELY] = FilterInfo{
            CAT_QUAD, 0, 1, 1, 0, Swamp_Hut, 512, 0, MC_1_4, MC_NEWEST, 0, 0, disp++,
            ":icons/quad.png",
            "四联女巫小屋(勉强型)",
            "种子的低48位(二进制)使得该种子具有成为普通配置的四联女巫小屋的可能性"
            "(只能保证4个小屋都在单人挂机距离内)"
        };

        list[F_QM_95] = FilterInfo{
            CAT_QUAD, 0, 1, 1, 0, Monument, 512, 0, MC_1_8, MC_NEWEST, 0, 0, disp++,
            ":icons/quad.png",
            "四联海底神殿(>95%)",
            "种子的低48位(二进制)使得该种子具有成为四联海底神殿的可能性，"
            "并且有超过95%的面积落在单人挂机距离内"
        };

        list[F_QM_90] = FilterInfo{
            CAT_QUAD, 0, 1, 1, 0, Monument, 512, 0, MC_1_8, MC_NEWEST, 0, 0, disp++,
            ":icons/quad.png",
            "四联海底神殿(>90%)",
            "种子的低48位(二进制)使得该种子具有成为四联海底神殿的可能性，"
            "并且有超过90%的面积落在单人挂机距离内"
        };

        list[F_BIOME] = FilterInfo{
            CAT_BIOMES, 1, 1, 1, L_VORONOI_1, 0, 1, 0, MC_1_0, MC_1_17, 0, 1, disp++, // disable for 1.18
            ":icons/map.png",
            "群系筛选 1:1",
            "在指定范围内包括所有你想要的(+)群系并排除所有你不要的(-)"
        };

        list[F_BIOME_4] = FilterInfo{
            CAT_BIOMES, 1, 1, 1, 0, 0, 4, 0, MC_1_0, MC_NEWEST, 0, 1, disp++,
            ":icons/map.png",
            "群系筛选 1:4",
            "在指定范围内包括所有你想要的(+)群系并排除所有你不要的(-)"
        };
        list[F_BIOME_16] = FilterInfo{
            CAT_BIOMES, 1, 1, 1, 0, 0, 16, 0, MC_1_0, MC_NEWEST, 0, 1, disp++,
            ":icons/map.png",
            "群系筛选 1:16",
            "在指定范围内包括所有你想要的(+)群系并排除所有你不要的(-)"
        };
        list[F_BIOME_64] = FilterInfo{
            CAT_BIOMES, 1, 1, 1, 0, 0, 64, 0, MC_1_0, MC_NEWEST, 0, 1, disp++,
            ":icons/map.png",
            "群系筛选 1:64",
            "在指定范围内包括所有你想要的(+)群系并排除所有你不要的(-)"
        };
        list[F_BIOME_256] = FilterInfo{
            CAT_BIOMES, 1, 1, 1, 0, 0, 256, 0, MC_1_0, MC_NEWEST, 0, 1, disp++,
            ":icons/map.png",
            "群系筛选 1:256",
            "在指定范围内包括所有你想要的(+)群系并排除所有你不要的(-)"
        };

        list[F_BIOME_4_RIVER] = FilterInfo{
            CAT_BIOMES, 1, 1, 1, L_RIVER_MIX_4, 0, 4, 0, MC_1_13, MC_1_17, 0, 0, disp++,
            ":icons/map.png",
            "群系筛选 1:4 河流",
            "在指定范围内包括所有你想要的(+)群系并排除所有你不要的(-)"
            "但是只生成到1:4层的河流为止，不生成海洋变种"
        };
        list[F_BIOME_256_OTEMP] = FilterInfo{
            CAT_BIOMES, 0, 1, 1, L_OCEAN_TEMP_256, 0, 256, 0, MC_1_13, MC_1_17, 0, 0, disp++,
            ":icons/map.png",
            "群系筛选 1:256 海洋温度",
            "在指定范围内包括所有你想要的(+)群系并排除所有你不要的(-)"
            "仅生成到决定海洋温度的1:256. "
            "这部分群系生成仅由种子低48位决定."
        };
        list[F_TEMPS] = FilterInfo{
            CAT_BIOMES, 1, 1, 1, 0, 0, 1024, 0, MC_1_7, MC_1_17, 0, 0, disp++,
            ":icons/tempcat.png",
            "温度类别",
            "检查这块区域是否包含大于等于你所指定的数目的温度群系"
        };

        list[F_BIOME_NETHER_1] = FilterInfo{
            CAT_NETHER, 1, 1, 1, 0, 0, 1, 0, MC_1_16, 0, -1, 1, disp++, // disabled
            ":icons/nether.png",
            "下界群系筛选 1:1 (disabled)",
            "通过泰森多边形法算到 1:1 的下界群系"
        };
        list[F_BIOME_NETHER_4] = FilterInfo{
            CAT_NETHER, 0, 1, 1, 0, 0, 4, 0, MC_1_16, MC_NEWEST, -1, 0, disp++,
            ":icons/nether.png",
            "下界群系筛选 1:4",
            "使用正常噪声生成后在 1:4 的尺度下取样的下界群系"
        };
        list[F_BIOME_NETHER_16] = FilterInfo{
            CAT_NETHER, 0, 1, 1, 0, 0, 16, 0, MC_1_16, MC_NEWEST, -1, 0, disp++,
            ":icons/nether.png",
            "下界群系筛选 1:16",
            "使用正常噪声生成后在 1:16 的尺度下取样的下界群系"
        };
        list[F_BIOME_NETHER_64] = FilterInfo{
            CAT_NETHER, 0, 1, 1, 0, 0, 64, 0, MC_1_16, MC_NEWEST, -1, 0, disp++,
            ":icons/nether.png",
            "下界群系筛选 1:64",
            "使用正常噪声生成后在 1:64 的尺度下取样的下界群系"
        };
        list[F_BIOME_NETHER_256] = FilterInfo{
            CAT_NETHER, 0, 1, 1, 0, 0, 256, 0, MC_1_16, MC_NEWEST, -1, 0, disp++,
            ":icons/nether.png",
            "下界群系筛选 1:256",
            "使用正常噪声生成后在 1:256 的尺度下取样的下界群系"
        };

        list[F_BIOME_END_1] = FilterInfo{
            CAT_END, 1, 1, 1, 0, 0, 1, 0, MC_1_9, 0, +1, 1, disp++, // disabled
            ":icons/the_end.png",
            "末地群系筛选 1:1 (disabled)",
            "通过泰森多边形法算到 1:1 的末地群系"
        };
        list[F_BIOME_END_4] = FilterInfo{
            CAT_END, 0, 1, 1, 0, 0, 4, 0, MC_1_9, MC_NEWEST, +1, 0, disp++,
            ":icons/the_end.png",
            "末地群系筛选 1:4",
            "在 1:4 尺度下取样的末地群系生成。注意其只是 1:16 的放大"
        };
        list[F_BIOME_END_16] = FilterInfo{
            CAT_END, 0, 1, 1, 0, 0, 16, 0, MC_1_9, MC_NEWEST, +1, 0, disp++,
            ":icons/the_end.png",
            "末地群系筛选 1:16",
            "在 1:16 尺度下正常取样的末地群系生成"
        };
        list[F_BIOME_END_64] = FilterInfo{
            CAT_END, 0, 1, 1, 0, 0, 64, 0, MC_1_9, MC_NEWEST, +1, 0, disp++,
            ":icons/the_end.png",
            "末地群系筛选 1:64",
            "在 1:64 尺度下松散取样的末地群系生成"
        };

        list[F_SPAWN] = FilterInfo{
            CAT_OTHER, 1, 1, 1, 0, 0, 1, 0, MC_1_0, MC_NEWEST, 0, 0, disp++,
            ":icons/spawn.png",
            "出生点",
            ""
        };

        list[F_SLIME] = FilterInfo{
            CAT_OTHER, 0, 1, 1, 0, 0, 16, 1, MC_1_0, MC_NEWEST, 0, 0, disp++,
            ":icons/slime.png",
            "史莱姆区块",
            ""
        };

        list[F_STRONGHOLD] = FilterInfo{
            CAT_STRUCT, 1, 1, 1, 0, 0, 1, 1, MC_1_0, MC_NEWEST, 0, 0, disp++,
            ":icons/stronghold.png",
            "要塞",
            ""
        };

        list[F_VILLAGE] = FilterInfo{
            CAT_STRUCT, 1, 1, 1, 0, Village, 1, 1, MC_1_0, MC_NEWEST, 0, 0, disp++,
            ":icons/village.png",
            "村庄",
            ""
        };

        list[F_MINESHAFT] = FilterInfo{
            CAT_STRUCT, 1, 1, 1, 0, Mineshaft, 1, 1, MC_1_0, MC_NEWEST, 0, 0, disp++,
            ":icons/mineshaft.png",
            "废弃矿井",
            ""
        };

        list[F_DESERT] = FilterInfo{
            CAT_STRUCT, 1, 1, 1, 0, Desert_Pyramid, 1, 1, MC_1_3, MC_NEWEST, 0, 0, disp++,
            ":icons/desert.png",
            "沙漠神殿",
            "注意，在1.18中，林地府邸、沙漠神殿和丛林神殿的生成还会考虑其表面的高度，"
            "所以其在（含水）洞穴、河流或者海洋群系（甚至是较高的沙丘）周围可能会生成失败"
        };

        list[F_JUNGLE] = FilterInfo{
            CAT_STRUCT, 1, 1, 1, 0, Jungle_Temple, 1, 1, MC_1_3, MC_NEWEST, 0, 0, disp++,
            ":icons/jungle.png",
            "丛林神殿",
            "注意，在1.18中，林地府邸、沙漠神殿和丛林神殿的生成还会考虑其表面的高度，"
            "所以其在（含水）洞穴、河流或者海洋群系周围可能会生成失败"
        };

        list[F_HUT] = FilterInfo{
            CAT_STRUCT, 1, 1, 1, 0, Swamp_Hut, 1, 1, MC_1_4, MC_NEWEST, 0, 0, disp++,
            ":icons/hut.png",
            "女巫小屋",
            ""
        };

        list[F_MONUMENT] = FilterInfo{
            CAT_STRUCT, 1, 1, 1, 0, Monument, 1, 1, MC_1_8, MC_NEWEST, 0, 0, disp++,
            ":icons/monument.png",
            "海底神殿",
            ""
        };

        list[F_IGLOO] = FilterInfo{
            CAT_STRUCT, 1, 1, 1, 0, Igloo, 1, 1, MC_1_9, MC_NEWEST, 0, 0, disp++,
            ":icons/igloo.png",
            "冰屋",
            ""
        };

        list[F_MANSION] = FilterInfo{
            CAT_STRUCT, 1, 1, 1, 0, Mansion, 1, 1, MC_1_11, MC_NEWEST, 0, 0, disp++,
            ":icons/mansion.png",
            "林地府邸",
            "注意，在1.18中，林地府邸、沙漠神殿和丛林神殿的生成还会考虑其表面的高度，"
            "所以其在（含水）洞穴、河流或者海洋群系周围可能会生成失败"
        };

        list[F_RUINS] = FilterInfo{
            CAT_STRUCT, 1, 1, 1, 0, Ocean_Ruin, 1, 1, MC_1_13, MC_NEWEST, 0, 0, disp++,
            ":icons/ruins.png",
            "海底遗迹",
            ""
        };

        list[F_SHIPWRECK] = FilterInfo{
            CAT_STRUCT, 1, 1, 1, 0, Shipwreck, 1, 1, MC_1_13, MC_NEWEST, 0, 0, disp++,
            ":icons/shipwreck.png",
            "沉船",
            ""
        };

        list[F_TREASURE] = FilterInfo{
            CAT_STRUCT, 1, 1, 1, 0, Treasure, 1, 1, MC_1_13, MC_NEWEST, 0, 0, disp++,
            ":icons/treasure.png",
            "宝藏",
            ""
        };

        list[F_OUTPOST] = FilterInfo{
            CAT_STRUCT, 1, 1, 1, 0, Outpost, 1, 1, MC_1_14, MC_NEWEST, 0, 0, disp++,
            ":icons/outpost.png",
            "掠夺者前哨站",
            ""
        };

        list[F_PORTAL] = FilterInfo{
            CAT_STRUCT, 0, 1, 1, 0, Ruined_Portal, 1, 1, MC_1_16, MC_NEWEST, 0, 0, disp++,
            ":icons/portal.png",
            "废弃的传送门(主世界)",
            ""
        };

        list[F_PORTALN] = FilterInfo{
            CAT_STRUCT, 0, 1, 1, 0, Ruined_Portal_N, 1, 1, MC_1_16, MC_NEWEST, -1, 0, disp++,
            ":icons/portal.png",
            "废弃的传送门(下界)",
            ""
        };

        list[F_FORTRESS] = FilterInfo{
            CAT_STRUCT, 0, 1, 1, 0, Fortress, 1, 1, MC_1_0, MC_NEWEST, -1, 0, disp++,
            ":icons/fortress.png",
            "下界堡垒",
            ""
        };

        list[F_BASTION] = FilterInfo{
            CAT_STRUCT, 0, 1, 1, 0, Bastion, 1, 1, MC_1_16, MC_NEWEST, -1, 0, disp++,
            ":icons/bastion.png",
            "堡垒遗迹",
            ""
        };

        list[F_ENDCITY] = FilterInfo{
            CAT_STRUCT, 0, 1, 1, 0, End_City, 1, 1, MC_1_9, MC_NEWEST, +1, 0, disp++,
            ":icons/endcity.png",
            "末地城",
            ""
        };

        list[F_GATEWAY] = FilterInfo{
            CAT_STRUCT, 0, 1, 1, 0, End_Gateway, 1, 1, MC_1_13, MC_NEWEST, +1, 0, disp++,
            ":icons/gateway.png",
            "末地(返程)折跃门",
            "这里特指返程折跃门，并非那些你打龙开的折跃门"
            ""
        };
    }
}
g_filterinfo;


struct Condition
{
    int type;
    int x1, z1, x2, z2;
    int save;
    int relative;
    BiomeFilter bfilter;
    int temps[9];
    int count;
    int y;
    int approx;
    int pad; // unused
    uint64_t variants;

    enum { // variant flags
        START_PIECE_MASK = (1ULL << 63),
        ABANDONED_MASK   = (1ULL << 62),
    };
    static int toVariantBit(int biome, int variant) {
        int bit = 0;
        switch (biome) {
        case meadow:
        case plains:        bit = 1; break;
        case desert:        bit = 2; break;
        case savanna:       bit = 3; break;
        case taiga:         bit = 4; break;
        case snowy_tundra:  bit = 5; break;
        }
        return (bit << 3) + variant;
    }
    static void fromVariantBit(int bit, int *biome, int *variant) {
        *variant = bit & 0x7;
        switch (bit >> 3) {
        case 1: *biome = plains; break;
        case 2: *biome = desert; break;
        case 3: *biome = savanna; break;
        case 4: *biome = taiga; break;
        case 5: *biome = snowy_tundra; break;
        }
    }
    inline bool villageOk(int mc, StructureVariant sv) {
        if ((variants & ABANDONED_MASK) && !sv.abandoned) return false;
        if (mc < MC_1_14) return true;
        if (!(variants & START_PIECE_MASK)) return true;
        uint64_t mask = 1ULL << toVariantBit(sv.biome, sv.variant);
        return mask & variants;
    }
};

struct StructPos
{
    StructureConfig sconf;
    int cx, cz; // effective center position
};


struct WorldGen
{
    Generator g;
    SurfaceNoise sn;

    int mc, large;
    uint64_t seed;
    bool initsurf;

    void init(int mc, bool large)
    {
        this->mc = mc;
        this->large = large;
        this->seed = 0;
        initsurf = false;
        setupGenerator(&g, mc, large);
    }

    void setSeed(uint64_t seed)
    {
        this->seed = seed;
    }

    void init4Dim(int dim)
    {
        uint64_t mask = (dim == 0 ? ~0ULL : MASK48);
        if (dim != g.dim || (seed & mask) != (g.seed & mask))
        {
            applySeed(&g, dim, seed);
            initsurf = false;
        }
        else if (g.mc >= MC_1_15 && seed != g.seed)
        {
            g.sha = getVoronoiSHA(seed);
        }
    }

    void setSurfaceNoise()
    {
        if (!initsurf)
        {
            initSurfaceNoiseEnd(&sn, seed);
            initsurf = true;
        }
    }
};


enum
{
    COND_FAILED = 0,            // seed does not meet the condition
    COND_MAYBE_POS_INVAL = 1,   // search pass insufficient for result
    COND_MAYBE_POS_VALID = 2,   // search pass insufficient, but known center
    COND_OK = 3,                // seed satisfies the condition
};

enum
{
    PASS_FAST_48,       // only do fast checks that do not require biome gen
    PASS_FULL_48,       // include possible biome checks for 48-bit seeds
    PASS_FULL_64,       // run full test on a 64-bit seed
};

/* Checks if a seeds satisfies the conditions vector.
 * Returns the lowest condition check status.
 */
int testSeedAt(
    Pos                         at,             // origin for conditions
    Pos                         cpos[100],      // [out] condition centers
    QVector<Condition>        * condvec,        // conditions vector
    int                         pass,           // final search pass
    WorldGen                  * gen,            // buffer for generator
    std::atomic_bool          * abort           // search abort signals
);

int testCondAt(
    Pos                         at,             // relative origin
    Pos                       * cent,           // output center position
    Condition                 * cond,           // condition to check
    int                         pass,
    WorldGen                  * gen,
    std::atomic_bool          * abort
);

struct QuadInfo
{
    uint64_t c; // constellation seed
    Pos p[4];   // individual positions
    Pos afk;    // optimal afk position
    int flt;    // filter id (quality)
    int typ;    // type of structure
    int spcnt;  // number of planar spawning spaces
    float rad;  // enclosing radius
};

void findQuadStructs(int styp, Generator *g, QVector<QuadInfo> *out);


#endif // SEARCH_H
