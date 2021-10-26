// © 2021 W. Masek

#pragma once

// Includes
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SDFParser.generated.h"

// Enums
//UENUM()
//enum ElementType
//{
//	OXYGEN,
//	NITROGEN,
//	CARBON,
//	HYDROGEN,
//	NUM_OF_ELEMENTS
//};

// Structs
//USTRUCT()
//struct FAtom
//{
//	GENERATED_BODY()
//
//	FVector Position;
//	ElementType Element;
//};
//
//USTRUCT()
//struct FBond
//{
//	GENERATED_BODY()
//
//	FAtom* StartAtom;
//	FAtom* EndAtom;
//};

// Forward declarations
class USDFFile;

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
	static void LoadSDFFile(FString FilePath, USDFFile* File);
};
