// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// ***********************************************************************
#pragma once
#ifndef ArgStringList_IncludeGuard
#define ArgStringList_IncludeGuard

#include <string>
#include <vector>

class ArgValue
{
public:
    std::string Value;
    int ArgPos;
    ArgValue()
    {
        Value = "";
        ArgPos = 0;
    }
    explicit operator std::string()
    {
        return Value;
    }
    /// <summary>
    /// Initializes a new instance of the <see cref="ArgValue"/> class.
    /// </summary>
    /// <param name="value">The value.</param>
    ArgValue(std::string value)
    {
        Value = value;
        ArgPos = 0;
    }
    //ArgValue(CString value)
    //{
    //    Value = value;
    //    ArgPos = 0;
    //}
};

class ArgStringList : public std::vector<ArgValue>
{
public:
    int ArgStart;
    ArgStringList()
    {
        ArgStart = 0;
    }
    void Add(std::string Value)
    {
        this->push_back(Value);
    }

    /// <summary>
    /// Loads the arguments.
    /// </summary>
    /// <param name="Content">The content.</param>
    void LoadArgs(const std::string& Content)
    {
        std::string CurrentElement = "";
        for (char const& CurrentChar : Content)
        {
            if (CurrentElement == "")
            {
                if (CurrentChar != '\n' && CurrentChar != ' ' && CurrentChar != '\t')
                {
                    CurrentElement = CurrentChar;
                }
            }
            else
            {
                if (CurrentChar != '\n' && CurrentChar != ' ' && CurrentChar != '\t')
                {
                    CurrentElement += CurrentChar;
                }
                else if (CurrentChar != ',')
                {
                    push_back(CurrentElement);
                    CurrentElement = "";
                }
            }
        }
    }
    /// <summary>
    /// Initializes a new instance of the <see cref="ArgStringList"/> class.(Loads the arguments detected inside the string)
    /// </summary>
    /// <param name="Content">The content.</param>
    ArgStringList(std::string Content) : ArgStringList()
    {
        ArgStart = 0;
        LoadArgs(Content);
    }
    /// <summary>
    /// Implements the operator std::string operator.
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator std::string()
    {
        std::string ConvertedString = "\"";
        for (vector<ArgValue>::iterator Arg = this->begin(), StartIndex = Arg, EndIndex = this->end(); Arg != EndIndex; ++Arg)
        {
            if (Arg != StartIndex)
            {
                ConvertedString += ",";
            }
            ConvertedString += Arg->Value;
        }
        ConvertedString += "\"";
        return ConvertedString;
    }

    explicit operator std::vector<std::string>()
    {
        std::vector<std::string> ConvertedVector;
        for (vector<ArgValue>::iterator Arg = this->begin(), StartIndex = Arg, EndIndex = this->end(); Arg != EndIndex; ++Arg)
        {
            ConvertedVector.push_back(Arg->Value);
        }
        return ConvertedVector;
    }


    explicit operator std::vector<int>()
    {
        std::vector<int> ConvertedVector;
        for (vector<ArgValue>::iterator Arg = this->begin(), StartIndex = Arg, EndIndex = this->end(); Arg != EndIndex; ++Arg)
        {
            ConvertedVector.push_back(VariableConversionFunctions::ReadIntFromString(Arg->Value));
        }
        return ConvertedVector;
    }
};
#endif