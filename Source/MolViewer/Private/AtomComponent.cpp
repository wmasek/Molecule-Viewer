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

void UAtomComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Find the engine sphere mesh and cast it to a mesh
	UObject* obj = StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	UStaticMesh* SphereAsset = Cast<UStaticMesh>(obj);

	// Check the cast worked and then add the data
	if (SphereAsset)
	{
		SetStaticMesh(SphereAsset);
		SetWorldLocation(Position * 100.0f);
		SetWorldScale3D(FVector(0.5f));
	}

	// Get the material
	Material = GetMaterial(0);

	// Create a dynamic material instance from the material above
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);

	SetMaterial(0, DynamicMaterial);

	DynamicMaterial->SetScalarParameterValue(TEXT("EmissiveStrength"), 50);
	DynamicMaterial->SetVectorParameterValue(TEXT("Colour"), FLinearColor::Black);
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
