// Fill out your copyright notice in the Description page of Project Settings.


#include "ClearDoor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include "TestPlayer.h"

// Sets default values
AClearDoor::AClearDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	base = CreateDefaultSubobject<USceneComponent>(TEXT("Base"));
	SetRootComponent(base);

	trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	trigger->SetBoxExtent(FVector(300));
	trigger->SetupAttachment(base);

	object = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object"));
	object->SetupAttachment(base);

	trigger->OnComponentBeginOverlap.AddDynamic(this, &AClearDoor::AutoOpen);
}

// Called when the game starts or when spawned
void AClearDoor::BeginPlay()
{
	Super::BeginPlay();
	
	//OpenDoor();
}

// Called every frame
void AClearDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	lerpTime += DeltaTime;

	if (lerpTime > 1)
	{
		lerpTime = 0;
		GetWorldTimerManager().PauseTimer(openDoorT);
	}
}

void AClearDoor::OpenDoor()
{
	//UE_LOG(LogTemp, Warning, TEXT(" DoorOpen"));
	lerpTime = 0;
	bIsOpen = true;
	GetWorldTimerManager().SetTimer(openDoorT, [&]()
		{
			float rot = FMath::Lerp(0.0, 90.0, lerpTime);
			object->SetRelativeRotation(FRotator(0, rot, 0));
		}, 0.03f, true, 0);
}



void AClearDoor::AutoOpen(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (bIsOpen)
	{
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Test"));
	ATestPlayer* player = Cast<ATestPlayer>(OtherActor);
	if (player)
	{
		if (player->bCanOpenDoor == true)
		{
			//UE_LOG(LogTemp, Warning, TEXT("overlap"));
			player->bCanActive = true;
			OpenDoor();
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("RTY"));
	}
}
