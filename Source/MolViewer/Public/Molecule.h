// © 2021 W. Masek

#pragma once

// Includes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SDFParser.h"
#include "Molecule.generated.h"

// Forward declarations
class UAtomComponent;
class UBondComponent;

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
	// Loads our SDF file and build our molecule
	UFUNCTION(BlueprintCallable)
	void Initialise(const FString& FilePath);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere);
	USceneComponent* RootSceneComp;

private:
	void CreateAtomComponent(const FVector& Pos, const TCHAR& Elementchar);

	void CreateBondComponent(UAtomComponent* SAtom, UAtomComponent* EAtom);
	
	void LoadFileToString(const FString& FilePath, TArray<FString>& FileContents);
	
	void FindCounts(const FString& CountsLine);

	FString Title;
	FString Timestamp;
	FString Comments;

	int AtomCount;
	int BondCount;

	bool IsChiral;
	
private:
	TArray<UAtomComponent*> Atoms;
	TArray<UBondComponent*> Bonds;
	
};
