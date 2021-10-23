// © 2021 W. Masek

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SDFParser.generated.h"

/**
 * 
 */
UCLASS()
class MOLVIEWER_API USDFParser : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "SDF Loading")
	static void LoadSDFFile();
};
