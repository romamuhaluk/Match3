// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Match3Block.h"
#include "Match3BlockGrid.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gem.generated.h"

UCLASS()
class MATCH3_API AGem : public AActor
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* GemRoot;

	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MyComponentGems;

	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

public:

	//UFUNCTION()
	//int randomNum(int max);

	UFUNCTION()
	void randomGem(UStaticMeshComponent* Component);

	UFUNCTION()
	void OnFingerPressedGem(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void GemClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

	AGem();

	UPROPERTY(EditAnywhere)
	int columnInGrid;

	UPROPERTY(EditAnywhere)
	int rowInGrid;

	UFUNCTION()
	void HandleClicked();

	UFUNCTION()
	void OnHovered(UPrimitiveComponent* HoveredComponent);

	UFUNCTION()
	void SwapGems(AGem* Object2);

	UFUNCTION()
	void SwapOnPoint(FVector point);

	FVector StartLocation;
	FVector EndLocation;
	FRotator StartAngle;
	FRotator EndAngle;
	bool bIsSwapping;
	bool straightAnimation;
	AGem* SwapObject;
	float ElapsedTime;

	void Tick(float DeltaTime);
	void UpdateGridPositionAfterSwap(AGem* Object2);

	FORCEINLINE class UStaticMeshComponent* GetGemMesh() const { return MyComponentGems; }

	FORCEINLINE class USceneComponent* GetGemRoot() const { return GemRoot; }
};

int randomNum(int max);