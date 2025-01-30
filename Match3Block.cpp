// Copyright Epic Games, Inc. All Rights Reserved.

#include "random"
#include "Match3Block.h"
#include "Match3BlockGrid.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"

static AMatch3Block* ActiveBlock;   // активний блок

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
		bIsActive = true;



		if (!(ActiveBlock == nullptr)) {
		ActiveBlock->BlockMesh->SetMaterial(0, BlueMaterial);
		ActiveBlock->bIsActive = false;
		}
		ActiveBlock = this;
		

		BlockMesh->SetMaterial(0, OrangeMaterial);


	
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
