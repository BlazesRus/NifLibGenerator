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
    class NifVersion
    {
        /// <summary>
        /// "VX_X_X_X": The unique identifier for this version. Can be used to generate C-style enums, etc.
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
        Other = -1,
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

    class BasicTypeOrOther
    {
        /// <summary>
        /// The storage type if OtherType is empty()
        /// </summary>
        BasicType StorageType;

        /// <summary>
        /// The storage type if it's not empty()
        /// </summary>
        std::string OtherType;
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
        tsl::ordered_map<int, BasicTypeOrOther> FieldValTargets;
    };

    /// <summary>
    /// Class niObjectTag.
    /// </summary>
    class niObjectTag
    {
        std::string Desc;
        tsl::ordered_map<int, BasicTypeOrOther> FieldValTargets;
    };

    /// <summary>
    /// Class NifGen.
    /// </summary>
    class NifGen
    {
    public:
        std::vector<NifVersion> versionData;
        std::vector<BitFieldTag> bitFieldData;
        std::vector<niObjectTag> niObjectData;
        bool LoadXML(std::string FilePath)
        {
            char LineChar;
            bool InsideXMLComment = false;
            //If false, then inside tag-content types instead of tags
            bool InsideTag = false;
            std::string ScanBuffer = "";

            //First name inside tag becomes CurrentTag
            std::string CurrentTag = "";
            std::string CurrentNodeName = "";
            unsigned int CurrentNodeIndex = 0;
            //0=NormalTag; 1:SelfContainedTag; 2:TagIsClosing; 3:XMLVersionTag
            int TagType = 0;

            bool ScanningArgData = false;
            std::string ArgElement;
            ArgStringList LastArg;
            ArgList ArgBuffer;

            bool PotentialComment = false;
            bool InsideParenthesis = false;
            bool InsideClassNodeSection = false;
            //0 = Not Scanning TagContent Yet: 1 = Potential SingleLine TagContent: 2 Multi-line target content
            short TagContentStage = 0;

            //Current state of code loading for certain sections of code
            size_t Stage = 0;

            //Index of hkobject class node
            unsigned int CurrentClassNodeIndex = 0;
            //Index of last entered node
            unsigned int CurrentNodeIndex = 0;
            TagDepthTree TagDepth;
            std::string HkClassType = "";

            unsigned EntryNodeIndex = 0;

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
                else
                {
/*
                    if (InsideTag)
                    {
                        if (ScanBuffer == "/" && LineChar == '>')
                        {
                            switch (TagType)
                            {
                            case 1://SelfContainedTag
                                NodeBank.Add(CurrentTag, ArgBuffer, CurrentNodeIndex);
                                break;
                            case 3://XMLVersionTag(Same as SelfContained Tag except for ? in front and such)
                                break;
                            default://TagIsClosing(TagType==2)
                                    //Decrease TagDepth
                                TagDepth.RemoveLastTagMatch(CurrentTag);
                                break;
                            }
                            CurrentTag = "";//Reset it to clear buffer so next tag has fresh storage
                            InsideTag = false; TagContentStage = 0;
                        }
                        else if (LineChar == '>')
                        {
                            CurrentNodeIndex = NodeBank.Add(CurrentTag, ArgBuffer, CurrentNodeIndex);//Index of Last Entered Node is it's parent
                            if (InsideClassNodeSection && CurrentClassNodeIndex == 0 && CurrentTag == hkobject && ArgBuffer.HasKey(Signature))
                            {
                                CurrentClassNodeIndex = CurrentNodeIndex;
                                CurrentNodeName = ArgBuffer["Name"][0];
                                NodeLinks.Add(CurrentNodeName, CurrentClassNodeIndex);
                            }
                            //Increase TagDepth
                            CurrentNodeName = CurrentTag;
                            CurrentTag = "";//Reset it to clear buffer so next tag has fresh storage
                            InsideTag = false; TagContentStage = 0;
                        }
                        else if (CurrentTag.empty())
                        {
                            if (!ScanBuffer.empty())
                            {
                                if (LineChar == '!')//Detecting potential Commented Out Parts
                                {
                                    PotentialComment = true;
                                }
                                else if (LineChar == ' ' || LineChar == '/t')
                                {
                                    CurrentTag = ScanBuffer;
                                }
                                else
                                {
                                    ScanBuffer += LineChar;
                                }
                            }
                            else if (LineChar != ' ' && LineChar != '	' && LineChar != '\n')
                            {
                                ScanBuffer += LineChar;
                                if (LineChar != '\\')
                                {
                                    ScanningArgData = true; Stage = 0;
                                }
                            }
                        }
                        else if (TagContentStage > 0)
                        {
                        }
                        else
                        {
                            if (LineChar == '<')
                            {
                                InsideTag = true;
                            }
                            else if (LineChar != ' ' && LineChar != '	' && LineChar != '\n')
                            {
                                ScanBuffer = LineChar;
                                TagContentStage = 1;
                            }
                        }
                    }
*/
                }
            }
            return true;
        }
        bool LoadXML()
        {
            return LoadXML("nif.xml");
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
    XmlData.GenerateFiles();
}