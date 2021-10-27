// © 2021 W. Masek

#pragma once

// Includes
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SDFParser.generated.h"

// Forward declarations
class AMolecule;

/**
 * 
 */
UCLASS()
class MOLVIEWER_API USDFParser : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "SDF Loading")
	static TArray<FString> FindSDFFiles();

	UFUNCTION(BlueprintCallable, Category = "SDF Loading")
	static void LoadSDFFile(FString FilePath);

	static void LoadFileToString(const FString& FilePath, TArray<FString>& FileContents);
	static void FindCounts(const FString& CountsLine, AMolecule* Mol);
};
