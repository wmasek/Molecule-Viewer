// © 2021 W. Masek

// This file's header
#include "Molecule.h"

// Other includes
#include "AtomComponent.h"
#include "BondComponent.h"

// Constants
const int GTitleLine = 0;
const int GProgramTimeStamp = 1;
const int GCommentLine = 2;
const int GCountsLine = 3;
const int GElementCharacterIndex = 31;

// Sets default values
AMolecule::AMolecule()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = RootSceneComp;
}

// Called when the game starts or when spawned
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
	while(LineNum < AtomCount + BondCount + 4)
	{
		if(LineNum < AtomCount + 4)
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
		FString CurrentString = AtomLines[i];
		TArray<FString> AtomPositions = {"", "", ""};
		int PosCount = 0;
		for(int j = 0; j < 30; j++)
		{
			if(CurrentString[j] == ' ')
			{
				continue;
			}
			else
			{
				AtomPositions[PosCount].AppendChar(CurrentString[j]);
				if(CurrentString[j+1] == ' ')
				{
					PosCount++;
				}
			}
		}

		// Grab our x,y,z values from the 
		FVector NewAtomPosition;
		NewAtomPosition.X = FCString::Atof(*AtomPositions[0]);
		NewAtomPosition.Y = FCString::Atof(*AtomPositions[1]);
		NewAtomPosition.Z = FCString::Atof(*AtomPositions[2]);

		// Create a new atom component with the new position and the element type at the end of the line
		CreateAtomComponent(NewAtomPosition, CurrentString[GElementCharacterIndex]);
	}

	// Create our bond components
	for(int i = 0; i < BondLines.Num(); i++)
	{
		FString CurrentString = BondLines[i];
		FString AtomNumberString;

		UAtomComponent* FirstAtom = nullptr;
		UAtomComponent* SecondAtom = nullptr;
		
		for(int j = 0; j < 6; j++)
		{
			if(CurrentString[j] != ' ')
			{
				// Append this char to our number string
				AtomNumberString.AppendChar(CurrentString[j]);

				// Look ahead, if we run out of chars of the next one is a space, finalise this number
				if(j == 5 || CurrentString[j+1] == ' ')
				{
					// Convert this number string into an int
					const int AtomIndex = FCString::Atoi(*AtomNumberString) - 1;

					// If we haven't got our first atom pointer then set it
					if (!FirstAtom)
					{
						FirstAtom = Atoms[AtomIndex];
						AtomNumberString.Empty();
					}
					else
					{
						// Otherwise, set the second pointer instead
						SecondAtom = Atoms[AtomIndex];
					}
				}
			}
		}

		// Create our bond
		CreateBondComponent(FirstAtom, SecondAtom);
	}
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

void AMolecule::CreateBondComponent(UAtomComponent* SAtom, UAtomComponent* EAtom)
{
	// Create a new bond component
	UBondComponent* NewBondComp = NewObject<UBondComponent>(this, UBondComponent::StaticClass());
	if(NewBondComp)
	{
		NewBondComp->SetStartAtom(SAtom);
		NewBondComp->SetEndAtom(EAtom);

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
	AtomCount = -1;
	BondCount = -1;
	
	FString NumString;
	for(int i = 0; i < CountsLine.Len(); i++)
	{
		if(CountsLine[i] != ' ')
		{
			NumString.AppendChar(CountsLine[i]);
			if(i+1 == CountsLine.Len() || CountsLine[i+1] == ' ')
			{
				if(AtomCount == -1)
				{
					AtomCount = FCString::Atoi(*NumString);
				}
				else if(BondCount == -1)
				{
					BondCount = FCString::Atoi(*NumString);
				}

				// Empty our temp num string
				NumString.Empty();
			}
		}
	}
}
