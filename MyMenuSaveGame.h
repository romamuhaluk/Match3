#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MyMenuSaveGame.generated.h"

// ��������� ��� ���������� ����� ��'����
USTRUCT(BlueprintType)
struct FMenuObjectSaveData
{
    GENERATED_BODY()

    UPROPERTY()
    FString ObjectName;

    UPROPERTY()
    FVector Location;

    UPROPERTY()
    FRotator Rotation;
};

UCLASS()
class MATCH3_API UMyMenuSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UMyMenuSaveGame();

    // ����� ��� ���������� ��� ��'����
    UPROPERTY()
    TArray<FMenuObjectSaveData> SavedObjects;
};



