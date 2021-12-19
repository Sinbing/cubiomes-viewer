#ifndef CUTIL_H
#define CUTIL_H

#include <QMutex>
#include <QString>

#include <random>

#include "cubiomes/finders.h"
#include "cubiomes/util.h"

extern unsigned char biomeColors[256][3];
extern unsigned char tempsColors[256][3];


inline const char* struct2str(int stype)
{
    switch (stype)
    {
    case Desert_Pyramid:    return "沙漠神殿";
    case Jungle_Temple:     return "丛林神殿";
    case Swamp_Hut:         return "女巫小屋";
    case Igloo:             return "冰屋";
    case Village:           return "村庄";
    case Ocean_Ruin:        return "海底遗迹";
    case Shipwreck:         return "沉船";
    case Monument:          return "海底神殿";
    case Mansion:           return "林地府邸";
    case Outpost:           return "掠夺者前哨站";
    case Ruined_Portal:     return "废弃传送门";
    case Ruined_Portal_N:   return "废弃传送门(下界)";
    case Treasure:          return "宝藏";
    case Mineshaft:         return "废弃矿坑";
    case Fortress:          return "下界要塞";
    case Bastion:           return "堡垒遗迹";
    case End_City:          return "末地城";
    case End_Gateway:       return "末地(返程)折跃门";
    }
    return "?";
}


// get a random 64-bit integer
static inline uint64_t getRnd64()
{
    static QMutex mutex;
    static std::random_device rd;
    static std::mt19937_64 mt(rd());
    static uint64_t x = (uint64_t) time(0);
    uint64_t ret = 0;
    mutex.lock();
    if (rd.entropy())
    {
        std::uniform_int_distribution<int64_t> d;
        ret = d(mt);
    }
    else
    {
        const uint64_t c = 0xd6e8feb86659fd93ULL;
        x ^= x >> 32;
        x *= c;
        x ^= x >> 32;
        x *= c;
        x ^= x >> 32;
        ret = x;
    }
    mutex.unlock();
    return ret;
}

enum { S_TEXT, S_NUMERIC, S_RANDOM };
inline int str2seed(const QString &str, uint64_t *out)
{
    if (str.isEmpty())
    {
        *out = getRnd64();
        return S_RANDOM;
    }

    bool ok = false;
    *out = (uint64_t) str.toLongLong(&ok);
    if (ok)
    {
        return S_NUMERIC;
    }

    // String.hashCode();
    int32_t hash = 0;
    const ushort *chars = str.utf16();
    for (int i = 0; chars[i] != 0; i++)
    {
        hash = 31 * hash + chars[i];
    }
    *out = hash;
    return S_TEXT;
}

#endif // CUTIL_H
