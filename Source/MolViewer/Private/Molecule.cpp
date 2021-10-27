// © 2021 W. Masek

// This file's header
#include "Molecule.h"

// Other includes
#include "AtomComponent.h"
#include "Engine/Public/DrawDebugHelpers.h"
#include "SDFParser.h"

// Sets default values
AMolecule::AMolecule()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMolecule::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMolecule::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(/*!Atoms ||*/ !Bonds)
	{
		return;
	}
	/*
	for (FAtom Atom : *Atoms)
	{
		switch(Atom.Element)
		{
			case OXYGEN:
			{
				DrawDebugSphere(GetWorld(), (Atom.Position * 100.0f), 25.0f, 12, FColor::Red);
				break;	
			}
			case NITROGEN:
			{
				DrawDebugSphere(GetWorld(), (Atom.Position * 100.0f), 25.0f, 12, FColor::Blue);
				break;	
			}
			case CARBON:
			{
				DrawDebugSphere(GetWorld(), (Atom.Position * 100.0f), 25.0f, 12, FColor::Black);
				break;	
			}
			case HYDROGEN:
			{
				DrawDebugSphere(GetWorld(), (Atom.Position * 100.0f), 25.0f, 12, FColor::White);
				break;	
			}
			default:
			{
				UE_LOG(LogTemp, Warning, TEXT("No element found."));
			}
		}		
	}
	*/
	for (FBond Bond : *Bonds)
	{
		DrawDebugLine(GetWorld(), (Bond.StartPos * 100.0f), (Bond.EndPos * 100.0f), FColor::White, false, -1, 0, 5.0f);
	}
}

void AMolecule::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	for (FAtom Atom : AtomList)
	{		
		// Create, register and attach new component to this actor.
		UAtomComponent* NewSubComponent = NewObject<UAtomComponent>(this, UAtomComponent::StaticClass());
		NewSubComponent->RegisterComponent();
		NewSubComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		Atoms.Add(NewSubComponent);

		// Find the engine sphere mesh
		UObject* obj = StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
		UStaticMesh* SphereAsset = Cast<UStaticMesh>(obj);

		if (SphereAsset)
		{
	        NewSubComponent->SetStaticMesh(SphereAsset);
	        NewSubComponent->SetWorldLocation(Atom.Position * 100.0f);
	        NewSubComponent->SetWorldScale3D(FVector(0.5f));
		}
	}

	Bonds = new TArray<FBond>();
	for (FBond Bond : BondList)
	{
		Bonds->Add(Bond);
	}
}

