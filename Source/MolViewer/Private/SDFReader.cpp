// © 2021 W. Masek

// This file's header
#include "SDFReader.h"

// Other headers
#include "String/ParseLines.h"
#include "Engine/Public/DrawDebugHelpers.h"

// Constants
const int GTitleLine = 0;
const int GProgramTimeStamp = 1;
const int GCommentLine = 2;
const int GCountsLine = 3;

// Sets default values
ASDFReader::ASDFReader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASDFReader::BeginPlay()
{
	Super::BeginPlay();

	FString file = FPaths::ProjectDir();
	file.Append(TEXT("Content/SDF/"));
	file.Append(TEXT("Paracetamol.sdf"));

	// We will use this FileManager to deal with the file.
	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

	// Always first check if the file that you want to manipulate exist.
	FString FileContent;
	if (FileManager.FileExists(*file))
	{
		// We use the LoadFileToString to load the file into FileContent
		if(FFileHelper::LoadFileToString(FileContent,*file,FFileHelper::EHashOptions::None))
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
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Expected file location: %s"),*file);
	}

	// Parse this string into an array
	TArray<FString> AllLines;
	FileContent.ParseIntoArray(AllLines, TEXT("\n"));

	// Grab our header lines
	HeaderLines.Add(AllLines[GTitleLine]);
	HeaderLines.Add(AllLines[GProgramTimeStamp]);
	HeaderLines.Add(AllLines[GCommentLine]);

	// Grab our count line
	CountsLine = AllLines[GCountsLine];
	int AtomCount = -1;
	int BondCount = -1;
	FString NumString;
	for(int i = 0; i < CountsLine.Len(); i++)
	{
		if(CountsLine[i] == ' ')
		{
			continue;
		}
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

	// Grab all our atom and bond string lines
	int LineNum = 4;
	while(LineNum < AtomCount + BondCount + 4)
	{
		if(LineNum < AtomCount + 4)
		{
			AtomLines.Add(AllLines[LineNum]);	
		}
		else
		{
			BondLines.Add(AllLines[LineNum]);
		}

		LineNum++;
	}

	// Convert our atom strings into atom structs
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

		FAtom NewAtom;
		NewAtom.Position.X = FCString::Atof(*AtomPositions[0]);
		NewAtom.Position.Y = FCString::Atof(*AtomPositions[1]);
		NewAtom.Position.Z = FCString::Atof(*AtomPositions[2]);

		const TCHAR ElementChar = CurrentString[31];
		if(ElementChar == 'O')
		{
			NewAtom.Element = OXYGEN;
		}
		else if(ElementChar == 'N')
		{
			NewAtom.Element = NITROGEN;
		}
		else if(ElementChar == 'C')
		{
			NewAtom.Element = CARBON;
		}
		else if(ElementChar == 'H')
		{
			NewAtom.Element = HYDROGEN;
		}
		
		AtomList.Add(NewAtom);
	}

	// Convert our Bond Lines into Bond structs
	for(int i = 0; i < BondLines.Num(); i++)
	{
		FString CurrentString = BondLines[i];
		bool bFoundFirstAtom = false;
		FString AtomNumberString;
		FBond NewBond;	
		for(int j = 0; j < 6; j++)
		{
			if(CurrentString[j] == ' ')
			{
				continue;
			}
			else
			{
				// Append this char to our number string
				AtomNumberString.AppendChar(CurrentString[j]);

				// Look ahead, if we run out of chars of the next one is a space, finalise this number
				if(j == 5 || CurrentString[j+1] == ' ')
				{
					// Convert this number string into an int
					const int AtomIndex = FCString::Atoi(*AtomNumberString) - 1;

					// If we haven't found the first atom connector yet then set it
					if(!bFoundFirstAtom)
					{						
						NewBond.StartAtom = &AtomList[AtomIndex];
						bFoundFirstAtom = true;
						AtomNumberString.Empty();
					}
					else
					{
						// Otherwise, set the end connector instead
						NewBond.EndAtom = &AtomList[AtomIndex];
					}
				}
			}
		}

		// Add our newly created bond
		BondList.Add(NewBond);
	}
}

// Called every frame
void ASDFReader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (auto Atom : AtomList)
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

	for (FBond Bond: BondList)
	{
		DrawDebugLine(GetWorld(), (Bond.StartAtom->Position * 100.0f), (Bond.EndAtom->Position * 100.0f), FColor::White, false, -1, 0, 5.0f);
	}

	
}

