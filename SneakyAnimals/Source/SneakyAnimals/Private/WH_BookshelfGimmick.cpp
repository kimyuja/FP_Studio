// Fill out your copyright notice in the Description page of Project Settings.


#include "WH_BookshelfGimmick.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include "TestPlayer.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>

AWH_BookshelfGimmick::AWH_BookshelfGimmick()
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


	trigger->OnComponentBeginOverlap.AddDynamic(this, &AWH_BookshelfGimmick::SetCanActiveT);
	trigger->OnComponentEndOverlap.AddDynamic(this, &AWH_BookshelfGimmick::SetCanActiveF);
}


void AWH_BookshelfGimmick::BeginPlay()
{
	Super::BeginPlay();

	SetActiveType(Myactivetype);

	//trigger->SetBoxExtent(activeRange);

}

void AWH_BookshelfGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	lerpTime += DeltaTime;

	if(lerpTime > 1)
	{
		lerpTime = 0;
		GetWorldTimerManager().PauseTimer(falloverT);
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


int32 AWH_BookshelfGimmick::OnMyActive(AActor* ActivePlayer)
{
	//Super::OnMyActive(ActivePlayer);

	UE_LOG(LogTemp, Warning, TEXT("%d"), activeType);

	switch (activeType)
	{
	case 0:
		FallOver();
		break;
	case 1:
		BookCanFly();
		break;
	case 2:
		ButtonBook();
		break;
	default:
		break;
	}

	return activeType;
}

void AWH_BookshelfGimmick::FallOver()
{
	UE_LOG(LogTemp, Warning, TEXT(" Death 1 : FallOver"));
	lerpTime = 0;
	GetWorldTimerManager().SetTimer(falloverT, [&]()
	{
		float rot = FMath::Lerp(0.0, 90.0, lerpTime);
		float loc = FMath::Lerp(0.0, 50.0, lerpTime);
		//rot = FMath::Clamp(rot, 0, 90.0);
		activeObject->SetRelativeLocation(FVector(0, 0, loc));
		activeObject->SetRelativeRotation(FRotator(0, 0, rot));
	}, 0.03f , true, 0);
	for (TActorIterator<ATestPlayer> it(GetWorld()); it; ++it)
	{
		if (FVector::Dist(GetActorLocation(), it->GetActorLocation()) < 500.0)
		{
			it->Death_Fallover();
		}
	}
}

void AWH_BookshelfGimmick::BookCanFly()
{
	UE_LOG(LogTemp, Warning, TEXT(" Death 2 : BookCanFly"));
}

void AWH_BookshelfGimmick::ButtonBook()
{
	UE_LOG(LogTemp, Warning, TEXT("Clear!"));
}

void AWH_BookshelfGimmick::SetCanActiveT(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATestPlayer* player = Cast<ATestPlayer>(OtherActor);
	if (player)
	{
		UE_LOG(LogTemp, Warning, TEXT("overlap"));
		player -> bCanActive = true;
		//OnMyActive(OtherActor);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("RTY"));
	}
}

void AWH_BookshelfGimmick::SetCanActiveF(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

