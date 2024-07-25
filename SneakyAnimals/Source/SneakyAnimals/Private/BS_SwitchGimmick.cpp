// Fill out your copyright notice in the Description page of Project Settings.


#include "BS_SwitchGimmick.h"

#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "TestPlayer.h"
#include "TrapDoor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h>
#include "SM_ComputerMoniter.h"

ABS_SwitchGimmick::ABS_SwitchGimmick()
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


void ABS_SwitchGimmick::BeginPlay()
{
	Super::BeginPlay();

	SetActiveType(Myactivetype);
}

void ABS_SwitchGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((_target && FVector::Dist(GetActorLocation(), _target->GetActorLocation()) < 100.0))
	{
		UE_LOG(LogTemp, Warning, TEXT(" Shoot!!!!!!!!!!!"));
		GetWorldTimerManager().ClearTimer(goldT);
		_target->ServerRPC_SetPlayerPhysics(_target);
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


int32 ABS_SwitchGimmick::OnMyActive(AActor* ActivePlayer)
{

	if (bIsFinished)
	{
		return -1;
	}

	switch (activeType)
	{
	case 0:
		DangerousGaurd(ActivePlayer);
		break;
	case 1:
		DropMe(ActivePlayer);
		break;
	case 2:
		SafeGaurd();
		break;
	default:
		break;
	}

	bIsFinished = true;

	return activeType;
}

void ABS_SwitchGimmick::DangerousGaurd(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 1 : DangerousGaurd"));

	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 2 : WarningAlarm"));
	FTimerHandle dieT;
	GetWorldTimerManager().SetTimer(dieT, [&]()
		{
			for (TActorIterator<ATestPlayer> it(GetWorld()); it; ++it)
			{
				it->ServerRPC_SetPlayerPhysics(*it);
				it->bIsDie = true;
				it->Respawn();
				it->DeathCounting();
			}
		}, 1.0, false, 3.0);
}

void ABS_SwitchGimmick::DropMe(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 2 : DropMe"));
	ATestPlayer* player = Cast<ATestPlayer>(ActivePlayer);
	player->GetMesh()->SetSimulatePhysics(true);
	player->bIsDie = true;
	player->Respawn();
	player->DeathCounting();
}

void ABS_SwitchGimmick::SafeGaurd()
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT("Clear!"));
}