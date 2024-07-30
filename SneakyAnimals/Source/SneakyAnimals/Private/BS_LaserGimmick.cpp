// Fill out your copyright notice in the Description page of Project Settings.


#include "BS_LaserGimmick.h"

#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "TestPlayer.h"
#include "SP_RollingCart.h"
#include "GI_SneakyAnimals.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include "ItemObject.h"

ABS_LaserGimmick::ABS_LaserGimmick()
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


void ABS_LaserGimmick::BeginPlay()
{
	Super::BeginPlay();

	SetActiveType(Myactivetype);

	//trigger->SetBoxExtent(activeRange);

}

void ABS_LaserGimmick::Tick(float DeltaTime)
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


UItemObject* ABS_LaserGimmick::GetDefaultItemObject()
{
	UE_LOG(LogTemp, Warning, TEXT("my pos : (%f, %f, %f)"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);

	newItemObject = NewObject<UItemObject>(this, UItemObject::StaticClass());

	if (newItemObject)
	{
		newItemObject->dimensions = FIntPoint(2, 2);
		newItemObject->rotationAngle = 0.0f;
		newItemObject->itemClass = ABS_LaserGimmick::StaticClass();
		newItemObject->itemActiveType = Myactivetype;
		newItemObject->beginLoc = FVector(-50000.f, -50000.f, -978.0f);
		newItemObject->beginRot = FRotator(0.f, -90.f, 0.f);
		newItemObject->setWorldActorRot = 0;
		newItemObject->rotationImgCheck = 0;
	}
	return newItemObject;
}

int32 ABS_LaserGimmick::OnMyActive(AActor* ActivePlayer)
{

	if (bIsFinished)
	{
		return -1;
	}

	switch (activeType)
	{
	case 0:
		ElectricGrill(ActivePlayer);
		break;
	case 1:
		WarningDeath(ActivePlayer);
		break;
	case 2:
		Faker(ActivePlayer);
		break;
	default:
		break;
	}

	bIsFinished = true;

	return activeType;
}

void ABS_LaserGimmick::ElectricGrill(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 1 : ElectricGrill"));
	ATestPlayer* player = Cast<ATestPlayer>(ActivePlayer);
	if (player)
	{
		player->ServerRPC_SetPlayerPhysics(player);
		player->bIsDie = true;
		player->Respawn();
		player->DeathCounting();
	}
}

void ABS_LaserGimmick::WarningDeath(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 2 : WarningDeath"));
	FTimerHandle dieT;
	GetWorldTimerManager().SetTimer(dieT, [&]()
		{
			for (TActorIterator<ATestPlayer> it(GetWorld()); it; ++it)
			{
				it->ServerRPC_SetPlayerPhysics(*it);
				it->bIsDie = true;
				it->Respawn();
				it->DeathCounting();
			}
		}, 1.0, false, 3.0);
}

void ABS_LaserGimmick::Faker(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT("Clear!"));
}