// © 2021 W. Masek

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ProjectFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MOLVIEWER_API UProjectFunctionLibrary : public UObject
{
	GENERATED_BODY()

public:
	// Static function to find our SDF files in the directory
	UFUNCTION(BlueprintCallable, Category = "SDF Loading")
	static TArray<FString> FindSDFFiles();

	static const float GPositionScale;
	static const float GAtomScale;
};
