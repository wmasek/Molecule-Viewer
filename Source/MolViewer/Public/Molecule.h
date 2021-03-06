// © 2021 W. Masek

#pragma once

// Includes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Molecule.generated.h"

// Forward declarations
class UAtomComponent;
class UBondComponent;

/**
 * Represents our molecule in our scene
 */
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

	UFUNCTION(BlueprintImplementableEvent)
	void InitialiseAtomWidget();

	// A root scene component to attach the others to
	UPROPERTY(VisibleAnywhere);
	USceneComponent* RootSceneComp;

	// Details of our molecule to set in UI
	UPROPERTY(BlueprintReadOnly);
	FString Title;
	UPROPERTY(BlueprintReadOnly);
	FString Timestamp;
	UPROPERTY(BlueprintReadOnly);
	FString Comments;
	UPROPERTY(BlueprintReadOnly);
	int AtomCount;
	UPROPERTY(BlueprintReadOnly);
	int BondCount;
	UPROPERTY(BlueprintReadOnly);
	bool IsChiral;

private:
	void CreateAtomComponent(const FVector& Pos, const TCHAR& Elementchar);

	void CreateBondComponent(UAtomComponent* SAtom, UAtomComponent* EAtom, const bool IsDouble);
	
	void LoadFileToString(const FString& FilePath, TArray<FString>& FileContents);
	
	void FindCounts(const FString& CountsLine);
	
private:
	TArray<UAtomComponent*> Atoms;
	TArray<UBondComponent*> Bonds;
	
};
