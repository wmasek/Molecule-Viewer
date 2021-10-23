// © 2021 W. Masek

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SDFReader.generated.h"

UENUM()
enum ElementType
{
	OXYGEN,
	NITROGEN,
	CARBON,
	HYDROGEN,
	NUM_OF_ELEMENTS
};

USTRUCT()
struct FAtom
{
	GENERATED_BODY()

	FVector Position;
	ElementType Element;
};

USTRUCT()
struct FBond
{
	GENERATED_BODY()

	FAtom* StartAtom;
	FAtom* EndAtom;
};

UCLASS()
class MOLVIEWER_API ASDFReader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASDFReader();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Each string line of our SDF file
	TArray<FString> HeaderLines;
	FString CountsLine;
	TArray<FString> AtomLines;
	TArray<FString> BondLines;

	// Constructed Atoms and Bonds
	TArray<FAtom> AtomList;
	TArray<FBond> BondList;

};
