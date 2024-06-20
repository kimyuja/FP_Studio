// Fill out your copyright notice in the Description page of Project Settings.


#include "WH_BookshelfGimmick.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include "TestPlayer.h"

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

	if (bCanActive)
	{
		activeObject->SetRenderCustomDepth(true);

	}
	else
	{
		activeObject->SetRenderCustomDepth(false);

	}
}


void AWH_BookshelfGimmick::OnMyActive(AActor* ActivePlayer)
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
}

void AWH_BookshelfGimmick::FallOver()
{
	UE_LOG(LogTemp, Warning, TEXT(" Death 1 : FallOver"));

	FTimerHandle falloverT;
	GetWorldTimerManager().SetTimer(falloverT, [&]()
	{
		float a = FMath::Lerp(0.0, -90.0, GetWorld()->GetDeltaSeconds() * 3.0);
		activeObject->SetRelativeRotation(GetActorRotation() + FRotator(0, a, 0));
	}, 3, false, 0);

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

