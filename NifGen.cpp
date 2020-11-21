// Program execution begins and ends here.
//

#include <iostream>

#include "AltNum\MediumDec.hpp"
using MediumDec = BlazesRusCode::MediumDec;

#include "AltNum\AltNumDebug.hpp"
#include "AltNum\FloatingOperations.hpp"
#include "Databases\MediumDecFormula.hpp"
#include "tsl/ordered_map.h"
using MediumDecFormula = BlazesRusCode::MediumDecFormula;

/// <summary>
/// Class NifVersion.
/// </summary>
class NifVersion
{
    std::string VerNum;
};

//class BasicTypeTag
//{
//    std::string Desc;
//};

/// <summary>
/// Storage type of target value
/// </summary>
enum class BasicType :unsigned __int8
{
    uint64Type = 0,//(unsigned int64)An unsigned 64 - bit integer.(integral="true" countable="true" size="8")
    int64Type,//(signed int64)An signed 64 - bit integer.(integral="true" countable="true" size="8")
    ulittle32,//A little-endian unsigned 32-bit integer.(integral="true" countable="true" size="4")
    uintType,//(unsigned int)An unsigned 32-bit integer.(integral="true" countable="true" size="4" convertible="uint64")
    intType,//(signed int)A signed 32-bit integer.(integral="true" countable="false" size="4" convertible="int64")
    ushortType,//(unsigned short)An unsigned 16-bit integer.(integral="true" countable="true" size="2" convertible="uint uint64")
    shortType,//(signed short)A signed 16-bit integer.(integral="true" countable="false" size="2" convertible="int int64")
    charType,//(char)An 8-bit character.(integral="true" countable="false" size="1" convertible="short int int64")
    byteType,//(byte)An unsigned 8-bit integer.(boolean="true" integral="true" countable="true" size="1" convertible="ushort uint uint64")
    boolType,//(bool)A boolean; 32-bit from 4.0.0.2, and 8-bit from 4.1.0.1 on.(boolean="true" integral="true" countable="false")
};

/// <summary>
/// Class SmallValPlusDesc.
/// </summary>
class SmallValPlusDesc
{
    short Value;
    std::string Desc;
};

/// <summary>
/// Class EnumTag.
/// </summary>
class EnumTag
{
    std::string Desc;
    tsl::ordered_map<std::string, SmallValPlusDesc> FlagValues;
    std::string Prefix;
    BasicType StorageType;
    std::vector<std::string> versionsUsed;
};

/// <summary>
/// <bitflags> tag content
/// </summary>
class BitFlag
{
    std::string Desc;
    tsl::ordered_map<std::string, SmallValPlusDesc> FlagValues;
};

/// <summary>
/// Class BitFieldTag.
/// </summary>
class BitFieldTag
{
    std::string Desc;
};

/// <summary>
/// Class CompoundTag.
/// </summary>
class CompoundTag
{
    std::string Desc;
    tsl::ordered_map<int, BasicType> FieldValTargets;
};

/// <summary>
/// Class niObjectTag.
/// </summary>
class niObjectTag
{
    std::string Desc;
    tsl::ordered_map<int, BasicType> FieldValTargets;
};

/// <summary>
/// Class NifGen.
/// </summary>
class NifGen
{

};

int main()
{

}