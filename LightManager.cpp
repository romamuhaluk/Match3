// Fill out your copyright notice in the Description page of Project Settings.


#include "LightManager.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Components/LightComponent.h"
//#include "Engine/Light.h"


// Sets default values
ALightManager::ALightManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALightManager::BeginPlay()
{
	Super::BeginPlay();
    FindAndControlLight();
}

void ALightManager::Highlight()
{
    if (LightActor2->GetLightComponent()->Intensity < 0.01f)
    {
        LightActor2->GetLightComponent()->SetIntensity(1);
    }
    else
    {
        LightActor2->GetLightComponent()->SetIntensity(0);
    }

}

// Called every frame
void ALightManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (LightActor)  // Перевіряємо, чи світло знайдено
    {
        FRotator NewRotation = LightActor->GetActorRotation();
        NewRotation.Yaw += DeltaTime * 30.0f;
        LightActor->SetActorRotation(NewRotation);

        
        FRotator NewRotation2 = LightActor2->GetActorRotation();
        NewRotation2.Yaw -= DeltaTime * 30.0f;
        LightActor2->SetActorRotation(NewRotation2);
    }
}

void ALightManager::FindAndControlLight()
{
    AActor* FoundActor = nullptr;
    AActor* FoundActor2 = nullptr;

    for (TActorIterator<ALight> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
        if (ActorItr->GetName().Contains(TEXT("LightSource2"))) // Перевірка назви
        {
            FoundActor = *ActorItr;
        }
        if (ActorItr->GetName().Contains(TEXT("LightSource3"))) // Перевірка назви
        {
            FoundActor2 = *ActorItr;
        }
    }

    if (FoundActor && FoundActor2)
    {
        LightActor = Cast<ALight>(FoundActor);
        LightActor2 = Cast<ALight>(FoundActor2);

        GetWorldTimerManager().SetTimer(TimerHandle, this, &ALightManager::Highlight, 3.0f, true);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Light Source2 or Light Source3 not found!"));
    }
}

