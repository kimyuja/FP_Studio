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
#include "ItemObject.h"
#include "Kismet/GameplayStatics.h"

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

	WorldContextObject = this;

}

void ABS_GoldBarGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsActived)
	{
		return;
	}
	if ((_target && FVector::Dist(activeObject->GetComponentLocation(), _target->GetActorLocation()) < 100.0) || timerCheck > 0.5)
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


UItemObject* ABS_GoldBarGimmick::GetDefaultItemObject()
{
	UE_LOG(LogTemp, Warning, TEXT("my pos : (%f, %f, %f)"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);

	newItemObject = NewObject<UItemObject>(this, UItemObject::StaticClass());

	if (newItemObject)
	{
		newItemObject->dimensions = FIntPoint(2, 2);
		newItemObject->rotationAngle = 0.0f;
		newItemObject->itemClass = ABS_GoldBarGimmick::StaticClass();
		newItemObject->itemActiveType = Myactivetype;
		newItemObject->beginLoc = FVector(-50000.f, -50000.f, -985.0f);
		newItemObject->beginRot = FRotator(0.f, 0.f, 0.f);
		newItemObject->setWorldActorRot = 0;
		newItemObject->rotationImgCheck = 0;
		newItemObject->bIsDraggable = true;
	}
	return newItemObject;
}

int32 ABS_GoldBarGimmick::OnMyActive(AActor* ActivePlayer)
{

	if (bIsFinished)
	{
		return -1;
	}

	switch (Myactivetype)
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

	/*for (TActorIterator<ATestPlayer> it(GetWorld()); it; ++it)
	{
		players.Add(*it);
	}

	int targetNum = FMath::RandRange(0, players.Num() - 1);

	players[targetNum]->bIsBlack = true;
	players[targetNum]->bIsDie = true;
	players[targetNum]->Respawn(5.0);
	players[targetNum]->DeathCounting();*/

	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, sounds[0], GetActorLocation());

	ATestPlayer* player = Cast<ATestPlayer>(ActivePlayer);
	if (player)
	{
		GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, [player]()
			{
				player->ServerRPC_SetPlayerPhysics(player);
				player->bIsDie = true;
				player->Respawn();
				player->DeathCounting();
			}, 1.5f, false);
	}

}

void ABS_GoldBarGimmick::Golden(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 2 : Golden"));
	ATestPlayer* player = Cast<ATestPlayer>(ActivePlayer);
	_target = Cast<ATestPlayer>(ActivePlayer);
	
	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, sounds[1], GetActorLocation());
	
	GetWorldTimerManager().SetTimer(goldT, [&]()
		{


			FVector targetLoc = (_target->GetActorLocation() - activeObject->GetComponentLocation()).GetSafeNormal();
			//targetLoc = FVector(targetLoc.X, targetLoc.Y, 0);
			activeObject->SetRelativeLocation(activeObject->GetRelativeLocation() + targetLoc * 30.0);
			timerCheck += GetWorld()->GetDeltaSeconds();
		}, 0.03f, true, 0);
}

void ABS_GoldBarGimmick::DoorOpen()
{
	bCanActive = false;

	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, sounds[2], GetActorLocation());

	UE_LOG(LogTemp, Warning, TEXT("Clear!"));
}
