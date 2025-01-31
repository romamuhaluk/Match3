// Copyright Epic Games, Inc. All Rights Reserved.

#include "Match3BlockGrid.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

TArray<TArray<AMatch3Block*>> GridBlock;
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


}

/*
перевіряємо кожне непарний гем, якщо вони однакові перевіряємо гем між ними, якщо розмір сітки парний перевіряємо також останнє значення
we check each odd gem, if they are the same we check the gem between them, if the grid size is even we also check the last value
*/
// де BlockIndex — це стовпець (column), а BlockIndex2 — це рядок (row).
void AMatch3BlockGrid::CheckMatch() 
{
	UStaticMesh* comparGem(nullptr);

	bool isGridEven = !(Size % 2);

	for (int32 column = 0; column < Size; column++)  //column
	{
		comparGem = nullptr;

		for (int32 row = 0; row < Size; row += 2)
		{
			if (!(row % 2)) {  // перевірка лише парних індексів (непарні гемів)
				continue;
			}
			if (comparGem == nullptr) {
				comparGem = GridBlock[column][row]->GetGemMesh()->GetStaticMesh();
				continue;
			}

			if (comparGem == GridBlock[column][row]->GetGemMesh()->GetStaticMesh())
			{
				if (comparGem == GridBlock[column][row - 1]->GetGemMesh()->GetStaticMesh())
				{
					GridBlock[column][row]->SetActorHiddenInGame(true);
					GridBlock[column][row - 1]->SetActorHiddenInGame(true);
					GridBlock[column][row - 2]->SetActorHiddenInGame(true);
				}
			}
		}
		if (isGridEven) {   //if the grid size is even we also check the last value
			if (comparGem == GridBlock[column][Size - 1]->GetGemMesh()->GetStaticMesh()) {
				if (comparGem == GridBlock[column][Size - 3]->GetGemMesh()->GetStaticMesh()) {
					GridBlock[column][Size - 1]->SetActorHiddenInGame(true);
					GridBlock[column][Size - 2]->SetActorHiddenInGame(true);
					GridBlock[column][Size - 3]->SetActorHiddenInGame(true);
				}
			}
		}
	}

	for (int32 row = 0; row < Size; row++)  //row
	{
		comparGem = nullptr;

		for (int32 column = 0; column < Size; column += 2)
		{
			if (!(column % 2)) {  // перевірка лише парних індексів (непарні гемів)
				continue;
			}
			if (comparGem == nullptr) {
				comparGem = GridBlock[row][column]->GetGemMesh()->GetStaticMesh();
				continue;
			}

			if (comparGem == GridBlock[row][column]->GetGemMesh()->GetStaticMesh())
			{
				if (comparGem == GridBlock[row][column - 1]->GetGemMesh()->GetStaticMesh())
				{
					GridBlock[row][column]->SetActorHiddenInGame(true);
					GridBlock[row][column - 1]->SetActorHiddenInGame(true);
					GridBlock[row][column - 2]->SetActorHiddenInGame(true);
				}
			}
		}
		if (isGridEven) {   //if the grid size is even we also check the last value
			if (comparGem == GridBlock[row][Size - 1]->GetGemMesh()->GetStaticMesh()) {
				if (comparGem == GridBlock[row][Size - 3]->GetGemMesh()->GetStaticMesh()) {
					GridBlock[row][Size - 1]->SetActorHiddenInGame(true);
					GridBlock[row][Size - 2]->SetActorHiddenInGame(true);
					GridBlock[row][Size - 3]->SetActorHiddenInGame(true);
				}
			}
		}
	}
}

//SetActorHiddenInGame(true);

void AMatch3BlockGrid::CreateGrid()
{
	GridBlock.SetNum(Size); //SetNum для ініціалізації розміру масиву

	// Loop to spawn each block
	for (int32 BlockIndex = 0; BlockIndex < Size; BlockIndex++)
	{
		GridBlock[BlockIndex].SetNum(Size);   // SetNum для кожного підмасиву

		for (int32 BlockIndex2 = 0; BlockIndex2 < Size; BlockIndex2++)
		{

			const float XOffset = (BlockIndex2 % Size) * BlockSpacing; // Divide by dimension
			const float YOffset = (BlockIndex % Size) * BlockSpacing;

			// Make position vector, offset from Grid location
			const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();


			// Spawn a block
			AMatch3Block* NewBlock = GetWorld()->SpawnActor<AMatch3Block>(BlockLocation, FRotator(0, 0, 0));

			if (NewBlock)
			{
				NewBlock->positionInGrid = BlockIndex*1000 + BlockIndex2;
				// Додаємо блок в відповідний рядок і стовпчик
				GridBlock[BlockIndex][BlockIndex2] = NewBlock;   // де BlockIndex — це стовпець, а BlockIndex2 — це рядок.
			}
		}
	}
}



#undef LOCTEXT_NAMESPACE