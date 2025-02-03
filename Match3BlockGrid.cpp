// Copyright Epic Games, Inc. All Rights Reserved.

#include "Match3BlockGrid.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

TArray<TArray<FGridElement*>> Grid;
int32 Size;

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

	AMatch3BlockGrid::CreateGrid();

    AMatch3BlockGrid::CheckMatch();

}

/*
перевіряємо кожне непарний гем, якщо вони однакові перевіряємо гем між ними, якщо розмір сітки парний перевіряємо також останнє значення з передостаннім
we check each odd gem, if they are the same we check the gem between them, if the grid size is even we also check the last value with the penultimate
*/
// де BlockIndex — це стовпець (column), а BlockIndex2 — це рядок (row).


void AMatch3BlockGrid::CheckMatch()
{
    //UStaticMesh* comparGem(nullptr);

    //UStaticMesh* ActiveGemMesh;

    int consecutiveMatches(0);

    for (int column = 0; column < Size; column++)  //column
    {  
        consecutiveMatches = 0;

        for (int row = 0; row < (Size - 1); row++)
        {
            if (Grid[column][row]->gem->GetGemMesh()->GetStaticMesh() == Grid[column][row + 1]->gem->GetGemMesh()->GetStaticMesh()
                && consecutiveMatches != 4) 
            { 
                consecutiveMatches++; 
            }
            
            else 
            {
                if (consecutiveMatches >= 2) 
                {
                    for (int tempRow = row; consecutiveMatches >= 0; consecutiveMatches--, tempRow--)
                    {
                        Grid[column][tempRow]->gem->SetActorHiddenInGame(true);
                    }
                }
                else{ consecutiveMatches = 0; }
            }
        }
    }

    consecutiveMatches = 0;
    
    for (int row = 0; row < Size; row++)  //row
    {
        consecutiveMatches = 0;

        for (int column = 0; column < (Size - 1); column++)
        {
            if (Grid[column][row]->gem->GetGemMesh()->GetStaticMesh() == Grid[column + 1][row]->gem->GetGemMesh()->GetStaticMesh()
                && consecutiveMatches != 4)
            {
                consecutiveMatches++;
            }

            else
            {
                if (consecutiveMatches >= 2)
                {
                    for (int tempColumn = column; consecutiveMatches >= 0; consecutiveMatches--, tempColumn--)
                    {
                        Grid[tempColumn][row]->gem->SetActorHiddenInGame(true);
                    }
                }
                else { consecutiveMatches = 0; }
            }
        }
    }
}

void AMatch3BlockGrid::CreateGrid()
{
	Grid.SetNum(Size); //SetNum для ініціалізації розміру масиву

	// Loop to spawn each block
	for (int32 BlockIndex = 0; BlockIndex < Size; BlockIndex++)
	{
		Grid[BlockIndex].SetNum(Size);   // SetNum для кожного підмасиву

		for (int32 BlockIndex2 = 0; BlockIndex2 < Size; BlockIndex2++)
		{

			const float XOffset = (BlockIndex2 % Size) * BlockSpacing; // Divide by dimension
			const float YOffset = (BlockIndex % Size) * BlockSpacing;

			// Make position vector, offset from Grid location
			const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

			// Spawn a block
			AMatch3Block* NewBlock = GetWorld()->SpawnActor<AMatch3Block>(BlockLocation, FRotator(0, 0, 0));

			AGem* NewGem = GetWorld()->SpawnActor<AGem>((BlockLocation + FVector(0, 0, 40.f)), FRotator(0, 0, 0));

			if (NewBlock && NewGem)
			{
				Grid[BlockIndex][BlockIndex2] = new FGridElement();  // Виділення пам'яті для кожного елемента

				NewBlock->positionInGrid = BlockIndex * 1000 + BlockIndex2;
				NewGem->positionInGrid = BlockIndex * 1000 + BlockIndex2;

				// Додаємо в відповідний рядок і стовпчик
				Grid[BlockIndex][BlockIndex2]->block = NewBlock;   // де BlockIndex — це стовпець, а BlockIndex2 — це рядок.
				Grid[BlockIndex][BlockIndex2]->gem = NewGem;
				
			}
		}
	}
}



#undef LOCTEXT_NAMESPACE