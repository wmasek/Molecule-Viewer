// © 2021 W. Masek

// This file's header
#include "BondComponent.h"

// Other includes
#include "AtomComponent.h"
#include "Engine/Public/DrawDebugHelpers.h"

// Sets default values for this component's properties
UBondComponent::UBondComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBondComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBondComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(StartAtom && EndAtom)
	{
		DrawDebugLine(GetWorld(), (StartAtom->GetPosition() * 100.0f), (EndAtom->GetPosition() * 100.0f), FColor::White, false, -1, 0, 5.0f);
	}	
}

UAtomComponent* UBondComponent::GetStartAtom() const
{
	return StartAtom;
}

void UBondComponent::SetStartAtom(UAtomComponent* Start)
{
	StartAtom = Start;
}

UAtomComponent* UBondComponent::GetEndAtom() const
{
	return EndAtom;
}

void UBondComponent::SetEndAtom(UAtomComponent* End)
{
	EndAtom = End;
}
