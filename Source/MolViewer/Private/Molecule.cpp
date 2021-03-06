// © 2021 W. Masek

// This file's header
//---------------------
#include "Molecule.h"

// Other includes
//---------------------
#include "AtomComponent.h"
#include "BondComponent.h"

// Constants
//---------------------
constexpr int GTitleLine = 0;
constexpr int GProgramTimeStamp = 1;
constexpr int GCommentLine = 2;
constexpr int GCountsLine = 3;
constexpr int GNumOfAtomsIndex = 0;
constexpr int GNumOfBondsIndex = 1;
constexpr int GChiralFlagIndex = 3;
constexpr int GNumOfHeaderLines = 4;

// Member functions
//---------------------
AMolecule::AMolecule() : AtomCount(-1), BondCount(-1), IsChiral(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = RootSceneComp;
}

void AMolecule::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMolecule::Initialise(const FString& FilePath)
{	
	// Load our file to an array of strings (one for each line)
	TArray<FString> FileContents;
	LoadFileToString(FilePath, FileContents);

	// Grab our header lines
	TArray<FString> HeaderLines;
	Title = FileContents[GTitleLine];
	Timestamp = FileContents[GProgramTimeStamp];
	Comments = FileContents[GCommentLine];

	// Grab our count line
	FindCounts(FileContents[GCountsLine]);
		
	// Grab all our atom and bond string lines
	int LineNum = 4;
	TArray<FString> AtomLines;
	TArray<FString> BondLines;
	while(LineNum < AtomCount + BondCount + GNumOfHeaderLines)
	{
		if(LineNum < AtomCount + GNumOfHeaderLines)
		{
			AtomLines.Add(FileContents[LineNum]);	
		}
		else
		{
			BondLines.Add(FileContents[LineNum]);
		}

		LineNum++;
	}

	// Create out atom components
	for(int i = 0; i < AtomLines.Num(); i++)
	{
		TArray<FString> CurrentAtomLine;
		AtomLines[i].ParseIntoArray(CurrentAtomLine, TEXT(" "));

		// Grab our x,y,z values from the 
		FVector NewAtomPosition;
		NewAtomPosition.X = FCString::Atof(*CurrentAtomLine[0]);
		NewAtomPosition.Y = FCString::Atof(*CurrentAtomLine[1]);
		NewAtomPosition.Z = FCString::Atof(*CurrentAtomLine[2]);

		// Create a new atom component with the new position and the element type at the end of the line
		CreateAtomComponent(NewAtomPosition, CurrentAtomLine[3][0]);
	}

	// Create our bond components
	for(int i = 0; i < BondLines.Num(); i++)
	{
		TArray<FString> CurrentBondLine;
		BondLines[i].ParseIntoArray(CurrentBondLine, TEXT(" "));

		// Get the indexes of the atoms
		const int StartAtomIndex = FCString::Atoi(*CurrentBondLine[0]) - 1;
		const int EndAtomIndex = FCString::Atoi(*CurrentBondLine[1]) - 1;

		// Find our atom components
		UAtomComponent* FirstAtom = Atoms[StartAtomIndex];
		UAtomComponent* SecondAtom = Atoms[EndAtomIndex];
		
		// IsDouble
		const bool IsDoubleBonded = FCString::Atoi(*CurrentBondLine[2]) == 2 ? true : false; 

		// Create our bond
		CreateBondComponent(FirstAtom, SecondAtom, IsDoubleBonded);
	}

	// Now we have our data, initialise our atom widget on the frontend
	InitialiseAtomWidget();
}


// Called every frame
void AMolecule::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMolecule::CreateAtomComponent(const FVector& Pos, const TCHAR& ElementChar)
{
	// Create a new atom component
	UAtomComponent* NewAtomComp = NewObject<UAtomComponent>(this, UAtomComponent::StaticClass());
	if(NewAtomComp)
	{
		// Add all the data
		NewAtomComp->SetPosition(Pos);
		NewAtomComp->SetElement(ElementChar);

		// Register, attach and record the new atom in our array
		NewAtomComp->RegisterComponent();
		NewAtomComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		Atoms.Add(NewAtomComp);	
	}	
}

void AMolecule::CreateBondComponent(UAtomComponent* SAtom, UAtomComponent* EAtom, const bool IsDouble)
{
	// Create a new bond component
	UBondComponent* NewBondComp = NewObject<UBondComponent>(this, UBondComponent::StaticClass());
	if(NewBondComp)
	{
		NewBondComp->SetStartAtom(SAtom);
		NewBondComp->SetEndAtom(EAtom);
		NewBondComp->SetIsDoubledBonded(IsDouble);

		// Register and record the new atom in our array
		NewBondComp->RegisterComponent();
		Bonds.Add(NewBondComp);	
	}
}


void AMolecule::LoadFileToString(const FString& FilePath, TArray<FString>& FileContents)
{	
	// We will use this FileManager to deal with the file.
	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

	// String to hold full text dump of the SDF file
	FString TextDump;
	
	// Always first check if the file that you want to manipulate exist.
	if (FileManager.FileExists(*FilePath))
	{
		// We use the LoadFileToString to load the file into FileContent
		if(FFileHelper::LoadFileToString(TextDump,*FilePath,FFileHelper::EHashOptions::None))
		{
			UE_LOG(LogTemp, Warning, TEXT("FileManipulation: File loaded successfully"));  
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Did not load text from file"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: ERROR: Can not read the file because it was not found."));
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Expected file location: %s"),*FilePath);
	}

	// Break that string up into an array of strings (one for each line, incl. empty lines)
	TextDump.ParseIntoArray(FileContents, TEXT("\n"), false);
}

void AMolecule::FindCounts(const FString& CountsLine)
{
	// Create an array to hold our parsed string
	TArray<FString> CountsArray;

	// Create the delim character (space separated)
	TCHAR Delim = ' ';

	// Parse our space separated values into the CountsArray
	CountsLine.ParseIntoArray(CountsArray, TEXT(" "));

	// Grab the values we need
	AtomCount = FCString::Atoi(*CountsArray[GNumOfAtomsIndex]);
	BondCount = FCString::Atoi(*CountsArray[GNumOfBondsIndex]);
	IsChiral = FCString::Atoi(*CountsArray[GChiralFlagIndex]) ? true : false;
}
