// © 2021 W. Masek

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SDFFile.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MOLVIEWER_API USDFFile : public UObject
{
	GENERATED_BODY()

public:
	USDFFile();

	UFUNCTION(BlueprintCallable)
	void ChopFileName();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FString FullFilePath;

	UPROPERTY(BlueprintReadOnly)
	FString FileName;
	
};
