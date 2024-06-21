// Fill out your copyright notice in the Description page of Project Settings.


#include "WH_PotionGimmick.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>

AWH_PotionGimmick::AWH_PotionGimmick()
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

	
	trigger->OnComponentBeginOverlap.AddDynamic(this, &AWH_PotionGimmick::SetCanActiveT);
	trigger->OnComponentEndOverlap.AddDynamic(this, &AWH_PotionGimmick::SetCanActiveF);
}


void AWH_PotionGimmick::BeginPlay()
{
	Super::BeginPlay();

	SetActiveType(Myactivetype);

	//trigger->SetBoxExtent(activeRange);

}

void AWH_PotionGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AWH_PotionGimmick::OnMyActive(AActor* ActivePlayer)
{
	Super::OnMyActive(ActivePlayer);

	switch (activeType)
	{
	case 0:
		Extincion(ActivePlayer);
		break;
	case 1:
		Poisoned(ActivePlayer);
		break;
	case 2:
		Berserk();
		break;
	case 3:
		SelfExplosion();
		break;
	case 4:
		knowledgeinjection();
		break;
	default:
		break;
	}
}

void AWH_PotionGimmick::Extincion(AActor* ActivePlayer)
{
	UE_LOG(LogTemp, Warning, TEXT(" Death 1 : Extincion"));
}

void AWH_PotionGimmick::Poisoned(AActor* ActivePlayer)
{
	UE_LOG(LogTemp, Warning, TEXT(" Death 2 : Poisoned"));
}

void AWH_PotionGimmick::Berserk()
{
	UE_LOG(LogTemp, Warning, TEXT(" Death 3 : Berserk"));
}

void AWH_PotionGimmick::SelfExplosion()
{
	UE_LOG(LogTemp, Warning, TEXT(" Death 4 : SelfExplosion"));
}

void AWH_PotionGimmick::knowledgeinjection()
{
	UE_LOG(LogTemp, Warning, TEXT("Clear!"));
}

void AWH_PotionGimmick::SetCanActiveT(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
		UE_LOG(LogTemp, Warning, TEXT("BanCho"));
	}
}

void AWH_PotionGimmick::SetCanActiveF(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bCanActive = false;
}
