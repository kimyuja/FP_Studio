// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_WhistleGimmick.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "TestPlayer.h"
#include "ItemObject.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include "Shark.h"
#include "Kismet/GameplayStatics.h"

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

	WorldContextObject = this;

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

UItemObject* ASM_WhistleGimmick::GetDefaultItemObject()
{
	newItemObject = NewObject<UItemObject>(this, UItemObject::StaticClass());

	if (newItemObject)
	{
		newItemObject->dimensions = FIntPoint(2, 2);
		newItemObject->rotationAngle = 0.0f;
		newItemObject->itemClass = ASM_WhistleGimmick::StaticClass();
		newItemObject->itemActiveType = Myactivetype;
		newItemObject->beginLoc = FVector(50000.f, 50000.f, -935.f);
		newItemObject->beginRot = FRotator(0.f, -90.f, 0.f);
		newItemObject->setWorldActorRot = 0;
		newItemObject->rotationImgCheck = 0;
		newItemObject->bIsDraggable = true;
	}
	return newItemObject;
}


int32 ASM_WhistleGimmick::OnMyActive(AActor* ActivePlayer)
{

	if (bIsFinished)
	{
		return -1;
	}

	switch (Myactivetype)
	{
	case 0:
		BabyShark(ActivePlayer);
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

void ASM_WhistleGimmick::BabyShark(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 1 : BabyShark"));

	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, sounds[0], GetActorLocation());

	for (TActorIterator<AShark> it(GetWorld()); it; ++it)
	{
		it->bIsActive = true;
	}
	
	ATestPlayer* player = Cast<ATestPlayer>(ActivePlayer);
	if (player)
	{
		player->bIsBlack = true;
		player->bIsDie = true;
		player->DeathCounting();
		player->Respawn(5.0);
	}
	FTimerHandle dieT;
	GetWorldTimerManager().SetTimer(dieT, [&]() {
		APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		player->ServerRPC_SetPlayerPhysics(player);
		}, 1.0, false, 4.0);
}

void ASM_WhistleGimmick::IronSupplementation(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 2 : IronSupplementation"));

	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, sounds[1], GetActorLocation());

	ATestPlayer* player = Cast<ATestPlayer>(ActivePlayer);
	if (player)
	{
		player->ServerRPC_SetPlayerPhysics(player);
		player->bIsDie = true;
		player->Respawn();
		player->DeathCounting();
	}
}

void ASM_WhistleGimmick::WhalePush()
{
	bCanActive = false;

	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, sounds[2], GetActorLocation());

	UE_LOG(LogTemp, Warning, TEXT("Clear!"));
	
}
