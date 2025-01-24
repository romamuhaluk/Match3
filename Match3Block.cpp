// Copyright Epic Games, Inc. All Rights Reserved.

#include "Match3Block.h"
#include "Match3BlockGrid.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"

AMatch3Block::AMatch3Block()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;

		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> AmberMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> OrangeGem;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))
			, BaseMaterial(TEXT("/Game/Puzzle/Meshes/BaseMaterial.BaseMaterial"))
			, BlueMaterial(TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial"))
			, OrangeMaterial(TEXT("/Game/Puzzle/Meshes/OrangeMaterial.OrangeMaterial"))

			, AmberMesh(TEXT("/Game/Assets/Gems/Amber.Amber"))
			, OrangeGem(TEXT("/Game/Assets/Gems/SolidOrangeGem.SolidOrangeGem"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	static FConstructorStatics ConstructorGemStatics;

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
	MyComponentGems->SetStaticMesh(ConstructorGemStatics.AmberMesh.Get());           //Встановлює статичну модель (меш) 
	MyComponentGems->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));			      //масштаб об'єкта
	MyComponentGems->SetRelativeLocation(FVector(0.f, 0.f, 100.f));					  //Встановлює відносне положення в координатах
	MyComponentGems->SetMaterial(0, ConstructorGemStatics.OrangeGem.Get());			  //Призначає матеріал, текстуру чи вигляд поверхні
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

	// Change material
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
