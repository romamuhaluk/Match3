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

	PrimaryActorTick.bCanEverTick = true;  
	ElapsedTime = 0.1f;     // Встановлюємо змінну для анімації
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

	if (Grid[columnInGrid][rowInGrid]->block != nullptr)
	{
		Grid[columnInGrid][rowInGrid]->block->Highlight(bOn);
	}
}

void AGem::HandleClicked()
{
	if (Grid[columnInGrid][rowInGrid]->block != nullptr)
	{
		Grid[columnInGrid][rowInGrid]->block->HandleClicked();
	}
}

void AGem::SwapOnPoint(FVector point)  // свап з пустою коміркою
{
	// Зберігаємо початкову позицію
	StartLocation = this->GetActorLocation();
	// Визначаємо кінцеву позицію
	EndLocation = point;

	SwapObject = nullptr;

	this->ElapsedTime = 0.0f;

	this->GetGemMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Починаємо анімацію (плавний рух)
	bIsSwapping = true;
}

void AGem::SwapGems(AGem* Object2)
{
	if (Object2 != nullptr) {
		FVector StartPos1 = this->GetActorLocation();
		FVector StartPos2 = Object2->GetActorLocation();

		// Зберігаємо початкові позиції
		this->StartLocation = StartPos1;
		Object2->StartLocation = StartPos2;

		// Визначаємо кінцеві позиції
		this->EndLocation = StartPos2;
		Object2->EndLocation = StartPos1;

		// Зберігаємо ссилки на об'єкти, щоб вони змінювали позиції в процесі
		this->SwapObject = Object2;
		Object2->SwapObject = this;

		//Вимикаємо взаємодію з обєктами поки вони рухаються
		this->GetGemMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Grid[columnInGrid][rowInGrid]->block->GetBlockMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Object2->GetGemMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Grid[Object2->columnInGrid][Object2->rowInGrid]->block->GetBlockMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		this->ElapsedTime = 0.0f;
		//Object2->ElapsedTime = 0.0f;

		// Починаємо анімацію (плавний рух)
		this->bIsSwapping = true;
		//Object2->bIsSwapping = true;
	}
}

void AGem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Якщо об'єкти в процесі обміну, оновлюємо їх позиції
	if (bIsSwapping)
	{
		float SwapDuration(0.4); //0.6

		ElapsedTime += DeltaTime;

		if (ElapsedTime < 0.15 * SwapDuration && ElapsedTime < 0.25) 
		{
			SwapDuration = 1.5;
		}

		float Progress = FMath::Clamp(ElapsedTime / SwapDuration, 0.0f, 1.0f);

		FVector RoundingVector(0, 250.0f, 350.0f);

		// Обчислюємо нові позиції для кожного об'єкта
		FVector NewPos1 = FMath::Lerp(StartLocation + RoundingVector * Progress, EndLocation, Progress);

		// Оновлюємо позиції об'єктів
		this->SetActorLocation(NewPos1);
		
		if(SwapObject)
		{
			FVector NewPos2 = FMath::Lerp(SwapObject->StartLocation - RoundingVector * Progress, SwapObject->EndLocation, Progress);
			SwapObject->SetActorLocation(NewPos2);
		}
		

		// Якщо анімація завершена, зупиняємо її
		if (Progress >= 1.0f)
		{
			// Завершуємо обмін позиціями 
			UpdateGridPositionAfterSwap(SwapObject);
		}
	}

	if (this->GetGemMesh()->IsSimulatingPhysics()) // вектор фізики
	{
		this->GetGemMesh()->AddForce(FVector(-13000.0f, 0.0f, 200.0f), NAME_None, true);
	}

	FVector Location = GetActorLocation();

	if (Location.X > 3000.0f || Location.X < -3000.0f ||
		Location.Y > 3000.0f || Location.Y < -3000.0f ||
		Location.Z > 1000.0f || Location.Z < -1000.0f)
	{
		Destroy();
	}
}

void AGem::UpdateGridPositionAfterSwap(AGem* Object2)
{
	bIsSwapping = false;

	this->SetActorLocation(EndLocation);

	this->GetGemMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Grid[columnInGrid][rowInGrid]->block->GetBlockMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	if(SwapObject != nullptr)
	{
		int tempColumn = this->columnInGrid;
		int tempRow = this->rowInGrid;

		SwapObject->bIsSwapping = false;

		SwapObject->SetActorLocation(SwapObject->EndLocation);

		Object2->GetGemMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Grid[Object2->columnInGrid][Object2->rowInGrid]->block->GetBlockMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		// Оновлюємо позиції в гриді
		Grid[columnInGrid][rowInGrid]->gem = Object2;
		Grid[Object2->columnInGrid][Object2->rowInGrid]->gem = this;

		// Оновлюємо знання обєкта про свою позицію в гриді
		this->columnInGrid = Object2->columnInGrid;
		this->rowInGrid = Object2->rowInGrid;

		Object2->columnInGrid = tempColumn;
		Object2->rowInGrid = tempRow;
	}

	GridObject->CheckMatch();
}