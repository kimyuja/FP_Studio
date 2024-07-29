// Fill out your copyright notice in the Description page of Project Settings.


#include "BS_GoldBarGimmick.h"

#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "TestPlayer.h"
#include "TrapDoor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h>
#include "SM_ComputerMoniter.h"

ABS_GoldBarGimmick::ABS_GoldBarGimmick()
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


void ABS_GoldBarGimmick::BeginPlay()
{
	Super::BeginPlay();

	SetActiveType(Myactivetype);
}

void ABS_GoldBarGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsActived)
	{
		return;
	}
	if ((_target && FVector::Dist(activeObject->GetComponentLocation(), _target->GetActorLocation()) < 100.0))
	{
		UE_LOG(LogTemp, Warning, TEXT(" Shoot!!!!!!!!!!!"));
		GetWorldTimerManager().ClearTimer(goldT);
		_target->ServerRPC_SetPlayerPhysics(_target);
		_target->bIsDie = true;
		_target->Respawn();
		_target->DeathCounting();
		activeObject->DestroyComponent(true);
		bIsActived = true;
	}
	/*else if((_target && FVector::Dist(activeObject->GetComponentLocation(), _target->GetActorLocation()) > 100.0))
	{
		UE_LOG(LogTemp, Warning, TEXT("%f"), FVector::Dist(activeObject->GetComponentLocation(), _target->GetActorLocation()));
	}*/

	if (bCanActive)
	{
		activeObject->SetRenderCustomDepth(true);
	}
	else
	{
		activeObject->SetRenderCustomDepth(false);
	}
}


int32 ABS_GoldBarGimmick::OnMyActive(AActor* ActivePlayer)
{

	if (bIsFinished)
	{
		return -1;
	}

	switch (activeType)
	{
	case 0:
		Alarm(ActivePlayer);
		break;
	case 1:
		Golden(ActivePlayer);
		break;
	case 2:
		DoorOpen();
		break;
	default:
		break;
	}

	bIsFinished = true;

	return activeType;
}

void ABS_GoldBarGimmick::Alarm(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 1 : Alarm"));

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

void ABS_GoldBarGimmick::Golden(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 2 : Golden"));
	ATestPlayer* player = Cast<ATestPlayer>(ActivePlayer);
	_target = Cast<ATestPlayer>(ActivePlayer);
	GetWorldTimerManager().SetTimer(goldT, [&]()
		{
			FVector targetLoc = (_target->GetActorLocation() - activeObject->GetComponentLocation()).GetSafeNormal();
			//targetLoc = FVector(targetLoc.X, targetLoc.Y, 0);
			activeObject->SetRelativeLocation(activeObject->GetRelativeLocation() + targetLoc * 30.0);
		}, 0.03f, true, 0);
}

void ABS_GoldBarGimmick::DoorOpen()
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT("Clear!"));
}
