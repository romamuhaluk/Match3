// Copyright Epic Games, Inc. All Rights Reserved.

#include "Match3BlockGrid.h"
//#include "Match3Block.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

AMatch3BlockGrid::AMatch3BlockGrid()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	//ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	//ScoreText->SetRelativeLocation(FVector(200.f,0.f,0.f));
	//ScoreText->SetRelativeRotation(FRotator(90.f,0.f,0.f));
	//ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(0)));
	//ScoreText->SetupAttachment(DummyRoot);

	// Set defaults
	Size = 9;
	BlockSpacing = 300.f;
}


void AMatch3BlockGrid::BeginPlay()
{
	Super::BeginPlay();
	
	// Number of blocks
	const int32 NumBlocks = Size;

	GridBlock.SetNum(Size); //SetNum для ініціалізації розміру масиву

	// Loop to spawn each block
	for(int32 BlockIndex = 0; BlockIndex< Size; BlockIndex++)
	{
		GridBlock[BlockIndex].SetNum(Size);   // SetNum для кожного підмасиву

		for (int32 BlockIndex2 = 0; BlockIndex2 < Size; BlockIndex2++)
		{

			const float XOffset = (BlockIndex2%Size) * BlockSpacing; // Divide by dimension
			const float YOffset = (BlockIndex%Size) * BlockSpacing;

			// Make position vector, offset from Grid location
			const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();


			// Spawn a block
			AMatch3Block* NewBlock = GetWorld()->SpawnActor<AMatch3Block>(BlockLocation, FRotator(0, 0, 0));

			if (NewBlock)
			{
				// Додаємо блок в відповідний рядок і стовпчик
				GridBlock[BlockIndex][BlockIndex2] = NewBlock;   // де BlockIndex — це рядок, а BlockIndex2 — це стовпець.
			}
		}
	}
	
}



#undef LOCTEXT_NAMESPACE
