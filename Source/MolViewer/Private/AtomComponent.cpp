// © 2021 W. Masek

// This file's header
#include "AtomComponent.h"

// Other includes
#include "ProjectFunctionLibrary.h"

UAtomComponent::UAtomComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UAtomComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Find the engine sphere mesh and cast it to a mesh
	UObject* MeshObj = StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	UStaticMesh* SphereMesh = Cast<UStaticMesh>(MeshObj);

	UObject* MatObj = StaticLoadObject(UMaterialInstance::StaticClass(), nullptr, TEXT("MaterialInstanceConstant'/Game/Materials/MI_MaterialSphere_Plain.MI_MaterialSphere_Plain'"));
	UMaterialInstance* SphereMaterial = Cast<UMaterialInstance>(MatObj);

	// Check the cast worked and then add the data
	if (SphereMesh && SphereMaterial)
	{
		// Set the position and scale of the mesh
		SetWorldLocation(Position * UProjectFunctionLibrary::GPositionScale);
		SetWorldScale3D(FVector(UProjectFunctionLibrary::GAtomScale));

		// Create a dynamic material instance from the material above
		UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(SphereMaterial, this);

		if(DynamicMaterial)
		{
			// Set the mesh and the material on our static mesh component
			SetStaticMesh(SphereMesh);
			SetMaterial(0, DynamicMaterial);
			SetElementColour(DynamicMaterial);
		}
	}	
}


void UAtomComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
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

FString UAtomComponent::GetElementName()
{
	FString ElementName = "";
	
	switch(Element)
	{
		case OXYGEN:
		{
			ElementName = "Oxygen";
			break;	
		}
		case NITROGEN:
		{
			ElementName = "Nitrogen";
			break;	
		}
		case CARBON:
		{
			ElementName = "Carbon";
			break;	
		}
		case HYDROGEN:
		{
			ElementName =  "Hydrogen";
			break;	
		}
		default:
		{
			UE_LOG(LogTemp, Warning, TEXT("No element found."));
		}
	}

	return ElementName;
}


void UAtomComponent::SetElementColour(UMaterialInstanceDynamic* Material)
{
	switch(Element)
	{
		case OXYGEN:
		{
			Material->SetVectorParameterValue(TEXT("Colour"), FLinearColor::Red);
			break;	
		}
		case NITROGEN:
		{
			Material->SetVectorParameterValue(TEXT("Colour"), FLinearColor::Blue);
			break;	
		}
		case CARBON:
		{
			Material->SetVectorParameterValue(TEXT("Colour"), FLinearColor::Black);
			break;	
		}
		case HYDROGEN:
		{
			Material->SetVectorParameterValue(TEXT("Colour"), FLinearColor::White);
			break;	
		}
		default:
		{
			UE_LOG(LogTemp, Warning, TEXT("No element found."));
		}
	}
}

