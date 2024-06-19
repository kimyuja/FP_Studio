// Fill out your copyright notice in the Description page of Project Settings.


#include "WH_BroomstickGimmick.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>

AWH_BroomstickGimmick::AWH_BroomstickGimmick()
{
	PrimaryActorTick.bCanEverTick = true;

	base = CreateDefaultSubobject<USceneComponent>(TEXT("Base"));
	SetRootComponent(base);

	trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	trigger->SetBoxExtent(FVector(300));
	trigger->SetupAttachment(base);

	object = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object"));
	object->SetupAttachment(base);

	activeObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Active Object"));
	activeObject->SetupAttachment(base);


	trigger->OnComponentBeginOverlap.AddDynamic(this, &AWH_BroomstickGimmick::SetCanActiveT);
	trigger->OnComponentEndOverlap.AddDynamic(this, &AWH_BroomstickGimmick::SetCanActiveF);
}


void AWH_BroomstickGimmick::BeginPlay()
{
	Super::BeginPlay();

	SetActiveType(Myactivetype);

	//trigger->SetBoxExtent(activeRange);

}

void AWH_BroomstickGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AWH_BroomstickGimmick::OnMyActive(AActor* ActivePlayer)
{
	Super::OnMyActive(ActivePlayer);

	switch (activeType)
	{
	case 0:
		SelfHeadSmash();
		break;
	case 1:
		PoorDriver();
		break;
	case 2:
		DoorBurst();
		break;
	default:
		break;
	}
}

void AWH_BroomstickGimmick::SelfHeadSmash()
{
	UE_LOG(LogTemp, Warning, TEXT(" Death 1 : SelfHeadSmash"));
}

void AWH_BroomstickGimmick::PoorDriver()
{
	UE_LOG(LogTemp, Warning, TEXT(" Death 2 : PoorDriver"));
}

void AWH_BroomstickGimmick::DoorBurst()
{
	UE_LOG(LogTemp, Warning, TEXT("Clear!"));
}

void AWH_BroomstickGimmick::SetCanActiveT(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void AWH_BroomstickGimmick::SetCanActiveF(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bCanActive = false;
}
