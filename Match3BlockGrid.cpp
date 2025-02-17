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

    wasMatchAfterSwap = false;
    trySwapToMatch = false;

    PrimaryActorTick.bCanEverTick = true;
	Size = 9;
	BlockSpacing = 300.f;
}


void AMatch3BlockGrid::BeginPlay()
{
	Super::BeginPlay();

	AMatch3BlockGrid::CreateGrid();

    AMatch3BlockGrid::CheckMatch();

    AMatch3BlockGrid::CheckEmptyCell();

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
            if (Grid[column][row]->gem == nullptr)     // ���� ��� ���������� 
            {
                for (int temprow = row + 1; temprow < Size; temprow++)       // ������ ��������� �������� ���
                {
                    if (Grid[column][temprow]->gem == nullptr)               
                    {
                        continue;
                    }
                    // �������� �������� ��� �� ���� ����������� � ����� grid
                    Grid[column][row]->gem = Grid[column][temprow]->gem;
                    Grid[column][temprow]->gem = nullptr;

                    // ��������� ������ ����� ��� ���� �������� ������� � grid
                    Grid[column][row]->gem->columnInGrid = column;
                    Grid[column][row]->gem->rowInGrid = row;

                    Grid[column][row]->gem->straightAnimation = true;

                    if (Grid[column][row]->block != nullptr)
                        {
                            Grid[column][row]->block->GetBlockMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
                        }

                    //���������� ��������� ��� �� ���� �����������
                    Grid[column][row]->gem->SwapOnPoint(Grid[column][row]->point);

                    break;
                }
            }
        }
    }

    for (int column = 0; column < Size; column++)
    {
        if(Grid[column][Size - 1]->gem == nullptr) 
        {
            for (int row = 0; row < Size; row++)
            {
                if (Grid[column][row]->gem == nullptr)
                {
                    const float XOffset = ((row % Size) + Size - 1) * BlockSpacing ; // Divide by dimension
                    const float YOffset = (column % Size) * BlockSpacing;

                    AGem* NewGem = GetWorld()->SpawnActor<AGem>((FVector(XOffset, YOffset, 40.f) + GetActorLocation()), FRotator(0, 0, 0));

                    if (NewGem)
                    {
                        NewGem->columnInGrid = column;
                        NewGem->rowInGrid = row;
                        NewGem->straightAnimation = true;

                        // ������ � ��������� ����� � ��������
                        Grid[column][row]->gem = NewGem;

                        Grid[column][row]->gem->straightAnimation = true;

                        if (Grid[column][row]->block != nullptr)
                        {
                            Grid[column][row]->block->GetBlockMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
                        }

                        //���������� ��������� ��� �� ���� ����������� � �������� ����
                        Grid[column][row]->gem->SwapOnPoint(Grid[column][row]->point);
                    }
                }
            }
        }
    }
}

void AMatch3BlockGrid::CheckMatch()
{
    GridObject->wasMatchAfterSwap = false;
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
                    wasMatchAfterSwap = true;
                    for (int tempRow = row; consecutiveMatches >= 0; consecutiveMatches--, tempRow--)
                    {
                        //Grid[column][tempRow]->gem->SetActorHiddenInGame(true);
                        Grid[column][tempRow]->gem->GetGemMesh()->SetSimulatePhysics(true);
                        Grid[column][tempRow]->gem->GetGemMesh()->SetEnableGravity(true);
                        Grid[column][tempRow]->gem->GetGemMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

                        //Grid[column][tempRow]->gem->GetGemMesh()->SetCollisionObjectType(ECC_PhysicsBody);

                        //�������� ���������� ���� � ��������� ����
                        //Grid[column][tempRow]->gem->GetGemMesh()->OnClicked.Clear(); 
                        //Grid[column][tempRow]->gem->GetGemMesh()->OnInputTouchBegin.Clear();
                        Grid[column][tempRow]->gem->GetGemMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore); // cursore ignore
                        
                        Grid[column][tempRow]->gem->GetGemMesh()->AddForce(FVector(50000.0f, (-100.0f * randomNum(50) + 100.0f * randomNum(50)), 10000.0f), NAME_None, true);
                        //Grid[column][tempRow]->gem->GetGemMesh()->AddForce(FVector(10000.0f, 0.0f, 5000.0f), NAME_None, true);

                        Grid[column][tempRow]->gem->SetLifeSpan(5);

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
                    wasMatchAfterSwap = true;
                    for (int tempColumn = column; consecutiveMatches >= 0; consecutiveMatches--, tempColumn--)
                    {
                        //Grid[tempColumn][row]->gem->SetActorHiddenInGame(true);
                        Grid[tempColumn][row]->gem->GetGemMesh()->SetSimulatePhysics(true);
                        Grid[tempColumn][row]->gem->GetGemMesh()->SetEnableGravity(true);
                        Grid[tempColumn][row]->gem->GetGemMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

                        //Grid[tempColumn][row]->gem->GetGemMesh()->SetCollisionObjectType(ECC_PhysicsBody);

                        //�������� ���������� ���� � ��������� ����
                        //Grid[tempColumn][row]->gem->GetGemMesh()->OnClicked.Clear();
                        //Grid[tempColumn][row]->gem->GetGemMesh()->OnInputTouchBegin.Clear();
                        Grid[tempColumn][row]->gem->GetGemMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore); // cursore ignore

                        Grid[tempColumn][row]->gem->GetGemMesh()->AddForce(FVector(50000.0f, (-10.0f * randomNum(10000) + 10.0f * randomNum(10000)), 10000.0f), NAME_None, true);
                        //Grid[tempColumn][row]->gem->GetGemMesh()->AddForce(FVector(10000.0f, 0.0f, 5000.0f), NAME_None, true);

                        Grid[tempColumn][row]->gem->SetLifeSpan(5);

                        Grid[tempColumn][row]->gem = nullptr;
                    }
                }
                else { consecutiveMatches = 0; }
            }
        }
    }
}

// �� column � �� ��������, � row � �� ����� (row).
void AMatch3BlockGrid::CreateGrid()
{
	Grid.SetNum(Size); //SetNum ��� ����������� ��������

	// Loop to spawn each block
	for (int32 column = 0; column < Size; column++)
	{
		Grid[column].SetNum(Size);   // SetNum ��� ������� �����

        //Grid[column][Size] = new FGridElement();  // � ����� ������� ���������� ����� ��� ��������� �������� ����

		for (int32 row = 0; row < Size; row++)
		{

			const float XOffset = (row % Size) * BlockSpacing; // Divide by dimension
			const float YOffset = (column % Size) * BlockSpacing;

			// Make position vector, offset from Grid location
			const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

			// Spawn a block
			AMatch3Block* NewBlock = GetWorld()->SpawnActor<AMatch3Block>(BlockLocation, FRotator(0, 0, 0));

			AGem* NewGem = GetWorld()->SpawnActor<AGem>((BlockLocation + FVector(0, 0, 30.f)), FRotator(0, 0, 0));

			if (NewBlock && NewGem)
			{
				Grid[column][row] = new FGridElement();  // �������� ���'�� ��� ������� ��������

                NewBlock->columnInGrid = column;
                NewBlock->rowInGrid = row;

                NewGem->columnInGrid = column;
                NewGem->rowInGrid = row;

				// ������ � ��������� ����� � ��������
				Grid[column][row]->block = NewBlock;   // �� column � �� ��������, � row � �� �����.
				Grid[column][row]->gem = NewGem;
                Grid[column][row]->point = NewGem->GetActorLocation();//(BlockLocation + FVector(0, 0, 40.f));
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE