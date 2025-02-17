// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Light.h"
#include "LightManager.generated.h"

UCLASS()
class MATCH3_API ALightManager : public AActor
{
	GENERATED_BODY()

	void FindAndControlLight();
	
public:	
	// Sets default values for this actor's properties
	ALightManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	ALight* LightActor;
	ALight* LightActor2;

	FTimerHandle TimerHandle;

	void Highlight();


	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
