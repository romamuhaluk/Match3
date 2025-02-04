// Copyright Epic Games, Inc. All Rights Reserved.

#include "Match3BlockGrid.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

TArray<TArray<FGridElement*>> Grid;
int32 Size;
AMatch3BlockGrid* GridObject;

AMatch3BlockGrid::AMatch3BlockGrid()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

    GridObject = this;

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

void AMatch3BlockGrid::CheckMatch()
{

    int consecutiveMatches(0);

    for (int column = 0; column < Size; column++)  //column
    {  
        consecutiveMatches = 0;

        for (int row = 0; row < Size; row++)
        {
            if (row != Size - 1 &&
                Grid[column][row]->gem->GetGemMesh()->GetStaticMesh() == Grid[column][row + 1]->gem->GetGemMesh()->GetStaticMesh()
                && consecutiveMatches != 4 ) 
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

        for (int column = 0; column < Size; column++)
        {
            if (column != Size - 1 &&
                Grid[column][row]->gem->GetGemMesh()->GetStaticMesh() == Grid[column + 1][row]->gem->GetGemMesh()->GetStaticMesh()
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

// де BlockIndex Ч це стовпець (column), а BlockIndex2 Ч це р€док (row).
void AMatch3BlockGrid::CreateGrid()
{
	Grid.SetNum(Size); //SetNum дл€ ≥н≥ц≥ал≥зац≥њ розм≥ру масиву

	// Loop to spawn each block
	for (int32 BlockIndex = 0; BlockIndex < Size; BlockIndex++)
	{
		Grid[BlockIndex].SetNum(Size);   // SetNum дл€ кожного п≥дмасиву

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
				Grid[BlockIndex][BlockIndex2] = new FGridElement();  // ¬ид≥ленн€ пам'€т≥ дл€ кожного елемента

				NewBlock->positionInGrid = BlockIndex * 1000 + BlockIndex2;
				NewGem->positionInGrid = BlockIndex * 1000 + BlockIndex2;

				// ƒодаЇмо в в≥дпов≥дний р€док ≥ стовпчик
				Grid[BlockIndex][BlockIndex2]->block = NewBlock;   // де BlockIndex Ч це стовпець, а BlockIndex2 Ч це р€док.
				Grid[BlockIndex][BlockIndex2]->gem = NewGem;
				
			}
		}
	}
}



#undef LOCTEXT_NAMESPACE