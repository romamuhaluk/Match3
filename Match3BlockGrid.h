// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Match3Block.h"
#include "Gem.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Match3BlockGrid.generated.h"

class AMatch3Block;
class AGem;

extern TArray<TArray<FGridElement*>> Grid;

USTRUCT()
struct FGridElement  // Unreal Engine рекомендує префікс "F" для структур
{
	GENERATED_BODY() // Додаємо макрос для віддзеркалення

	UPROPERTY() // Додаємо UPROPERTY, якщо потрібно серіалізувати
	AMatch3Block* block;

	UPROPERTY()
	AGem* gem;
};

/** Class used to spawn blocks and manage score */
UCLASS(minimalapi)
class AMatch3BlockGrid : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	//UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPROPERTY(Category = Grid, EditAnywhere)
	class USceneComponent* DummyRoot;

	/** Text component for the score */
	//UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//class UTextRenderComponent* ScoreText;

public:
	AMatch3BlockGrid();

	/** Number of blocks along each side of grid */
	//UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
	//int32 Size;

	/** Spacing of blocks */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
	float BlockSpacing;

protected:
	
	// Begin AActor interface
	virtual void BeginPlay() override;
	// End AActor interface
	

public:
	void CreateGrid();

	//void CheckMatch();

	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }

};

