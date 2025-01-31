// Copyright Epic Games, Inc. All Rights Reserved.

#include "Match3Block.h"
#include "random"

#include "Match3BlockGrid.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"

static AMatch3Block* ActiveBlock = nullptr;   // активний блок
extern TArray<TArray<AMatch3Block*>> GridBlock;
extern int32 Size;

// Structure to hold one-time initialization
struct FConstructorStatics
{
	ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
	ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
	ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
	ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;

	ConstructorHelpers::FObjectFinderOptional<UStaticMesh> AmberMesh;
	ConstructorHelpers::FObjectFinderOptional<UMaterial> OrangeGem;

	ConstructorHelpers::FObjectFinderOptional<UStaticMesh> AmethystMesh;
	ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> PurpleGem;

	ConstructorHelpers::FObjectFinderOptional<UStaticMesh> GarnetMesh;
	ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> RedGem;

	ConstructorHelpers::FObjectFinderOptional<UStaticMesh> EmeraldMesh;
	ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> GreenGem;
	FConstructorStatics()
		: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))
		, BaseMaterial(TEXT("/Game/Puzzle/Meshes/BaseMaterial.BaseMaterial"))
		, BlueMaterial(TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial"))
		, OrangeMaterial(TEXT("/Game/Puzzle/Meshes/OrangeMaterial.OrangeMaterial"))

		, AmberMesh(TEXT("/Game/Assets/Gems/Amber.Amber"))
		, OrangeGem(TEXT("/Game/Assets/Gems/SolidOrangeGem.SolidOrangeGem"))

		, AmethystMesh(TEXT("/Game/Assets/Gems/Amethyst.Amethyst"))
		, PurpleGem(TEXT("/Game/Assets/Gems/TransparentPurpleGem.TransparentPurpleGem"))
		
		, GarnetMesh(TEXT("/Game/Assets/Gems/Garnet.Garnet"))
		, RedGem(TEXT("/Game/Assets/Gems/SolidRedGem.SolidRedGem"))

		, EmeraldMesh(TEXT("/Game/Assets/Gems/Emerald.Emerald"))
		, GreenGem(TEXT("/Game/Assets/Gems/TransparentGreenGem.TransparentGreenGem"))
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

	MyComponentGems = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gem0")); //Створює компонент
	randomGem(MyComponentGems);  
	//MyComponentGems->SetStaticMesh(ConstructorGemStatics.AmberMesh.Get());           //Встановлює статичну модель (меш)
	//MyComponentGems->SetMaterial(0, ConstructorGemStatics.OrangeGem.Get());			  //Призначає матеріал, текстуру чи вигляд поверхні
	MyComponentGems->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));			      //масштаб об'єкта
	MyComponentGems->SetRelativeLocation(FVector(0.f, 0.f, 100.f));					  //Встановлює відносне положення в координатах
	//Закріплює до батьківського компонента DummyRoot, щоб рухи/зміни DummyRoot впливали на об'єкт.
	MyComponentGems->SetupAttachment(DummyRoot);	
	MyComponentGems->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	MyComponentGems->OnClicked.AddDynamic(this, &AMatch3Block::BlockClicked);         //Реєструє динамічний обробник події OnClicked
	//MyComponentGems->OnInputTouchBegin.AddDynamic(this, &AMatch3Block::OnFingerPressedBlock);  //Реєструє обробник події OnInputTouchBegin

	// Save a pointer to the orange material
	BaseMaterial = ConstructorStatics.BaseMaterial.Get();
	BlueMaterial = ConstructorStatics.BlueMaterial.Get();
	OrangeMaterial = ConstructorStatics.OrangeMaterial.Get();
}

void AMatch3Block::randomGem(UStaticMeshComponent* Component) {
	static FConstructorStatics ConstructorGemStatics;
	int maxGems(4);

	switch (randomNum(maxGems))
	{
	case 1:
		Component->SetStaticMesh(ConstructorGemStatics.AmberMesh.Get());
		Component->SetMaterial(0, ConstructorGemStatics.OrangeGem.Get());
		break;

	case 2:
		Component->SetStaticMesh(ConstructorGemStatics.AmethystMesh.Get());
		Component->SetMaterial(0, ConstructorGemStatics.PurpleGem.Get());
		break;

	case 3:
		Component->SetStaticMesh(ConstructorGemStatics.GarnetMesh.Get());
		Component->SetMaterial(0, ConstructorGemStatics.RedGem.Get());
		break;

	case 4:
		Component->SetStaticMesh(ConstructorGemStatics.EmeraldMesh.Get());
		Component->SetMaterial(0, ConstructorGemStatics.GreenGem.Get());
		break;

	default:
		// Якщо рандомне число виходить за межі, можна встановити стандартний меш або нічого не робити
		Component->SetStaticMesh(nullptr);
		Component->SetMaterial(0, nullptr);
		break;
	}
}

int AMatch3Block::randomNum(int maxGems) {
	std::random_device rd;
	std::mt19937 mersenne(rd());
	std::uniform_int_distribution<> dist(1, maxGems);
	return dist(mersenne);
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
			
			int column = ActiveBlock->positionInGrid / 1000;
			int row = ActiveBlock->positionInGrid % 1000;
			//AMatch3Block* temp = this;

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


				if (this == GridBlock[newColumn][newRow])
				{
					ActiveBlock->positionInGrid = newColumn * 1000 + newRow;
					GridBlock[newColumn][newRow] = ActiveBlock;
					positionInGrid = column * 1000 + row;
					GridBlock[column][row] = this;
					MoveGems(this, ActiveBlock);
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
		}
		
}

void AMatch3Block::MoveGems(AMatch3Block* Object1, AMatch3Block* Object2)
{
	FVector StartPos1, StartPos2;
	//FRotator StartRot1, StartRot2;
	//USceneComponent* StartRoot1, * StartRoot2;  // Для збереження початкових RootComponent
	
	// Отримуємо початкові позиції та орієнтації для об'єктів
	StartPos1 = Object1->GetActorLocation();
	StartPos2 = Object2->GetActorLocation();

	//StartRot1 = Object1->GetActorRotation();
	//StartRot2 = Object2->GetActorRotation();

	// Зберігаємо початкові RootComponent
	//StartRoot1 = Object1->GetRootComponent();
	//StartRoot2 = Object2->GetRootComponent();

	// Інтерполяція між початковими позиціями
	//float Alpha = FMath::Clamp(CurrentTime / MoveDuration, 0.0f, 1.0f);

	//FVector NewPos1 = FMath::Lerp(StartPos1, StartPos2, 0);//Alpha);
	//FVector NewPos2 = FMath::Lerp(StartPos2, StartPos1, 0);//Alpha);

	//FRotator NewRot1 = FMath::Lerp(StartRot1, StartRot2, Alpha);
	//FRotator NewRot2 = FMath::Lerp(StartRot2, StartRot1, Alpha);

	// Оновлюємо позиції та орієнтації
	//Object1->SetActorLocation(NewPos1);
	//Object2->SetActorLocation(NewPos2);

	//Object1->SetActorRotation(NewRot1);
	//Object2->SetActorRotation(NewRot2);

	// Оновлюємо RootComponent після інтерполяції
	{
//		Object1->SetRootComponent(StartRoot2);  // Поміняти RootComponent
//		Object2->SetRootComponent(StartRoot1);  // Поміняти RootComponent
	}

	// Фіксуємо остаточні позиції та орієнтації
	Object1->SetActorLocation(StartPos2);
	Object2->SetActorLocation(StartPos1);

	//Object1->SetActorRotation(StartRot2);
	//Object2->SetActorRotation(StartRot1);
		
	



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
