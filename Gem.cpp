// Fill out your copyright notice in the Description page of Project Settings.


#include "Gem.h"
#include "random"

//static AGem* ActiveGem = nullptr;   // активний блок
//AMatch3Block* ActiveBlock;   // активний блок

//extern TArray<TArray<FGridElement*>> Grid;

struct FGemConstructorStatics
{
	ConstructorHelpers::FObjectFinderOptional<UStaticMesh> AmberMesh;
	ConstructorHelpers::FObjectFinderOptional<UMaterial> OrangeGem;

	ConstructorHelpers::FObjectFinderOptional<UStaticMesh> AmethystMesh;
	ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> PurpleGem;

	ConstructorHelpers::FObjectFinderOptional<UStaticMesh> GarnetMesh;
	ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> RedGem;

	ConstructorHelpers::FObjectFinderOptional<UStaticMesh> EmeraldMesh;
	ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> GreenGem;
	FGemConstructorStatics()
		: AmberMesh(TEXT("/Game/Assets/Gems/Amber.Amber"))
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

// Sets default values
AGem::AGem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	MyComponentGems = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GemMesh0")); //Створює компонент
	randomGem(MyComponentGems);
	//MyComponentGems->SetStaticMesh(ConstructorGemStatics.AmberMesh.Get());           //Встановлює статичну модель (меш)
	//MyComponentGems->SetMaterial(0, ConstructorGemStatics.OrangeGem.Get());			  //Призначає матеріал, текстуру чи вигляд поверхні
	MyComponentGems->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));			      //масштаб об'єкта
	MyComponentGems->SetRelativeLocation(FVector(0.f, 0.f, 100.f));					  //Встановлює відносне положення в координатах
	//Закріплює до батьківського компонента DummyRoot, щоб рухи/зміни DummyRoot впливали на об'єкт.
	MyComponentGems->SetupAttachment(GemRoot);
	MyComponentGems->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	MyComponentGems->OnClicked.AddDynamic(this, &AGem::GemClicked);         //Реєструє динамічний обробник події OnClicked
	MyComponentGems->OnInputTouchBegin.AddDynamic(this, &AGem::OnFingerPressedGem);  //Реєструє обробник події OnInputTouchBegin
}

// Called when the game starts or when spawned
//void AGem::BeginPlay() {	Super::BeginPlay();	}

// Called every frame
//void AGem::Tick(float DeltaTime)	{	Super::Tick(DeltaTime);	}

void AGem::randomGem(UStaticMeshComponent* Component) {
	static FGemConstructorStatics ConstructorGemStatics;
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

int AGem::randomNum(int maxGems) {
	std::random_device rd;
	std::mt19937 mersenne(rd());
	std::uniform_int_distribution<> dist(1, maxGems);
	return dist(mersenne);
}

void AGem::GemClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	HandleClicked();
}

void AGem::OnFingerPressedGem(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	HandleClicked();
}

void AGem::Highlight(bool bOn)
{
	int column = this->positionInGrid / 1000;
	int row = this->positionInGrid % 1000;

	if (Grid[column][row]->block != nullptr)
	{
		Grid[column][row]->block->Highlight(bOn);
	}
}

void AGem::HandleClicked()
{
	int column = this->positionInGrid / 1000;
	int row = this->positionInGrid % 1000;

	if (Grid[column][row]->block != nullptr) 
	{
		Grid[column][row]->block->HandleClicked();
	}
}

void AGem::SwapGems(AGem* Object2)
{
	FVector StartPos1, StartPos2;

	StartPos1 = this->GetActorLocation();
	StartPos2 = Object2->GetActorLocation();

	this->SetActorLocation(StartPos2);
	Object2->SetActorLocation(StartPos1);



	int column = this->positionInGrid / 1000;
	int row = this->positionInGrid % 1000;

	int column2 = Object2->positionInGrid / 1000;
	int row2 = Object2->positionInGrid % 1000;

	Grid[column][row]->gem = Object2;
	Grid[column2][row2]->gem = this;

	int temp = this->positionInGrid;
	this->positionInGrid = Object2->positionInGrid;
	Object2->positionInGrid = temp;
}
