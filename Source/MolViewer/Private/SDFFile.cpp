// © 2021 W. Masek


#include "SDFFile.h"

USDFFile::USDFFile()
{
	
}

void USDFFile::ChopFileName()
{
	for(int i = FullFilePath.Len() - 1; i >= 0; i--)
	{
		if(FullFilePath[i] == '/')
		{
			FileName = FullFilePath.RightChop(i+1);
			break;
		}
	}
}
