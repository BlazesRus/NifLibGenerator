// Program execution begins and ends here.
//

#include <iostream>

#include "AltNum\MediumDec.hpp"
using MediumDec = BlazesRusCode::MediumDec;

#include "AltNum\AltNumDebug.hpp"
#include "AltNum\FloatingOperations.hpp"
#include "Databases\MediumDecFormula.hpp"
#include "tsl/ordered_map.h"
#include "TagDepthTree.h"
#include "ArgStringList.h"
#include "ArgList.h"

#include <iostream>
#include <fstream>

using MediumDecFormula = BlazesRusCode::MediumDecFormula;

namespace NifGenerator
{
    /// <summary>
    /// Class NifVersion.(summaries from nif.xml)
    /// </summary>
    class VersionInfo
    {
    public:
        /// <summary>
        /// "VX_X_X_X": The unique identifier for this version. Can be used to generate C-style enumerations, etc.
        /// </summary>
        std::string id;
        /// <summary>
        /// "X.X.X.X": The numeric representation created by storing each part of the version in a byte.
        /// </summary>
        std::string num;
        /// <summary>
        /// False if the XML does not fully support reading of this version.
        /// </summary>
        bool supported;
        /// <summary>
        /// "<List[integer]>": The custom User Version(s) for a specific Version from a game/developer.
        /// Note: Grouping multiple user in a list means that these user produce identical NIF layout.
        /// Example : user = "12"
        /// user = "0x20000 0x30000"
        /// </summary>
        std::vector<int> user;
        /// <summary>
        /// "<List[integer]>": The custom Bethesda Version(s) for a specific Version and User Version.
        /// Note: Grouping multiple bsver in a list means that these bsver produce identical NIF layout.
        /// Example : bsver="34"
        /// bsver="30 31 32 33"
        /// </summary>
        std::vector<int> bsver;
        /// <summary>
        /// True when version contains extensions to the format not originating from Gamebryo.
        /// Note: A user or bsver of non - zero implies custom = "true" and is not necessary.
        /// </summary>
        bool custom;
        std::vector<std::string> ext;
        /// <summary>
        /// A comma-separated list of games which use this version.
        /// `{{}
        /// }` around a game name denotes that this version is the primary version for that game.
        /// A version without any `{{}}` names implies that this version is "secondary".
        /// Note: A unique game name should only have `{{}}` around it once.
        /// </summary>
        std::string Content;

        //VersionInfo(ArgList targetArgs)
        //{
        //    //std::string ArgContentBuffer;
        //    if (targetArgs.HasKey("id"))
        //        id = targetArgs["id"].back().Value;
        //    else
        //        id = "";
        //    if (targetArgs.HasKey("num"))
        //        num = targetArgs["num"].back().Value;
        //    else
        //        num = "";
        //    if (targetArgs.HasKey("supported"))
        //        supported = targetArgs["supported"].back().Value == "false" ? false : true;
        //    else
        //        supported = true;
        //    if (targetArgs.HasKey("supported"))
        //        supported = targetArgs["supported"].back().Value == "false" ? false : true;
        //    else
        //        supported = true;
        //    if (targetArgs.HasKey("user"))
        //        user = (std::vector<int>)targetArgs["user"];
        //    if (targetArgs.HasKey("bsver"))
        //        bsver = (std::vector<int>)targetArgs["bsver"];
        //    if (targetArgs.HasKey("custom"))
        //        custom = targetArgs["custom"].back().Value == "true" ? true : false;
        //    else
        //        custom = false;
        //    if (targetArgs.HasKey("ext"))
        //        ext = (std::vector<std::string>)targetArgs["ext"];
        //}

        /// <summary>
        /// Initializes a new instance of the <see cref="VersionInfo"/> class.
        /// </summary>
        VersionInfo()
        {
            supported = true;
            custom = false;
        }
    };

