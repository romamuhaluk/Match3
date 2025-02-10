// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Gem.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UIFallingGridGem.generated.h"

UCLASS()
class MATCH3_API AUIFallingGridGem : public AActor
{
	GENERATED_BODY()

	USceneComponent* DummyRoot;
	
public:	
	// Sets default values for this actor's properties
	AUIFallingGridGem();

	TArray<AGem*> FallingGemGrid;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void CreateGrid();

	void CreateGem();

	static AUIFallingGridGem* FallingGrid;

	FVector gridLocation;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	friend class AGem;
};

UCLASS()
class MATCH3_API AFallingGem : public AGem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFallingGem();

	FRotator GemSpin;
	int fallingForce;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	friend class AUIFallingGridGem;
};
