// © 2021 W. Masek

// This file's header
#include "BondComponent.h"

// Other includes
#include "AtomComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/LineBatchComponent.h"
#include "ProjectFunctionLibrary.h"

// Constants
constexpr float GDoublePositionScale = 0.1f;
constexpr float GDoubleLineWidth = 2.5f;
constexpr float GSingleLineWidth = 5.0f;

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

}


// Called every frame
void UBondComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(StartAtom && EndAtom)
	{
		if(IsDouble)
		{
			// Grab our initial start/end atom positions
			const FVector StartPos = StartAtom->GetPosition();
			const FVector EndPos = EndAtom->GetPosition();

			// Find the direction vector from one to the other
			FVector BondDirection = (StartPos - EndPos);
			BondDirection.Normalize();

			// Rotate the bond direction 90 degrees along the Z
			const FVector RotatedBondDirection = BondDirection.RotateAngleAxis(90.0f, FVector(0.0f, 0.0f, 1.0f));

			// Create double bond positions by adding/subtracting the rotated bond direction
			const FVector BondedStartPos1 = StartPos + (RotatedBondDirection * GDoublePositionScale);
			const FVector BondedStartPos2 = StartPos - (RotatedBondDirection * GDoublePositionScale);
			const FVector BondedEndPos1 = EndPos + (RotatedBondDirection * GDoublePositionScale);
			const FVector BondedEndPos2 = EndPos - (RotatedBondDirection * GDoublePositionScale);

			// Finally draw our two lines
			GetWorld()->LineBatcher->DrawLine((BondedStartPos1 * UProjectFunctionLibrary::GPositionScale), (BondedEndPos1 * UProjectFunctionLibrary::GPositionScale), FLinearColor::White, SDPG_World, GDoubleLineWidth);
			GetWorld()->LineBatcher->DrawLine((BondedStartPos2 * UProjectFunctionLibrary::GPositionScale), (BondedEndPos2 * UProjectFunctionLibrary::GPositionScale), FLinearColor::White, SDPG_World, GDoubleLineWidth);
		}
		else
		{
			// Draw single line
			GetWorld()->LineBatcher->DrawLine((StartAtom->GetPosition() * 100.0f), (EndAtom->GetPosition() * 100.0f), FLinearColor::White, SDPG_World, GSingleLineWidth);	
		}		
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

bool UBondComponent::IsDoubleBonded() const
{
	return IsDouble;
}

void UBondComponent::SetIsDoubledBonded(bool Double)
{
	IsDouble = Double;
}


