// Fill out your copyright notice in the Description page of Project Settings.


#include "WH_WitchCauldronGimmick.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "TestPlayer.h"
#include "ClearDoor.h"

AWH_WitchCauldronGimmick::AWH_WitchCauldronGimmick()
{
	PrimaryActorTick.bCanEverTick = true;

	base = CreateDefaultSubobject<USceneComponent>(TEXT("Base"));
	SetRootComponent(base);

	trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	trigger->SetBoxExtent(FVector(150));
	trigger->SetupAttachment(base);

	object = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object"));
	object->SetupAttachment(base);

	activeObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Active Object"));
	activeObject->SetupAttachment(base);


	trigger->OnComponentBeginOverlap.AddDynamic(this, &AWH_WitchCauldronGimmick::SetCanActiveT);
	trigger->OnComponentEndOverlap.AddDynamic(this, &AWH_WitchCauldronGimmick::SetCanActiveF);
}


void AWH_WitchCauldronGimmick::BeginPlay()
{
	Super::BeginPlay();

	SetActiveType(Myactivetype);

	//trigger->SetBoxExtent(activeRange);

}

void AWH_WitchCauldronGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AWH_WitchCauldronGimmick::OnMyActive(AActor* ActivePlayer)
{
	Super::OnMyActive(ActivePlayer);

	switch (activeType)
	{
	case 0:
		BlindFog();
		break;
	case 1:
		HereIsAWitch();
		break;
	case 2:
		KindWitch();
		break;
	default:
		break;
	}
}

void AWH_WitchCauldronGimmick::BlindFog()
{
	UE_LOG(LogTemp, Warning, TEXT(" Death 1 : BlindFog"));

	for (TActorIterator<ATestPlayer> it(GetWorld()); it; ++it)
	{
		players.Add(*it);
	}

	int targetNum = FMath::RandRange(0, players.Num() - 1);

	players[targetNum]->Respawn();
}

void AWH_WitchCauldronGimmick::HereIsAWitch()
{
	UE_LOG(LogTemp, Warning, TEXT(" Death 2 : HereIsAWitch"));

	for (TActorIterator<ATestPlayer> it(GetWorld()); it; ++it)
	{
		players.Add(*it);
	}

	int targetNum = FMath::RandRange(0, players.Num() - 1);

	players[targetNum]->Respawn();
}

void AWH_WitchCauldronGimmick::KindWitch()
{
	UE_LOG(LogTemp, Warning, TEXT("Clear!"));

	for (TActorIterator<AClearDoor> it(GetWorld()); it; ++it)
	{
		if (it->roomType == 0)
		{
			it->OpenDoor();
		}
	}

}

void AWH_WitchCauldronGimmick::SetCanActiveT(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* player = Cast<ACharacter>(OtherActor);
	if (player)
	{
		UE_LOG(LogTemp, Warning, TEXT("overlap"));
		bCanActive = true;
		OnMyActive(OtherActor);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("RTY"));
	}
}

void AWH_WitchCauldronGimmick::SetCanActiveF(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bCanActive = false;
}


