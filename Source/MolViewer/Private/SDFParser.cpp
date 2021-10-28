// © 2021 W. Masek

// This file's header
#include "SDFParser.h"

// Other includes
#include "HAL/FileManagerGeneric.h"
#include "SDFFile.h"

// Constants
const int GTitleLine = 0;
const int GProgramTimeStamp = 1;
const int GCommentLine = 2;
const int GCountsLine = 3;

TArray<FString> USDFParser::FindSDFFiles()
{
	FString Directory = FPaths::ProjectDir();
	Directory.Append(TEXT("Content/SDF/"));

	TArray<FString> Output;
	Output.Empty();
	if (FPaths::DirectoryExists(Directory))
	{
	    const FString Path = Directory + "*.sdf";
	    FFileManagerGeneric::Get().FindFiles(Output, *Path, true, false);
	    for (int i = 0; i < Output.Num(); i++)
	    {
	        Output[i] = Directory + Output[i];
	    }
	}

	return Output;
}