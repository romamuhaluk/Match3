// Copyright Epic Games, Inc. All Rights Reserved.

#include "Match3Block.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"

AMatch3Block* ActiveBlock = nullptr;   // активний блок

//extern TArray<TArray<FGridElement*>> Grid;
extern int32 Size;


// Structure to hold one-time initialization
struct FConstructorStatics
{
	ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
	ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
	ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
	ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;

	FConstructorStatics()
		: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))
		, BaseMaterial(TEXT("/Game/Puzzle/Meshes/BaseMaterial.BaseMaterial"))
		, BlueMaterial(TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial"))
		, OrangeMaterial(TEXT("/Game/Puzzle/Meshes/OrangeMaterial.OrangeMaterial"))
	{
	}
};

AMatch3Block::AMatch3Block()
{
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	BlockMesh->SetRelativeScale3D(FVector(1.f,1.f,0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.f,0.f,25.f));
	BlockMesh->SetMaterial(0, ConstructorStatics.BlueMaterial.Get());
	BlockMesh->SetupAttachment(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &AMatch3Block::BlockClicked);
	BlockMesh->OnInputTouchBegin.AddDynamic(this, &AMatch3Block::OnFingerPressedBlock);

	// Save a pointer to the orange material
	BaseMaterial = ConstructorStatics.BaseMaterial.Get();
	BlueMaterial = ConstructorStatics.BlueMaterial.Get();
	OrangeMaterial = ConstructorStatics.OrangeMaterial.Get();
}

void AMatch3Block::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	HandleClicked();
}

void AMatch3Block::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	HandleClicked();
}

void AMatch3Block::HandleClicked()
{
	if (ActiveBlock != nullptr && this != ActiveBlock) {

		int column = ActiveBlock->columnInGrid;
		int row = ActiveBlock->rowInGrid;

		int directions[4][2] = {
			{0, 1},   // вверх
			{0, -1},  // вниз
			{1, 0},   // вправо
			{-1, 0}   // вліво
		};

		if (column == (Size - 1)) { //заборона доступу вправо
			directions[2][0] = 0;
		}
		if (row == (Size - 1)) {    //заборона доступу вгору
			directions[0][1] = 0;
		}
		if (column == 0) {          //заборона доступу ліворуч
			directions[3][0] = 0;
		}
		if (row == 0) {				//заборона доступу вниз
			directions[1][1] = 0;
		}

		for (int i = 0; i < 4; ++i) {

			int newColumn = column + directions[i][0];
			int newRow = row + directions[i][1];


			if (this == Grid[newColumn][newRow]->block)        // якщо новий блок знаходиться біля активного відбувається swap
			{	
				if (Grid[newColumn][newRow]->gem == nullptr)                          // якщо новий блок без гема, міняємо активний гем з точкою на місці нового блоку
				{
					if (Grid[column][row]->gem != nullptr)							  // якщо активний блок з гемом свапаємо його з точкою на місці пустого гему
					{
						// Оновлюємо знання обєкта про свою майбутню позицію в grid
						Grid[column][row]->gem->columnInGrid = newColumn;
						Grid[column][row]->gem->rowInGrid = newRow;

						// Оновлюємо позицію в гриді
						Grid[newColumn][newRow]->gem = Grid[column][row]->gem;
						Grid[column][row]->gem = nullptr;

						Grid[newColumn][newRow]->gem->SwapOnPoint(Grid[newColumn][newRow]->point);
					}
				}
				else
				{
					if (Grid[column][row]->gem != nullptr)           // якщо є два геми свапаємо їх
					{
						Grid[column][row]->gem->SwapGems(Grid[newColumn][newRow]->gem);
					}
				}				
				//GridObject->CheckMatch();
				break;  // Якщо вже знайдений збіг, більше не потрібно перевіряти інші напрямки
			}
		}

		ActiveBlock->BlockMesh->SetMaterial(0, BlueMaterial);
		BlockMesh->SetMaterial(0, BlueMaterial);
		ActiveBlock->bIsActive = false;
		bIsActive = false;
		ActiveBlock = nullptr;
	}
	else
	{
		BlockMesh->SetMaterial(0, OrangeMaterial);
		bIsActive = true;
		ActiveBlock = this;
	};
}

void AMatch3Block::Highlight(bool bOn)
{
	// Do not highlight if the block has already been activated.
	if (bIsActive)
	{
		return;
	}

	if (bOn)
	{
		BlockMesh->SetMaterial(0, BaseMaterial);
	}
	else
	{
		BlockMesh->SetMaterial(0, BlueMaterial);
	}
}

/*
// Видаляємо компонент з ієрархії та відключаємо його оновлення
Component1->UnregisterComponent();
Component2->UnregisterComponent();

RegisterComponent() - додаємо
*/