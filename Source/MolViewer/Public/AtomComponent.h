// © 2021 W. Masek

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "AtomComponent.generated.h"

// Enums
UENUM()
enum EElement
{
	OXYGEN,
	NITROGEN,
	CARBON,
	HYDROGEN,
	NUM_OF_ELEMENTS
};

/**
 * 
 */
UCLASS()
class MOLVIEWER_API UAtomComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UAtomComponent();

	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	const FVector& GetPosition() const;
	void SetPosition(const FVector Pos);

	const EElement& GetElement() const;
	void SetElement(const TCHAR& ElementChar);

	UFUNCTION(BlueprintCallable)
	FString GetElementName();
	
private:
	void SetElementColour(UMaterialInstanceDynamic* Material);
	
	FVector Position;
	EElement Element;
};
