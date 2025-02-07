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

	// Create static mesh component
	//ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	//ScoreText->SetRelativeLocation(FVector(200.f,0.f,0.f));
	//ScoreText->SetRelativeRotation(FRotator(90.f,0.f,0.f));
	//ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(0)));
	//ScoreText->SetupAttachment(DummyRoot);

	// Set defaults
    GridObject = this;

    PrimaryActorTick.bCanEverTick = true;
	Size = 9;
	BlockSpacing = 300.f;
}


void AMatch3BlockGrid::BeginPlay()
{
	Super::BeginPlay();

	AMatch3BlockGrid::CreateGrid();

    AMatch3BlockGrid::CheckMatch();

    CheckEmptyCell();

}

void AMatch3BlockGrid::Tick(float DeltaTime)
{
    //AMatch3BlockGrid::CheckMatch();

    //CheckEmptyCell();

}


void AMatch3BlockGrid::CheckEmptyCell()
{
    for (int column = 0; column < Size; column++)  //column
    {
        for (int row = 0; row < Size; row++)
        {
            if (Grid[column][row]->gem == nullptr)     // якщо гем пропущений 
            {
                for (int temprow = row + 1; temprow < Size; temprow++)       // шукаємо наступний існуючий гем
                {
                    if (Grid[column][temprow]->gem == nullptr)               
                    {
                        continue;
                    }
                    // розміщуємо існуючий гем на місці пропущеного в памяті grid
                    Grid[column][row]->gem = Grid[column][temprow]->gem;
                    Grid[column][temprow]->gem = nullptr;

                    // Оновлюємо знання обєкта про свою майбутню позицію в grid
                    Grid[column][row]->gem->columnInGrid = column;
                    Grid[column][row]->gem->rowInGrid = row;

                    //переміщуюмо знайдений гем на місце пропущеного
                    Grid[column][row]->gem->SwapOnPoint(Grid[column][row]->point);

                    break;
                }
            }
        }
    }
}

void AMatch3BlockGrid::CheckMatch()
{

    int consecutiveMatches(1);

    for (int column = 0; column < Size; column++)  //column
    {  
        consecutiveMatches = 0;

        for (int row = 0; row < Size; row++)
        {
            if (Grid[column][row]->gem != nullptr &&
                row != Size - 1 &&
                Grid[column][row + 1]->gem != nullptr &&
                Grid[column][row]->gem->GetGemMesh()->GetStaticMesh() == Grid[column][row + 1]->gem->GetGemMesh()->GetStaticMesh() &&
                consecutiveMatches != 4 )
            {
                consecutiveMatches++;
            }
            else
            {
                if (consecutiveMatches >= 2)
                {
                    for (int tempRow = row; consecutiveMatches >= 0; consecutiveMatches--, tempRow--)
                    {
                        //Grid[column][tempRow]->gem->SetActorHiddenInGame(true);
                        Grid[column][tempRow]->gem->GetGemMesh()->SetSimulatePhysics(true);
                        Grid[column][tempRow]->gem->GetGemMesh()->SetEnableGravity(true);

                        //заборона викликання кліку у падаючого гема
                        Grid[column][tempRow]->gem->GetGemMesh()->OnClicked.Clear(); 
                        Grid[column][tempRow]->gem->GetGemMesh()->OnInputTouchBegin.Clear();

                        Grid[column][tempRow]->gem->GetGemMesh()->AddForce(FVector(10000.0f, 0.0f, 5000.0f), NAME_None, true);

                        Grid[column][tempRow]->gem = nullptr;
                    }
                }
                else { consecutiveMatches = 0; }
            }
        }
    }

    consecutiveMatches = 0;
    
    for (int row = 0; row < Size; row++)  //row
    {
        consecutiveMatches = 0;

        for (int column = 0; column < Size; column++)
        {
            if (Grid[column][row]->gem != nullptr &&
                column != Size - 1 &&
                Grid[column + 1][row]->gem != nullptr &&
                Grid[column][row]->gem->GetGemMesh()->GetStaticMesh() == Grid[column + 1][row]->gem->GetGemMesh()->GetStaticMesh() &&
                consecutiveMatches != 4)
            {
                consecutiveMatches++;
            }
            else
            {
                if (consecutiveMatches >= 2)
                {
                    for (int tempColumn = column; consecutiveMatches >= 0; consecutiveMatches--, tempColumn--)
                    {
                        //Grid[tempColumn][row]->gem->SetActorHiddenInGame(true);
                        Grid[tempColumn][row]->gem->GetGemMesh()->SetSimulatePhysics(true);
                        Grid[tempColumn][row]->gem->GetGemMesh()->SetEnableGravity(true);

                        Grid[tempColumn][row]->gem->GetGemMesh()->AddForce(FVector(50000.0f, 0.0f, 10000.0f), NAME_None, true);

                        Grid[tempColumn][row]->gem = nullptr;
                    }
                }
                else { consecutiveMatches = 0; }
            }
        }
    }
}

// де BlockIndex — це стовпець (column), а BlockIndex2 — це рядок (row).
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

                NewBlock->columnInGrid = BlockIndex;
                NewBlock->rowInGrid = BlockIndex2;

                NewGem->columnInGrid = BlockIndex;
                NewGem->rowInGrid = BlockIndex2;

				// Додаємо в відповідний рядок і стовпчик
				Grid[BlockIndex][BlockIndex2]->block = NewBlock;   // де BlockIndex — це стовпець, а BlockIndex2 — це рядок.
				Grid[BlockIndex][BlockIndex2]->gem = NewGem;
                Grid[BlockIndex][BlockIndex2]->point = NewGem->GetActorLocation();//(BlockLocation + FVector(0, 0, 40.f));
			}
		}
	}
}


#undef LOCTEXT_NAMESPACE