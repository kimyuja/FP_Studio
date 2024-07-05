// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_PressButtonGimmick.h"

#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "TestPlayer.h"
#include "TrapDoor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>

ASM_PressButtonGimmick::ASM_PressButtonGimmick()
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
	activeObject->SetupAttachment(object);
	activeObject->SetRelativeLocation(FVector(0, 0, -150.0));


	trigger->OnComponentBeginOverlap.AddDynamic(this, &AGimmick::SetCanActiveT);
	trigger->OnComponentEndOverlap.AddDynamic(this, &AGimmick::SetCanActiveF);
}


void ASM_PressButtonGimmick::BeginPlay()
{
	Super::BeginPlay();

	SetActiveType(Myactivetype);

}

void ASM_PressButtonGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	lerpTime += DeltaTime;

	if (lerpTime > 1)
	{
		lerpTime = 0;
		GetWorldTimerManager().PauseTimer(pressT);
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


int32 ASM_PressButtonGimmick::OnMyActive(AActor* ActivePlayer)
{

	if (bIsFinished)
	{
		return -1;
	}

	switch (activeType)
	{
	case 0:
		Waterbomb();
		break;
	case 1:
		Blinklife();
		break;
	case 2:
		Autopilot();
		break;
	default:
		break;
	}

	bIsFinished = true;

	return activeType;
}

void ASM_PressButtonGimmick::Waterbomb()
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 1 : Waterbomb"));
	lerpTime = 0;
	GetWorldTimerManager().SetTimer(pressT, [&]()
		{
			float loc = FMath::Lerp(0, -3.0, lerpTime);
			activeObject->SetRelativeLocation(FVector(0, 0, 90 + loc));
		}, 0.03f, true, 0);
	for (TActorIterator<ATrapDoor> it(GetWorld()); it; ++it)
	{
		it->OpenDoor();
	}
	for (TActorIterator<ATestPlayer> player(GetWorld()); player; ++player)
	{
		player->Respawn();
		player->DeathCounting();
	}
	FLatentActionInfo actionInfo;
	UKismetSystemLibrary::Delay(GetWorld(),3.0, actionInfo);
}

void ASM_PressButtonGimmick::Blinklife()
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 2 : Blinklife"));
	for (TActorIterator<ATestPlayer> it(GetWorld()); it; ++it)
	{
		it->BlackScreen();
		players.Add(*it);
	}

	int targetNum = FMath::RandRange(0, players.Num() - 1);

	players[targetNum]->Respawn();
	players[targetNum]->DeathCounting();
}

void ASM_PressButtonGimmick::Autopilot()
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT("Clear!"));
}

//void ASM_PressButtonGimmick::SetCanActiveT(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (bIsFinished)
//	{
//		return;
//	}
//	ATestPlayer* player = Cast<ATestPlayer>(OtherActor);
//	if (player)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("overlap"));
//		player->bCanActive = true;
//		//OnMyActive(OtherActor);
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("RTY"));
//	}
//}
//
//void ASM_PressButtonGimmick::SetCanActiveF(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	ATestPlayer* player = Cast<ATestPlayer>(OtherActor);
//	if (player)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("overlap"));
//		player->bCanActive = false;
//		player->g = nullptr;
//		bCanActive = false;
//		//OnMyActive(OtherActor);
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("RTY"));
//	}
//}