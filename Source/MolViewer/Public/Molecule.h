// © 2021 W. Masek

#pragma once

// Includes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SDFParser.h"
#include "Molecule.generated.h"

// Forward declarations
class UAtomComponent;

// Enums
UENUM()
enum ElementType
{
	OXYGEN,
	NITROGEN,
	CARBON,
	HYDROGEN,
	NUM_OF_ELEMENTS
};

// Structs
USTRUCT(BlueprintType)
struct FAtom
{
	GENERATED_BODY()

	FVector Position;
	ElementType Element;
};

USTRUCT(BlueprintType)
struct FBond
{
	GENERATED_BODY()

	FVector StartPos;
	FVector EndPos;
};

UCLASS()
class MOLVIEWER_API AMolecule : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMolecule();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PreInitializeComponents() override;

	FString Title;
	FString Timestamp;
	FString Comments;

	int AtomCount;
	int BondCount;

	bool IsChiral;
	
	TArray<FAtom> AtomList;
	TArray<FBond> BondList;
	
private:
	TArray<UAtomComponent*> Atoms;
	TArray<FBond>* Bonds;
	
};
