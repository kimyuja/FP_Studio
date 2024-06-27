// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_PressButtonGimmick.h"

#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "TestPlayer.h"

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


	trigger->OnComponentBeginOverlap.AddDynamic(this, &ASM_PressButtonGimmick::SetCanActiveT);
	trigger->OnComponentEndOverlap.AddDynamic(this, &ASM_PressButtonGimmick::SetCanActiveF);
}


void ASM_PressButtonGimmick::BeginPlay()
{
	Super::BeginPlay();

	SetActiveType(Myactivetype);

	//trigger->SetBoxExtent(activeRange);

}

void ASM_PressButtonGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	lerpTime += DeltaTime;

	if (lerpTime > 1)
	{
		lerpTime = 0;
		GetWorldTimerManager().PauseTimer(spinT);
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

	Super::OnMyActive(ActivePlayer);

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

}

void ASM_PressButtonGimmick::Blinklife()
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 2 : Blinklife"));
	
}

void ASM_PressButtonGimmick::Autopilot()
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT("Clear!"));
	lerpTime = 0;
	GetWorldTimerManager().SetTimer(clearSpinT, [&]()
		{
			float rot = FMath::Lerp(0, 360.0, lerpTime);
			activeObject->SetRelativeRotation(FRotator(0, rot, 90));
		}, 0.03f, true, 0);

}

void ASM_PressButtonGimmick::SetCanActiveT(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsFinished)
	{
		return;
	}
	ATestPlayer* player = Cast<ATestPlayer>(OtherActor);
	if (player)
	{
		UE_LOG(LogTemp, Warning, TEXT("overlap"));
		player->bCanActive = true;
		//OnMyActive(OtherActor);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("RTY"));
	}
}

void ASM_PressButtonGimmick::SetCanActiveF(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ATestPlayer* player = Cast<ATestPlayer>(OtherActor);
	if (player)
	{
		UE_LOG(LogTemp, Warning, TEXT("overlap"));
		player->bCanActive = false;
		player->g = nullptr;
		bCanActive = false;
		//OnMyActive(OtherActor);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("RTY"));
	}
}