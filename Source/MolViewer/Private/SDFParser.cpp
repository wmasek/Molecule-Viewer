// © 2021 W. Masek

// This file's header
#include "SDFParser.h"

// Other includes
#include "HAL/FileManagerGeneric.h"
#include "Molecule.h"
#include "SDFFile.h"
#include "Kismet/GameplayStatics.h"

// Constants
const int GTitleLine = 0;
const int GProgramTimeStamp = 1;
const int GCommentLine = 2;
const int GCountsLine = 3;

TArray<FString> USDFParser::FindSDFFiles()
{
	FString Directory = FPaths::ProjectDir();
	Directory.Append(TEXT("Content/SDF/"));

	TArray<FString> Output;
	Output.Empty();
	if (FPaths::DirectoryExists(Directory))
	{
	    const FString Path = Directory + "*.sdf";
	    FFileManagerGeneric::Get().FindFiles(Output, *Path, true, false);
	    for (int i = 0; i < Output.Num(); i++)
	    {
	        Output[i] = Directory + Output[i];
	    }
	}

	return Output;
}

void USDFParser::LoadSDFFile(FString FilePath)
{
	// Create a Molecule actor ready to receive the data.
	FActorSpawnParameters SpawnInfo;
	UWorld* World = GEngine->GameViewport->GetWorld();
	AMolecule* NewMolecule = Cast<AMolecule>(World->SpawnActorDeferred<AMolecule>(AMolecule::StaticClass(), FTransform()));
	
	// Load our file to one big FString
	TArray<FString> FileContents;
	LoadFileToString(FilePath, FileContents);

	// Grab our header lines
	TArray<FString> HeaderLines;
	NewMolecule->Title = FileContents[GTitleLine];
	NewMolecule->Timestamp = FileContents[GProgramTimeStamp];
	NewMolecule->Comments = FileContents[GCommentLine];

	// Grab our count line
	FindCounts(FileContents[GCountsLine], NewMolecule);
		
	// Grab all our atom and bond string lines
	int LineNum = 4;
	TArray<FString> AtomLines;
	TArray<FString> BondLines;
	while(LineNum < NewMolecule->AtomCount + NewMolecule->BondCount + 4)
	{
		if(LineNum < NewMolecule->AtomCount + 4)
		{
			AtomLines.Add(FileContents[LineNum]);	
		}
		else
		{
			BondLines.Add(FileContents[LineNum]);
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
		
		NewMolecule->AtomList.Add(NewAtom);
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
						NewBond.StartPos = NewMolecule->AtomList[AtomIndex].Position;
						bFoundFirstAtom = true;
						AtomNumberString.Empty();
					}
					else
					{
						// Otherwise, set the end connector instead
						NewBond.EndPos = NewMolecule->AtomList[AtomIndex].Position;
					}
				}
			}
		}

		// Add our newly created bond
		NewMolecule->BondList.Add(NewBond);
	}

	// Finish spawning the molecule now it has all the data
	UGameplayStatics::FinishSpawningActor(NewMolecule, FTransform());
}

void USDFParser::LoadFileToString(const FString& FilePath, TArray<FString>& FileContents)
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

	// Break that string up into an array of strings (one for each line
	TextDump.ParseIntoArray(FileContents, TEXT("\n"));
}

void USDFParser::FindCounts(const FString& CountsLine, AMolecule* Mol)
{
	Mol->AtomCount = -1;
	Mol->BondCount = -1;
	
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
				if(Mol->AtomCount == -1)
				{
					Mol->AtomCount = FCString::Atoi(*NumString);
				}
				else if(Mol->BondCount == -1)
				{
					Mol->BondCount = FCString::Atoi(*NumString);
				}

				// Empty our temp num string
				NumString.Empty();
			}
		}
	}
}