    ///// <summary>
    ///// Storage type of target value
    ///// </summary>
    //enum class BasicType :unsigned __int8
    //{
    //    Other = -1,
    //    uint64Type = 0,//(unsigned int64)An unsigned 64 - bit integer.(integral="true" countable="true" size="8")
    //    int64Type,//(signed int64)An signed 64 - bit integer.(integral="true" countable="true" size="8")
    //    ulittle32,//A little-endian unsigned 32-bit integer.(integral="true" countable="true" size="4")
    //    uintType,//(unsigned int)An unsigned 32-bit integer.(integral="true" countable="true" size="4" convertible="uint64")
    //    intType,//(signed int)A signed 32-bit integer.(integral="true" countable="false" size="4" convertible="int64")
    //    ushortType,//(unsigned short)An unsigned 16-bit integer.(integral="true" countable="true" size="2" convertible="uint uint64")
    //    shortType,//(signed short)A signed 16-bit integer.(integral="true" countable="false" size="2" convertible="int int64")
    //    charType,//(char)An 8-bit character.(integral="true" countable="false" size="1" convertible="short int int64")
    //    byteType,//(byte)An unsigned 8-bit integer.(boolean="true" integral="true" countable="true" size="1" convertible="ushort uint uint64")
    //    boolType,//(bool)A boolean; 32-bit from 4.0.0.2, and 8-bit from 4.1.0.1 on.(boolean="true" integral="true" countable="false")
    //};

    /// <summary>
    /// Class SmallValPlusDesc.
    /// </summary>
    class SmallValPlusDesc
    {
    public:
        short Value;
        std::string Desc;
    };

    /// <summary>
    /// Class EnumTag.
    /// </summary>
    class EnumTag
    {
    public:
        std::string Desc;
        //Argument Fields
        std::string Name;
        std::string Prefix;
        std::string StorageType;
        std::vector<std::string> versionsUsed;
        
        /// <summary>
        /// The flag values
        /// </summary>
        tsl::ordered_map<std::string, SmallValPlusDesc> FlagValues;

        EnumTag()
        {
            Desc = "";
            Prefix = "";
        }
        void AddOption(std::string optionName, short val)
        {
            FlagValues.insert_or_assign(optionName, val);
        }
        //EnumTag(ArgList targetArgs)
        //{
        //}
    };

    /// <summary>
    /// bitflags tag content
    /// </summary>
    class BitFlag
    {
    public:
        std::string Desc;
        //Argument Fields
        std::string Name;
        std::string StorageType;

        /// <summary>
        /// The option flag values
        /// </summary>
        tsl::ordered_map<std::string, SmallValPlusDesc> FlagValues;
        /// <summary>
        /// Adds the option.
        /// </summary>
        /// <param name="optionName">Name of the option.</param>
        /// <param name="val">The value.</param>
        void AddOption(std::string optionName, short val)
        {
            FlagValues.insert_or_assign(optionName, val);
        }
    };

    class memberTag
    {
    public:
        //int width;

        /// <summary>
        /// The argument fields of the xml tag(could also store as separate specific arg storage later)
        /// </summary>
        ArgList ArgFields;
    };

    /// <summary>
    /// bitfield tag content
    /// </summary>
    class BitFieldTag
    {
    public:
        std::string Desc;
        //Argument Fields
        std::string Name;
        /// <summary>
        /// The member inner tags
        /// </summary>
        std::vector<memberTag> memberTags;
        BitFieldTag()
        {
            Desc = "";
        }
    };

    class InnerTagIndex
    {
    public:
        bool InsideInnerTag;
        int PrimaryIndex;
        int SecondaryIndex;
        bool ParentTagWasPrimary;
        int ParentIndex;
        int CurrentIndex;
        void clear()
        {
            InsideInnerTag = false;
            PrimaryIndex = -1;
            SecondaryIndex = -1;
            ParentTagWasPrimary = true;
            ParentIndex = -1;
            CurrentIndex = -1;
        }
        InnerTagIndex()
        {
            InsideInnerTag = false;
            PrimaryIndex = -1;
            SecondaryIndex = -1;
            ParentTagWasPrimary = true;
            ParentIndex = -1;
            CurrentIndex = -1;
        }
    };

