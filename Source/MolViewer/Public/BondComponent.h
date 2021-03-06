// © 2021 W. Masek

#pragma once

// Includes
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BondComponent.generated.h"

// Forward declarations
class UAtomComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MOLVIEWER_API UBondComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBondComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UAtomComponent* GetStartAtom() const;
	void SetStartAtom(UAtomComponent* Start);

	UAtomComponent* GetEndAtom() const;
	void SetEndAtom(UAtomComponent* End);

	bool IsDoubleBonded() const;
	void SetIsDoubledBonded(bool Double);
	
private:
	UAtomComponent* StartAtom;
	UAtomComponent* EndAtom;
	bool IsDouble;
};
