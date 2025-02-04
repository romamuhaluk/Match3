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
	
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* GemRoot;

	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MyComponentGems;

public:	
	// Sets default values for this actor's properties
	AGem();

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

	UFUNCTION()
	int randomNum(int maxGems);

	UFUNCTION()
	void randomGem(UStaticMeshComponent* Component);

	UFUNCTION()
	void OnFingerPressedGem(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void GemClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

public:	

	UPROPERTY(EditAnywhere)
	int positionInGrid;

	UFUNCTION()
	void SwapGems(AGem* Object2);

	UFUNCTION()
	void HandleClicked();

	UFUNCTION()
	void Highlight(bool bOn);

	FVector StartLocation;
	FVector EndLocation;
	bool bIsSwapping;
	AGem* SwapObject;
	float ElapsedTime;

	void Tick(float DeltaTime);
	void UpdateGridPositionAfterSwap(AGem* Object2);

	//NewObject<  >(this);


	FORCEINLINE class UStaticMeshComponent* GetGemMesh() const { return MyComponentGems; }

	FORCEINLINE class USceneComponent* GetGemRoot() const { return GemRoot; }
};
