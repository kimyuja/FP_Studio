// Fill out your copyright notice in the Description page of Project Settings.


#include "SP_CartGimmick.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "TestPlayer.h"

ASP_CartGimmick::ASP_CartGimmick()
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
	object->SetRelativeLocation(FVector(0, 0, -150.0));

	activeObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Active Object"));
	activeObject->SetupAttachment(object);


	trigger->OnComponentBeginOverlap.AddDynamic(this, &AGimmick::SetCanActiveT);
	trigger->OnComponentEndOverlap.AddDynamic(this, &AGimmick::SetCanActiveF);
}


void ASP_CartGimmick::BeginPlay()
{
	Super::BeginPlay();

	SetActiveType(Myactivetype);

	//trigger->SetBoxExtent(activeRange);

}

void ASP_CartGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	lerpTime += DeltaTime;

	if (lerpTime > 1)
	{
		lerpTime = 0;
		
	}
	if (cartTarget && FVector::Dist(GetActorLocation(), cartTarget->GetActorLocation()) < 10.0)
	{
		UE_LOG(LogTemp, Warning, TEXT(" Boom!!!!!!!!!!!"));
		GetWorldTimerManager().PauseTimer(roadRollerT);
		cartTarget->bIsDie = true;
		cartTarget->Respawn();
		cartTarget->DeathCounting();
		Destroy();
	}
	if (Myactivetype == 2 && FVector::Dist(GetActorLocation(), FVector(0)) < 100.0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Go!!!!!!!!!!"));
		GetWorldTimerManager().PauseTimer(doorT);
		Destroy();
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


int32 ASP_CartGimmick::OnMyActive(AActor* ActivePlayer)
{

	if (bIsFinished)
	{
		return -1;
	}

	switch (activeType)
	{
	case 0:
		Defective(ActivePlayer);
		break;
	case 1:
		RoadRoller(ActivePlayer);
		break;
	case 2:
		RollingCart();
		break;
	default:
		break;
	}

	bIsFinished = true;

	return activeType;
}

void ASP_CartGimmick::Defective(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 2 : RoadRoller"));
	activeObject->SetSimulatePhysics(true);
	ATestPlayer* player = Cast<ATestPlayer>(ActivePlayer);
	if (player)
	{
		player->bIsDie = true;
		player->Respawn();
		player->DeathCounting();
	}
}

void ASP_CartGimmick::RoadRoller(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 2 : RoadRoller"));
	int32 ranNum = FMath::RandRange(0,3);
	ATestPlayer* player = Cast<ATestPlayer>(ActivePlayer);
	if (player)
	{
		ranNum = FMath::RandRange(0, 3);
		UE_LOG(LogTemp, Warning, TEXT("1"));
		for (TActorIterator<ATestPlayer> it(GetWorld()); it; ++it)
		{
			if (it->playerNum == ranNum)
			{
				UE_LOG(LogTemp, Warning, TEXT("3"));
				cartTarget = *it;
				GetWorldTimerManager().SetTimer(roadRollerT, [&]()
					{
						FVector targetLoc = (cartTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();
						UE_LOG(LogTemp, Warning, TEXT("Go!"));
						SetActorLocation(GetActorLocation() + targetLoc * 100.0);
					}, 0.03f, true, 0);
			}
		}
		lerpTime = 0;
	}
}

void ASP_CartGimmick::RollingCart()
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT("Clear!"));
	GetWorldTimerManager().SetTimer(doorT, [&]()
		{
			FVector targetLoc = (FVector(0, 0, 0) - GetActorLocation()).GetSafeNormal();
			UE_LOG(LogTemp, Warning, TEXT("Door Boom!"));
			SetActorLocation(GetActorLocation() + targetLoc * 100.0);
		}, 0.03f, true, 0);
	lerpTime = 0;


}
