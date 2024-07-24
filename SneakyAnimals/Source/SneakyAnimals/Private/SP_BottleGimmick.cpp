// Fill out your copyright notice in the Description page of Project Settings.


#include "SP_BottleGimmick.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "TestPlayer.h"
#include "SP_RollingCart.h"
#include "GI_SneakyAnimals.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>

ASP_BottleGimmick::ASP_BottleGimmick()
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
	object->SetRelativeLocation(FVector(0, 0, -150.0));

	activeObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Active Object"));
	activeObject->SetupAttachment(object);


	trigger->OnComponentBeginOverlap.AddDynamic(this, &AGimmick::SetCanActiveT);
	trigger->OnComponentEndOverlap.AddDynamic(this, &AGimmick::SetCanActiveF);
}


void ASP_BottleGimmick::BeginPlay()
{
	Super::BeginPlay();

	SetActiveType(Myactivetype);

	//trigger->SetBoxExtent(activeRange);

}

void ASP_BottleGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((_target && FVector::Dist(GetActorLocation(), _target->GetActorLocation()) < 100.0))
	{
		UE_LOG(LogTemp, Warning, TEXT(" Shoot!!!!!!!!!!!"));
		GetWorldTimerManager().ClearTimer(sonT);
		_target->bIsDie = true;
		_target->Respawn();
		_target->DeathCounting();
		Destroy();
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


int32 ASP_BottleGimmick::OnMyActive(AActor* ActivePlayer)
{

	if (bIsFinished)
	{
		return -1;
	}

	switch (activeType)
	{
	case 0:
		BottleTrap(ActivePlayer);
		break;
	case 1:
		Son(ActivePlayer);
		break;
	case 2:
		MasterKey(ActivePlayer);
		break;
	default:
		break;
	}

	bIsFinished = true;

	return activeType;
}

void ASP_BottleGimmick::BottleTrap(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 1 : BottleTrap"));
	ATestPlayer* player = Cast<ATestPlayer>(ActivePlayer);
	if (player)
	{
		player->ServerRPC_SetPlayerPhysics(player);
		player->bIsDie = true;
		player->Respawn();
		player->DeathCounting();
	}
}

void ASP_BottleGimmick::Son(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 2 : Son"));
	_target = Cast<ATestPlayer>(ActivePlayer);
	GetWorldTimerManager().SetTimer(sonT, [&]()
		{
			FVector targetLoc = (_target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			targetLoc = FVector(targetLoc.X, targetLoc.Y, 0);
			SetActorLocation(GetActorLocation() + targetLoc * 30.0);
		}, 0.03f, true, 0);
}

void ASP_BottleGimmick::MasterKey(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT("Clear!"));
}
