// Fill out your copyright notice in the Description page of Project Settings.


#include "BS_HandleGimmick.h"

#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "TestPlayer.h"
#include "TrapDoor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h>
#include "SM_ComputerMoniter.h"
#include "ItemObject.h"

ABS_HandleGimmick::ABS_HandleGimmick()
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


void ABS_HandleGimmick::BeginPlay()
{
	Super::BeginPlay();

	SetActiveType(Myactivetype);
}

void ABS_HandleGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (bCanActive)
	{
		activeObject->SetRenderCustomDepth(true);
	}
	else
	{
		activeObject->SetRenderCustomDepth(false);
	}
}


UItemObject* ABS_HandleGimmick::GetDefaultItemObject()
{
	UE_LOG(LogTemp, Warning, TEXT("my pos : (%f, %f, %f)"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);

	newItemObject = NewObject<UItemObject>(this, UItemObject::StaticClass());

	if (newItemObject)
	{
		newItemObject->dimensions = FIntPoint(2, 2);
		newItemObject->rotationAngle = 0.0f;
		newItemObject->itemClass = ABS_HandleGimmick::StaticClass();
		newItemObject->itemActiveType = Myactivetype;
		newItemObject->beginLoc = FVector(-50000.f, -50000.f, -940.0f);
		newItemObject->beginRot = FRotator(0.f, 0.f, 0.f);
		newItemObject->setWorldActorRot = 0;
		newItemObject->rotationImgCheck = 0;
	}
	return newItemObject;
}

int32 ABS_HandleGimmick::OnMyActive(AActor* ActivePlayer)
{

	if (bIsFinished)
	{
		return -1;
	}
	
	switch (activeType)
	{
	case 0:
		Pikachu(ActivePlayer);
		break;
	case 1:
		WarningAlarm();
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

void ABS_HandleGimmick::Pikachu(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 1 : Pikachu"));
	ATestPlayer* player = Cast<ATestPlayer>(ActivePlayer);
	if (player)
	{
		player->Death_Thunderclap();
		player->DeathCounting();
	}
}

void ABS_HandleGimmick::WarningAlarm()
{
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

void ABS_HandleGimmick::DoorOpen()
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT("Clear!"));
}