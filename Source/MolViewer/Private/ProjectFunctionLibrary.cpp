// © 2021 W. Masek

// This file's header
#include "ProjectFunctionLibrary.h"

// Other includes
#include "HAL/FileManagerGeneric.h"

// Static values
//---------------------
const float UProjectFunctionLibrary::GPositionScale = 100.0f;
const float UProjectFunctionLibrary::GAtomScale = 0.5f;

// Static functions
//---------------------
TArray<FString> UProjectFunctionLibrary::FindSDFFiles()
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