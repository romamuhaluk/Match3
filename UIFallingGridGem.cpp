// Fill out your copyright notice in the Description page of Project Settings.


#include "UIFallingGridGem.h"
#include "Engine/World.h"

AUIFallingGridGem* AUIFallingGridGem::FallingGrid = nullptr;

// Sets default values
AUIFallingGridGem::AUIFallingGridGem()
{
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy"));
	RootComponent = DummyRoot;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	gridLocation = GetActorLocation();

	FallingGrid = this;
}

void AUIFallingGridGem::BeginPlay()
{
	CreateGrid();
}

// Called every frame
void AUIFallingGridGem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AUIFallingGridGem::CreateGrid()
{
	int NumburGem = 150;
	FallingGemGrid.SetNum(NumburGem); //SetNum для ініціалізації стовбців

	// Loop to spawn each block
	for (int32 i = 0; i < NumburGem; i++)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AUIFallingGridGem::CreateGem, 0.3f, false);
	}
}

void AUIFallingGridGem::CreateGem()
{
	int XOffset = randomNum(2000);
	int YOffset = randomNum(4000);

	// Make position vector, offset from Grid location
	const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();;

	AFallingGem* NewGem = GetWorld()->SpawnActor<AFallingGem>((BlockLocation + FVector(0, 0, 40.f)), FRotator(0, 0, 0));
}



AFallingGem::AFallingGem()
	: AGem()  // Виклик батьківського конструктора
{	
	GetGemMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetGemMesh()->SetSimulatePhysics(true);
	GetGemMesh()->SetEnableGravity(false);

	fallingForce = -10.0f * randomNum(50);
}

void AFallingGem::Tick(float DeltaTime)
{
	//GetGemMesh()->AddForce(FVector(fallingForce, 0, 0), NAME_None, true);

	GetGemMesh()->SetPhysicsLinearVelocity(FVector(0, 0, fallingForce)); // Постійна швидкість вниз

	FVector Location = GetActorLocation();

	if (Location.X > 5000.0f || Location.X < -5000.0f ||
		Location.Y > 5000.0f || Location.Y < -5000.0f ||
		Location.Z > 2500.0f || Location.Z < -1500.0f)
	{
		int XOffset = randomNum(2000);
		int YOffset = randomNum(4000);
		SetActorLocation(FVector(XOffset, YOffset, 0.f) + AUIFallingGridGem::FallingGrid->GetActorLocation());

		fallingForce = -10.0f * randomNum(50);

		//GetGemMesh()->AddForce(FVector(0, 0, fallingForce), NAME_None, true);

		//GemSpin = FRotator(randomNum(10), randomNum(10), randomNum(10));
		//SetActorRotation(GetActorRotation() + GemSpin);
	}
}


