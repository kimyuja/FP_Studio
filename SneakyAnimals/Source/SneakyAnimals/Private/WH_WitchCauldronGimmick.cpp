
// Fill out your copyright notice in the Description page of Project Settings.


#include "WH_WitchCauldronGimmick.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "ItemObject.h"
#include "TestPlayer.h"
#include "ClearDoor.h"
#include "Kismet/GameplayStatics.h"

AWH_WitchCauldronGimmick::AWH_WitchCauldronGimmick()
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
	object->SetRelativeLocation(FVector(0, 0, -1000.0));

	activeObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Active Object"));
	activeObject->SetupAttachment(base);
	activeObject->SetRelativeLocation(FVector(0, 0, -150.0));


	trigger->OnComponentBeginOverlap.AddDynamic(this, &AGimmick::SetCanActiveT);
	trigger->OnComponentEndOverlap.AddDynamic(this, &AGimmick::SetCanActiveF);
}


void AWH_WitchCauldronGimmick::BeginPlay()
{
	Super::BeginPlay();

	SetActiveType(Myactivetype);

	//trigger->SetBoxExtent(activeRange);

	WorldContextObject = this;
}

void AWH_WitchCauldronGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (lerpTime > 10.0)
	{
		GetWorldTimerManager().PauseTimer(fogT);
		object->SetVisibility(false);
		lerpTime = 0;
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


UItemObject* AWH_WitchCauldronGimmick::GetDefaultItemObject()
{
	UE_LOG(LogTemp, Warning, TEXT("my pos : (%f, %f, %f)"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);

	newItemObject = NewObject<UItemObject>(this, UItemObject::StaticClass());

	if (newItemObject)
	{
		newItemObject->dimensions = FIntPoint(2, 2);
		newItemObject->rotationAngle = 0.0f;
		newItemObject->itemClass = AWH_WitchCauldronGimmick::StaticClass();
		newItemObject->itemActiveType = Myactivetype;
		newItemObject->beginLoc = FVector(50130.f, -50100.f, -883.0f);
		newItemObject->beginRot = FRotator(0.f, 0.f, 0.f);
		newItemObject->setWorldActorRot = 0;
		newItemObject->rotationImgCheck = 0;
		newItemObject->bIsDraggable = true;
	}
	return newItemObject;
}

int32 AWH_WitchCauldronGimmick::OnMyActive(AActor* ActivePlayer)
{
	bCanActive = false;

	if (bIsFinished)
	{
		return -1;
	}

	switch (Myactivetype)
	{
	case 0:
		BlindFog();
		break;
	case 1:
		HereIsAWitch(ActivePlayer);
		break;
	case 2:
		KindWitch();
		break;
	default:
		break;
	}

	bIsFinished = true;

	return activeType;
}

void AWH_WitchCauldronGimmick::BlindFog()
{
	bCanActive = false;

	UE_LOG(LogTemp, Warning, TEXT(" Death 1 : BlindFog"));

	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, sounds[0], GetActorLocation());
	
	GetWorldTimerManager().SetTimer(fogT, [&]()
		{
			if (object->GetComponentLocation().Z >= 0)
			{
				return;
			}
			float loc = FMath::Lerp(object->GetComponentLocation().Z, 0, lerpTime);
			object->SetRelativeLocation(FVector(0,0,loc));
			lerpTime += GetWorld()->DeltaTimeSeconds;


		}, 0.03f, true, 0);
	for (TActorIterator<ATestPlayer> it(GetWorld()); it; ++it)
	{
		GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, [it]()
			{
				it->ServerRPC_SetPlayerPhysics(*it);
				it->bIsDie = true;
				it->Respawn();
				it->DeathCounting();
			}, 2.5f, false);
		//players.Add(*it);
	}
	/*for (TActorIterator<ATestPlayer> it(GetWorld()); it; ++it)
	{
		it->bIsDie = true;
		it->Respawn(10.0);
		it->DeathCounting();
	}*/

	//int targetNum = FMath::RandRange(0, players.Num() - 1);

	//players[targetNum]->Respawn();
}

void AWH_WitchCauldronGimmick::HereIsAWitch(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 2 : HereIsAWitch"));

	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, sounds[1], GetActorLocation());

	ATestPlayer* player = Cast<ATestPlayer>(ActivePlayer);
	if (player)
	{
		GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, [player]()
			{
				player->ServerRPC_SetPlayerPhysics(player);
				player->bIsDie = true;
				player->Respawn();
				player->DeathCounting();
			}, 1.5f, false);
	}
}

void AWH_WitchCauldronGimmick::KindWitch()
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT("Clear!"));

	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, sounds[2], GetActorLocation());

	for (TActorIterator<AClearDoor> it(GetWorld()); it; ++it)
	{
		if (it->roomType == 0)
		{
			it->OpenDoor();
		}
	}

}

//void AWH_WitchCauldronGimmick::SetCanActiveT(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
//void AWH_WitchCauldronGimmick::SetCanActiveF(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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


