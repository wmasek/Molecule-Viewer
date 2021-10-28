// © 2021 W. Masek

// This file's header
#include "AtomComponent.h"

// Other includes
#include "Engine/Public/DrawDebugHelpers.h"

UAtomComponent::UAtomComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}


void UAtomComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	switch(Element)
	{
		case OXYGEN:
		{
			DrawDebugSphere(GetWorld(), (Position * 100.0f), 25.0f, 12, FColor::Red);
			break;	
		}
		case NITROGEN:
		{
			DrawDebugSphere(GetWorld(), (Position * 100.0f), 25.0f, 12, FColor::Blue);
			break;	
		}
		case CARBON:
		{
			DrawDebugSphere(GetWorld(), (Position * 100.0f), 25.0f, 12, FColor::Black);
			break;	
		}
		case HYDROGEN:
		{
			DrawDebugSphere(GetWorld(), (Position * 100.0f), 25.0f, 12, FColor::White);
			break;	
		}
		default:
		{
			UE_LOG(LogTemp, Warning, TEXT("No element found."));
		}
	}
}

const FVector& UAtomComponent::GetPosition() const
{
	return Position;
}

void UAtomComponent::SetPosition(const FVector Pos)
{
	Position = Pos;
}

const EElement& UAtomComponent::GetElement() const
{
	return Element;	
}

void UAtomComponent::SetElement(const TCHAR& ElementChar)
{
	if(ElementChar == 'O')
	{
		Element = OXYGEN;
	}
	else if(ElementChar == 'N')
	{
		Element = NITROGEN;
	}
	else if(ElementChar == 'C')
	{
		Element = CARBON;
	}
	else if(ElementChar == 'H')
	{
		Element = HYDROGEN;
	}
}