    class fieldTag
    {
    public:
        /// <summary>
        /// TagContent of the tag(before inner xml tag(s))
        /// </summary>
        std::string Desc;
        /// <summary>
        /// The argument fields of the xml tag(could also store as separate specific arg storage later)
        /// </summary>
        ArgList ArgFields;
        /// <summary>
        /// Vector of index references in InnerTag
        /// </summary>
        std::vector<int> ChildTags;
    };



    /// <summary>
    /// compound and niObject tag content
    /// </summary>
    class FieldStorageTag
    {
    public:
        InnerTagIndex IndexTracker;
        std::string Desc;
        /// <summary>
        /// The argument fields of the xml tag
        /// </summary>
        ArgList ArgFields;
        /// <summary>
        /// The base level <field> tags
        /// </summary>
        std::vector<fieldTag> PrimaryTags;
        /// <summary>
        /// Child tags of PrimaryTags and other InnerTags(not likely to be used in most cases)
        /// </summary>
        std::vector<fieldTag> InnerTags;
        /// <summary>
        /// Initializes a new instance of the <see cref="FieldStorageTag"/> class.
        /// </summary>
        FieldStorageTag()
        {

        }
    };

    class OtherTagMember
    {
    public:
        std::string TagName;
        std::string Desc;
        /// <summary>
        /// The argument fields of the xml tag
        /// </summary>
        ArgList ArgFields;
        /// <summary>
        /// Vector of index references in InnerTag
        /// </summary>
        std::vector<int> ChildTags;
        /// <summary>
        /// Initializes a new instance of the <see cref="OtherTagMember"/> class.
        /// </summary>
        OtherTagMember()
        {
            Desc = "";
        }
    };

    /// <summary>
    /// Store other specific entry level tags with this (such as basic tags)
    /// </summary>
    class GeneralTag
    {public:
        InnerTagIndex IndexTracker;
        std::string Desc;
        /// <summary>
        /// The argument fields of the xml tag
        /// </summary>
        ArgList ArgFields;
        /// <summary>
        /// The primary tags
        /// </summary>
        std::vector<OtherTagMember> PrimaryTags;
        /// <summary>
        /// Child tags of PrimaryTags and other InnerTags
        /// </summary>
        std::vector<OtherTagMember> InnerTags;

        /// <summary>
        /// Initializes a new instance of the <see cref="GeneralTag"/> class.
        /// </summary>
        GeneralTag()
        {
            Desc = "";
        }
    };

    /// <summary>
    /// Class NifGen.
    /// </summary>
    class NifGen
    {
    protected:
        /// <summary>
        /// Equal to std::pair(std::string, int)
        /// </summary>
        class DataOrderInfo
        {
        public:
            std::string EntryTagName;
            int Index;
            DataOrderInfo(std::string entryTagName, int indexPosition)
            {
                EntryTagName = entryTagName;
                Index = indexPosition;
            }
        };
        /// <summary>
        /// Strings to string vector.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <returns>vector(std::string)</returns>
        std::vector<std::string> StringToStringVector(std::string value)
        {

        }

        /// <summary>
        /// Strings to int vector.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <returns>vector(int)</returns>
        std::vector<int> StringToIntVector(std::string value)
        {

        }
    public:
        /// <summary>
        /// The loaded XML data order(EntryTagName, Index in Vector);
        /// Only needed for regeneration of xml file(mainly for debugging purposes) 
        /// </summary>
        std::vector<DataOrderInfo> LoadedXmlDataOrder;

        /// <summary>
        /// The compound tag xml data
        /// </summary>
        std::vector<FieldStorageTag> compoundData;

        /// <summary>
        /// The enum tag xml data
        /// </summary>
        std::vector<EnumTag> enumData;

        /// <summary>
        /// The bitflags tag xml data
        /// </summary>
        std::vector<BitFlag> bitflagsData;

        /// <summary>
        /// The version tag xml data
        /// </summary>
        std::vector<VersionInfo> versionData;

        /// <summary>
        /// The bitfield tag xml data
        /// </summary>
        std::vector<BitFieldTag> bitFieldData;

        /// <summary>
        /// The niObject tag xml data
        /// </summary>
        std::vector<FieldStorageTag> niObjectData;

