// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_WhistleGimmick.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "TestPlayer.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include "Shark.h"

ASM_WhistleGimmick::ASM_WhistleGimmick()
{
	PrimaryActorTick.bCanEverTick = true;

	base = CreateDefaultSubobject<USceneComponent>(TEXT("Base"));
	SetRootComponent(base);

	trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	trigger->SetBoxExtent(FVector(150));
	trigger->SetupAttachment(base);
	trigger->SetCollisionObjectType(ECC_GameTraceChannel1);

	object = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object"));
	object->SetupAttachment(base);

	activeObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Active Object"));
	activeObject->SetupAttachment(base);
	activeObject->SetRelativeLocation(FVector(0, 0, -150.0));


	trigger->OnComponentBeginOverlap.AddDynamic(this, &AGimmick::SetCanActiveT);
	trigger->OnComponentEndOverlap.AddDynamic(this, &AGimmick::SetCanActiveF);

}


void ASM_WhistleGimmick::BeginPlay()
{
	Super::BeginPlay();

	SetActiveType(Myactivetype);

}

void ASM_WhistleGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	lerpTime += DeltaTime;

	if (lerpTime > 1)
	{
		lerpTime = 0;
	}

	if (bCanActive)
	{
		activeObject->SetRenderCustomDepth(true);
	}
	else
	{
		activeObject->SetRenderCustomDepth(false);
	}
}


int32 ASM_WhistleGimmick::OnMyActive(AActor* ActivePlayer)
{

	if (bIsFinished)
	{
		return -1;
	}

	switch (activeType)
	{
	case 0:
		BabyShark();
		break;
	case 1:
		IronSupplementation(ActivePlayer);
		break;
	case 2:
		WhalePush();
		break;
	default:
		break;
	}

	bIsFinished = true;

	return activeType;
}

void ASM_WhistleGimmick::BabyShark()
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 1 : BabyShark"));
	for (TActorIterator<AShark> it(GetWorld()); it; ++it)
	{
		it->bIsActive = true;
	}
	
	for (TActorIterator<ATestPlayer> it(GetWorld()); it; ++it)
	{
		players.Add(*it);
	}

	int targetNum = FMath::RandRange(0, players.Num() - 1);

	players[targetNum]->bIsBlack = true;
	players[targetNum]->bIsDie = true;
	players[targetNum]->Respawn(5.0);
	players[targetNum]->DeathCounting();
}

void ASM_WhistleGimmick::IronSupplementation(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 2 : IronSupplementation"));

	ATestPlayer* player = Cast<ATestPlayer>(ActivePlayer);
	if (player)
	{
		player->Respawn();
		player->DeathCounting();
	}
}

void ASM_WhistleGimmick::WhalePush()
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT("Clear!"));
	
}