        /// <summary>
        /// The basic tag xml data (not really needed for generation of c++ files)
        /// </summary>
        std::vector<GeneralTag> basicData;

        /// <summary>
        /// The token tag xml data (not really needed for generation of c++ files)
        /// </summary>
        std::vector<GeneralTag> tokenData;

        /// <summary>
        /// The module tag xml data (not really needed for generation of c++ files)
        /// </summary>
        std::vector<GeneralTag> moduleData;

        /// <summary>
        /// Generate XML files based on loaded content 
        /// </summary>
        void GenerateXMLsFromContent()
        {

        }

        /// <summary>
        /// Loads the XML.
        /// </summary>
        /// <param name="FilePath">The file path.</param>
        /// <returns>bool</returns>
        bool LoadXML(std::string FilePath)
        {
            //Blank Element for added Compounds and NiObjects
            FieldStorageTag NewFieldStorage;
            //Blank Element for added Enum
            EnumTag NewEnum;
            //Blank Element for added BitFlag
            BitFlag NewBitFlag;
            //Blank Element for added BitField
            BitFieldTag NewBitField;
            //Blank Element for added Version
            VersionInfo NewVersionInfo;
            //Blank Element for added basic tags, tokens, and modules
            GeneralTag NewGeneralTag;


            char LineChar;
            bool InsideXMLComment = false;
            //If false, then inside tag-content types instead of tags
            bool InsideTag = false;
            std::string ScanBuffer = "";

            //First name inside tag becomes CurrentTag
            std::string CurrentTag = "";
            unsigned int CurrentNodeIndex = 0;
            //0=NormalTag; 1:SelfContainedTag; 2:TagIsClosing; 3:XMLVersionTag
            int TagType = 0;

            bool ScanningArgData = false;
            std::string ArgElement;
            ArgStringList LastArg;
            ArgList ArgBuffer;

            bool PotentialComment = false;
            bool InsideParenthesis = false;
            //0 = Not Scanning TagContent Yet: 1 = Potential SingleLine TagContent: 2 Multi-line target content
            short TagContentStage = 0;

            //Current state of code loading for certain sections of code
            size_t Stage = 0;

            TagDepthTree TagDepth;

            int EntryNodeIndex = -1;
            std::string EntryTagName = "";
            bool InsideClosingTag = false;

            bool StartedTagRead = false;
            bool SkipCurrentTag = false;
            std::ifstream inFile;
            inFile.open(FilePath);
            if (!inFile)
            {
                return false;
            }
            while (inFile >> LineChar)
            {
                if (PotentialComment)
                {
                    ScanBuffer += LineChar;
                    if (ScanBuffer == "--")
                    {
                        InsideXMLComment = true;
                        PotentialComment = false;
                        ScanBuffer = "";
                    }
                    else if (ScanBuffer.size() >= 2)//Detecting non-normal format TagName?
                    {
                        PotentialComment = false;
                        ScanBuffer = "!" + ScanBuffer;
                    }
                }
                else if (InsideXMLComment)//Ignoring all xml inside xml formatted comment
                {
                    Stage = ScanBuffer.size();
                    if (Stage == 0)
                    {
                        if (LineChar == '-')
                        {
                            ScanBuffer = "-";
                        }
                    }
                    else if (Stage == 1)
                    {
                        if (LineChar == '-')
                        {
                            ScanBuffer = "--";
                        }
                        else
                        {
                            ScanBuffer = "";
                        }
                    }
                    else
                    {
                        if (LineChar == '>')
                        {
                            InsideXMLComment = false;
                        }
                        ScanBuffer = "";
                    }
                }
                else if (ScanningArgData)
                {
                    if (Stage == 0)
                    {
                        if (LineChar == '=')
                        {
                            Stage = 1;
                            //CurrentArgName = ScanBuffer;//Don't need to store in separate variable since ScanBuffer not going to be changed until exit scanning argument data
                            ArgElement = "";
                        }
                        else if (LineChar != ' ' && LineChar != '\t' && LineChar != '\n')//Skip Whitespace
                        {
                            ScanBuffer += LineChar;
                        }
                    }
                    else if (Stage == 1)
                    {
                        if (LineChar == '\"')
                        {
                            Stage = 2;
                        }
                    }
                    else if (Stage == 2)
                    {
                        if (LineChar == '\"') { Stage = 3; }
                    }
                    else if (Stage == 3)
                    {
                        if (LineChar == ',')
                        {
                            LastArg.Add(ArgElement);
                        }
                        else if (LineChar == '\"')
                        {
                            ArgBuffer.Add(ScanBuffer, LastArg);
                            ScanningArgData = false;
                        }
                        else
                        {
                            ArgElement += LineChar;
                        }
                    }
                }
                else if(StartedTagRead)//Only start saving scans once enter certain depth of xml file
                {
                    if(InsideClosingTag)
                    {
                        if(LineChar=='>')
                        {
                            if (CurrentTag == EntryTagName)//Exiting entry tag
                            {
                                LoadedXmlDataOrder.push_back(DataOrderInfo(EntryTagName,EntryNodeIndex));
                                EntryTagName.clear();
                            }
                            else//Exiting inner tag 
                            {
                                if (EntryTagName == "compound")
                                {
                                }
                                else if (EntryTagName == "enum")
                                {
                                }
                                else if (EntryTagName == "bitflags")
                                {
                                }
                                else if (EntryTagName == "bitfield")
                                {
                                }
                                else if (EntryTagName == "niobject")
                                {
                                }
                                else if (EntryTagName == "version")
                                {
                                }
                                else if (EntryTagName == "token")
                                {
                                }
                                else if (EntryTagName == "module")
                                {
                                }
                                else if (EntryTagName == "basic")
                                {
                                }
                            }
                            CurrentTag = "";//Reset it to clear buffer so next tag has fresh storage
                            TagContentStage = 0;
                            InsideClosingTag = false; InsideTag = false;
                        }
                    }
                    else if (InsideTag)
                    {
                        if (LineChar == '>')//End of a Tag detected
                        {
                            if (EntryTagName==CurrentTag)
                            {
                                if (EntryTagName == "compound")
                                    compoundData.back().ArgFields = ArgBuffer;
                                //else if (EntryTagName == "enum")
                                //    enumData.back().ArgFields = ArgBuffer;
                                //else if (EntryTagName == "bitflags")
                                //    bitflagsData.back().ArgFields = ArgBuffer;
                                //else if (EntryTagName == "bitfield")
                                //    bitFieldData.back().ArgFields = ArgBuffer;
                                else if (EntryTagName == "niobject")
                                    niObjectData.back().ArgFields = ArgBuffer;
                                //else if (EntryTagName == "version")
                                //    versionData.push_back(ArgBuffer);
                                else if (EntryTagName == "token")
                                    tokenData.back().ArgFields = ArgBuffer;
                                else if (EntryTagName == "module")
                                    moduleData.back().ArgFields = ArgBuffer;
                                else if (EntryTagName == "basic")
                                    basicData.back().ArgFields = ArgBuffer;
                                ArgBuffer.clear();
                                if (ScanBuffer == "/")//Self-Contained Tag
                                    EntryTagName.clear();
                            }
                            else
                            {   //Treat both bit and value parameter as the value of the option(so that supports both 0.9.0 and 0.9.2 option structures)
                                //if (EntryTagName == "compound")
                                //{
                                //}
                                //else if (EntryTagName == "enum")
                                //{
                                //}
                                //else if (EntryTagName == "bitflags")
                                //{
                                //}
                                //else if (EntryTagName == "bitfield")
                                //{
                                //}
                                //else if (EntryTagName == "niobject")
                                //{
                                //}
                                //else if (EntryTagName == "token")
                                //{
                                //}
                                if (ScanBuffer == "/")//Self-Contained Tag
                                {

                                }
                            }
                            InsideTag = false;
                        }
                        else if (CurrentTag.empty())
                        {
                            if (ScanBuffer.empty())
                            {
                                if (LineChar == '!')//Detecting potential Commented Out Parts
                                    PotentialComment = true;
                                else if(LineChar=='/')
                                {

                                }
                                else if (LineChar != ' ' && LineChar != '	' && LineChar != '\n')
                                    ScanBuffer += LineChar;
                            }
                            else if (LineChar == '/')//Closed Tag without any arguments
                            {
                                CurrentTag = ScanBuffer;
                                ScanBuffer = "/";
                            }
                            else if (LineChar == ' ' || LineChar == '	' || LineChar == '\n')
                            {
                                if (EntryTagName.empty())
                                {
                                    EntryTagName = ScanBuffer;
                                    //Add Blank data at default values
                                    if (EntryTagName == "compound")
                                        compoundData.push_back(NewFieldStorage);
                                    else if (EntryTagName == "enum")
                                        enumData.push_back(NewEnum);
                                    else if (EntryTagName == "bitflags")
                                        bitflagsData.push_back(NewBitFlag);
                                    else if (EntryTagName == "bitfield")
                                        bitFieldData.push_back(NewBitField);
                                    else if (EntryTagName == "niobject")
                                        niObjectData.push_back(NewFieldStorage);
                                    else if (EntryTagName == "version")
                                        versionData.push_back(NewVersionInfo);
                                    else if (EntryTagName == "token")
                                        tokenData.push_back(NewGeneralTag);
                                    else if (EntryTagName == "module")
                                        moduleData.push_back(NewGeneralTag);
                                    else if (EntryTagName == "basic")
                                        basicData.push_back(NewGeneralTag);
                                    if (!TagDepth.empty())
                                        TagDepth.clear();
                                    //Start Generic Argument Scanning for those with ArgData field (manual code for the others)
                                    if(EntryTagName=="compound"||EntryTagName=="niobject"|| EntryTagName == "token"|| EntryTagName == "module"|| EntryTagName == "basic")
                                        ScanningArgData = true; Stage = 0;
                                }
                                else
                                {//Starting inner tag
                                    //if (EntryTagName == "compound")
                                    //    compoundData.back()//.push_back(NewFieldStorage);
                                    //else if (EntryTagName == "enum")
                                    //    enumData.back().push_back(NewEnum);
                                    //else if (EntryTagName == "bitflags")
                                    //    bitflagsData.back().push_back(NewBitFlag);
                                    //else if (EntryTagName == "bitfield")
                                    //    bitFieldData.back().push_back(NewBitField);
                                    //else if (EntryTagName == "niobject")
                                    //    niObjectData.back().push_back(NewFieldStorage);
                                    //else if (EntryTagName == "version")
                                    //    versionData.back().push_back(NewVersionInfo);
                                    //else if (EntryTagName == "token")
                                    //    tokenData.back().push_back(NewGeneralTag);
                                    //else if (EntryTagName == "module")
                                    //    moduleData.back().push_back(NewGeneralTag);
                                    //else if (EntryTagName == "basic")
                                    //    basicData.back().push_back(NewGeneralTag);
                                    ScanningArgData = true; Stage = 0;
                                }
                                CurrentTag = ScanBuffer;
                                ScanBuffer.clear();
                            }
                            else if (LineChar != ' ' && LineChar != '	' && LineChar != '\n')
                            {
                                ScanBuffer += LineChar;
                            }
                        }
                        //------------------Scanning Argument Field/Values-------------------------------
                        else if(CurrentTag==EntryTagName)
                        {
                            if(LineChar=='=')
                            {
                                ArgElement = ScanBuffer; ScanBuffer.clear();
                                Stage = 1;
                            }
                            else if(Stage==1)
                            {
                                if(LineChar=='\"')
                                {
                                    Stage = 2;
                                }
                            }
                            else if(Stage==2)
                            {
                                if (LineChar == '\"')
                                {
                                    if (EntryTagName == "enum")
                                    {
                                        if (ArgElement == "name")
                                        {

                                        }
                                    }
                                    else if (EntryTagName == "bitflags")
                                    {
                                        if (ArgElement == "name")
                                        {

                                        }
                                    }
                                    else if (EntryTagName == "bitfield")
                                    {
                                        if (ArgElement == "name")
                                        {

                                        }
                                    }
                                    else if (EntryTagName == "version")
                                    {
                                        if (ArgElement == "id")
                                            versionData.back().id = ScanBuffer;
                                        else if (ArgElement == "num")
                                            versionData.back().num = ScanBuffer;
                                        else if (ArgElement == "supported")
                                            versionData.back().supported = ScanBuffer == "false" ? false : true;
                                        else if (ArgElement == "custom")
                                            versionData.back().custom = ScanBuffer == "true" ? true : false;
                                        else if (ArgElement == "user")
                                            versionData.back().user = StringToIntVector(ScanBuffer);
                                        else if (ArgElement == "bsver")
                                            versionData.back().bsver = StringToIntVector(ScanBuffer);
                                        else if (ArgElement == "ext")
                                            versionData.back().ext = StringToStringVector(ScanBuffer);
                                    }
                                    Stage = 0; ScanBuffer.clear();
                                }
                                else
                                {
                                    ScanBuffer += LineChar;
                                }
                            }
                            else if (LineChar != ' ' && LineChar != '	' && LineChar != '\n')
                            {
                                ScanBuffer += LineChar;
                            }
                        }
                        else
                        {
                            if (ScanBuffer.empty())
                            {
                                if (LineChar != ' ' && LineChar != '	' && LineChar != '\n')
                                {
                                    ScanBuffer += LineChar;
                                }
                            }
                            else if (LineChar == ' ' || LineChar == '	' || LineChar == '\n')
                            {
                                CurrentTag = ScanBuffer;
                                ScanBuffer.clear();
                                ScanningArgData = true; Stage = 0;
                            }
                            else if (LineChar != ' ' && LineChar != '	' && LineChar != '\n')
                            {
                                ScanBuffer += LineChar;
                            }
                        }
                    }
                    else
                    {
                        if (LineChar == '<')
                        {
                            //Send Description field into tag target

                            InsideTag = true; ScanBuffer.clear();
                        }
                        else//If description value is empty, add data to description field buffer
                        {
                            ScanBuffer += LineChar;
                        }
                    }
                }
                else
                {
                    if (InsideTag)
                    {
                        if(SkipCurrentTag)
                        {
                            if(LineChar=='>')
                            {
                                SkipCurrentTag = false; ScanBuffer.clear(); InsideTag = false;
                            }
                        }
                        else
                        {
                            if (ScanBuffer.empty())
                            {
                                if (LineChar == '!')//Detecting potential Commented Out Parts
                                {
                                    PotentialComment = true;
                                }
                                else if (LineChar == '?')
                                    SkipCurrentTag = true;
                                else if (LineChar != ' ' && LineChar != '	')
                                {
                                    ScanBuffer += LineChar;
                                }
                            }
                            else if (LineChar == ' ' || LineChar == '	')
                            {
                                if (ScanBuffer == "niftoolsxml")//Don't need version number info from the file in order to process it(since assuming minimum version of 9.0)
                                    SkipCurrentTag = true;
                                else
                                {
                                    CurrentTag = ScanBuffer;
                                    ScanBuffer.clear();
                                    StartedTagRead = true;
                                }
                            }
                            else
                            {
                                ScanBuffer += LineChar;
                            }
                        }
                    }
                    else
                    {
                        if (LineChar == '<')
                        {
                            InsideTag = true;
                        }
                    }
                }
            }
            return true;
        }
        bool LoadXML()
        {
            return LoadXML("nif.xml");
        }
        void GenerateDebugOutput()
        {
            for (std::vector<VersionInfo>::iterator CurrentVal = versionData.begin(), LastVal = versionData.end(); CurrentVal != LastVal; ++CurrentVal)
            {

            }
            for (std::vector<BitFieldTag>::iterator CurrentVal = bitFieldData.begin(), LastVal = bitFieldData.end(); CurrentVal != LastVal; ++CurrentVal)
            {

            }
            for (std::vector<FieldStorageTag>::iterator CurrentVal = niObjectData.begin(), LastVal = niObjectData.end(); CurrentVal != LastVal; ++CurrentVal)
            {

            }
        }
        void GenerateFiles()
        {

        }
    };
}

int main()
{
    NifGenerator::NifGen XmlData;
    XmlData.LoadXML();
    //XmlData.GenerateFiles();
}